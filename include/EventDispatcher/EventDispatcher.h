//
// Created by wings on 2016/8/19.
//

#ifndef NET_EVENTDISPATCHER_H
#define NET_EVENTDISPATCHER_H

#include "../Utility/NetExeceptions.h"

#include <sys/epoll.h>
#include <cstdint>
#include <map>
#include <typeinfo>

//from libevent
#define MAX_EPOLL_TIMEOUT_MSEC (35*60*1000)
#define MAX_EPOLL_EVENT_SIZE 36

typedef int FD;
typedef struct epoll_event EpollEvent;

typedef enum {
  FD_READ,
  FD_WRITE,
  FD_CLOSE,
} FileDescriptorState;

typedef int Option;
typedef FD EventKey;
typedef struct EventHandler EventHandler;
typedef std::map<EventKey, EventHandler> EventMap;

class CallBackObject {

};

class EventHandler {
public:
  EventHandler() = default;
  EventHandler(EventKey key, Option option)
      : m_key(key)
      , m_option(option)
  {}

  const EventKey GetKey() const {
    return m_key;
  }

  const Option GetOption() const {
    return m_option;
  }

private:
  EventKey m_key;
  Option m_option;
  CallBackObject m_callback;
};

class Event {
public:
  Event(EventKey key, Option option)
      : m_key(key)
      , m_handler(key, option)
  {
  }

  const EventKey &GetKey() const {
    return m_key;
  }

  const EventHandler &GetHandler() const {
    return m_handler;
  }

  bool ModifyHandler(const EventHandler &new_handler) {
    if (GetKey() == new_handler.GetKey()) {
      m_handler = new_handler;
      return true;
    } else {
      return false;
    }
  }

private:
  EventKey m_key;
  EventHandler m_handler;
};

//Event dispatcher interface definition
class EventDispatcher {
public:
  virtual bool AddEvent(const Event& event) = 0;

  virtual bool ModifyEvent(const Event &event) = 0;

  virtual bool DeleteEvent(const Event& event) = 0;

  virtual bool Dispatch() = 0;

protected:
  /*
   * Those virtual functions are prepared for inherited class. Inherited
   * class should implement methods below to get the behaviour their expect
   *
   * Subclasses dispatcher have to implement AddEventToDispatcher/
   * ModifyEventInDispatcher/DeleteEventInDispatcher mthod to explicit define
   * how to add Event object to it
   */

  virtual bool AddEventToDispatcher(const Event &event) = 0;
  virtual bool ModifyEventInDispatcher(const Event &event) = 0;
  virtual bool DeleteEventInDispatcher(const Event &event) = 0;

protected:
  const EventHandler GetHandlerFromKey(EventKey key) const {
    auto it = m_event_map.find(key);
    if (it == m_event_map.end()) {
      EventHandler invalid;
      return invalid;
    }
    return m_event_map.find(key)->second;
  }

  bool isKeyExist(EventKey key) const {
    return (m_event_map.find(key) != m_event_map.end());
  }

private:
  bool AddNewEventToMap(const Event &event) {
    m_event_map[event.GetKey()] = event.GetHandler();
  }

  bool ModifyEventInMap(const Event &event) {
    if (m_event_map.end() == m_event_map.find(event.GetKey())) {
      return false;
    } else {
      m_event_map[event.GetKey()] = event.GetHandler();
      return true;
    }
  }

  bool DeleteEventInMap(const Event &event) {
    if (m_event_map.end() == m_event_map.find(event.GetKey())) {
      return false;
    } else {
      m_event_map.erase(event.GetKey());
      return true;
    }
  }

private:
  EventMap m_event_map;
};

/*
 * Event dispatcher use epoll as I/O multiplexer
 */

class EpollDispatcher : public EventDispatcher{
protected:
  typedef std::pair<FD, EpollEvent> EpollPair;
public:
  EpollDispatcher()
      : m_timeout(-1)
  {
    init(0);
  }

  EpollDispatcher(int time_out)
      : m_timeout(CheckTimeOut(time_out))
  {
    init(0);
  }

  EpollDispatcher(int time_out, int epoll_flags)
      : m_timeout(CheckTimeOut(time_out))
  {
    init(epoll_flags);
  }

protected:
  bool AddEventToDispatcher(const Event &event) override {
    EpollPair pair = TransferEventToEpollPair(event);
    return AddEventToEpoll(pair);
  }

  bool ModifyEventInDispatcher(const Event &event) override {
    EpollPair pair = TransferEventToEpollPair(event);
    return ModifyEventInEpoll(pair);
  }

  bool DeleteEventInDispatcher(const Event &event) override {
    EpollPair pair = TransferEventToEpollPair(event);
    return DeleteEventInEpoll(pair);
  }

  /**
   * @param ready_events value-result parameter, events return from epoll_wait will
   * be fill in to the space pointed by ready_events
   * @param size The size of space pointed by ready_events in bytes
   * epoll_wait is called, and the ready evnets will be placed in an array
   */
  int CallEpollWait(EpollEvent *ready_events, int size) {
    int read_queue_size = epoll_wait(m_epfd, ready_events, size, m_timeout);
    if (read_queue_size == -1) {
      throw NetSocketException(errno);
    }
    return read_queue_size;
  }

protected:
  /*
   * Subclasses have to
   * 1):
   * Implement TransferEventToEpollPair to define how to convert Event object
   * to epoll_event object and corresponding file descriptor
   * 2):
   * Implement GetKeyFromEpollEvent to define how to get the Key of
   * corresponding EventHandler
   *
   * EpollDispatcher won't take care how the Event handled, subclass indeed.
   */
  virtual EpollPair TransferEventToEpollPair(const Event &event) = 0;
  virtual EventKey GetKeyFromEpollEvent(const EpollEvent &event) = 0;

private:
  /**
   * Add a event to epoll
   * @param event Event reference
   */
  bool AddEventToEpoll(EpollPair pair) {
    if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, pair.first, &pair.second) == -1) {
      throw NetSocketException(errno);
    } else {
      return true;
    }
  }

  /**
   * Modify event in epoll
   * @param event This Event reference contians a event already in epoll's
   * events and a new handler. The old event's handler will be repalaced by
   * new handler
   */
 bool ModifyEventInEpoll(EpollPair pair) {
    if (epoll_ctl(m_epfd, EPOLL_CTL_MOD, pair.first, &pair.second) == -1) {
      throw NetSocketException(errno);
    } else {
      return true;
    }
  }

  /**
   * Delete a event in epoll
   * @param event Event reference refers to the event to be deleted
   */
  bool DeleteEventInEpoll(EpollPair pair) {
    if (epoll_ctl(m_epfd, EPOLL_CTL_DEL, pair.first, &pair.second)) {
      throw NetSocketException(errno);
    } else {
      return true;
    }
  }

private:
  /*
   * Member function in this private scope are expected to used only by
   * EpollDispatcher itself.
   *
   * Inherited class should consider carefully and implement their own
   * procedures to accomplish ADD/MOD/DEL functions
   */
  void init(int flags) {
    m_epfd = epoll_create1(flags);
    if (m_epfd == -1) {
      throw NetSocketException(errno);
    }
  }

  static int CheckTimeOut(int expected_time) {
    return (expected_time > MAX_EPOLL_TIMEOUT_MSEC)
           ? MAX_EPOLL_TIMEOUT_MSEC : expected_time;
  }


private:
  int m_epfd;
  int m_timeout;
};
#endif //NET_EVENTDISPATCHER_H

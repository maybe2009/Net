//
// Created by wings on 2016/8/19.
//

#ifndef NET_EVENTDISPATCHER_H
#define NET_EVENTDISPATCHER_H

#include "../Utility/NetExeceptions.h"
#include "../Utility/StringHelpers.h"

#include <sys/epoll.h>
#include <cstdint>
#include <map>
#include <typeinfo>
#include <memory>
#include <vector>
#include <iostream>

//from libevent
#define MAX_EPOLL_TIMEOUT_MSEC (35*60*1000)
#define MAX_EPOLL_EVENT_SIZE 36
#define EPOLL_ERROR -1

typedef int FD;
typedef struct epoll_event EpollEvent;
typedef int Option;
typedef int EventState;
typedef FD EventKey;
typedef struct EventHandler EventHandler;
typedef std::map<EventKey, EventHandler> EventMap;
typedef int LinuxEventState;

class LinuxEvent{
public:
  LinuxEvent() = default;

  LinuxEvent(FD fd, EventState state)
      : m_fd(fd)
      , m_state(state)
  {}

  LinuxEvent(const LinuxEvent &rhs) {
    this->m_fd = rhs.m_fd;
    this->m_key = rhs.m_key;
    this->m_state = rhs.m_state;
  }

  FD FileDescriptor() const {
    return m_fd;
  }

  EventState State() const {
    return m_state;
  }

  EventKey Key() const {
    return m_key;
  }

private:
  FD m_fd;
  EventState m_state;
  EventKey  m_key;
};

typedef std::vector<LinuxEvent> LinuxEventVector;

class CallBackObject {

};

class EventHandler {
public:
  EventHandler() = default;
  EventHandler(EventKey event, Option option)
      : m_event(event)
      , m_option(option)
  {}

  const EventKey GetEvent() const {
    return m_event;
  }

  const Option GetOption() const {
    return m_option;
  }

private:
  EventKey m_event;
  Option m_option;
  CallBackObject m_callback;
};

class Event {
public:
  virtual int FileDescriptor() const = 0;
  virtual int Option() const = 0;
  virtual EventHandler Handler() const = 0;
};



//Event dispatcher interface definition
class EventDispatcher {
public:
  virtual bool AddEvent(const LinuxEvent &event) = 0;

  virtual bool ModifyEvent(const LinuxEvent &event) = 0;

  virtual bool DeleteEvent(const LinuxEvent &event) = 0;

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

//  virtual bool AddEventToDispatcher(const Event &event) = 0;
//  virtual bool ModifyEventInDispatcher(const Event &event) = 0;
//  virtual bool DeleteEventInDispatcher(const Event &event) = 0;

protected:
//  const EventHandler GetHandlerFromKey(EventKey key) const {
//    auto it = m_event_map.find(key);
//    if (it == m_event_map.end()) {
//      EventHandler invalid;
//      return invalid;
//    }
//    return m_event_map.find(key)->second;
//  }
//
//  bool isKeyExist(EventKey key) const {
//    return (m_event_map.find(key) != m_event_map.end());
//  }

private:
//  bool AddNewEventToMap(const Event &event) {
//    m_event_map[event.FileDescriptor()] = event.GetHandler();
//  }
//
//  bool ModifyEventInMap(const Event &event) {
//    if (m_event_map.end() == m_event_map.find(event.FileDescriptor())) {
//      return false;
//    } else {
//      m_event_map[event.FileDescriptor()] = event.GetHandler();
//      return true;
//    }
//  }
//
//  bool DeleteEventInMap(const Event &event) {
//    if (m_event_map.end() == m_event_map.find(event.FileDescriptor())) {
//      return false;
//    } else {
//      m_event_map.erase(event.FileDescriptor());
//      return true;
//    }
//  }

private:
  EventMap m_event_map;
};

class LinuxEventDispatcher {
public:
  virtual void AddEvent(const LinuxEvent &event) = 0;
  virtual void DeleteEvent(const LinuxEvent &event) = 0;
  virtual void ModifyEvent(const LinuxEvent &event) = 0;
  virtual LinuxEventVector  Wait() = 0;
};

class Epoll : public LinuxEventDispatcher {
protected:
  typedef std::pair<FD, EpollEvent> EpollPair;
  typedef UniquePtrArray<EpollEvent> EventArray;

public:
  Epoll()
      : m_timeout(-1)
      , m_max_wait(MAX_SIZE)\
      , m_event_array()
  {
    init(0);
  }

  Epoll(int time_out, int max_wait)
      : m_timeout(checkTimeout(time_out))
      , m_max_wait(checkWaitSize(max_wait))
      , m_event_array()
  {
    init(0);
  }

  Epoll(int time_out, int max_wait, int epoll_flags)
      : m_timeout(checkTimeout(time_out))
      , m_max_wait(checkWaitSize(max_wait))
      , m_event_array()
  {
    init(epoll_flags);
  }

public:
  /*
   * Add/Modify/Delete/Wait interfaces
   */

  virtual void AddEvent(const LinuxEvent &event) override {
    EpollEvent _event = ToEpollEvent(event);
    EpollPair _pair(event.FileDescriptor(), _event);

    if (AddEventToEpoll(_pair) == EPOLL_ERROR) {
      throw NetSocketException(errno);
    }
  }

  virtual void ModifyEvent(const LinuxEvent &event) override {
    EpollEvent _event = ToEpollEvent(event);
    EpollPair _pair(event.FileDescriptor(), _event);

    if (ModifyEventInEpoll(_pair) == EPOLL_ERROR) {
      throw NetSocketException(errno);
    }
  }

  virtual void DeleteEvent(const LinuxEvent &event) override {
    EpollEvent _event = ToEpollEvent(event);
    EpollPair _pair(event.FileDescriptor(), _event);

    if (DeleteEventInEpoll(_pair) == EPOLL_ERROR) {
      throw NetSocketException(errno);
    }
  }

  virtual LinuxEventVector Wait() override {
    //call epoll_wait to get epoll_event array
    int _count = EpollWait(m_event_array.Raw(), m_event_array.Size());

    //push all epll_event to container
    LinuxEventVector _events;
    for (int idx = 0; idx < _count; ++idx) {

      EpollEvent *_event_ptr = m_event_array.Raw() + idx;
      std::cout << "epoll idx " << idx << std::endl;
      std::cout << "epoll fd " << _event_ptr->data.fd << std::endl;
      LinuxEvent _state = FromEpollEvent(*_event_ptr);
      std::cout << "State: " << std::endl;
      std::cout << "fd" << _state.FileDescriptor() << std::endl;
      _events.push_back(_state);
    }

    return _events;
  }

protected:
  /**
   * Subclasses have to
   * 1):
   * Implement ToEpollEvent to define how to convert LinuxEvent
   * to LinuxEvent
   * 2):
   * Implement FromEpollEvent to define how to get LinuxEvent
   * from LinuxEvent
   */

  virtual LinuxEvent FromEpollEvent(EpollEvent ev) {
    return LinuxEvent(ev.data.fd, ev.events);
  }

  virtual EpollEvent ToEpollEvent(const LinuxEvent &event) {
    struct epoll_event _epoll_event;
    _epoll_event.events = event.State();
    _epoll_event.data.fd = event.FileDescriptor();
    return _epoll_event;
  }

protected:
  /**
   * Add a event to epoll
   * @param event Event reference
   */
  int AddEventToEpoll(EpollPair pair) {
    if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, pair.first, &pair.second) == -1) {
      return EPOLL_ERROR;
    } else {
      return 0;
    }
  }

 /**
  * Modify event in epoll
  * @param event This Event reference contians a event already in epoll's
  * events and a new handler. The old event's handler will be repalaced by
  * new handler
  */
 int ModifyEventInEpoll(EpollPair pair) {
    if (epoll_ctl(m_epfd, EPOLL_CTL_MOD, pair.first, &pair.second) == -1) {
      return EPOLL_ERROR;
    } else {
      return 0;
    }
  }

  /**
   * Delete a event in epoll
   * @param event Event reference refers to the event to be deleted
   */
  int DeleteEventInEpoll(EpollPair pair) {
    if (epoll_ctl(m_epfd, EPOLL_CTL_DEL, pair.first, &pair.second)) {
      return EPOLL_ERROR;
    } else {
      return 0;
    }
  }

  /**
   * @param ready_events value-result parameter, events return from epoll_wait will
   * be fill in to the space pointed by ready_events
   * @param size The size of space pointed by ready_events in bytes
   * epoll_wait is called, and the ready evnets will be placed in an array
   */
  int EpollWait(EpollEvent *ready_events, int size) {
    int _event_count = epoll_wait(m_epfd, ready_events, size, m_timeout);
    if (_event_count == -1) {
      return EPOLL_ERROR;
    }
    return _event_count;
  }

private:
  /**
   * Helper functions used by Epoll itself
   */

  void init(int flags) {
    m_epfd = epoll_create1(flags);
    if (m_epfd == -1) {
      throw NetSocketException(errno);
    }

    //delay memory allocation in init for exception safe
    m_event_array.Relocate(m_max_wait);
  }

  static int checkTimeout(int expected_time) {
    return (expected_time > MAX_EPOLL_TIMEOUT_MSEC)
           ? MAX_EPOLL_TIMEOUT_MSEC : expected_time;
  }

  static int checkWaitSize(int wait_size) {
    return (wait_size > MAX_SIZE)
           ? MAX_SIZE : wait_size;
  }

private:
  int m_epfd;
  int m_timeout;
  int m_max_wait;
  EventArray m_event_array;
};
#endif //NET_EVENTDISPATCHER_H

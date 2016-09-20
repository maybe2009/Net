//
// Created by DW on 2016/8/29.
//

#ifndef NET_IOMULTIPLEXER_H
#define NET_IOMULTIPLEXER_H

#include "EventDispatcher.h"

#include <memory>
#include <iostream>
#include <utility>
#include <string>

typedef enum {
  ON_READ,
  ON_WRITE,
  ON_ERROR,
} FileDescriptorState;

#define ON_READ   0x01
#define ON_WRITE  0X02
#define ON_ERROR  0x03

typedef enum {
  READ = ON_READ,
  WRITE = ON_WRITE,
  ERROR = ON_ERROR,
} SubscribeState;

typedef std::function<void(void)> MultiplexerCallBack;

class EventSubscriber {
public:
  EventSubscriber() = default;

  EventSubscriber(FD fd, SubscribeState state)
      : m_key(fd)
      , m_state(state)
  {}

  EventKey Key() const {
    return m_key;
  }

  LinuxEventState State() const {
    return m_state;
  }

  MultiplexerCallBack CallBack() const {
    return m_callback;
  }

  void SetCallBack(MultiplexerCallBack callBack) {
    m_callback = callBack;
  }

private:
  FD m_key;
  LinuxEventState m_state;
  MultiplexerCallBack m_callback;
};

class IOMultiplexer {
  typedef std::map<EventKey, EventSubscriber> EventMap;
public:
  IOMultiplexer(LinuxEventDispatcher& dispatcher)
      : m_dispatcher(dispatcher)
  {
  }

  bool AddEvent(const LinuxEvent &event) {
    m_dispatcher.AddEvent(event);
  }

  bool ModifyEvent(const LinuxEvent &event) {
    m_dispatcher.ModifyEvent(event);
  }

  bool DeleteEvent(const LinuxEvent &event) {
    m_dispatcher.DeleteEvent(event);
  }

  virtual bool Subscribe(EventSubscriber subscriber) {
    EventKey _key = subscriber.Key();
    m_event_map[_key] = subscriber;
  }

  virtual void Run() {
    LinuxEventVector _active_event = getActiveEvent();

    for (auto &&item : _active_event) {
      EventKey _key = item.FileDescriptor();
      EventSubscriber _subscriber = getSubscriber(_key);
      MultiplexerCallBack _callback =  _subscriber.CallBack();

      std::cout << "Call now..." << std::endl;
      _callback();
    }
  }

protected:
  LinuxEventVector getActiveEvent() {
    return m_dispatcher.Wait();
  }

  EventSubscriber getSubscriber(EventKey key) const {
    auto it = m_event_map.find(key);
    if (it != m_event_map.end()) {
      return it->second;
    } else {
      std::cout << "Subscriber not register" << std::endl;
        
    }
  }

private:
  LinuxEventDispatcher &m_dispatcher;
  EventMap m_event_map;
};

#endif //NET_IOMULTIPLEXER_H

//
// Created by DW on 2016/8/29.
//

#ifndef NET_IOMULTIPLEXER_H
#define NET_IOMULTIPLEXER_H

#include "EventDispatcher.h"

#include <memory>
#include <iostream>

class EpollEventArray {
public:
  EpollEventArray(int size)
      : m_event_arary_size(size)
      , m_event_array(new EpollEvent[size])
  {
  }

  EpollEvent *GetArrayPointer() {
    return m_event_array.get();
  }

  int GetArraySize() {
    return m_event_arary_size;
  }

private:
  int m_event_arary_size;
  std::unique_ptr<EpollEvent> m_event_array;
};

class IOMultiplexer : public EpollDispatcher{
public:
  IOMultiplexer(int event_array_size)
      : m_events_array(event_array_size)
  {

  }

  bool Dispatch() override {
    int ready_queue_size = CallEpollWait(m_events_array.GetArrayPointer(),
                                         m_events_array.GetArraySize());
    EpollEvent *ready_queue = m_events_array.GetArrayPointer();

    for (int idx = 0; idx < ready_queue_size; idx++) {
      //test whether the associate key is exist
      auto key = GetKeyFromEpollEvent(ready_queue[idx]);
      if (!isKeyExist(key)) {
        continue;
      }
      //do something...
      const EventHandler &handler =
          GetHandlerFromKey(key);

      std::cout << "Say Hello! " << key << " : " << handler.GetKey()<< std::endl;

    }
  }

private:
  EpollPair TransferEventToEpollPair(const Event &event) override {
    struct epoll_event new_epoll_event;
    new_epoll_event.events = event.GetHandler().GetOption();
    new_epoll_event.data.fd = event.GetKey();
    EpollPair ret(event.GetKey(), new_epoll_event);
    return ret;
  }

  EventKey GetKeyFromEpollEvent(const EpollEvent &event) override {
    return event.data.fd;
  }

private:
  EpollEventArray m_events_array;
};

#endif //NET_IOMULTIPLEXER_H

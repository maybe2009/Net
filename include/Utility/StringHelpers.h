//
// Created by DW on 2016/9/19.
//

#ifndef NET_STRINGHELPERS_H
#define NET_STRINGHELPERS_H

#include <memory>

template<typename T>
class UniquePtrArray {
  typedef std::unique_ptr<T> AutoPtr;
  typedef typename AutoPtr::pointer RawPtr;
public:
  UniquePtrArray()
      : m_array()
      , m_size(0)
  {}

  UniquePtrArray(size_t size)
      : m_array(new T[size])
      , m_size(size)
  {}

  size_t Fill(const char *data, size_t size) {
      if (size > m_size) {
        return 0;
      } else {
        memcpy(m_array.get(), data, size);
        return size;
      }
  }

  size_t Reset(AutoPtr&& array, size_t size) {
    m_array = array;
    m_size = size;
    return m_size;
  }

  size_t Relocate(size_t size) {
    m_array.reset(new T[size]);
    m_size = size;
    return m_size;
  }

  RawPtr Raw() {
    return m_array.get();
  }

  size_t Size() const {
    return m_size;
  }

private:
  AutoPtr m_array;
  size_t m_size;
};

#endif //NET_STRINGHELPERS_H

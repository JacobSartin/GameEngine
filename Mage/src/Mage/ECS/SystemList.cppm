module;

#include "Mage/Core/Api.h"

#include <cstddef>
#include <iterator>

export module Mage.ECS:SystemList;

export import :System;

export namespace Mage {

class SystemManager;

class MAGE_API SystemList {
  friend class SystemManager;

public:
  struct MAGE_API Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = System *;
    using pointer = System **;
    using reference = System *&;

    explicit Iterator(pointer ptr) : ptr(ptr) {}

    Iterator(Iterator &&) = default;
    Iterator(Iterator const &) = default;
    Iterator &operator=(Iterator &&) = default;
    Iterator &operator=(Iterator const &) = default;
    ~Iterator() = default;

    reference operator*() const { return *ptr; }
    pointer operator->() const { return ptr; }

    Iterator &operator++() {
      ptr++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.ptr == b.ptr;
    }

    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.ptr != b.ptr;
    }

  private:
    pointer ptr = nullptr;
  };

  struct MAGE_API ConstIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = System *;
    using pointer = System **;
    using reference = System *&;

    explicit ConstIterator(pointer ptr) : ptr(ptr) {}

    ConstIterator(ConstIterator &&) = default;
    ConstIterator(ConstIterator const &) = default;
    ConstIterator &operator=(ConstIterator &&) = default;
    ConstIterator &operator=(ConstIterator const &) = default;
    ~ConstIterator() = default;

    reference operator*() const { return *ptr; }
    pointer operator->() const { return ptr; }

    ConstIterator &operator++() {
      ptr++;
      return *this;
    }

    ConstIterator operator++(int) {
      ConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const ConstIterator &a, const ConstIterator &b) {
      return a.ptr == b.ptr;
    }

    friend bool operator!=(const ConstIterator &a, const ConstIterator &b) {
      return a.ptr != b.ptr;
    }

  private:
    pointer ptr = nullptr;
  };

  ~SystemList() = default;

  SystemList(SystemList &&) = delete;
  SystemList(SystemList const &) = delete;
  SystemList &operator=(SystemList &&) = delete;
  SystemList &operator=(SystemList const &) = delete;

  System *at(size_t pos) { return _list[pos]; }
  const System *at(size_t pos) const { return _list[pos]; }

  System *operator[](size_t pos) { return _list[pos]; }
  const System *operator[](size_t pos) const { return _list[pos]; }

  System *front() { return _list[0]; }
  const System *front() const { return _list[0]; }

  System *back() { return _list[_count - 1]; }
  const System *back() const { return _list[_count - 1]; }

  System **data() { return _list; }
  const System **data() const { return const_cast<const System **>(_list); }

  bool empty() const { return _count <= 0; }

  size_t size() const { return _count; }

  Iterator begin() {
    if (_count < 1) {
      return Iterator(nullptr);
    }
    return Iterator(&_list[0]);
  }

  Iterator end() {
    if (_count < 1) {
      return Iterator(nullptr);
    }
    return Iterator(&_list[_count]);
  }

  ConstIterator cbegin() const {
    if (_count < 1) {
      return ConstIterator(nullptr);
    }
    return ConstIterator(&_list[0]);
  }

  ConstIterator cend() const {
    if (_count < 1) {
      return ConstIterator(nullptr);
    }
    return ConstIterator(&_list[_count]);
  }

private:
  SystemList(System **list, size_t count) : _list(list), _count(count) {}

  System **_list;
  size_t _count;
};

} // namespace Mage

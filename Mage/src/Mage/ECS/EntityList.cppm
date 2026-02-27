module;

#include "Mage/Core/Api.h"

#include <cstddef>
#include <iterator>

export module Mage:ECS.EntityList;

export import :ECS.Entity;

export namespace Mage {

class MAGE_API EntityList {
public:
  struct MAGE_API Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Entity *;
    using pointer = Entity **;
    using reference = Entity *&;

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

    friend bool MAGE_API operator==(const Iterator &a, const Iterator &b) {
      return a.ptr == b.ptr;
    }

    friend bool MAGE_API operator!=(const Iterator &a, const Iterator &b) {
      return a.ptr != b.ptr;
    }

  private:
    pointer ptr = nullptr;
  };

  struct MAGE_API ConstIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Entity *;
    using pointer = Entity **;
    using reference = Entity *&;

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

    friend bool MAGE_API operator==(const ConstIterator &a,
                                    const ConstIterator &b) {
      return a.ptr == b.ptr;
    }

    friend bool MAGE_API operator!=(const ConstIterator &a,
                                    const ConstIterator &b) {
      return a.ptr != b.ptr;
    }

  private:
    pointer ptr = nullptr;
  };

  EntityList(Entity **list, size_t count) : _list(list), _count(count) {}

  ~EntityList() = default;

  EntityList(EntityList &&) = delete;
  EntityList(EntityList const &) = delete;
  EntityList &operator=(EntityList &&) = delete;
  EntityList &operator=(EntityList const &) = delete;

  Entity *at(size_t pos) { return _list[pos]; }
  const Entity *at(size_t pos) const { return _list[pos]; }

  Entity *operator[](size_t pos) { return _list[pos]; }
  const Entity *operator[](size_t pos) const { return _list[pos]; }

  Entity *front() { return _list[0]; }
  const Entity *front() const { return _list[0]; }

  Entity *back() { return _list[_count - 1]; }
  const Entity *back() const { return _list[_count - 1]; }

  Entity **data() { return _list; }
  const Entity **data() const { return const_cast<const Entity **>(_list); }

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
  Entity **_list;
  size_t _count;
};

} // namespace Mage

#pragma once

#include "../Core/Core.h"
#include "EntityList.h"
#include "Mage/Core/Exception.h"
#include "Mage/Core/ICopyDisable.h"
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <unordered_map>


namespace Mage {
class MAGE_API ComponentVectorBase : ICopyDisable {
public:
  ComponentVectorBase() = default;
  virtual ~ComponentVectorBase() = default;

  virtual void entity_destroyed(Entity &entity) = 0;

  virtual EntityList get_entities() = 0;
};

template <typename T> class ComponentVector final : public ComponentVectorBase {
public:
  ComponentVector() = default;
  ~ComponentVector() override = default;

  T *get(uint64_t entity_id) {
    auto it = _entity_to_index_map.find(entity_id);
    if (it == _entity_to_index_map.end()) {
      LOG_E_WARN(
          "Component retrieval for entity which does not contain it; eid: %d",
          entity_id);
      return nullptr;
    }

    return &_components.at(it->second);
  }

  void add(Entity &entity, T &component) {
    auto eid = entity.get_id();

    // already exists
    if (_entity_to_index_map.contains(eid)) {
      std::stringstream ss;
      ss << "Component addition to entity which already contained it; eid: "
         << eid;
      throw Exception(ss.str().c_str());
    }

    _entities.push_back(&entity);
    _components.push_back(component);

    _entity_to_index_map.insert_or_assign(eid, _components.size() - 1);
    _index_to_entity_map.insert_or_assign(_components.size() - 1, eid);
  }

  void remove(const Entity &entity) {
    auto eid = entity.get_id();
    // not found
    if (!_entity_to_index_map.contains(eid)) {
      return;
    }

    // move last element to remove elements place to keep removal constant time
    uint64_t idx_of_removed = _entity_to_index_map[eid];
    uint64_t idx_of_last = _components.size() - 1;
    _components[idx_of_removed] = _components[idx_of_last];

    // now idx_of_removed we have the last component, update its entity mapping
    auto entity_id_of_last = _index_to_entity_map[idx_of_last];
    _entity_to_index_map[entity_id_of_last] = idx_of_removed;
    _index_to_entity_map[idx_of_removed] = entity_id_of_last;

    // remove now incorrect map entries
    _entity_to_index_map.erase(eid);
    _index_to_entity_map.erase(idx_of_last);

    // remove last element
    _components.erase(_components.begin() + idx_of_last);

    // this is O(n), can be made constant time with different data structure
    _entities.erase(std::remove(_entities.begin(), _entities.end(), &entity),
                    _entities.end());
  }

  void entity_destroyed(Entity &entity) override { remove(entity); }

  EntityList get_entities() override {
    return {_entities.data(), _entities.size()};
  }

private:
  // There are ok because this is a template class
  // so will be compiled when the game is compiled, so will use their std
  // library
  std::vector<T> _components;
  std::vector<Entity *> _entities;
  std::unordered_map<uint64_t, uint64_t> _entity_to_index_map;
  std::unordered_map<uint64_t, uint64_t> _index_to_entity_map;
};
} // namespace Mage

#include "EntityManager.h"
#include "ComponentManager.h"
#include "Entity.h"
#include "EntityList.h"
#include "SystemManager.h"
#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <vcruntime_typeinfo.h>

namespace Mage {
struct EntityManager::Impl {
  std::mutex sync_object;
  ComponentManager *component_manager;
  SystemManager *system_manager;
  std::vector<std::unique_ptr<Entity>> all_entities;
  std::vector<std::unique_ptr<Entity>> pending_entity_additions;
  std::vector<Entity *> all_entity_ptrs;
  std::map<uint_fast32_t, std::vector<Entity *>> entity_type_map;
  uint64_t last_entity_id = 0;
};

EntityManager::EntityManager() : _impl(new Impl()) {

  LOG_E_INFO("EntityManager Starting");
}

EntityManager::~EntityManager() {
  delete _impl;
  LOG_E_INFO("EntityManager Stopping");
}

void EntityManager::update() {
  LOG_E_DEBUG("EntityManager update called");

  // move pending entities into use
  for (auto &entity : _impl->pending_entity_additions) {
    Entity *entity_ptr = entity.get();
    _impl->all_entities.push_back(std::move(entity));
    _impl->all_entity_ptrs.push_back(entity_ptr);
    _impl->entity_type_map[entity_ptr->get_type()].push_back(entity_ptr);
  }
  _impl->pending_entity_additions.clear();

  // remove destroyed entities
  auto new_end = std::remove_if(
      _impl->all_entity_ptrs.begin(), _impl->all_entity_ptrs.end(),
      [](const Entity *entity_ptr) { return entity_ptr->is_destroyed(); });
  _impl->all_entity_ptrs.erase(new_end, _impl->all_entity_ptrs.end());

  for (auto &t : _impl->entity_type_map) {
    auto &entity_list = t.second;
    auto new_end = std::remove_if(
        entity_list.begin(), entity_list.end(),
        [](const Entity *entity_ptr) { return entity_ptr->is_destroyed(); });
    entity_list.erase(new_end, entity_list.end());
  }

  for (auto &e : _impl->all_entities) {
    if (e->is_destroyed()) {
      _impl->system_manager->entity_destroyed(*e);
      _impl->component_manager->entity_destroyed(*e);
    }
  }

  auto new_end_all =
      std::remove_if(_impl->all_entities.begin(), _impl->all_entities.end(),
                     [](const std::unique_ptr<Entity> &entity_ptr) {
                       return entity_ptr->is_destroyed();
                     });
  _impl->all_entities.erase(new_end_all, _impl->all_entities.end());
}

Entity *EntityManager::add_entity(uint_fast32_t type) {
  // lock for thread safety, caller may be in another thread
  std::lock_guard<std::mutex> lock(_impl->sync_object);

  uint64_t new_id = ++_impl->last_entity_id;
  // can't use make_unique bc constructor is private, only available to friend
  auto new_entity = std::unique_ptr<Entity>(new Entity(new_id, type));
  Entity *new_entity_ptr = new_entity.get();
  _impl->pending_entity_additions.push_back(std::move(new_entity));
  LOG_E_DEBUG("Entity added to EntityManager; id: %d; type: %d", new_id, type);

  return new_entity_ptr;
}

EntityList EntityManager::get_all_entities() const {
  return EntityList(_impl->all_entity_ptrs.data(),
                    _impl->all_entity_ptrs.size());
}

EntityList EntityManager::get_entities_by_type(uint_fast32_t type) const {
  return EntityList(_impl->entity_type_map.at(type).data(),
                    _impl->entity_type_map.at(type).size());
}

void EntityManager::set_component_manager(ComponentManager &component_manager) {
  _impl->component_manager = &component_manager;
}

void EntityManager::set_system_manager(SystemManager &system_manager) {
  _impl->system_manager = &system_manager;
}
} // namespace Mage

module;

#include "Mage/Core/Log.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <vector>

module Mage.ECS:ComponentManager.impl;

import :EntityList;
import :ComponentManager;
import :SystemManager;

namespace Mage {

struct ComponentManager::Impl {
  std::mutex sync_object;
  SystemManager *system_manager = nullptr;
  std::unordered_map<size_t, uint_fast32_t> component_type_to_id_map;
  std::unordered_map<uint_fast32_t, ComponentVectorBase *> component_vectors;
  uint_fast32_t last_component_id = 0;
};

ComponentManager::ComponentManager() : _impl(new Impl()) {
  LOG_E_INFO("ComponentManager Starting");
}

ComponentManager::~ComponentManager() {
  for (auto &pair : _impl->component_vectors) {
    delete pair.second;
  }

  delete _impl;
  LOG_E_INFO("ComponentManager Stopping");
}

void ComponentManager::set_system_manager(SystemManager &system_manager) {
  _impl->system_manager = &system_manager;
}

void ComponentManager::register_component_internal(
    size_t type_idx, ComponentVectorBase *component_vector) {
  std::lock_guard<std::mutex> lock(_impl->sync_object);

  uint_fast32_t component_id = ++_impl->last_component_id;
  _impl->component_type_to_id_map.insert(std::make_pair(type_idx, component_id));
  _impl->component_vectors.insert(std::make_pair(component_id, component_vector));

  LOG_E_INFO("Registered component with ID %u", component_id);
}

ComponentVectorBase *
ComponentManager::get_component_vector(uint_fast32_t component_id) const {
  return _impl->component_vectors[component_id];
}

bool ComponentManager::get_component_id_internal(
    size_t type_idx, uint_fast32_t &component_id) const {
  auto it = _impl->component_type_to_id_map.find(type_idx);
  if (it == _impl->component_type_to_id_map.end()) {
    return false;
  }

  component_id = it->second;
  return true;
}

void ComponentManager::notify_systems_of_entity_component_addition(
    Entity &entity, uint_fast32_t component_id) const {
  std::vector<uint_fast32_t> entity_component_ids;
  entity_component_ids.push_back(component_id);

  for (const auto &pair : _impl->component_vectors) {
    auto entities = pair.second->get_entities();
    if (std::find(entities.begin(), entities.end(), &entity) != entities.end()) {
      entity_component_ids.push_back(pair.first);
    }
  }

  _impl->system_manager->notify_systems_of_entity_component_addition(
      entity, entity_component_ids.data(), entity_component_ids.size());
}

void ComponentManager::notify_systems_of_entity_component_removal(
    Entity &entity, uint_fast32_t component_id) const {
  _impl->system_manager->notify_systems_of_entity_component_removal(entity,
                                                                    component_id);
}

void ComponentManager::entity_destroyed(Entity &entity) {
  for (auto pair : _impl->component_vectors) {
    pair.second->entity_destroyed(entity);
  }
}

} // namespace Mage

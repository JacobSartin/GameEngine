module;

#include "Mage/Core/Log.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <map>
#include <mutex>
#include <utility>
#include <vector>

module Mage.ECS:SystemManager.impl;

import :EntityList;
import :ComponentManager;
import :System;
import :SystemList;
import :SystemManager;

namespace Mage {

struct SystemManager::Impl {
  std::mutex sync_object;
  ComponentManager *component_manager = nullptr;
  std::map<size_t, uint_fast32_t> system_type_to_id_map;
  std::map<uint_fast32_t, std::vector<uint_fast32_t>> system_to_component_map;
  std::map<uint_fast32_t, System *> systems_map;
  std::vector<System *> systems;
  uint_fast32_t last_system_id = 0;
};

SystemManager::SystemManager() : _impl(new Impl()) {
  LOG_E_INFO("SystemManager Starting");
}

SystemManager::~SystemManager() {
  delete _impl;
  LOG_E_INFO("SystemManager Stopping");
}

SystemList SystemManager::get_all_systems() const {
  return SystemList(_impl->systems.data(), _impl->systems.size());
}

uint_fast32_t SystemManager::get_system_id(size_t type_idx) const {
  return _impl->system_type_to_id_map.at(type_idx);
}

ComponentManager &SystemManager::get_component_manager() const {
  return *_impl->component_manager;
}

void SystemManager::set_component_manager(ComponentManager &component_manager) {
  _impl->component_manager = &component_manager;
}

bool SystemManager::is_system_registered(const System &system) const {
  return std::find(_impl->systems.begin(), _impl->systems.end(), &system) !=
         _impl->systems.end();
}

void SystemManager::register_system_internal(System &system, size_t type_idx) {
  std::lock_guard<std::mutex> lock(_impl->sync_object);

  uint_fast32_t system_id = ++_impl->last_system_id;
  _impl->systems_map[system_id] = &system;
  _impl->systems.push_back(&system);

  _impl->system_type_to_id_map.insert(std::make_pair(type_idx, system_id));
}

void SystemManager::add_system_component(uint_fast32_t system_id,
                                         uint_fast32_t component_id) {
  _impl->system_to_component_map[system_id].push_back(component_id);
}

void SystemManager::notify_systems_of_entity_component_addition(
    Entity &entity, const uint_fast32_t *component_ids, size_t component_count) {
  std::lock_guard<std::mutex> lock(_impl->sync_object);

  std::vector entity_components(component_ids, component_ids + component_count);

  for (const auto &s : _impl->system_to_component_map) {
    uint_fast32_t system_id = s.first;
    const auto &system_component_ids = s.second;

    bool cares = std::all_of(
        system_component_ids.begin(), system_component_ids.end(),
        [&](uint_fast32_t cid) {
          return std::find(entity_components.begin(), entity_components.end(),
                           cid) != entity_components.end();
        });

    if (cares) {
      auto *system = _impl->systems_map[system_id];
      if (!system->has_entity(entity)) {
        system->add_entity(entity);
      }
    }
  }
}

void SystemManager::notify_systems_of_entity_component_removal(
    Entity &entity, const uint_fast32_t component_id) {
  std::lock_guard<std::mutex> lock(_impl->sync_object);

  for (const auto &s : _impl->system_to_component_map) {
    uint_fast32_t system_id = s.first;
    const auto &system_component_ids = s.second;

    if (std::find(system_component_ids.begin(), system_component_ids.end(),
                  component_id) != system_component_ids.end()) {
      auto *system = _impl->systems_map[system_id];
      if (system->has_entity(entity)) {
        system->remove_entity(entity);
      }
    }
  }
}

void SystemManager::entity_destroyed(const Entity &entity) {
  std::lock_guard<std::mutex> lock(_impl->sync_object);

  for (auto *system : _impl->systems) {
    if (system->has_entity(entity)) {
      system->remove_entity(entity);
    }
  }
}

} // namespace Mage

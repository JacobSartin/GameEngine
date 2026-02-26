module;

#include "Mage/Core/Log.h"

#include <algorithm>
#include <vector>

module Mage.ECS:System.impl;

import :EntityList;
import :ComponentManager;
import :System;

namespace Mage {

struct System::Impl {
  std::vector<Entity *> entities;
};

System::System() : _impl(new Impl()) { LOG_E_DEBUG("System Created"); }

System::~System() {
  delete _impl;
  LOG_E_DEBUG("System Destroyed");
}

EntityList System::get_entities() const {
  return EntityList(_impl->entities.data(), _impl->entities.size());
}

bool System::has_entity(const Entity &entity) const {
  return std::find(_impl->entities.begin(), _impl->entities.end(), &entity) !=
         _impl->entities.end();
}

void System::add_entity(Entity &entity) {
  if (std::find(_impl->entities.begin(), _impl->entities.end(), &entity) ==
      _impl->entities.end()) {
    _impl->entities.push_back(&entity);
  } else {
    LOG_E_WARN(
        "Attempted to add entity that already exists in system; id: %d; ",
        entity.get_id());
  }
}

void System::remove_entity(const Entity &entity) {
  if (auto it =
          std::find(_impl->entities.begin(), _impl->entities.end(), &entity);
      it != _impl->entities.end()) {
    _impl->entities.erase(it);
  } else {
    LOG_E_WARN(
        "Attempted to remove entity that does not exist in system; id: %d; ",
        entity.get_id());
  }
}

} // namespace Mage

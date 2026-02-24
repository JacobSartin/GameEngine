#include "System.h"
#include <algorithm>
/* TODO be smart about notifying systems of entity components and dont notify
 systems of components they dont care about. Maybe have a bitset of component
 types that each system cares about, and only notify systems if the component
type matches one in the bitset?
*/
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
  // only add if not already present
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

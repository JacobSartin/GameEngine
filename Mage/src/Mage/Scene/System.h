#pragma once

#include "../Core/Core.h"
#include "EntityList.h"
#include "Mage/Core/ICopyDisable.h"

namespace Mage {

class ComponentManager;

class MAGE_API System : ICopyDisable {
  friend class SystemManager;

public:
  System();

  virtual ~System();

  // don't define default behavior
  virtual void update(ComponentManager &component_manager,
                      float delta_time) = 0;

  EntityList get_entities() const;

private:
  struct Impl;
  Impl *_impl;

  bool has_entity(const Entity &entity) const;

  void add_entity(Entity &entity);

  void remove_entity(const Entity &entity);
};
} // namespace Mage
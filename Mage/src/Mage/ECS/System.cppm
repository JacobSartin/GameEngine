module;

#include "Mage/Core/Api.h"
#include "Mage/Core/ICopyDisable.h"

export module Mage.ECS:System;

export import :EntityList;
export import :ComponentManager;

export namespace Mage {

class SystemManager;

class MAGE_API System : ICopyDisable {
  friend class SystemManager;

public:
  System();
  virtual ~System();

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
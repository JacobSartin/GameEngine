module;

#include "Mage/Core/Api.h"
#include "Mage/Core/ICopyDisable.h"

#include <cstdint>

export module Mage.ECS:EntityManager;

export import :EntityList;
export import :ComponentManager;
export import :SystemManager;

namespace Mage {
extern "C++" class Application; 
}

export namespace Mage {

class MAGE_API EntityManager : ICopyDisable {
  friend class Application;

public:
  ~EntityManager();

  void update();

  Entity *add_entity(uint_fast32_t type);

  EntityList get_all_entities() const;

  EntityList get_entities_by_type(uint_fast32_t type) const;

private:
  struct Impl;
  Impl *_impl;

  EntityManager();

  void set_component_manager(ComponentManager &component_manager);
  void set_system_manager(SystemManager &system_manager);
};

} // namespace Mage
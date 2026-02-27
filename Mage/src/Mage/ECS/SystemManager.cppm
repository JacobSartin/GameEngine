module;

#include "Mage/Core/Api.h"
#include "Mage/Core/Exception.h"
#include "Mage/Core/ICopyDisable.h"
#include "Mage/Core/Log.h"

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <typeindex>

export module Mage.ECS:SystemManager;

export import :SystemList;
export import :ComponentManager;

namespace Mage {
extern "C++" class Application; 
}

export namespace Mage {

class EntityManager;

class MAGE_API SystemManager : ICopyDisable {
  friend class Application;
  friend class ComponentManager;
  friend class EntityManager;

public:
  ~SystemManager();

  template <typename... Ts> void register_system(System &system) {
    LOG_E_INFO("System '%s' registered", typeid(system).name());
    if (is_system_registered(system)) {
      LOG_E_ERROR("System '%s' is already registered", typeid(system).name());
      throw Exception("Attempting to register a system that is already "
                      "registered is not allowed");
    }

    auto type_idx = std::type_index(typeid(system)).hash_code();

    register_system_internal(system, type_idx);

    auto system_id = get_system_id(type_idx);

    (void)std::initializer_list<int>{
        (add_system_component(system_id,
                              get_component_manager().get_component_id<Ts>()),
         0)...};
  }

private:
  struct Impl;
  Impl *_impl;

  SystemManager();

  ComponentManager &get_component_manager() const;
  bool is_system_registered(const System &system) const;
  void register_system_internal(System &system, size_t type_idx);
  void add_system_component(uint_fast32_t system_id,
                            uint_fast32_t component_id);

  void entity_destroyed(const Entity &entity);
  void set_component_manager(ComponentManager &component_manager);

  SystemList get_all_systems() const;
  uint_fast32_t get_system_id(size_t type_idx) const;

  void notify_systems_of_entity_component_addition(
      Entity &entity, const uint_fast32_t *component_ids,
      size_t component_count);

  void notify_systems_of_entity_component_removal(Entity &entity,
                                                  uint_fast32_t component_id);
};

} // namespace Mage

module;

#include "Mage/Core/Api.h"
#include "Mage/Core/Exception.h"
#include "Mage/Core/ICopyDisable.h"
#include "Mage/Core/Log.h"

#include <cstddef>
#include <cstdint>
#include <typeindex>

export module Mage.ECS:ComponentManager;

export import :ComponentVector;

export namespace Mage {

class Application;
class EntityManager;
class SystemManager;

class MAGE_API ComponentManager : ICopyDisable {
  friend class Application;
  friend class EntityManager;
  friend class SystemManager;

public:
  ~ComponentManager();

  template <typename T> void register_component() {
    LOG_E_INFO("Component '%s' registered", typeid(T).name());
    auto component_type_index = std::type_index(typeid(T)).hash_code();

    uint_fast32_t component_id;
    if (get_component_id_internal(component_type_index, component_id)) {
      throw Exception("Component is already registered, cannot register again");
      return;
    }

    register_component_internal(component_type_index, new ComponentVector<T>());
  }

  template <typename T> T *get_component(Entity &entity) {
    uint_fast32_t component_id;
    if (!get_component_id_internal(std::type_index(typeid(T)).hash_code(),
                                   component_id)) {
      throw Exception("Component is not registered, cannot get component");
    }
    return static_cast<ComponentVector<T> *>(get_component_vector(component_id))
        ->get(entity.get_id());
  }

  template <typename T> void add_component(Entity &entity, T component) {
    LOG_E_DEBUG("Component %s added to entity %d", typeid(T).name(),
                entity.get_id());

    uint_fast32_t component_id;
    if (!get_component_id_internal(std::type_index(typeid(T)).hash_code(),
                                   component_id)) {
      throw Exception("Component is not registered, cannot add component");
    }

    static_cast<ComponentVector<T> *>(get_component_vector(component_id))
        ->add(entity, component);
    notify_systems_of_entity_component_addition(entity, component_id);
  }

  template <typename T> void remove_component(Entity &entity) {
    LOG_E_DEBUG("Component %s removed from entity %d", typeid(T).name(),
                entity.get_id());

    uint_fast32_t component_id;
    if (!get_component_id_internal(std::type_index(typeid(T)).hash_code(),
                                   component_id)) {
      throw Exception("Component is not registered, cannot remove component");
    }

    static_cast<ComponentVector<T> *>(get_component_vector(component_id))
        ->remove(entity);
    notify_systems_of_entity_component_removal(entity, component_id);
  }

  template <typename T> uint_fast32_t get_component_id() const {
    uint_fast32_t component_id;
    if (!get_component_id_internal(std::type_index(typeid(T)).hash_code(),
                                   component_id)) {
      throw Exception("Component is not registered, cannot get ID");
    }
    return component_id;
  }

private:
  struct Impl;
  Impl *_impl;

  ComponentManager();

  void set_system_manager(SystemManager &system_manager);

  void register_component_internal(size_t type_idx,
                                   ComponentVectorBase *component_vector);

  bool get_component_id_internal(size_t type_idx,
                                 uint_fast32_t &out_component_id) const;

  ComponentVectorBase *get_component_vector(uint_fast32_t component_id) const;

  void
  notify_systems_of_entity_component_addition(Entity &entity,
                                              uint_fast32_t component_id) const;

  void
  notify_systems_of_entity_component_removal(Entity &entity,
                                             uint_fast32_t component_id) const;

  void entity_destroyed(Entity &entity);
};

} // namespace Mage

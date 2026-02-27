module;

#include "Mage/Core/Api.h"
#include "Mage/Core/ICopyDisable.h"
#include "Mage/Core/Log.h"

#include <cstdint>

export module Mage:ECS.Entity;

export namespace Mage {

class EntityManager;

class MAGE_API Entity : ICopyDisable {
  friend class EntityManager;

public:
  ~Entity() { LOG_E_DEBUG("Entity Destroyed; id: %d; type: %d", _id, _type); };

  uint64_t get_id() const { return _id; }
  uint_fast32_t get_type() const { return _type; }
  bool is_destroyed() const { return _destroyed; }

  void destroy() { _destroyed = true; }

private:
  Entity(uint64_t id, uint_fast32_t type)
      : _id(id), _type(type), _destroyed(false) {
    LOG_E_DEBUG("Entity Created; id: %d; type: %d", _id, _type);
  };

  const uint64_t _id;
  const uint_fast32_t _type;
  bool _destroyed;
};

} // namespace Mage

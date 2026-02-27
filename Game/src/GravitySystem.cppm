module;

#include <glm/glm.hpp>

export module Game.GravitySystem;

import Game.Components;
import Mage;

export class GravitySystem : public Mage::System {
public:
  void update(Mage::ComponentManager &component_manager,
              float delta_time) override {
    for (auto &entity : get_entities()) {
      if (entity->is_destroyed()) {
        continue;
      }

      auto *rigid_body =
          component_manager.get_component<RigidBody2DComponent>(*entity);
      auto *gravity = component_manager.get_component<GravityComponent>(*entity);

      rigid_body->velocity += gravity->force * delta_time;
    }
  }
};
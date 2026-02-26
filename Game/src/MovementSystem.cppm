module;

#include <glm/glm.hpp>

export module Game.MovementSystem;

import Game.Components;
import Mage.ECS;

export class MovementSystem : public Mage::System {
public:
  void update(Mage::ComponentManager &component_manager,
              float delta_time) override {
    for (auto &entity : get_entities()) {
      if (entity->is_destroyed()) {
        continue;
      }

      auto *r = component_manager.get_component<RigidBody2DComponent>(*entity);

      auto *t = component_manager.get_component<Transform2DComponent>(*entity);

      t->prev_translation = t->translation;
      t->translation += r->velocity * delta_time;
    }
  }
};
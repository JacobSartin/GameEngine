#pragma once

#include "Components.h"
#include "Mage/Scene/ComponentManager.h"
#include <Mage/Mage.h>

class SpriteRenderingSystem : public Mage::System {
public:
  explicit SpriteRenderingSystem(Mage::SpriteRenderer *sprite_renderer)
      : _sprite_renderer(sprite_renderer) {}

  void update(Mage::ComponentManager &component_manager,
              float delta_time) override {
    for (auto &entity : get_entities()) {
      if (entity->is_destroyed()) {
        continue;
      }

      auto *s = component_manager.get_component<SpriteComponent>(*entity);

      auto *t = component_manager.get_component<Transform2DComponent>(*entity);

      _sprite_renderer->render(*s->sprite, t->translation, t->scale,
                               t->rotation, delta_time);
    }
  }

private:
  Mage::SpriteRenderer *_sprite_renderer;
};

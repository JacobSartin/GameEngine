module;

#include <functional>
#include <utility>

export module Game.SpriteRenderingSystem;

import Game.Components;
import Mage.ECS;

export class SpriteRenderingSystem : public Mage::System {
public:
  using RenderSpriteFn =
      std::function<void(const SpriteComponent &, const Transform2DComponent &,
                         float)>;

  explicit SpriteRenderingSystem(RenderSpriteFn render_sprite)
      : _render_sprite(std::move(render_sprite)) {}

  void update(Mage::ComponentManager &component_manager,
              float delta_time) override {
    for (auto &entity : get_entities()) {
      if (entity->is_destroyed()) {
        continue;
      }

      auto *s = component_manager.get_component<SpriteComponent>(*entity);

      auto *t = component_manager.get_component<Transform2DComponent>(*entity);

      _render_sprite(*s, *t, delta_time);
    }
  }

private:
  RenderSpriteFn _render_sprite;
};
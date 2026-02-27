export module Game.LifetimeSystem;

import Game.Components;
import Mage;

export class LifetimeSystem : public Mage::System {
public:
  void update(Mage::ComponentManager &component_manager,
              float delta_time) override {
    for (auto &entity : get_entities()) {
      if (entity->is_destroyed()) {
        continue;
      }

      auto *l = component_manager.get_component<LifetimeComponent>(*entity);

      l->remaining -= delta_time;

      if (l->remaining <= 0.0f) {
        l->on_expire();
        entity->destroy();
      }
    }
  }
};
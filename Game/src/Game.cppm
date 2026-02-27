module;

#include <memory>
#include <vector>
#pragma warning(disable : 4100)
#include <Mage/Mage.h>

export module Game;

import Mage;
import Game.GravitySystem;
import Game.LifetimeSystem;
import Game.MovementSystem;
import Game.SpriteRenderingSystem;

export class Game final : public Mage::Application,
                          Mage::OnAppClosingEventListener {
public:
  Game();
  ~Game() override;

  void on_app_closing() override;

private:
  std::vector<std::unique_ptr<Mage::Sprite>> _sprites;

  std::unique_ptr<SpriteRenderingSystem> _sprite_rendering_system = nullptr;
  std::unique_ptr<MovementSystem> _movement_system = nullptr;
  std::unique_ptr<GravitySystem> _gravity_system = nullptr;
  std::unique_ptr<LifetimeSystem> _lifetime_system = nullptr;
};
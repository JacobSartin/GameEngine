#pragma once

#include "Components.h"
#include <memory>
#pragma warning(disable : 4100)

#include "CollisionSystem.h"
#include "DeathByYSystem.h"
#include "DestructionNotificationSystem.h"
#include "EnemySpawningSystem.h"
#include "GravitySystem.h"
#include "LifetimeSystem.h"
#include "MovementSystem.h"
#include "PlayerSystem.h"
#include "SpriteRenderingSystem.h"
#include <Mage/Mage.h>
#include <random>

class Game final : public Mage::Application {
public:
  Game();
  ~Game() override = default;

private:
  std::vector<std::unique_ptr<Mage::Sprite>> _sprites;

  std::unique_ptr<SpriteRenderingSystem> _sprite_rendering_system = nullptr;
  std::unique_ptr<MovementSystem> _movement_system = nullptr;
  std::unique_ptr<GravitySystem> _gravity_system = nullptr;
  std::unique_ptr<LifetimeSystem> _lifetime_system = nullptr;
};
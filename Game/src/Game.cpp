#include "Game.h"

import Game.Components;
import Game.GravitySystem;
import Game.LifetimeSystem;
import Game.MovementSystem;
import Game.Rand;
import Game.SpriteRenderingSystem;

Game::Game() : Mage::Application("Basic Game", false, 1024, 768, 1) {
  Mage::Log::get().set_engine_log_level(Mage::Log::Level::Info);

  get_component_manager()->register_component<Transform2DComponent>();
  get_component_manager()->register_component<RigidBody2DComponent>();
  get_component_manager()->register_component<GravityComponent>();
  get_component_manager()->register_component<SpriteComponent>();
  get_component_manager()->register_component<LifetimeComponent>();

  _sprite_rendering_system =
      std::make_unique<SpriteRenderingSystem>([sprite_renderer = get_sprite_renderer()](
          const SpriteComponent &sprite_component,
          const Transform2DComponent &transform, float elapsed_time) {
        sprite_renderer->render(*sprite_component.sprite, transform.translation,
                                transform.scale, transform.rotation,
                                elapsed_time);
      });
  _gravity_system = std::make_unique<GravitySystem>();
  _movement_system = std::make_unique<MovementSystem>();
  _lifetime_system = std::make_unique<LifetimeSystem>();

  get_system_manager()->register_system<Transform2DComponent, SpriteComponent>(
      *_sprite_rendering_system);
  get_system_manager()
      ->register_system<Transform2DComponent, RigidBody2DComponent>(
          *_movement_system);
  get_system_manager()->register_system<GravityComponent, RigidBody2DComponent>(
      *_gravity_system);
  get_system_manager()->register_system<LifetimeComponent>(*_lifetime_system);

  Rand::add_uniform_real_distribution("rotation", -90.0f, 90.0f);
  Rand::add_uniform_real_distribution("scale_x", 10.0f, 50.0f);
  Rand::add_uniform_real_distribution("scale_y", 1.0f, 10.0f);
  Rand::add_uniform_real_distribution("color", 0.5f, 1.0f);
  Rand::add_uniform_real_distribution("lifetime", 2.0f, 30.0f);
  Rand::add_uniform_real_distribution("gravity", 5.0f, 50.0f);
  Rand::add_uniform_real_distribution("pos_x", 0.0f, get_camera()->right);
  Rand::add_uniform_real_distribution("pos_y", get_camera()->top,
                                      get_camera()->top * 2.0f);
  for (auto i = 0; i < 5000; ++i) {
    auto *entity = get_entity_manager()->add_entity(1);

    _sprites.push_back(std::make_unique<Mage::Sprite>(Mage::Color::custom(
        Rand::get_uniform_real("color"), Rand::get_uniform_real("color"),
        Rand::get_uniform_real("color"), Rand::get_uniform_real("color"))));

    get_component_manager()->add_component<SpriteComponent>(
        *entity, SpriteComponent{.sprite = _sprites.back().get()});

    get_component_manager()->add_component<LifetimeComponent>(
        *entity,
        LifetimeComponent{.remaining = Rand::get_uniform_real("lifetime")});

    get_component_manager()->add_component<Transform2DComponent>(
        *entity, Transform2DComponent{
                     .translation = {Rand::get_uniform_real("pos_x"),
                                     Rand::get_uniform_real("pos_y")},
                     .scale = {Rand::get_uniform_real("scale_x"),
                               Rand::get_uniform_real("scale_y")},
                     .rotation = Rand::get_uniform_real("rotation"),
                 });

    get_component_manager()->add_component<RigidBody2DComponent>(
        *entity, RigidBody2DComponent{
                     .velocity = {0.0f, 0.0f},
                 });

    get_component_manager()->add_component<GravityComponent>(
        *entity, GravityComponent{
                     .force = {0.0f, Rand::get_uniform_real("gravity")},
                 });
  }
}

Game::~Game() = default;
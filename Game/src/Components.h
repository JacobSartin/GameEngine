#pragma once

#include "Mage/Renderer/Sprite.h"
#include <Mage/Mage.h>

struct Transform2DComponent {
  glm::vec2 translation = {0.0f, 0.0f};
  glm::vec2 scale = {1.0f, 1.0f};
  float rotation = 0.0f;
  glm::vec2 prev_translation = {0.0f, 0.0f};
};

// something that can move
struct RigidBody2DComponent {
  glm::vec2 velocity = {0.0f, 0.0f};
  glm::vec2 prev_velocity = {0.0f, 0.0f};
};

struct GravityComponent {
  glm::vec2 force = {0.0f, 0.0f};
};

struct SpriteComponent {
  Mage::Sprite *sprite;
};

struct LifetimeComponent {
  float remaining = 0.0f;
  std::function<void()> on_expire = []() {}; // default to no-op
};
#pragma once

#include "Core.h"
#include <cstdint>

import Mage.ECS;

namespace Mage {

class Window;
class Camera;
class EventManager;
class TextRenderer;
class SpriteRenderer;

class MAGE_API Application : ICopyDisable {
public:
  explicit Application(const char *title, bool fullscreen = true,
                       int_fast32_t w = 0, uint_fast32_t h = 0,
                       int_fast8_t swap_interval = 0);

  virtual ~Application();

  void run();

  Window *get_window() const;
  Camera *get_camera() const;
  EventManager *get_event_manager() const;
  EntityManager *get_entity_manager() const;
  ComponentManager *get_component_manager() const;
  SystemManager *get_system_manager() const;
  TextRenderer *get_text_renderer() const;
  SpriteRenderer *get_sprite_renderer() const;

  void close();

private:
  struct Impl;
  Impl *_impl;
};

} // namespace Mage

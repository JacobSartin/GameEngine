#include "Application.h"
#include "../Renderer/Camera.h"
#include "../Renderer/SpriteRenderer.h"
#include "../Renderer/TextRenderer.h"
#include "Log.h"
#include "Mage/Core/Window.h"
#include "Mage/Renderer/TextRenderer.h"
#include <SDL_events.h>
#include <chrono>
#include <memory>

import Mage.ECS;

namespace Mage {

struct Application::Impl final {
  std::unique_ptr<Window> window;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<EventManager> eventManager;
  std::unique_ptr<EntityManager> entityManager;
  std::unique_ptr<ComponentManager> componentManager;
  std::unique_ptr<SystemManager> systemManager;
  std::unique_ptr<TextRenderer> textRenderer;
  std::unique_ptr<SpriteRenderer> spriteRenderer;

  bool closing = false;

  Impl(const char *title, bool fullscreen, int_fast32_t w, uint_fast32_t h,
       int_fast8_t swap_interval)
      : window(std::unique_ptr<Window>(
            new Window(title, fullscreen, w, h, swap_interval))),
        camera(std::unique_ptr<Camera>(new Camera())),
        eventManager(std::unique_ptr<EventManager>(new EventManager())),
        entityManager(std::unique_ptr<EntityManager>(new EntityManager())),
        componentManager(
            std::unique_ptr<ComponentManager>(new ComponentManager())),
        systemManager(std::unique_ptr<SystemManager>(new SystemManager())),
        textRenderer(std::unique_ptr<TextRenderer>(new TextRenderer())),
        spriteRenderer(
            std::unique_ptr<SpriteRenderer>(new SpriteRenderer(*camera))) {

    entityManager->set_component_manager(*componentManager);
    entityManager->set_system_manager(*systemManager);

    componentManager->set_system_manager(*systemManager);
    systemManager->set_component_manager(*componentManager);

    camera->left = 0.0f;
    camera->right = static_cast<float>(window->get_width());
    camera->top = 0.0f;
    camera->bottom = static_cast<float>(window->get_height());
  }
};

struct ElapsedTime {
  std::chrono::time_point<std::chrono::system_clock> start =
      std::chrono::system_clock::now();

  std::chrono::time_point<std::chrono::system_clock> end =
      std::chrono::system_clock::now();

  float elapsed = 0.0f;

  void update() {
    end = std::chrono::system_clock::now();
    elapsed =
        std::chrono::duration<float>(end - start).count(); // seconds as float
    start = end;
  }
};

Application::Application(const char *title, bool fullscreen, int_fast32_t w,
                         uint_fast32_t h, int_fast8_t swap_interval)
    : _impl(new Impl(title, fullscreen, w, h, swap_interval)) {
  LOG_E_INFO("Application Starting");
}
Application::~Application() {
  LOG_E_INFO("Application Shutting Down");
  delete _impl;
}

Window *Application::get_window() const { return _impl->window.get(); }
Camera *Application::get_camera() const { return _impl->camera.get(); }
EventManager *Application::get_event_manager() const {
  return _impl->eventManager.get();
}
EntityManager *Application::get_entity_manager() const {
  return _impl->entityManager.get();
}
ComponentManager *Application::get_component_manager() const {
  return _impl->componentManager.get();
}
SystemManager *Application::get_system_manager() const {
  return _impl->systemManager.get();
}
TextRenderer *Application::get_text_renderer() const {
  return _impl->textRenderer.get();
}
SpriteRenderer *Application::get_sprite_renderer() const {
  return _impl->spriteRenderer.get();
}

void Application::close() {
  _impl->closing = true;
  LOG_E_INFO("Application::close() called");
}

void Application::run() {
  LOG_E_INFO("Application::run() started");

  ElapsedTime delta_time;

  while (!_impl->closing) {
    // game loop

    // update entity manager
    _impl->entityManager->update();
    // poll events
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      ;
    }

    // clear window
    _impl->window->clear_window();

    // for each system
    // call update on that system
    for (auto &system : _impl->systemManager->get_all_systems()) {
      system->update(*get_component_manager(), delta_time.elapsed);
    }

    // present window
    _impl->window->present();

    delta_time.update();
  }

  LOG_E_INFO("Application::run() ended");
}
} // namespace Mage

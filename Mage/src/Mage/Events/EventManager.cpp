module; 

#include "Mage/Core/Log.h"
#include <cstdint>
#include <vector>
#include <typeinfo>
#include <SDL.h>

module Mage.Events.EventManager;

namespace Mage {
struct EventManager::Impl final {
  Window *window = nullptr;

  bool motion_event_logging_enabled = false;
  bool sensor_event_logging_enabled = false;
  bool window_event_logging_enabled = false;
  bool keyboard_event_logging_enabled = false;
  bool mouse_event_logging_enabled = false;
  bool controller_event_logging_enabled = false;

  std::vector<OnAppClosingEventListener *> on_app_closing_event_listeners;
  std::vector<OnWindowMinimizedEventListener *>
      on_window_minimized_event_listeners;
  std::vector<OnWindowMaximizedEventListener *>
      on_window_maximized_event_listeners;
  std::vector<OnWindowRestoredEventListener *>
      on_window_restored_event_listeners;
  std::vector<OnWindowMouseEnteredEventListener *>
      on_window_mouse_entered_event_listeners;
  std::vector<OnWindowMouseLeftEventListener *>
      on_window_mouse_left_event_listeners;
  std::vector<OnWindowFocusGainedEventListener *>
      on_window_focus_gained_event_listeners;
  std::vector<OnWindowFocusLostEventListener *>
      on_window_focus_lost_event_listeners;

  std::vector<OnMouseButtonDownEventListener *>
      on_mouse_button_down_event_listeners;
  std::vector<OnMouseButtonUpEventListener *>
      on_mouse_button_up_event_listeners;

  void on_app_closing() {
    LOG_E_INFO("Calling on_app_closing on all OnAppClosingEventListeners");
    if (on_app_closing_event_listeners.empty()) {
      LOG_E_WARN("No OnAppClosingEventListeners registered");
    }
    for (auto *listener : on_app_closing_event_listeners) {
      listener->on_app_closing();
    }
  }

  void on_window_minimized() {
    if (window_event_logging_enabled) {
      LOG_E_INFO(
          "calling on_window_minimized on all OnWindowMinimizedEventListeners");
    }
    for (auto *listener : on_window_minimized_event_listeners) {
      listener->on_window_minimized();
    }
  }

  void on_window_maximized() {
    if (window_event_logging_enabled) {
      LOG_E_INFO(
          "calling on_window_maximized on all OnWindowMaximizedEventListeners");
    }
    for (auto *listener : on_window_maximized_event_listeners) {
      listener->on_window_maximized();
    }
  }

  void on_window_restored() {
    if (window_event_logging_enabled) {
      LOG_E_INFO(
          "calling on_window_restored on all OnWindowRestoredEventListeners");
    }
    for (auto *listener : on_window_restored_event_listeners) {
      listener->on_window_restored();
    }
  }

  void on_window_mouse_entered() {
    if (window_event_logging_enabled) {
      LOG_E_INFO("calling on_window_mouse_entered on all "
                 "OnWindowMouseEnteredEventListeners");
    }
    for (auto *listener : on_window_mouse_entered_event_listeners) {
      listener->on_window_mouse_entered();
    }
  }

  void on_window_mouse_left() {
    if (window_event_logging_enabled) {
      LOG_E_INFO("calling on_window_mouse_left on all "
                 "OnWindowMouseLeftEventListeners");
    }
    for (auto *listener : on_window_mouse_left_event_listeners) {
      listener->on_window_mouse_left();
    }
  }

  void on_window_focus_gained() {
    if (window_event_logging_enabled) {
      LOG_E_INFO("calling on_window_focus_gained on all "
                 "OnWindowFocusGainedEventListeners");
    }
    for (auto *listener : on_window_focus_gained_event_listeners) {
      listener->on_window_focus_gained();
    }
  }

  void on_window_focus_lost() {
    if (window_event_logging_enabled) {
      LOG_E_INFO("calling on_window_focus_lost on all "
                 "OnWindowFocusLostEventListeners");
    }
    for (auto *listener : on_window_focus_lost_event_listeners) {
      listener->on_window_focus_lost();
    }
  }

  void on_mouse_button_down(const SDL_Event &event) {
    MouseButton button = translate_mouse_button(event.button.button);
    auto mouse_x = translate_mouse_x(static_cast<float>(event.button.x));
    auto mouse_y = translate_mouse_y(static_cast<float>(event.button.y));

    if (mouse_event_logging_enabled) {
      LOG_E_INFO("Mouse Button Down: button=%d, x=%.2f, y=%.2f, clicks=%d",
                 button, mouse_x, mouse_y, event.button.clicks);
    }
    for (auto *listener : on_mouse_button_down_event_listeners) {
      listener->on_mouse_button_down(button, mouse_x, mouse_y,
                                     event.button.clicks);
    }
  }

  void on_mouse_button_up(const SDL_Event &event) {
    MouseButton button = translate_mouse_button(event.button.button);
    auto mouse_x = translate_mouse_x(static_cast<float>(event.button.x));
    auto mouse_y = translate_mouse_y(static_cast<float>(event.button.y));
    uint8_t click_count = event.button.clicks;

    if (mouse_event_logging_enabled) {
      LOG_E_INFO("Mouse Button Up: button=%d, x=%.2f, y=%.2f, clicks=%d",
                 button, mouse_x, mouse_y, click_count);
    }
    for (auto *listener : on_mouse_button_up_event_listeners) {
      listener->on_mouse_button_up(button, mouse_x, mouse_y, click_count);
    }
  }

  // noop, already works how we expect
  static float translate_mouse_x(float mouse_x) { return mouse_x; }

  // SDL gives mouse y with 0 at the top, but we want 0 at the bottom, so
  // translate it
  float translate_mouse_y(float mouse_y) const {
    return static_cast<float>(window->get_height()) - mouse_y;
  }

  static MouseButton translate_mouse_button(uint8_t sdl_button) {
    switch (sdl_button) {
    case SDL_BUTTON_LEFT:
      return MouseButton::Left;
    case SDL_BUTTON_MIDDLE:
      return MouseButton::Middle;
    case SDL_BUTTON_RIGHT:
      return MouseButton::Right;
    case SDL_BUTTON_X1:
      return MouseButton::Extra1;
    case SDL_BUTTON_X2:
      return MouseButton::Extra2;
    default:
      return MouseButton::Left; // default to left if unknown
    }
  }

  Impl(Window *window) : window(window) {}
};

EventManager::EventManager(Window &window) : _impl(new Impl(&window)) {
  LOG_E_INFO("EventManager created");
}
EventManager::~EventManager() {
  delete _impl;
  LOG_E_INFO("EventManager destroyed");
}

void EventManager::motion_event_logging(bool enabled) {
  _impl->motion_event_logging_enabled = enabled;
  LOG_E_INFO("Motion event logging changed: %d", enabled);
}

void EventManager::sensor_event_logging(bool enabled) {
  _impl->sensor_event_logging_enabled = enabled;
  LOG_E_INFO("Sensor event logging changed: %d", enabled);
}

void EventManager::window_event_logging(bool enabled) {
  _impl->window_event_logging_enabled = enabled;
  LOG_E_INFO("Window event logging changed: %d", enabled);
}

void EventManager::keyboard_event_logging(bool enabled) {
  _impl->keyboard_event_logging_enabled = enabled;
  LOG_E_INFO("Keyboard event logging changed: %d", enabled);
}

void EventManager::mouse_event_logging(bool enabled) {
  _impl->mouse_event_logging_enabled = enabled;
  LOG_E_INFO("Mouse event logging changed: %d", enabled);
}

void EventManager::controller_event_logging(bool enabled) {
  _impl->controller_event_logging_enabled = enabled;
  LOG_E_INFO("Controller event logging changed: %d", enabled);
}

void EventManager::add_on_app_closing_event_listener(
    OnAppClosingEventListener *listener) {
  _impl->on_app_closing_event_listeners.push_back(listener);
  LOG_E_INFO("OnAppClosingEventListener added: %s", typeid(*listener).name());
}

void EventManager::add_on_window_minimized_event_listener(
    OnWindowMinimizedEventListener *listener) {
  _impl->on_window_minimized_event_listeners.push_back(listener);
  LOG_E_INFO("OnWindowMinimizedEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::add_on_window_maximized_event_listener(
    OnWindowMaximizedEventListener *listener) {
  _impl->on_window_maximized_event_listeners.push_back(listener);
  LOG_E_INFO("OnWindowMaximizedEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::add_on_window_restored_event_listener(
    OnWindowRestoredEventListener *listener) {
  _impl->on_window_restored_event_listeners.push_back(listener);
  LOG_E_INFO("OnWindowRestoredEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::add_on_window_mouse_entered_event_listener(
    OnWindowMouseEnteredEventListener *listener) {
  _impl->on_window_mouse_entered_event_listeners.push_back(listener);
  LOG_E_INFO("OnWindowMouseEnteredEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::add_on_window_mouse_left_event_listener(
    OnWindowMouseLeftEventListener *listener) {
  _impl->on_window_mouse_left_event_listeners.push_back(listener);
  LOG_E_INFO("OnWindowMouseLeftEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::add_on_window_focus_gained_event_listener(
    OnWindowFocusGainedEventListener *listener) {
  _impl->on_window_focus_gained_event_listeners.push_back(listener);
  LOG_E_INFO("OnWindowFocusGainedEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::add_on_window_focus_lost_event_listener(
    OnWindowFocusLostEventListener *listener) {
  _impl->on_window_focus_lost_event_listeners.push_back(listener);
  LOG_E_INFO("OnWindowFocusLostEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::add_on_mouse_button_down_event_listener(
    OnMouseButtonDownEventListener *listener) {
  _impl->on_mouse_button_down_event_listeners.push_back(listener);
  LOG_E_INFO("OnMouseButtonDownEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::add_on_mouse_button_up_event_listener(
    OnMouseButtonUpEventListener *listener) {
  _impl->on_mouse_button_up_event_listeners.push_back(listener);
  LOG_E_INFO("OnMouseButtonUpEventListener added: %s",
             typeid(*listener).name());
}

void EventManager::poll_events() {
  // poll events
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_QUIT:
      _impl->on_app_closing();
      break;
    case SDL_WINDOW_MINIMIZED:
      _impl->on_window_minimized();
      break;
    case SDL_WINDOW_MAXIMIZED:
      _impl->on_window_maximized();
      break;
    case SDL_WINDOWEVENT_RESTORED:
      _impl->on_window_restored();
      break;
    case SDL_WINDOWEVENT_ENTER:
      _impl->on_window_mouse_entered();
      break;
    case SDL_WINDOWEVENT_LEAVE:
      _impl->on_window_mouse_left();
      break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
      _impl->on_window_focus_gained();
      break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
      _impl->on_window_focus_lost();
      break;
    case SDL_MOUSEBUTTONDOWN:
      _impl->on_mouse_button_down(event);
      break;
    case SDL_MOUSEBUTTONUP:
      _impl->on_mouse_button_up(event);
      break;
    default:;
    }
  }
}

} // namespace Mage

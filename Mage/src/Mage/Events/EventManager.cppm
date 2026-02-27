module;

#include "Mage/Core/Api.h"
#include "Mage/Core/ICopyDisable.h"
#include <cstdint>

export module Mage:Events.EventManager;

import Mage.Events.EventListenerBase;
import Mage.Events.EventEnums;
import :Core.Window;

export namespace Mage {

class MAGE_API OnAppClosingEventListener : public EventListenerBase {
public:
  virtual void on_app_closing() = 0;
};

class MAGE_API OnWindowMinimizedEventListener : public EventListenerBase {
public:
  virtual void on_window_minimized() = 0;
};

class MAGE_API OnWindowMaximizedEventListener : public EventListenerBase {
public:
  virtual void on_window_maximized() = 0;
};

class MAGE_API OnWindowRestoredEventListener : public EventListenerBase {
public:
  virtual void on_window_restored() = 0;
};

class MAGE_API OnWindowMouseEnteredEventListener : public EventListenerBase {
public:
  virtual void on_window_mouse_entered() = 0;
};

class MAGE_API OnWindowMouseLeftEventListener : public EventListenerBase {
public:
  virtual void on_window_mouse_left() = 0;
};

class MAGE_API OnWindowFocusGainedEventListener : public EventListenerBase {
public:
  virtual void on_window_focus_gained() = 0;
};

class MAGE_API OnWindowFocusLostEventListener : public EventListenerBase {
public:
  virtual void on_window_focus_lost() = 0;
};

class MAGE_API OnMouseButtonDownEventListener : public EventListenerBase {
public:
  virtual void on_mouse_button_down(MouseButton button, float x, float y,
                                    uint8_t click_count) = 0;
};

class MAGE_API OnMouseButtonUpEventListener : public EventListenerBase {
public:
  virtual void on_mouse_button_up(MouseButton button, float x, float y,
                                  uint8_t click_count) = 0;
};

class MAGE_API EventManager : ICopyDisable {
  friend class Application;

public:
  ~EventManager();

  void poll_events();

  void motion_event_logging(bool enabled);
  void sensor_event_logging(bool enabled);
  void window_event_logging(bool enabled);
  void keyboard_event_logging(bool enabled);
  void mouse_event_logging(bool enabled);
  void controller_event_logging(bool enabled);

  void add_on_app_closing_event_listener(OnAppClosingEventListener *listener);
  void add_on_window_minimized_event_listener(
      OnWindowMinimizedEventListener *listener);
  void add_on_window_maximized_event_listener(
      OnWindowMaximizedEventListener *listener);
  void add_on_window_restored_event_listener(
      OnWindowRestoredEventListener *listener);
  void add_on_window_mouse_entered_event_listener(
      OnWindowMouseEnteredEventListener *listener);
  void add_on_window_mouse_left_event_listener(
      OnWindowMouseLeftEventListener *listener);
  void add_on_window_focus_gained_event_listener(
      OnWindowFocusGainedEventListener *listener);
  void add_on_window_focus_lost_event_listener(
      OnWindowFocusLostEventListener *listener);
  void add_on_mouse_button_down_event_listener(
      OnMouseButtonDownEventListener *listener);
  void
  add_on_mouse_button_up_event_listener(OnMouseButtonUpEventListener *listener);

private:
  explicit EventManager(Window &window);

  struct Impl;
  Impl *_impl;
};
} // namespace Mage


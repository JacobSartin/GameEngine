module;

#pragma once

#include "Mage/Core/Api.h"
#include "Mage/Core/ICopyDisable.h"

export module Mage.Events.EventListenerBase;

export namespace Mage {
class MAGE_API EventListenerBase : public ICopyDisable {
public:
  EventListenerBase() = default;

  virtual ~EventListenerBase() = default;
};
} // namespace Mage
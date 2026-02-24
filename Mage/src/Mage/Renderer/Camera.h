#pragma once

#include "../Core/Core.h"
#include "Mage/Core/Application.h"
#include "Mage/Core/ICopyDisable.h"

namespace Mage {
class MAGE_API Camera : ICopyDisable {
  friend class Application;

public:
  ~Camera() = default;

  float left;
  float right;
  float top;
  float bottom;

private:
  Camera() = default;
};
} // namespace Mage

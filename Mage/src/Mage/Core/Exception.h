#pragma once

#include "Core.h"
#include "Mage/Core/ICopyDisable.h"

namespace Mage {
class MAGE_API Exception : ICopyDisable {
public:
  explicit Exception(const char *message);

  ~Exception();

  const char *what() const;

private:
  struct Impl;
  Impl *_impl;
};
} // namespace Mage

module;

#include "Mage/Core/Core.h"
#include "Shader.h"
#include "Sprite.h"

export module Mage.Renderer.SpriteRenderer;

import Mage.Renderer.Camera;

namespace Mage {
extern "C++" class Application; 
}

export namespace Mage {

class MAGE_API SpriteRenderer
    : ICopyDisable // NOLINT(cppcoreguidelines-special-member-functions)
{
  /* TODO figure out a better way to allow Application to create SpriteRenderer
  modules can't forward declare classes without causing conflicts with original definitions,
  so we can't just forward declare Application here and make the constructor private.
  But we also don't want to make the constructor public, 
  since we don't want users to create SpriteRenderer instances directly. 
  So for now, we'll just make the constructor protected and make Application a friend class. 
  But this is not ideal and we should figure out a better way to handle this in the future. */
  friend class Application;

public:
  ~SpriteRenderer();

  void render(const Sprite &sprite, const glm::vec2 &translation,
              const glm::vec2 &scale, float rotation, float elapsed_time) const;

private:
  explicit SpriteRenderer(Camera &camera);

  struct Impl;
  Impl *_impl;
};

} // namespace Mage
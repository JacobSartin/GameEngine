#include "Sprite.h"
#include "Mage/Core/Color.h"
#include "Mage/Core/Exception.h"
#include "Mage/Core/Log.h"
#include "Mage/Core/pch.h"
#include <cstdint>

namespace Mage {
struct Sprite::Impl {
  uint_fast32_t width = 1;
  uint_fast32_t height = 1;
  GLuint vao = 0;
  GLuint vbo = 0;
  bool is_textured = false;

  Color color = Color::white;

  std::string sprite_image_file;
  uint_fast32_t frames = 1;
  uint_fast32_t current_frame = 0;
  float frame_time = 0.0f;
  float elapsed_time_frame = 0.0f;
  bool flip_x = false;
  GLuint texture_id = 0;

  Impl() {
    auto *first_pos_byte = reinterpret_cast<void *>(0);
    // NOLINTNEXTLINE (performance-no-int-to-ptr)
    auto *first_uv_byte = reinterpret_cast<void *>(sizeof(GLfloat) * 2);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr,
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
                          first_pos_byte);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
                          first_uv_byte);
    glEnableVertexAttribArray(1);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
};

Sprite::Sprite(const Color &color) : _impl(new Impl()) {
  LOG_E_INFO("Loading sprite...");

  _impl->color = color;
}

Sprite::Sprite(const char *sprite_image_file, uint_fast32_t frames,
               float frame_time)
    : _impl(new Impl()) {
  LOG_E_INFO("Loading sprite: %s", sprite_image_file);

  _impl->is_textured = true;
  _impl->sprite_image_file = sprite_image_file;
  _impl->frames = frames;
  _impl->frame_time = frame_time;

  stbi_set_flip_vertically_on_load(1);

  int width;
  int height;
  int channels;
  auto *image_data =
      stbi_load(sprite_image_file, &width, &height, &channels, STBI_rgb_alpha);
  if (image_data == nullptr) {
    throw Exception(
        (std::string("Failed to load sprite image: ") + sprite_image_file)
            .c_str());
  }
  _impl->width = static_cast<uint_fast32_t>(width) / frames;
  _impl->height = static_cast<uint_fast32_t>(height);

  glGenTextures(1, &_impl->texture_id);
  glBindTexture(GL_TEXTURE_2D, _impl->texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);

  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(image_data);
}

Sprite::~Sprite() {
  if (_impl->is_textured) {
    LOG_E_INFO("Unloading sprite: %s", _impl->sprite_image_file.c_str());
    glDeleteTextures(1, &_impl->texture_id);
  } else {
    LOG_E_INFO("Unloading sprite...");
  }

  glDeleteBuffers(1, &_impl->vbo);
  glDeleteVertexArrays(1, &_impl->vao);
}

bool Sprite::is_textured() const { return _impl->is_textured; }

uint_fast32_t Sprite::get_width() const { return _impl->width; }

uint_fast32_t Sprite::get_height() const { return _impl->height; }

Color &Sprite::get_color() const { return _impl->color; }
void Sprite::set_color(const Color &color) { _impl->color = color; }

bool Sprite::get_flip_x() const { return _impl->flip_x; }
void Sprite::set_flip_x(bool flip_x) { _impl->flip_x = flip_x; }

void Sprite::start_over() {
  _impl->flip_x = false;
  _impl->current_frame = 0;
  _impl->elapsed_time_frame = 0.0f;
}

void Sprite::draw(float elapsed_time) const {
  float x_start = 0.0f;
  float x_end = 1.0f;

  if (_impl->is_textured) {
    _impl->elapsed_time_frame += elapsed_time;
    if (_impl->elapsed_time_frame >= _impl->frame_time) {

      _impl->current_frame =
          (_impl->current_frame + 1) % _impl->frames; // loop back to first
      _impl->elapsed_time_frame = 0.0f;

      x_start = static_cast<float>(_impl->current_frame) /
                static_cast<float>(_impl->frames);
      x_end = static_cast<float>(_impl->current_frame + 1) /
              static_cast<float>(_impl->frames);
    }

    if (_impl->flip_x) {
      std::swap(x_start, x_end);
    }
  }

  float vertices[6][4] = {
      {0.0f, 1.0f, x_start, 1.0f}, // top-left
      {1.0f, 0.0f, x_end, 0.0f},   // bottom-right
      {0.0f, 0.0f, x_start, 0.0f}, // bottom-left

      {0.0f, 1.0f, x_start, 1.0f}, // top-left
      {1.0f, 1.0f, x_end, 1.0f},   // top-right
      {1.0f, 0.0f, x_end, 0.0f}    // bottom-right
  };

  glBindVertexArray(_impl->vao);

  if (_impl->is_textured) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _impl->texture_id);
  }

  glBindBuffer(GL_ARRAY_BUFFER, _impl->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0); // unbind

  if (_impl->is_textured) {
    glBindTexture(GL_TEXTURE_2D, 0); // unbind
  }
}

} // namespace Mage

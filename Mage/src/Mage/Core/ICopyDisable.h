#pragma once

class ICopyDisable {
public:
  ICopyDisable() = default;
  // Disable copy and move semantics
  ICopyDisable(const ICopyDisable &) = delete;
  ICopyDisable &operator=(const ICopyDisable &) = delete;

  ICopyDisable(ICopyDisable &&) = delete;
  ICopyDisable &operator=(ICopyDisable &&) = delete;
};
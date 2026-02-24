#pragma once

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#pragma warning(push, 0)

#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <gl/glew.h>

#define NO_SDL_GLEXT
#include <SDL_opengl.h>
#include <gl/glu.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <GLM/gtx/rotate_vector.hpp>

#include "stb_image.h"

#pragma warning(pop)

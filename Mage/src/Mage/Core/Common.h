#pragma once

#pragma warning(push, 0)

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

#pragma once

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <limits>
#include <set>
#include <unordered_set>
#include <array>
#include <memory>
#include <chrono>
#include <unordered_map>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.h>
#pragma once

#include "KeyCode.h"

#include <glm/glm.hpp>

namespace Confuse{
    class Input{
    public:
        static bool isKeyDown(KeyCode keycode);
        static bool isMouseButtonDown(MouseButton button);

        static glm::vec2 getMousePosition();

        static void setCursorMode(CursorMode mode);
    };
}
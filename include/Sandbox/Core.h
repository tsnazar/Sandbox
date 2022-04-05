#pragma once

#include <memory>
#include <functional>

#include "GLDebug.h"

#ifdef SANDBOX_DEBUG
#define SANDBOX_ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    SANDBOX_ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else 
#define SANDBOX_ASSERT(x) x
#define GLCall(x) x
#endif

#define GLCORE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
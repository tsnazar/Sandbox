#pragma once

#include <memory>
#include <functional>

#ifdef SANDBOX_DEBUG
#define GLCORE_ENABLE_ASSERTS
#endif

#ifdef GLCORE_ENABLE_ASSERTS
#define GLCORE_ASSERT(x) { if(!(x)) { __debugbreak(); } }
#else
#define GLCORE_ASSERT(x, ...)
#endif

#define GLCORE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
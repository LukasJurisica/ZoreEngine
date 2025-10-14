#pragma once

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#else
#define ZoneScoped
#define ZoneScopedC(color)
#define ZoneScopedN(name)
#define ZoneScopedNC(name, color)
#define FrameMark
#define FrameMarkStart
#define FrameMarkEnd
#endif
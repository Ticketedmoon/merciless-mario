#pragma once

#ifndef PRIMITIVE_WARS_COMMON_CONSTANTS_H
#define PRIMITIVE_WARS_COMMON_CONSTANTS_H

#include <cstdint>

static constexpr std::string_view WINDOW_TITLE = "merciless-mario";
static const bool USE_VERTICAL_SYNC = true;
static const uint32_t APP_FRAME_RATE = 60;
static const uint32_t WINDOW_WIDTH = 1280;
static const uint32_t WINDOW_HEIGHT = 720;
static constexpr uint32_t MAX_LEVEL_WIDTH = 16384;
static constexpr uint32_t LEVEL_DEPTH_KILL_ZONE = 1648;
static constexpr uint32_t MAX_LEVEL_HEIGHT = 2048;

#endif //PRIMITIVE_WARS_COMMON_CONSTANTS_H

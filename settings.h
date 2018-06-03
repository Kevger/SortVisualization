#pragma once
/**
* settings.h
* @author: Kevin German
**/
#include <chrono>

//Resolution
static constexpr auto defaultScreenWidth = 1200u;
static constexpr auto defaultScreenHeight = 800u;
//Settings
static constexpr size_t defaultNumberOfElements = 1200;
static constexpr std::chrono::nanoseconds defaultCompareDelay{ 500 };
static constexpr std::chrono::nanoseconds defaultAssignmentDelay{ 2000 };
static constexpr auto defaultMaxThreads = 4;
static constexpr auto configFileName = "config.txt";

static enum {
	ERROR_SDL_INIT = -101,

} _SDL_Errortypes;
#pragma once

#include <OLEDDisplayUi.h>
#include "mesh/NodeDB.h"

class FavoriteWeatherModule
{
public:
    static int drawFrame(
        OLEDDisplay *display,
        OLEDDisplayUiState *state,
        int16_t x,
        int16_t y);
};
#include "FavoriteWeatherModule.h"

#include "NodeDB.h"
#include "graphics/Screen.h"
#include "graphics/TimeFormatters.h"
#include "gps/RTC.h"

extern NodeDB *nodeDB;

// Replace with your weather node ID
static constexpr uint32_t FAVORITE_NODE =
    0x12345678;

int FavoriteWeatherModule::drawFrame(
    OLEDDisplay *display,
    OLEDDisplayUiState *state,
    int16_t x,
    int16_t y)
{
    meshtastic_EnvironmentMetrics env;

    if (!nodeDB->copyNodeEnvironment(FAVORITE_NODE, env))
    {
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        display->drawString(
            x + 5,
            y + 20,
            "No Weather Data");

        return 0;
    }

    display->setTextAlignment(TEXT_ALIGN_LEFT);

    // Display title
    display->drawString(
        x + 0,
        y + 0,
        "Weather");

    char buffer[64];

    // Display current time
    time_t now = getTime();
    struct tm *timeinfo = localtime(&now);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    display->drawString(
        x + 0,
        y + 10,
        buffer);

    // Display temperature
    snprintf(
        buffer,
        sizeof(buffer),
        "Temp: %.1f C",
        env.temperature);

    display->drawString(
        x + 0,
        y + 20,
        buffer);

    // Display humidity
    snprintf(
        buffer,
        sizeof(buffer),
        "Hum: %.0f%%",
        env.relative_humidity);

    display->drawString(
        x + 0,
        y + 30,
        buffer);

    // Display pressure
    snprintf(
        buffer,
        sizeof(buffer),
        "Pres: %.0f",
        env.barometric_pressure);

    display->drawString(
        x + 0,
        y + 40,
        buffer);

    return 0;
}
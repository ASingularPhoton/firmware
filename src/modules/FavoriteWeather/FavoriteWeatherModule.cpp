#include "FavoriteWeatherModule.h"

#include "NodeDB.h"
#include "graphics/Screen.h"
#include "graphics/TimeFormatters.h"
#include "gps/RTC.h"

extern NodeDB *nodeDB;

// Replace with your weather node ID
// Replace with your weather node ID. If left as the sentinel value
// `AUTO_SELECT_FAVORITE`, the module will auto-pick the first favorite
// node or first peer that provides environment telemetry.
static constexpr uint32_t AUTO_SELECT_FAVORITE = 0x12345678;
static constexpr uint32_t FAVORITE_NODE = AUTO_SELECT_FAVORITE;

int FavoriteWeatherModule::drawFrame(
    OLEDDisplay *display,
    OLEDDisplayUiState *state,
    int16_t x,
    int16_t y)
{
    meshtastic_EnvironmentMetrics env;

    uint32_t nodeToUse = FAVORITE_NODE;

    // If user left the sentinel, try to auto-select a favorite node or any
    // peer that has environment telemetry available.
    if (nodeToUse == AUTO_SELECT_FAVORITE) {
        const size_t num = nodeDB->getNumMeshNodes();
        for (size_t i = 0; i < num; ++i) {
            const meshtastic_NodeInfoLite *n = nodeDB->getMeshNodeByIndex(i);
            if (!n)
                continue;
            // skip self
            if (n->num == nodeDB->getNodeNum())
                continue;
            // prefer nodes the user marked as favorite
            if (nodeInfoLiteIsFavorite(n)) {
                nodeToUse = n->num;
                break;
            }
            // otherwise remember the first node that actually has env data
            meshtastic_EnvironmentMetrics tmp;
            if (nodeDB->copyNodeEnvironment(n->num, tmp)) {
                nodeToUse = n->num;
                break;
            }
        }
    }

    if (!nodeDB->copyNodeEnvironment(nodeToUse, env)) {
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        display->drawString(x + 5, y + 20, "No Weather Data");
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
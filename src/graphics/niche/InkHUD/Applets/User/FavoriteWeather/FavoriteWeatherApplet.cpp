#ifdef MESHTASTIC_INCLUDE_NICHE_GRAPHICS

#include <string>

#include "FavoriteWeatherApplet.h"

#include "NodeDB.h"
#include "graphics/niche/InkHUD/InkHUD.h"
#include "mesh/generated/meshtastic/telemetry.pb.h"

extern NodeDB *nodeDB;

namespace NicheGraphics::InkHUD
{

FavoriteWeatherApplet::FavoriteWeatherApplet()
    : SinglePortModule("FavoriteWeatherApplet", meshtastic_PortNum_TELEMETRY_APP)
{
}

void FavoriteWeatherApplet::onRender(bool full)
{
    meshtastic_EnvironmentMetrics env;
    uint32_t nodeToUse = 0;

    size_t num = nodeDB->getNumMeshNodes();
    for (size_t i = 0; i < num; ++i) {
        const meshtastic_NodeInfoLite *n = nodeDB->getMeshNodeByIndex(i);
        if (!n)
            continue;
        if (n->num == nodeDB->getNodeNum())
            continue;
        if (nodeInfoLiteIsFavorite(n)) {
            nodeToUse = n->num;
            break;
        }
        meshtastic_EnvironmentMetrics tmp;
        if (nodeDB->copyNodeEnvironment(n->num, tmp)) {
            nodeToUse = n->num;
            break;
        }
    }

    if (nodeToUse == 0 || !nodeDB->copyNodeEnvironment(nodeToUse, env)) {
        printAt(0, 0, "Weather");
        printAt(0, 24, "No weather data");
        return;
    }

    printAt(0, 0, "Weather");
    printAt(0, 16, getTimeString().c_str());

    if (env.has_temperature) {
        std::string tempLine = "Temp: " + std::to_string(env.temperature) + " C";
        printAt(0, 32, tempLine.c_str());
    }
    if (env.has_relative_humidity) {
        std::string humidityLine = "Hum: " + std::to_string((int)env.relative_humidity) + "%";
        printAt(0, 48, humidityLine.c_str());
    }
    if (env.has_barometric_pressure) {
        std::string pressureLine = "Pres: " + std::to_string((int)env.barometric_pressure);
        printAt(0, 64, pressureLine.c_str());
    }
}

ProcessMessage FavoriteWeatherApplet::handleReceived(const meshtastic_MeshPacket &mp)
{
    if (!isActive())
        return ProcessMessage::CONTINUE;

    // Telemetry packets can update the weather data display.
    if (mp.which_payload_variant == meshtastic_MeshPacket_decoded_tag && mp.decoded.portnum == meshtastic_PortNum_TELEMETRY_APP) {
        requestAutoshow();
        requestUpdate();
    }

    return ProcessMessage::CONTINUE;
}

} // namespace NicheGraphics::InkHUD

#endif

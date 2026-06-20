#ifdef MESHTASTIC_INCLUDE_NICHE_GRAPHICS

#pragma once

#include "configuration.h"

#include "graphics/niche/InkHUD/Applet.h"
#include "SinglePortModule.h"
#include "mesh/generated/meshtastic/portnums.pb.h"

namespace NicheGraphics::InkHUD
{

class FavoriteWeatherApplet : public Applet, public SinglePortModule
{
  public:
    FavoriteWeatherApplet();

    void onRender(bool full) override;

  protected:
    ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;
};

} // namespace NicheGraphics::InkHUD

#endif

#include "Fonts.hpp"
#include "Draw Manager.hpp"

namespace FONTS
{
	unsigned int visuals_lby_font;
	unsigned int visuals_esp_font;
	unsigned int visuals_name_font;
	unsigned int visuals_event_log_font;
	unsigned int visuals_crosshair_font;
	unsigned int visuals_weapon_icon_font;
	unsigned int visuals_grenade_icon_font;
	unsigned int visuals_information_font;
	unsigned int visuals_nade_helper_font;
	unsigned int visuals_anti_aim_side_font;

	bool ShouldReloadFonts()
	{
		int width, height;
		static int old_width, old_height;
		g_EngineClient->GetScreenSize(width, height);

		if (width != old_width || height != old_height)
		{
			old_width = width;
			old_height = height;
			return true;
		}

		return false;
	}

	void InitializeFonts()
	{
		visuals_anti_aim_side_font = RENDER::CreateF("Verdana", 46, 900, 0, 0, NULL);
		visuals_crosshair_font = RENDER::CreateF("Verdana", 18, 600, 0, 0, FONTFLAG_OUTLINE);
		visuals_event_log_font = RENDER::CreateF("Verdana", 14, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		visuals_esp_font = RENDER::CreateF("Verdana", 10, 100, 0, 0, FONTFLAG_OUTLINE);
		visuals_information_font = RENDER::CreateF("System", 16, 100, 0, 0, FONTFLAG_DROPSHADOW);
		visuals_weapon_icon_font = RENDER::CreateF("undefeated", 16, 400, 0, 0, FONTFLAG_ANTIALIAS);
		visuals_grenade_icon_font = RENDER::CreateF("undefeated", 20, 400, 0, 0, FONTFLAG_ANTIALIAS);
		visuals_lby_font = RENDER::CreateF("Verdana", 32, 650, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		visuals_name_font = RENDER::CreateF("Verdana", 12, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		visuals_nade_helper_font = RENDER::CreateF("undefeated", 16, FW_NORMAL, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
	}
}
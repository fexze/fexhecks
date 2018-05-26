#pragma once

#include "SDK.hpp"

namespace FONTS
{
	void InitializeFonts();
	bool ShouldReloadFonts();

	extern unsigned int visuals_lby_font;
	extern unsigned int visuals_esp_font;
	extern unsigned int visuals_name_font;
	extern unsigned int visuals_event_log_font;
	extern unsigned int visuals_crosshair_font;
	extern unsigned int visuals_weapon_icon_font;
	extern unsigned int visuals_grenade_icon_font;
	extern unsigned int visuals_information_font;
	extern unsigned int visuals_nade_helper_font;
	extern unsigned int visuals_anti_aim_side_font;
}
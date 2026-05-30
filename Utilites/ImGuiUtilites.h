#ifndef __IMGUI_UTILITES_HEADER__
#define __IMGUI_UTILITES_HEADER__

#include "../../Resources/imgui/imgui.h"

struct GradientBlock
{
	ImU32 TopColour;
	ImU32 BottomColour;
};

class ImGuiUtilies
{
	public:
						ImGuiUtilies(void);
		static void		GradientRect(ImVec2 Minimum, ImVec2 Maximum, GradientBlock CurrentGradient);
		static bool		ButtonGradient(const char* Label, ImVec2 Size, ImVec4 BorderColour, GradientBlock HoveredGradient, GradientBlock ActiveGradient, GradientBlock CurrentGradient);
		static void		BeginGradient(const char* Name, GradientBlock CurrentGradient, bool* Open = (bool*)0, ImGuiWindowFlags Flags = 0);
	private:
};

#endif

#include "ImGuiUtilites.h"

bool ImGuiUtilites::ButtonGradient(const char* Label, ImVec2 Size,ImVec4 BorderColour, GradientBlock HoveredGradient, GradientBlock ActiveGradient, GradientBlock CurrentGradient)
{
    ImU32 Top, Bottom;
    ImVec2 Position = ImGui::GetCursorScreenPos();
    ImGui::Button(Label, Size);

    bool Hovered            = ImGui::IsItemHovered();
    bool Active             = ImGui::IsItemActive();
    ImDrawList* CurrentDraw = ImGui::GetWindowDrawList();

    if (Active)
    {
        Top     = ActiveGradient.TopColour;
        Bottom  = ActiveGradient.BottomColour;
    }
    else if (Hovered)
    {
        Top     = HoveredGradient.TopColour;
        Bottom  = HoveredGradient.BottomColour;
    }
    else
    {
        Top     = CurrentGradient.TopColour;
        Bottom  = CurrentGradient.BottomColour;
    }

    CurrentDraw->AddRectFilledMultiColor(
        Position,
        ImVec2(Position.x + Size.x, Position.y + Size.y),
        Top, Top,
        Bottom, Bottom
    );

    CurrentDraw->AddRect(
        Position,
        ImVec2(Position.x + Size.x, Position.y + Size.y),
        IM_COL32(BorderColour.x, BorderColour.y, BorderColour.z, BorderColour.w), 6.0f
    );

    ImVec2 TextSize         = ImGui::CalcTextSize(Label);
    ImVec2 TextPosition     = ImVec2(
        Position.x + (Size.x - TextSize.x) * 0.5f,
        Position.y + (Size.y - TextSize.y) * 0.5f
    );

    CurrentDraw->AddText(TextPosition, IM_COL32(255, 255, 255, 255), Label);

    return ImGui::IsItemClicked();
}

void ImGuiUtilites::GradientRect(ImVec2 Minimum, ImVec2 Maximum, GradientBlock CurrentGradient)
{
    ImDrawList* CurrentDrawList = ImGui::GetWindowDrawList();

    CurrentDrawList->AddRectFilledMultiColor(
        Minimum, Maximum,
        CurrentGradient.TopColour, CurrentGradient.TopColour,
        CurrentGradient.BottomColour, CurrentGradient.BottomColour
    );
}

void ImGuiUtilites::BeginGradient(const char* Name, GradientBlock CurrentGradient, bool* Open, ImGuiWindowFlags Flags)
{
    ImGui::Begin(Name, Open, Flags);

    ImGuiUtilites::GradientRect(
        ImGui::GetWindowPos(),
        ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y),
        CurrentGradient
    );
}
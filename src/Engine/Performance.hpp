#pragma once
#define LT_MEAUSRE_PERFORMANCE

#include <time.h>
clock_t t;

#define LT_START_TIME() t = clock()
#define LT_END_TIME() t = clock() - t

#ifdef LT_MEAUSRE_PERFORMANCE
#define LT_MEASURE_FUNC(x)                                                     \
  t = clock();                                                                 \
  x;                                                                           \
  t = clock() - t

#else
#define LT_MEASURE_FUNC(x) x;
#endif

/* ImGui Style backup
void AppGui::SetupImGuiStyle()
{
  ImGuiStyle& style = ImGui::GetStyle();
  style.Colors[ImGuiCol_Text]                  = ImVec4(0.93f, 0.94f, 0.95f, 1.00f);
  style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.93f, 0.94f, 0.95f, 0.58f);
  style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
  style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.13f, 0.13f, 0.13f, 0.00f);
  style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.13f, 0.13f, 0.82f);
  style.Colors[ImGuiCol_Border]                = ImVec4(0.39f, 0.39f, 0.39f, 0.31f);
  style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.13f, 0.13f, 0.13f, 0.00f);
  style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
  style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.27f, 0.60f, 0.93f, 1.00f);
  style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.35f, 0.67f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.31f, 0.31f, 0.31f, 0.75f);
  style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.20f, 0.60f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.39f, 0.39f, 0.39f, 0.63f);
  style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.22f, 0.54f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.27f, 0.60f, 0.93f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.35f, 0.67f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
  style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.20f, 0.60f, 1.00f, 0.98f);
  style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.22f, 0.54f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.35f, 0.67f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_Button]                = ImVec4(0.22f, 0.54f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.27f, 0.60f, 0.93f, 1.00f);
  style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.35f, 0.67f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_Header]                = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
  style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.27f, 0.60f, 0.93f, 1.00f);
  style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.35f, 0.67f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_Column]                = ImVec4(0.20f, 0.60f, 1.00f, 0.32f);
  style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.20f, 0.60f, 1.00f, 0.78f);
  style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.20f, 0.60f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.20f, 0.60f, 1.00f, 0.00f);
  style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.20f, 0.60f, 1.00f, 0.78f);
  style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.20f, 0.60f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.93f, 0.94f, 0.95f, 0.16f);
  style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.93f, 0.94f, 0.95f, 0.39f);
  style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.93f, 0.94f, 0.95f, 1.00f);
  style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.93f, 0.94f, 0.95f, 0.63f);
  style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.20f, 0.60f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.93f, 0.94f, 0.95f, 0.63f);
  style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.20f, 0.60f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.60f, 1.00f, 0.43f);
  style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.31f, 0.31f, 0.31f, 0.73f);

  style.WindowRounding    = 0.f;
  style.GrabRounding      = std::floor (2.f * ImGui::GetIO().FontGlobalScale);
  style.FrameRounding     = std::floor (2.f * ImGui::GetIO().FontGlobalScale);
  style.ScrollbarRounding = std::floor (2.f * ImGui::GetIO().FontGlobalScale);

  style.FramePadding.x = std::floor (6.0f * ImGui::GetIO().FontGlobalScale);
  style.FramePadding.y = std::floor (3.0f * ImGui::GetIO().FontGlobalScale);
  style.ItemSpacing.x  = std::floor (8.0f * ImGui::GetIO().FontGlobalScale);
  style.ItemSpacing.y  = std::floor (4.0f * ImGui::GetIO().FontGlobalScale);
  style.ScrollbarSize  = std::floor (16.f * ImGui::GetIO().FontGlobalScale);
}
*/

#include "style.h"

void SetLightStyle(ImGuiStyle& style)
{
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.93f, 0.93f, 0.93f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.53f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.67f, 0.84f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.67f, 0.84f, 0.96f, 0.47f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.67f, 0.84f, 0.96f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.93f, 0.93f, 0.93f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.82f, 0.86f, 0.91f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.72f, 0.77f, 0.79f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.64f, 0.69f, 0.71f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.64f, 0.69f, 0.71f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.98f, 0.82f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.90f, 0.75f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.82f, 0.67f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
}
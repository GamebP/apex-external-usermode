#pragma once


void draw_rectangle(float x, float y, float x2, float y2, ImColor color, float thickness) {
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + x2, y + y2), color, 0, -1, thickness);
	return;
}

void draw_text_outline(float x, float y, ImColor color, const char* string, ...) {
	char buf[512];
	va_list arg_list;

	ZeroMemory(buf, sizeof(buf));

	va_start(arg_list, string);
	vsnprintf(buf, sizeof(buf), string, arg_list);
	va_end(arg_list);

	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(x - 1, y), ImColor(0.0f, 0.0f, 0.0f, 1.0f), buf, 0, 0, 0);
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(x + 1, y), ImColor(0.0f, 0.0f, 0.0f, 1.0f), buf, 0, 0, 0);
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(x, y - 1), ImColor(0.0f, 0.0f, 0.0f, 1.0f), buf, 0, 0, 0);
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(x, y + 1), ImColor(0.0f, 0.0f, 0.0f, 1.0f), buf, 0, 0, 0);

	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(x, y), color, buf, 0, 0, 0);
	return;
}




namespace Settings {
	static bool Aimbot = 1;
	static int AimbotFOV = 150;
	static int AimbotSmooth = 1;

	static bool PlayerBox = 0;
	static bool Box3d = 0;
	static bool Cornerbox = 0;
	bool filledfov = false;
	static bool BoundedBox = 1;
	static bool Skeleton = 0;
	static bool Snaplines = 0;
	static bool PlayerName = 1;
	static bool PlayerHealth = 0;
	static bool Glow = 0;
	static bool DebugBones = 0;

	static bool SkydiveSpeed = 0;
	static bool backtrack = 0;

	static bool Crosshair = 1;
	static bool ShowFov = 1;
	static bool TeamCheck = 1;

	bool loot;
	bool lootbox;
	bool lootdistance;

	static ImColor ESP;
	static ImColor Visible = ImColor(0, 255, 0, 255);
	static ImColor Invisible = ImColor(255, 182, 193);
}

namespace Addresses {
	uintptr_t LocalPlayer;
	uintptr_t EntityList;
	uintptr_t ViewRenderer;
	uintptr_t ViewMatrix;
	uintptr_t NameList;

	static int Width = 1920;
	static int Height = 1080;
}
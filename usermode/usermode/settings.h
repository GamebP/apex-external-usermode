#pragma once

namespace Settings {
	static bool Aimbot = 1;
	static int AimbotFOV = 150;
	static int AimbotSmooth = 1;

	static bool PlayerBox = 1;
	static bool Box3d = 0;
	static bool Cornerbox = 0;
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
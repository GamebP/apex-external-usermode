#pragma once
#include <io.h>
static bool MenuIsOpen = true;
static int tab = 1;


bool aimbottt = false;
bool aimkey = false;
bool aimbotpredticiton = false;
bool noswap = false;
bool nospread = false;
bool aimbotvischeck = false;
bool ignoredowned = false;
bool aimatfriendlies = false;
int prioritytype = 3;
bool drawcircle = false;
bool silentaim = false;
bool autoshoot = false;
int bone = 5;


void colors()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	style->WindowBorderSize = 0;
	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 1;

	style->FramePadding = ImVec2(8, 6);

	style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	style->Colors[ImGuiCol_WindowBg] = ImColor(24, 24, 24);

	style->Colors[ImGuiCol_TitleBg] = ImColor(44, 44, 48);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(44, 44, 48);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(44, 44, 48);

	style->Colors[ImGuiCol_Button] = ImColor(44, 44, 48);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(44, 44, 48);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(44, 44, 48);

	style->Colors[ImGuiCol_CheckMark] = ImColor(144, 16, 12);

	style->Colors[ImGuiCol_FrameBg] = ImColor(44, 44, 48);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(44, 44, 48);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(44, 44, 48);

	style->Colors[ImGuiCol_SliderGrab] = ImColor(144, 16, 12);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(144, 16, 12);




	//ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 15);

}


auto menu()->VOID
{

	colors();
	if (MenuIsOpen)
	{
		static int maintabs = 0;
		ImGui::SetNextWindowSize({ 450, 450 });
		if (ImGui::Begin(std::string(std::string(("apex legends"))).c_str(), 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{

			ImGui::Columns(2);
			ImGui::SetColumnOffset(1, 130);
			if (ImGui::Button("Aim", ImVec2(120 - 5, 30)))
			{
				maintabs = 1;

			}
			if (ImGui::Button("Visuals", ImVec2(120 - 5, 30)))
			{
				maintabs = 2;
			}
			if (ImGui::Button("Misc", ImVec2(120 - 5, 30)))
			{
				maintabs = 3;
			}
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
			ImGui::Text("", 50, 1, false);

		}

		ImGui::NextColumn();
		if (maintabs == 1)
		{
			ImGui::Checkbox("Aimbot", &Settings::Aimbot);
			ImGui::SliderInt("Fov Size", &Settings::AimbotFOV, 1.0f, 500.0f);
			ImGui::SliderInt("Smoothness", &Settings::AimbotSmooth, 1.0f, 10.0f);
		}

		if (maintabs == 2)
		{
			ImGui::Checkbox("Corner Box", &cornerbox);
			ImGui::Checkbox("Bounded Box", &Settings::PlayerBox);
			ImGui::Checkbox("3D Box", &Settings::Box3d);
			ImGui::Checkbox("Health", &Settings::PlayerHealth);
			//ImGui::Checkbox("Skeleton, &Settings::Skeleton);
			ImGui::Checkbox("Player Name", &Settings::PlayerName);
			ImGui::Checkbox("Snaplines", &Settings::Snaplines);
			ImGui::Checkbox("Chams", &Settings::Glow);

			ImGui::SliderInt("Vis Distance", &visdistance, 1.0f, 500.0f);
		}

		if (maintabs == 3)
		{
			ImGui::Checkbox("Draw Circle", &Settings::ShowFov);
		}
	}


}


auto render()->VOID
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Settings::ShowFov) {
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), Settings::AimbotFOV, ImColor(255, 182, 193), 100);
	}

	if (GetAsyncKeyState(VK_INSERT))
	{
		Sleep(100);
		MenuIsOpen = !MenuIsOpen;
	}

	menu();
	ActorLoop();

	ImGui::Render();
	ImVec4 clear_color = ImVec4(0., 0., 0., 0.);
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present(1, 0);
}

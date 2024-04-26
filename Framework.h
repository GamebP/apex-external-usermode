#pragma once
#include <io.h>
#include "comms.h"

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



auto menu()->VOID
{
	static ImVec2 pos(250, 250);
	if (ZeroGui::begin_window(0, pos, ImVec2(585.0f, 425.0f), MenuIsOpen, true))
	{
		ZeroGui::draw_filled_rect(ImVec2(pos.x - 2, pos.y - 2), 589.0f, 429.0f, ZeroGui::MenuColors::Window_Header);

		ZeroGui::draw_filled_rect(ImVec2(pos.x, pos.y), 585.0f, 425.0f, ZeroGui::MenuColors::Window_Background);
		ZeroGui::draw_filled_rect(ImVec2(pos.x, pos.y), 585.0f, 25.0f, ZeroGui::MenuColors::Window_Header);

		ImVec2 titlePos = ImVec2(pos.x + 585.0f / 2, pos.y + 25 / 2);
		ZeroGui::text_center(_("safecall apex legends" ), titlePos, IM_COL32(255, 255, 255, 255));

		ZeroGui::text_normal(_(""));

		ZeroGui::next_column(30.0f);
		if (ZeroGui::button_tab(_("Main"), ImVec2(110, 25), tab == 1)) tab = 1;

		ZeroGui::next_column(165.0f);
		if (ZeroGui::button_tab(_("Visuals"), ImVec2(110, 25), tab == 2)) tab = 2;

		ZeroGui::next_column(300.0f);
		if (ZeroGui::button_tab(_("Misc"), ImVec2(110, 25), tab == 3)) tab = 3;

		ZeroGui::next_column(435.0f);
		if (ZeroGui::button_tab(_("Loot"), ImVec2(110, 25), tab == 4)) tab = 4;

		ZeroGui::next_column(435.0f);
		if (ZeroGui::button_tab(_("Exploits"), ImVec2(110, 25), tab == 4)) tab = 4;


		if (tab == 1)
		{
			ZeroGui::next_column(30.0f);
			ZeroGui::text_normal(_(" "));

			ZeroGui::check_box(_("Aimbot"), &Settings::Aimbot);
			ZeroGui::zero_slider(_("Smoothness"), &Settings::AimbotSmooth, 1.0f, 10.0f);

			//add hitboxes
		}

		if (tab == 2)
		{
			ZeroGui::next_column(30.0f);
			ZeroGui::text_normal(_(" "));

			ZeroGui::check_box(_("3D Box"), &Settings::Box3d);
			ZeroGui::check_box(_("Snaplines"), &Settings::Snaplines);
			ZeroGui::check_box(_("Chams (glow)"), &Settings::Glow);
			ZeroGui::zero_slider(_("Distance"), &visdistance, 1.0f, 250);
			//ZeroGui::zero_slider(_("Esp Distance"), &visdistance, 1.0f, 250.f);
			//ZeroGui::check_box(_("Bounded box"), &Settings::PlayerBox);
			//ZeroGui::check_box(_("Health"), &Settings::PlayerHealth);

		}

		if (tab == 3)
		{
			ZeroGui::next_column(30.0f);
			ZeroGui::text_normal(_(" "));


			ZeroGui::check_box(_("FOV"), &Settings::ShowFov);
			ZeroGui::check_box(_("FOV Filled"), &Settings::filledfov);
			ZeroGui::zero_slider(_("Fov Circle"), &Settings::AimbotFOV, 1.0f, 500.0f);
		}

		if (tab == 4)
		{
			ZeroGui::next_column(30.0f);
			ZeroGui::text_normal(_(" "));
			ZeroGui::check_box(_("Enable Exploits"), &exploits);
			if (exploits)
			{
				ZeroGui::check_box(_("ThirdPerson"), &thirdpersom);
				ZeroGui::check_box(_("Infinite Charge Rifle (LMOUSE)"), &infrifle);
				ZeroGui::check_box(_("AirStuck (SHIFT)"), &airstuck);
			}
		}

		ZeroGui::draw_filled_rect(ImVec2(ZeroGui::cursor_pos().x, ZeroGui::cursor_pos().y), 8, 8, IM_COL32(63.75, 63.75, 63.75, 255));

		if (SonyDriverHelper::api::GetKey(VK_LBUTTON))
		{
			ZeroGui::Input::mouseDown[0] = true;
			ZeroGui::Input::mouseDown[1] = true;
		}
		else
		{
			ZeroGui::Input::mouseDown[0] = false;
			ZeroGui::Input::mouseDown[1] = false;
		}
	}

	ZeroGui::zero_render();

	ImGui::End();
	
}


auto render()->VOID
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Settings::ShowFov) {
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), Settings::AimbotFOV, ImColor(124, 252, 0), 100);
	}

	if (Settings::filledfov)
	{
		ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), Settings::AimbotFOV, ImColor(0, 0, 0, 110), 100);
	}

	if (SonyDriverHelper::api::GetKey(VK_INSERT))
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

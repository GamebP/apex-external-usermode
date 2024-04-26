#pragma once

static int visdistance = 300.0f;
bool cornerbox = false;
bool crosshair = false;
bool streamproof = false;
bool ThirdPerson = false;
bool fovcircle = false;

int Width;
int Height;


void __cdecl WorldCache(void*) {
	while (true) {
		Addresses::ViewRenderer = readd<uint64_t>(baseaddy + Offsets::ViewRender);
		Addresses::ViewMatrix = readd<uint64_t>(Addresses::ViewRenderer + Offsets::ViewMatrix);
		Addresses::LocalPlayer = readd<uintptr_t>(baseaddy + Offsets::Local_Player);
		Addresses::NameList = readd<uintptr_t>(baseaddy + Offsets::NameList);
	}
	Sleep(1);
}

float ToMeters(float x)
{
	return x / 39.62f;
}


auto ActorLoop()->VOID {
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(15, 15), ImColor(255, 255, 255, 255), _("Damage Apex Legends"));
	for (int i = 0; i < 65; i++)
	{
		uintptr_t Entity = GetEntityById(i);
		float closestDistance = FLT_MAX;
		uintptr_t closestPawn = NULL;
		if (Entity == 0)
			continue;
		uintptr_t EntityHandle = readd<uintptr_t>(Entity + Offsets::M_IName);
		std::string Identifier = readd<std::string>(EntityHandle);
		LPCSTR IdentifierC = Identifier.data();
		if (strcmp(IdentifierC, _("player")))
		{
			Vector3 head2 = GetEntityBonePosition(Entity, 8);  Vector3 Head = ProjectWorldToScreen(head2); if (Head.z <= 0.f) continue;
			Vector3 neck2 = GetEntityBonePosition(Entity, 7); Vector3 Neck = ProjectWorldToScreen(neck2); if (Neck.z <= 0.f) continue;
			Vector3 chest2 = GetEntityBonePosition(Entity, 5); Vector3 Chest = ProjectWorldToScreen(chest2); if (Chest.z <= 0.f) continue;
			Vector3 hips2 = GetEntityBonePosition(Entity, 0); Vector3 Hips = ProjectWorldToScreen(hips2); if (Hips.z <= 0.f) continue;
			Vector3 bottom2 = readd<Vector3>(Entity + Offsets::M_VecAbsOrigin);  Vector3 Bottom = ProjectWorldToScreen(bottom2);
			Vector3 localpos = readd<Vector3>(Addresses::LocalPlayer + Offsets::M_VecAbsOrigin);


			float dist = readd<Vector3>(Offsets::Local_Player + 0x14c).DistTo(head2);
			float distM = ToMeters(dist);



			int EntityDBNO = readd<int>(Entity + Offsets::M_BleedOutState);
			int PlayerTeamIndex = readd<int>(Addresses::LocalPlayer + Offsets::M_ITeamNum);
			int EntityTeamIndex = readd<int>(Entity + Offsets::M_ITeamNum);
			int EntityHealth = readd<int>(Entity + Offsets::m_iHealth);

			if (isVisible(Entity, i)) {
				Settings::ESP = Settings::Visible;
			}
			else {
				Settings::ESP = Settings::Invisible;
			}


			float ActorHeight = (float)(Head.y - Bottom.y);
			float ActorWidth = ActorHeight * 0.780;

			if (distM <= visdistance)
			{

				if (EntityTeamIndex != PlayerTeamIndex && !EntityDBNO && EntityHealth != 0)
				{

					if (Settings::PlayerBox)
					{
						float box_height = (head2.y - head2.z);
						float box_width = box_height / 1.2f;

						if (Settings::BoundedBox)
							Render_2D_Box(Bottom.x + (ActorWidth / 2), Bottom.y, Head.x - (ActorWidth / 2), Head.y, Settings::ESP, 1.5);
					}
					if (Settings::Glow)
					{
						Writee<int>(Entity + Offsets::Glow_Enabled, 1);
						Writee<int>(Entity + Offsets::Glow_Through_Walls, 2);
						Writee<GlowMode>(Entity + Offsets::Glow_Type, { 101,101,46, 90 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
						if (isVisible(Entity, i)) {
							Writee<float>(Entity + 0x1D0, 0);//r
							Writee<float>(Entity + 0x1D4, 255);//g						
							Writee<float>(Entity + 0x1D8, 0);//b
						}
						else {
							Writee<float>(Entity + 0x1D0, 255);
							Writee<float>(Entity + 0x1D4, 0);
							Writee<float>(Entity + 0x1D8, 0);
						}
					}
					if (Settings::Snaplines)
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y - 100), ImVec2(Bottom.x, Bottom.y), Settings::ESP);
					}
					if (Settings::PlayerHealth)
					{
						float ActorHeight = (float)(Head.y - Bottom.y);
						int Health = readd<int>(Entity + Offsets::m_iHealth);
						int Shield = readd<int>(Entity + Offsets::m_shieldHealth);
						int MaxShield = readd<int>(Entity + Offsets::m_shieldHealthMax);
						int ShieldType = readd<int>(Entity + Offsets::CPlayerarmorType);
						std::string health = std::to_string(Health).c_str();
						auto size = ImGui::CalcTextSize(health.c_str());
						SeerHealth(Head.x - size.x / 2.f, Head.y - 20, Shield, MaxShield, ShieldType, Health);
					}
					if (Settings::PlayerName)
					{
						//int afwehuoi = (int)(dist(localpos, bottom2) / 100.f);
						char* name = GetPlayerName(Entity);

						std::string ballsesp = name;

						auto size = ImGui::CalcTextSize(ballsesp.c_str());

						ImGui::GetBackgroundDrawList()->AddText(ImVec2(Head.x - size.x / 2.f, Head.y - 20), ImColor(255, 182, 193), ballsesp.c_str());
					}



					if (Settings::Box3d)
					{
						Render_Head_Angle(bottom2.x, bottom2.y, bottom2.z, head2.x, head2.y, head2.z);
					}

					if (Settings::SkydiveSpeed)
					{
						bool isDiving = readd<bool>(Addresses::LocalPlayer + 0x45e8);
						if (isDiving)
						{
							//Writee<float>(Addresses::LocalPlayer + 0x45ec, 2500.0f);
						}
					}
					if (Settings::Aimbot) {
						auto dx = Head.x - (ImGui::GetIO().DisplaySize.x / 2);
						auto dy = Head.y - (ImGui::GetIO().DisplaySize.y / 2);
						auto dist = sqrtf(dx * dx + dy * dy) / 100.0f;
						if (dist < Settings::AimbotFOV && dist < closestDistance) {
							closestDistance = dist;
							closestPawn = Entity;
						}
					}
				}

			}
			if (Settings::Aimbot && GetAsyncKeyState(VK_RBUTTON) && isVisible(Entity, i)) {
				if ((GetCrossDistance(Head.x, Head.y, ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2) <= Settings::AimbotFOV)) {
					Aimbot(closestPawn, Head.x, Head.y);
				}
			}
		}
	}
}
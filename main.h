#pragma once
#include "comms.h"
#include "offsets2.h"



static int visdistance = 250.f;
int loot_distance = 150;
bool cornerbox = false;
bool crosshair = false;
bool streamproof = false;
bool ThirdPerson = false;
bool fovcircle = false;

bool exploits;
bool thirdpersom;
bool airstuck;
bool infrifle;


int Width;
int Height;


void __cdecl WorldCache(void*) {
	while (true) {
		Addresses::ViewRenderer = vmread<uint64_t>(module + Offsets::ViewRender);
		Addresses::ViewMatrix = vmread<uint64_t>(Addresses::ViewRenderer + Offsets::ViewMatrix);
		Addresses::LocalPlayer = vmread<uintptr_t>(module + Offsets::Local_Player);
		Addresses::NameList = vmread<uintptr_t>(module + Offsets::NameList);
	}
	Sleep(1);
}

float ToMeters(float x)
{
	return x / 39.62f;
}




auto ActorLoop()->VOID {
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(15, 15), ImColor(255, 255, 255, 255), _("safecall apex legends"));
	for (int i = 0; i < 65; i++)
	{

		uintptr_t Entity = GetEntityById(i);

		float closestDistance = FLT_MAX;
		uintptr_t closestPawn = NULL;
		if (Entity == 0)
			continue;
		uintptr_t EntityHandle = vmread<uintptr_t>(Entity + Offsets::M_IName);
		std::string Identifier = vmread<std::string>(EntityHandle);
		LPCSTR IdentifierC = Identifier.data();
		if (strcmp(IdentifierC, _("player")))
		{
			Vector3 head2 = GetEntityBonePosition(Entity, 8);  Vector3 Head = ProjectWorldToScreen(head2); if (Head.z <= 0.f) continue;
			Vector3 neck2 = GetEntityBonePosition(Entity, 7); Vector3 Neck = ProjectWorldToScreen(neck2); if (Neck.z <= 0.f) continue;
			Vector3 chest2 = GetEntityBonePosition(Entity, 5); Vector3 Chest = ProjectWorldToScreen(chest2); if (Chest.z <= 0.f) continue;
			Vector3 hips2 = GetEntityBonePosition(Entity, 0); Vector3 Hips = ProjectWorldToScreen(hips2); if (Hips.z <= 0.f) continue;
			Vector3 bottom2 = vmread<Vector3>(Entity + Offsets::M_VecAbsOrigin);  Vector3 Bottom = ProjectWorldToScreen(bottom2);
			Vector3 localpos = vmread<Vector3>(Addresses::LocalPlayer + Offsets::M_VecAbsOrigin);


			float dist = vmread<Vector3>(Offsets::Local_Player + 0x14c).DistTo(head2);
			float distM = ToMeters(dist);
			int EntityDBNO = vmread<int>(Entity + Offsets::M_BleedOutState);
			int PlayerTeamIndex = vmread<int>(Addresses::LocalPlayer + Offsets::M_ITeamNum);
			int EntityTeamIndex = vmread<int>(Entity + Offsets::M_ITeamNum);
			int EntityHealth = vmread<int>(Entity + Offsets::m_iHealth);

			if (exploits)
			{
				if (thirdpersom)
				{
					vmwrite<int>(module + Offsets::thirdperson_override, true);
					vmwrite<int>(Entity + Offsets::m_thirdPersonShoulderView, true);
				}
				else
				{
					vmwrite<int>(module + Offsets::thirdperson_override, false);
					vmwrite<int>(Entity + Offsets::m_thirdPersonShoulderView, false);
				}

				if (airstuck)
				{
					if (SonyDriverHelper::api::GetKey(VK_SHIFT))
					{
						vmwrite<float>(module + 0x014ad600 + 0x68, 0); //host_timescale
					}
					else
					{
						vmwrite<float>(module + 0x014ad600 + 0x68, 1); //host_timescale
					}
				}

				if (infrifle)
				{
					DWORD64 EntityNerds = vmread<DWORD64>(module + Offsets::Local_Player);
					DWORD64 LastWeapon = vmread<DWORD64>(EntityNerds + Offsets::CurrentWeapon) & 0xFFFF;
					DWORD64 Weapon = vmread<DWORD64>(module + Offsets::Cl_EntityList + (LastWeapon << 5));
					float BulletSpeed = vmread<float>(Weapon + 0x1ef0); // CWeaponX!m_flProjectileSpeed
					int ammoInclip = vmread<int>(Weapon + Offsets::m_ammoInClip);
					if (ammoInclip != 0 && BulletSpeed == 1 && SonyDriverHelper::api::GetKey(VK_LBUTTON)) {
						vmwrite<float>(module + 0x014ad600 + 0x68, 0.000000000000000000000000000000000000000001);  //host_timescale
					}
					else
					{
						vmwrite<float>(module + 0x014ad600 + 0x68, 1);
					}
				}
			}

			if (isVisible(Entity, i)) {
				Settings::ESP = Settings::Visible;
			}
			else {
				Settings::ESP = Settings::Invisible;
			}


			float ActorHeight = (float)(Head.y - Bottom.y);
			float ActorWidth = ActorHeight * 0.780;

			//if (distM <= visdistance)
			//{
				if (EntityTeamIndex != PlayerTeamIndex) //!EntityDBNO && EntityHealth != 0
				{
					if (Settings::Glow)
					{
						vmwrite<int>(Entity + Offsets::Glow_Enabled, 1);
						vmwrite<int>(Entity + Offsets::Glow_Through_Walls, 2);
						vmwrite<GlowMode>(Entity + Offsets::Glow_Type, { 101,101,46, 90 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
						if (isVisible(Entity, i)) {
							vmwrite<float>(Entity + 0x1D0, 0);//r
							vmwrite<float>(Entity + 0x1D4, 255);//g						
							vmwrite<float>(Entity + 0x1D8, 0);//b
						}
						else {
							vmwrite<float>(Entity + 0x1D0, 255);
							vmwrite<float>(Entity + 0x1D4, 0);
							vmwrite<float>(Entity + 0x1D8, 255);
						}
					}

					if (Settings::Snaplines)
					{
						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y - 100), ImVec2(Bottom.x, Bottom.y), Settings::ESP);
					}

					if (Settings::PlayerHealth)
					{
						float ActorHeight = (float)(Head.y - Bottom.y);
						int Health = vmread<int>(Entity + Offsets::m_iHealth);
						int Shield = vmread<int>(Entity + Offsets::m_shieldHealth);
						int MaxShield = vmread<int>(Entity + Offsets::m_shieldHealthMax);
						int ShieldType = vmread<int>(Entity + Offsets::CPlayerarmorType);
						std::string health = std::to_string(Health).c_str();
						auto size = ImGui::CalcTextSize(health.c_str());
						SeerHealth(Head.x - size.x / 2.f, Head.y - 20, Shield, MaxShield, ShieldType, Health);
					}

					if (Settings::Box3d)
					{
						Render_Head_Angle(bottom2.x, bottom2.y, bottom2.z, head2.x, head2.y, head2.z);
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

			if (Settings::Aimbot && SonyDriverHelper::api::GetKey(VK_RBUTTON) && isVisible(Entity, i)) {
				if ((GetCrossDistance(Head.x, Head.y, ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2) <= Settings::AimbotFOV)) {
					Aimbot(closestPawn, Head.x, Head.y);
				}
			}

		}
	}
}
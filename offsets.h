#pragma once
#include "comms.h"

struct CUserCmd {
	int command_number; //0x0000
	int tick_count; //0x0004
	float command_time; //0x0008
	Vector3 viewangles; //0x000C
	BYTE pad_0x0018[0x14]; //0x0018
	float forwardmove; //0x002C
	float sidemove; //0x0030
	float upmove; //0x0034
	int buttons; //0x0038
	BYTE impulse; //0x003C
};

struct visibleTime
{
	float lastTime[100];
	bool lastState[100];
	uintptr_t lastCheck[100];
} lastVis;

namespace Offsets {

	static DWORD Local_Player = 0x1ebf488;//updated
	static DWORD Cl_EntityList = 0x1b0e678;//updated
	static DWORD Local_Entity_Handle = 0x1352c7c;//updated
	static DWORD M_VecAbsOrigin = 0x14c;//updated
	static DWORD M_bConstrainBetweenEndpoints = 0xEE0; //m_nForceBone + 0x48 updated
	static DWORD M_IName = 0x0589; // updated
	static DWORD m_scriptNameIndex = 0x0690;// updated
	static DWORD m_nSkin = 0x0e50; // updated
	static DWORD ViewMatrix = 0x11a210;//updated
	static DWORD ViewRender = 0x7641830;//updated
	static DWORD lastVisibleTime = 0x1a70;//updated
	static DWORD M_ITeamNum = 0x044c;//updated
	static DWORD M_BleedOutState = 0x2718;//updated
	static DWORD M_IWorldModelIndex = 0x1658;//updated
	static DWORD Glow_Type = 0x1D0;//updated
	static DWORD Glow_Enabled = 0x3c8;//updated
	static DWORD Glow_Through_Walls = 0x3d0;//updated
	static DWORD m_highlightFunctionBits = 0x02c0; //item_glow
	static DWORD m_ActiveState = 0x166c; //updated may be
	static DWORD CurrentWeapon = 0x1a14; //m_latestPrimaryWeapons //updated
	static DWORD m_ammoInClip = 0x1644;//updated
	static DWORD m_iHealth = 0x043c;//updated
	static DWORD m_iMaxHealth = 0x0578;//updated
	static DWORD m_shieldHealth = 0x0170;//updated
	static DWORD m_shieldHealthMax = 0x0174;//updated
	static DWORD CPlayerarmorType = 0x460c;//updated
	static DWORD m_thirdPersonShoulderView = 0x36a0;//updated
	static DWORD thirdperson_override = 0x01af3520;//updated
	static DWORD m_playerMoveSpeedScale = 0x31ec; // updated
	static DWORD m_flModelScale = 0x0f08; // updated
	static DWORD m_skydiveIsDiving = 0x4680; // updated
	static DWORD NameList = 0xbc1c040; // updated
}


//static DWORD Local_Player = 0x1ebf488; //done
//static DWORD Cl_EntityList = 0x1b0e678; //done
//static DWORD Local_Entity_Handle = 0x1352c7c; //done
//static DWORD M_VecAbsOrigin = 0x014c; //done
//static DWORD M_bConstrainBetweenEndpoints = 0x0e98 + 0x48; //m_nForceBone + 0x48 updated
//
//static DWORD M_IName = 0x0589; //done
//static DWORD BulletSpeed = 0x1ef0; //CWeaponX!m_flProjectileSpeed
//static DWORD m_scriptNameIndex = 0x0690; //done
//static DWORD m_nSkin = 0x0e50; //done
//static DWORD ViewMatrix = 0x11a210; //done
//static DWORD ViewRender = 0x763f830; //done
//static DWORD lastVisibleTime = 0x1a70; //done
//static DWORD M_ITeamNum = 0x044c; //done
//static DWORD M_BleedOutState = 0x2718; //done
//static DWORD M_IWorldModelIndex = 0x1658; //done
//
//static DWORD Glow_Type = 0x1D0;//updated
//static DWORD Glow_Enabled = 0x3c8;//updated
//static DWORD Glow_Through_Walls = 0x3d0;//updated
//
//static DWORD m_highlightFunctionBits = 0x02c0; //item_glow //done
//static DWORD m_ActiveState = 0x166c; //done
//static DWORD CurrentWeapon = 0x1a14; //m_latestPrimaryWeapons //done
//static DWORD m_ammoInClip = 0x1644; //done
//static DWORD m_iHealth = 0x043c; //done
//static DWORD m_iMaxHealth = 0x0578; //done
//static DWORD m_shieldHealth = 0x0170; //done
//static DWORD m_shieldHealthMax = 0x0174; //done
//static DWORD CPlayerarmorType = 0x460c; //done CPlayer!armorType=0x460c
//static DWORD m_thirdPersonShoulderView = 0x36a0; //done
//static DWORD thirdperson_override = 0x01af3520 + 0x6c; //done
//static DWORD m_playerMoveSpeedScale = 0x31ec; //done
//static DWORD m_flModelScale = 0x0f08; //done
//static DWORD m_skydiveIsDiving = 0x4680; //done
//static DWORD NameList = 0xbbd1840;  //done

static double GetCrossDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

uintptr_t GetEntityById(int Ent)
{
	uintptr_t EntityList = module + Offsets::Cl_EntityList;
	uintptr_t BaseEntity = vmread<DWORD64>(EntityList);
	if (!BaseEntity)
		return NULL;
	return vmread<uintptr_t>(EntityList + (Ent << 5));
}

Vector3 ProjectWorldToScreen(const struct Vector3 pos) {
	Matrix M = vmread<Matrix>(Addresses::ViewMatrix);
	struct Vector3 out;
	float _x = M.matrix[0] * pos.x + M.matrix[1] * pos.y + M.matrix[2] * pos.z + M.matrix[3];
	float _y = M.matrix[4] * pos.x + M.matrix[5] * pos.y + M.matrix[6] * pos.z + M.matrix[7];
	out.z = M.matrix[12] * pos.x + M.matrix[13] * pos.y + M.matrix[14] * pos.z + M.matrix[15];

	_x *= 1.f / out.z;
	_y *= 1.f / out.z;

	out.x = ImGui::GetIO().DisplaySize.x * .5f;
	out.y = ImGui::GetIO().DisplaySize.y * .5f;

	out.x += 0.5f * _x * ImGui::GetIO().DisplaySize.x + 0.5f;
	out.y -= 0.5f * _y * ImGui::GetIO().DisplaySize.y + 0.5f;

	return out;
}

Vector3 GetEntityBonePosition(uintptr_t ent, int boneIndex)
{
	Bone bone = {};
	Vector3 vec_bone = Vector3();
	Vector3 BasePosition = vmread<Vector3>(ent + Offsets::M_VecAbsOrigin);
	Vector3 pos = BasePosition;

	ULONG64 bone_array = vmread<ULONG64>(ent + Offsets::M_bConstrainBetweenEndpoints);
	ULONG64 bone_location = (boneIndex * 0x30);

	bone = vmread<Bone>(bone_array + bone_location);
	vec_bone.x = bone.x + pos.x;
	vec_bone.y = bone.y + pos.y;
	vec_bone.z = bone.z + pos.z;
	return vec_bone;
}

char* GetPlayerName(uintptr_t ent)
{
	uintptr_t nameIndex = vmread<uintptr_t>(ent + 0x38);
	uintptr_t nameOffset = vmread<uintptr_t>((module + Offsets::NameList) + ((nameIndex - 1) << 4));
	char buf[64] = { 0 };
	//vmreadProcessMemory(nameOffset, &buf, 64);
	return buf;
}


uintptr_t GetMs()
{
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount64();
	}
}
inline bool isVisible(DWORD64 Entity, int index)
{
	if (GetMs() >= (lastVis.lastCheck[index] + 10))
	{
		float visTime = vmread<float>(Entity + Offsets::lastVisibleTime);
		lastVis.lastState[index] = visTime > lastVis.lastTime[index] || visTime < 0.f && lastVis.lastTime[index] > 0.f;
		lastVis.lastTime[index] = visTime;
		lastVis.lastCheck[index] = GetMs();
	}
	return lastVis.lastState[index];
}

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

inline void SetMouseAbsPosition(DWORD x, DWORD y)
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.dx = x;
	input.mi.dy = y;
	SendInput(1, &input, sizeof(input));
}

struct AimContext {
	int crosshairX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int crosshairY = GetSystemMetrics(SM_CYSCREEN) / 2;
	int closestX = 0;
	int closestY = 0;
	int closestZ = 0;
	float entNewVisTime = 0;
	float entOldVisTime[100];
	int visCooldownTime[100];
};

inline void AimbotChecks(DWORD64 Entity, AimContext* Ctx, int index) {
	Vector3 head = GetEntityBonePosition(Entity, 8);
	Vector3 head2 = ProjectWorldToScreen(head); if (head2.z <= 0.f) return;

	int entX = head2.x;
	int entY = head2.y;
	int entZ = head2.z;
	Ctx->entNewVisTime = vmread<float>(Entity + Offsets::lastVisibleTime);

	if (Ctx->entNewVisTime != Ctx->entOldVisTime[index])
	{
		Ctx->visCooldownTime[index] = 24;

		if (abs(Ctx->crosshairX - entX) < abs(Ctx->crosshairX - Ctx->closestX) && abs(Ctx->crosshairX - entX) < Settings::AimbotFOV && abs(Ctx->crosshairY - entY) < abs(Ctx->crosshairY - Ctx->closestY) && abs(Ctx->crosshairY - entY) < Settings::AimbotFOV)
		{
			Ctx->closestX = entX;
			Ctx->closestY = entY;
			Ctx->closestZ = entZ;
		}
		Ctx->entOldVisTime[index] = Ctx->entNewVisTime;
	}
	if (Ctx->visCooldownTime[index] >= 0) Ctx->visCooldownTime[index] -= 1;

}

inline void AimbotMove(AimContext* Ctx, float X, float Y) {

	if (Ctx->closestX != 50000 && Ctx->closestY != 50000)
	{
		if (SonyDriverHelper::api::GetKey(VK_RBUTTON))
		{

			X = (Ctx->closestX - Ctx->crosshairX) / 2;
			Y = (Ctx->closestY - Ctx->crosshairY) / 2;

			CURSORINFO ci = { sizeof(CURSORINFO) };
			if (GetCursorInfo(&ci))
			{
				if (ci.flags == 0)
					SetMouseAbsPosition(static_cast<DWORD>(X / Settings::AimbotSmooth), static_cast<DWORD>(Y / Settings::AimbotSmooth));
			}
		}
	}
}

inline void Aimbot(DWORD64 Entity, float X, float Y) {
	AimContext aimCtx; aimCtx.closestX = 50000; aimCtx.closestY = 50000;
	for (int i = 0; i < 100; i++)
	{
		AimbotChecks(Entity, &aimCtx, i);
	}
	if (SonyDriverHelper::api::GetKey(VK_RBUTTON)) {
		CURSORINFO ci = { sizeof(CURSORINFO) };
		if (GetCursorInfo(&ci))
		{
			if (ci.flags == 0)
			AimbotMove(&aimCtx, X, Y);
		}
	}
}

class vec3_t {
public:
	vec3_t();
	vec3_t(float, float, float);
	~vec3_t();

	float x, y, z;

	vec3_t& operator+=(const vec3_t& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}
	vec3_t& operator-=(const vec3_t& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	vec3_t& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}
	bool operator==(const vec3_t& v) {
		return (x == v.x) && (y == v.y);
	}
	vec3_t operator+(const vec3_t& v) {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}
	vec3_t operator-(const vec3_t& v) {
		return vec3_t{ x - v.x, y - v.y, z - v.z };
	}
	vec3_t operator*(float fl) const {
		return vec3_t(x * fl, y * fl, z * fl);
	}
	vec3_t operator*(const vec3_t& v) const {
		return vec3_t(x * v.x, y * v.y, z * v.z);
	}
	vec3_t& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	auto operator-(const vec3_t& other) const -> vec3_t {
		auto buf = *this;

		buf.x -= other.x;
		buf.y -= other.y;
		buf.z -= other.z;

		return buf;
	}

	auto operator/(float other) const {
		vec3_t vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}
	float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline float Length2D() const
	{
		return sqrt((x * x) + (y * y));
	}
	void crossproduct(vec3_t v1, vec3_t v2, vec3_t cross_p) const //ijk = xyz
	{
		cross_p.x = (v1.y * v2.z) - (v1.z * v2.y); //i
		cross_p.y = -((v1.x * v2.z) - (v1.z * v2.x)); //j
		cross_p.z = (v1.x * v2.y) - (v1.y * v2.x); //k
	}
	vec3_t Cross(const vec3_t& vOther) const
	{
		vec3_t res;
		crossproduct(*this, vOther, res);
		return res;
	}

	inline bool is_Zero() {
		return (x == 0) && (y == 0) && (z == 0);
	}

	void init(float ix, float iy, float iz);
	vec3_t clamp();
	vec3_t clamped();
	vec3_t normalized();
	float normalize_float();
	float distance_to(const vec3_t& other);
	void normalize();
	float length();
	float length_2d_sqr(void) const;
	float dot(const vec3_t other);
	float dot(const float* other);
};


class HitBoxManager
{
private:

public:

	Vector3 ScreenHeadBone{};
	Vector3 ScreenNeckBone{};
	Vector3 ScreenChestBone{};
	Vector3 ScreenWaistBone{};
	Vector3 ScreenBotmBone{};

	Vector3 ScreenLeftshoulderBone{};
	Vector3 ScreenLeftelbowBone{};
	Vector3 ScreenLeftHandBone{};
	Vector3 ScreenRightshoulderBone{};
	Vector3 ScreenRightelbowBone{};
	Vector3 ScreenRightHandBone{};

	Vector3 ScreenLeftThighsBone{};
	Vector3 ScreenLeftkneesBone{};
	Vector3 ScreenLeftlegBone{};
	Vector3 ScreenRightThighsBone{};
	Vector3 ScreenRightkneesBone{};
	Vector3 ScreenRightlegBone{};
};
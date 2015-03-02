#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "../Math Lib/MathLib.h"

class Rand
{
private:
	THREADLOCAL static u32 ms_Seed1;
	THREADLOCAL static u32 ms_Seed2;

public:
	Rand();// = delete;
	~Rand();// = delete;

	static void Seed(u32 seed = 1);

	static s32 Draw();
	static s32 Draw(s32 nMin, s32 nMax);
	static float Draw(float fMin, float fMax);

	static float DrawNormalized();

	static bool DrawBool();

	static Vec2V_Out Draw2();
	static Vec2V_Out Draw2(float fMin, float fMax);
	static Vec2V_Out Draw2(Vec2V_In vMin, Vec2V_In vMax);

	static Vec3V_Out Draw3();
	static Vec3V_Out Draw3(float fMin, float fMax);
	static Vec3V_Out Draw3(Vec3V_In vMin, Vec3V_In vMax);

	static Vec4V_Out Draw4();
	static Vec4V_Out Draw4(float fMin, float fMax);
	static Vec4V_Out Draw4(Vec4V_In vMin, Vec4V_In vMax);
};

THREADLOCAL u32 Rand::ms_Seed1 = 0x1A2B3C4DU;
THREADLOCAL u32 Rand::ms_Seed2 = 0x2F01BEEFU;

void Rand::Seed(u32 seed)
{
	ms_Seed1 = (seed & 0xFFFFFFFE) + 1; // See Can't be 0
	ms_Seed2 = ms_Seed1 ^ U32_MAX;
}

s32 Rand::Draw()
{
	u64 mulWithCarry = (u64)1557985959 * ms_Seed1 + ms_Seed2;

	ms_Seed1 = mulWithCarry & U32_MAX;
	ms_Seed2 = (mulWithCarry >> 32) & U32_MAX;

	return ms_Seed1;
}

s32 Rand::Draw(s32 nMin, s32 nMax)
{
	return s32((DrawNormalized() * (nMax - nMin)) + nMin);
}

float Rand::Draw(float fMin, float fMax)
{
	return (DrawNormalized() * (fMax - fMin)) + fMin;
}

float Rand::DrawNormalized()
{
									// 1.0f/((2^23)-1)
	return (Draw() & ((1<<23)-1)) * 1.1920930376163765926810017443897e-7f;
}

bool Rand::DrawBool()
{
	return (Draw() & 1);
}

Vec2V_Out Rand::Draw2()
{
	s32 seed1 = Draw();
	s32 seed2 = ms_Seed2;
	return Vec2VInt(seed1, (seed1 & 0x55555555) | (seed2 & 0xAAAAAAAA));
}

Vec2V_Out Rand::Draw2(float fMin, float fMax)
{
	return Draw2(Vec2V(fMin), Vec2V(fMax));
}

Vec2V_Out Rand::Draw2(Vec2V_In vMin, Vec2V_In vMax)
{
	Vec2V oneOver2Pow23MinusOne(1.1920930376163765926810017443897e-7f);

	Vec2V floatMask(IntToFloat(Draw2() & Vec2VInt((1<<23)-1)) * oneOver2Pow23MinusOne);
	return floatMask * (vMax - vMin) + vMin;
}

Vec3V_Out Rand::Draw3()
{
	s32 seed1 = Draw();
	s32 seed2 = ms_Seed2;
	return Vec3VInt(seed1, (seed1 & 0x55555555) | (seed2 & 0xAAAAAAAA), seed2);
}

Vec3V_Out Rand::Draw3(float fMin, float fMax)
{
	return Draw3(Vec3V(fMin), Vec3V(fMax));
}

Vec3V_Out Rand::Draw3(Vec3V_In vMin, Vec3V_In vMax)
{
	Vec3V oneOver2Pow23MinusOne(1.1920930376163765926810017443897e-7f);
	Vec3V maskedDraw(Draw3() & Vec3VInt((1<<23)-1));
	Vec3V floatMask(IntToFloat(maskedDraw) * oneOver2Pow23MinusOne);
	return floatMask * (vMax - vMin) + vMin;
}

Vec4V_Out Rand::Draw4()
{
	s32 seed1 = Draw();
	s32 seed2 = ms_Seed2;
	return Vec4VInt(seed1, (seed1 & 0x55555555) | (seed2 & 0xAAAAAAAA), seed2, (seed1 & 0xAAAAAAAA) | (seed2 & 0x55555555));
}

Vec4V_Out Rand::Draw4(float fMin, float fMax)
{
	return Draw4(Vec4V(fMin), Vec4V(fMax));
}

Vec4V_Out Rand::Draw4(Vec4V_In vMin, Vec4V_In vMax)
{
	Vec4V oneOver2Pow23MinusOne(1.1920930376163765926810017443897e-7f);
	Vec4V maskedDraw(Draw4() & Vec4VInt((1<<23)-1));
	Vec4V floatMask(IntToFloat(maskedDraw) * oneOver2Pow23MinusOne);
	return floatMask * (vMax - vMin) + vMin;
}

#endif // __RANDOM_H__
#include <iostream>
using namespace std;

#include <thread>

#include "Math Lib\MathLib.h"
#include "Utilities\Color.h"
#include "Utilities\Timer.h"
#include "Utilities\Thread.h"
#include "Utilities\BitSet.h"

using namespace VecElem;

//#include "../Utilities/DebugHelp.h"

#define TEST_SETS 1
#define TEST_REPS 1000000000

int TstThreadProc(void* pArgs);
void TstThreadCallback(int retVal, void* pArgs);

struct TstThreadArgs
{
	char operation[256];
	char vals1[256];
	char vals2[256];

	TstThreadArgs()
	{
		for(int i = 0; i < 256; ++i)
		{
			operation[i] = rand() & 3;
			vals1[i] = (rand() & 15) + 1;
			vals1[i] = (rand() & 15) + 1;
		}
	}
};

//__forceinline u32 GetNonZeroMask(u32 x) // returns (x ? 0xffffffff : 0) without branching
//{
//	return (u32)( ( (s32)(x) | -(s32)(x) ) >> 31 );
//}
//
//float GetFloat32_FromFloat16_NoIntrinsics(u16 m_data)
//{
//	u32 e,z,s;
//
//	e = (u32)m_data << 16; // [seeeeemm.mmmmmmmm.00000000.00000000]
//	s = 0x80000000u & e;   // [s0000000.00000000.00000000.00000000]
//	e = 0x7fff0000u & e;   // [0eeeeemm.mmmmmmmm.00000000.00000000]
//	z = GetNonZeroMask(e); // all 1's if e!=0
//	z = 0x38000000u & z;
//	e = ((e >> 3) + z)|s;
//
//	return *(float*)&e; // [LHS]
//}

#define PERMUTE_TEST(x, y, z, w, vecs) \
		; \
			if(!((x < VecElem::X2 && y < VecElem::X2 && z < VecElem::X2 && w < VecElem::X2) || (x > VecElem::W1 && y > VecElem::W1 && z > VecElem::W1 && w > VecElem::W1))) \
			{ \
				Vector perResult = Permute<x, y, z, w>(vecs[0], vecs[1]); \
				cout << perResult.m128_f32[0] << ", " << perResult.m128_f32[1] << ", " << perResult.m128_f32[2] << ", " << perResult.m128_f32[3]; \
				cout << "\tExpected: " << vecs[x >> 4].m128_f32[x & 0x3] << ", " << vecs[y >> 4].m128_f32[y & 0x3] << ", " << vecs[z >> 4].m128_f32[z & 0x3] << ", " << vecs[w >> 4].m128_f32[w & 0x3] << endl; \
				Assert(perResult.m128_f32[0] == vecs[x >> 4].m128_f32[x & 0x3], ""); \
				Assert(perResult.m128_f32[1] == vecs[y >> 4].m128_f32[y & 0x3], ""); \
				Assert(perResult.m128_f32[2] == vecs[z >> 4].m128_f32[z & 0x3], ""); \
				Assert(perResult.m128_f32[3] == vecs[w >> 4].m128_f32[w & 0x3], ""); \
			}

#define W_VARIATIONS(x, y, z, vecs) \
		PERMUTE_TEST(x, y, z, X1, vecs); \
		PERMUTE_TEST(x, y, z, Y1, vecs); \
		PERMUTE_TEST(x, y, z, Z1, vecs); \
		PERMUTE_TEST(x, y, z, W1, vecs); \
		PERMUTE_TEST(x, y, z, X2, vecs); \
		PERMUTE_TEST(x, y, z, Y2, vecs); \
		PERMUTE_TEST(x, y, z, Z2, vecs); \
		PERMUTE_TEST(x, y, z, W2, vecs);



#define Z_VARIATIONS(x, y, vecs) \
		W_VARIATIONS(x, y, X1, vecs); \
		W_VARIATIONS(x, y, Y1, vecs); \
		W_VARIATIONS(x, y, Z1, vecs); \
		W_VARIATIONS(x, y, W1, vecs); \
		W_VARIATIONS(x, y, X2, vecs); \
		W_VARIATIONS(x, y, Y2, vecs); \
		W_VARIATIONS(x, y, Z2, vecs); \
		W_VARIATIONS(x, y, W2, vecs);

#define Y_VARIATIONS(x, vecs) \
		Z_VARIATIONS(x, X1, vecs); \
		Z_VARIATIONS(x, Y1, vecs); \
		Z_VARIATIONS(x, Z1, vecs); \
		Z_VARIATIONS(x, W1, vecs); \
		Z_VARIATIONS(x, X2, vecs); \
		Z_VARIATIONS(x, Y2, vecs); \
		Z_VARIATIONS(x, Z2, vecs); \
		Z_VARIATIONS(x, W2, vecs);
		

#define PERM_TEST (vecs) \
		Y_VARIATIONS(X1, vecs); \
		Y_VARIATIONS(Y1, vecs); \
		Y_VARIATIONS(Z1, vecs); \
		Y_VARIATIONS(W1, vecs); \
		Y_VARIATIONS(X2, vecs); \
		Y_VARIATIONS(Y2, vecs); \
		Y_VARIATIONS(Z2, vecs); \
		Y_VARIATIONS(W2, vecs);
		

int main()
{
	//Timer::Init();
	/*Timer timer;
	double totalTime = 0;
	Vec3f tst(1, 2, 3);
	Vec3f tst1(1, 0, 0);

	Vec4f vec1(1, 2, 3, 4);
	Vec4f vec2(1, 0, 0, 0);
	float result;*/

	//float thisVal = GetFloat32_FromFloat16_NoIntrinsics(27362);

	/*TimeCounter startTime = Timer::UpdateCounter();
	Sleep(5500);
	TimeCounter totalTime = Timer::UpdateCounter() - startTime;
	cout << "Time in S: " << totalTime.InSeconds();
	cout << "Time in MS: " << totalTime.InMilliseconds();*/

	Mat44V rotMat(INIT_IDENTITY);
	rotMat.Rotate_LocalX(DEGREES_TO_RADIANS(50.0f));

	Vector vecs[2] = { VectorSet(1.0f, 2.0f, 3.0f, 4.0f), VectorSet(5.0f, 6.0f, 7.0f, 8.0f) };

	/*PERM_TEST(vecs);*/
	//Y_VARIATIONS(X1, vecs);
	//Y_VARIATIONS(Y1, vecs);
	//Y_VARIATIONS(Z1, vecs);
	//Y_VARIATIONS(W1, vecs);
	//Y_VARIATIONS(X2, vecs);
	//Y_VARIATIONS(Y2, vecs);
	//Y_VARIATIONS(Z2, vecs);
	//Y_VARIATIONS(W2, vecs);

	/*Vector perResult = Permute<X1, X1, X2, X2>(vecs[0], vecs[1]);

	for(int i = 0; i < 4; ++i)
	{
		cout << perResult.m128_f32[i] << endl;
	}*/

	//Assert(0, "This is a Test %i, %i", 1, 2);

	//std::thread tstThread;

	//Mat44V convertTst(INIT_IDENTITY);
	//Mat44 convTst1;
	//convTst1 = MAT44V_TO_MAT44(convertTst);

	//int structSize = sizeof(Vec4f);

	//Vec4V vectst(1, 2, 3, 4);
	//Vec4V vectst2;
	//vectst2 = vectst;

	//Mat44 invMat = MAT44V_TO_MAT44(Mat44V(INIT_IDENTITY));
	//invMat.Invert();

	////invMat.GetWAxisRef().SetXYZ(vectst2.GetXYZ());

	//for(unsigned long long i = 0; i < TEST_SETS; ++i)
	//{
	//	timer.Reset();
	//	timer.StartTimer();
	//	for(unsigned long long j = 0; j < TEST_REPS; ++j)
	//	{
	//		result = vec1.MagSq();
	//	}
	//	timer.StopTimer();
	//	totalTime += timer.GetDeltaTimeD();

	//	cout << "Rep Time: " << timer.GetDeltaTimeD() << endl;
	//	cout << "Waste Time: " << result << endl;
	//}

	//cout << "Average Time: " << totalTime / TEST_SETS << endl;

	return 0;
}

int TstThreadProc(void* pArgs)
{
	TstThreadArgs* pThreadArgs = (TstThreadArgs*)pArgs;

	unsigned int finalVal = 0;

	for(int i = 0; i < 256; ++i)
	{
		switch(pThreadArgs->operation[i])
		{
		case 0:
			{
				finalVal += pThreadArgs->vals1[i] + pThreadArgs->vals2[i];
			}
			break;

		case 1:
			{
				finalVal += pThreadArgs->vals1[i] - pThreadArgs->vals2[i];
			}
			break;
		
		case 2:
			{
				finalVal += pThreadArgs->vals1[i] * pThreadArgs->vals2[i];
			}
			break;

		case 3:
			{
				finalVal += pThreadArgs->vals1[i] / pThreadArgs->vals2[i];
			}
			break;
		}
	}

	cout << "Final Val: " << finalVal << endl;
	return 0;
}

void TstThreadCallback(int retVal, void* pArgs)
{
	cout << "Thread Returned Val of :" << retVal << endl;
}
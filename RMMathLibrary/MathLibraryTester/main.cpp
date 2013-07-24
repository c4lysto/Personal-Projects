#include <iostream>
using namespace std;

//#pragma comment(lib, "RMMathLibrary")

#include <RMMath.h>
#include "Timer.h"
using namespace RMMath;

#include <vector>

#define TEST_AMT 30
#define TEST_REPS 1000000000

int main()
{
	Timer timer;

	Matrix4f mat1, mat2, mat3;
	Matrix4f mat4(1, 2, 3, 4,
				  5, 6, 7, 8,
				  9, 10, 11, 12,
				  13, 14, 15, 16);
	Matrix4f mat5(16, 15, 14, 13,
				  12, 11, 10, 9,
				  8, 7, 6, 5,
				  4, 3, 2, 1);

	mat2.Translate(0.0f, 0.0f, 1.0f);

	Vec4f yes, no;

	Matrix4f mat6(Vec4f(), Vec4f(), Vec4f(), Vec4f());

	Vec3f vec(1.0f, 1.0f, 1.0f);
	Vec3f vec1(0.0f, 1.0f, 0.0f);
	Vec3f tmpVec;
	Vec2f vec5(1.0f, 1.0f);
	Vec4f vec6(1.0f, 1.0f, 1.0f, 1.0f);

	vec1 = -vec1;

	double deltaTime = 0, totalDeltaTime = 0;

	for(unsigned long long testAmt = 0; testAmt < TEST_AMT; ++testAmt)
	{
		timer.Reset();

		for(unsigned long long i = 0; i < TEST_REPS; ++i)
		{
			vec1.normalize();
		}

		timer.Update();
		deltaTime = timer.GetDeltaTimeD();
		totalDeltaTime += deltaTime;
		cout << "Test " << testAmt + 1 << " Delta Time: " << deltaTime << endl;
	}

	totalDeltaTime /= TEST_AMT;

	cout << "\nAvg Delta Time over " << TEST_AMT << " tests: " << totalDeltaTime << endl;

	return 0;
}
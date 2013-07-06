#include <iostream>
using namespace std;

#pragma comment(lib, "RMMathLibrary")

#include <Matrix4f.h>
#include <Timer.h>

#define TEST_AMT 30
#define TEST_REPS 10000000

int main()
{
	Timer timer;

	Matrix4f mat1, mat2, mat3;
	Matrix4f mat4(1, 2, 3, 4,
				  5, 6, 7, 8,
				  9, 10, 11, 12,
				  13, 14, 15, 16);

	mat2.Translate(0.0f, 0.0f, 1.0f);
	//XMMATRIX mat2 = XMMatrixIdentity();

	Vec3f vec(1.0f, 0.0f, 0.0f), vec1(1.0f, 0.0f, 0.0f);

	double deltaTime = 0, totalDeltaTime = 0;

	for(unsigned long long testAmt = 0; testAmt < TEST_AMT; ++testAmt)
	{
		timer.Reset();

		for(unsigned long long i = 0; i < TEST_REPS; ++i)
		{
			mat1.LookAt(mat2.position);
		}

		timer.UpdateTimer();
		deltaTime = timer.GetDeltaTimeD();
		totalDeltaTime += deltaTime;
		cout << "Test " << testAmt << " Delta Time: " << deltaTime << endl;
	}

	totalDeltaTime /= TEST_AMT;

	cout << "\nAvg Delta Time over " << TEST_AMT << " tests: " << totalDeltaTime << endl;

	return 0;
}
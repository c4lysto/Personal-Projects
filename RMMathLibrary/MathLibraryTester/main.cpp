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

	double deltaTime = 0, totalDeltaTime = 0;

	for(unsigned long long testAmt = 0; testAmt < TEST_AMT; ++testAmt)
	{
		timer.Reset();

		for(unsigned long long i = 0; i < TEST_REPS; ++i)
		{
			
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
#include <iostream>
using namespace std;
#include <assert.h>

#include "../../Math Library/MathLib.h"
#include "../../RMMath/Timer.h"

//#include "../Utilities/DebugHelp.h"

#define TEST_SETS 1
#define TEST_REPS 1000000000

int main()
{
	Timer timer;
	double totalTime = 0;
	Vec3f tst(1, 0, 0);
	Vec3f tst1(1, 0, 0);

	Vec4f vec1(1, 0, 0, 0);
	Vec4f vec2(1, 0, 0, 0);
	float result;

	//Assert(0, "This is a Test %i, %i", 1, 2);

	//if(!(0))
	//{
	//	wchar_t buff[500]; 
	//	swprintf_s(buff, 500, _CRT_WIDE("This is a Test %i, %i"), 1, 2); 
	//	_ASSERT_EXPR(0, buff);
	//	//_wassert((const wchar_t*)buff, _CRT_WIDE(__FILE__), __LINE__); 
	//}

	int structSize = sizeof(Vec4f);

	Vec4V vectst(1, 2, 3, 4);
	Vec4V vectst2;
	vectst2 = vectst;

	Mat44 invMat = MAT44V_TO_MAT44(Mat44V(INIT_IDENTITY));
	invMat.Invert();

	//invMat.GetWAxisRef().SetXYZ(vectst2.GetXYZ());

	for(unsigned long long i = 0; i < TEST_SETS; ++i)
	{
		timer.Reset();
		timer.StartTimer();
		for(unsigned long long j = 0; j < TEST_REPS; ++j)
		{
			result = vec1.MagSq();
		}
		timer.StopTimer();
		totalTime += timer.GetDeltaTimeD();

		cout << "Rep Time: " << timer.GetDeltaTimeD() << endl;
		cout << "Waste Time: " << result << endl;
	}

	cout << "Average Time: " << totalTime / TEST_SETS << endl;

	return 0;
}
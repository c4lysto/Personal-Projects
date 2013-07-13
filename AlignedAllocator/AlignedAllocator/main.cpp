#include "AlignedAllocator.hpp"

#include <vector>
using std::vector;

#include <xmmintrin.h>

_declspec(align(16)) struct TestStruct
{
	__m128 tst1, tst2, tst3;
	float yada;
};

struct TestStruct2
{
	float thrice[2];
	TestStruct yup;
	float derp;
};

struct TestStruct3 : public TestStruct2
{
	float another[3];
};

int main()
{
	vector<TestStruct3> oYea;

	oYea.push_back(TestStruct3());
	oYea.push_back(TestStruct3());

	oYea.pop_back();

	oYea.push_back(TestStruct3());
	oYea.push_back(TestStruct3());
	oYea.push_back(TestStruct3());
	oYea.push_back(TestStruct3());

	oYea.erase(oYea.begin() + 2, oYea.end());

	return 0;
}
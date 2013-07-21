#include "AlignedAllocator.hpp"

#include <vector>
using std::vector;

#include <xmmintrin.h>

_declspec(align(16)) struct TestStruct
{
	__m128 tst1, tst2, tst3;
	char yada;
};

struct TestStruct2
{
<<<<<<< .mine
	short thrice;
=======
>>>>>>> .r20
	TestStruct yup;
};

_declspec(align(16)) struct TestStruct3 : public TestStruct2
{
};

int main()
{
	float* testing = nullptr;

	SAFE_DELETE(testing);

	return 0;
}
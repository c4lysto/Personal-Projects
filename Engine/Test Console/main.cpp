#include <iostream>
using namespace std;

#include <tchar.h>

#include <thread>
#include <list>

#include "../../RMMath/Timer.h"
#include "Math Lib\MathLib.h"
#include "Utilities\ThreadPool.h"
//#include "Utilities\SmartPointer.h"
#include "Utilities\FunctionPointer.h"
#include "Utilities\HashString.h"

using namespace VecElem;

//#include "../Utilities/DebugHelp.h"

#define TEST_SETS 1
#define TEST_REPS 1000000000

int __stdcall TestThisBitch(int& nVal)
{
	//cout << "Val: " << nVal << endl;
	cout << "TestThisBitch() Called!" << endl;
	return -1;
}

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
			vals2[i] = (rand() & 15) + 1;
		}
	}

	int TstMethod(int& nVal) const
	{
		//cout << "Val: " << nVal << endl;
		cout << "TstMethod() Called!" << endl;
		nVal = -1;
		return -1;
	}

	virtual void TstVirtual()
	{
		cout << "TstThreadArgs::TstVirtual()" << endl;
	}
};

struct TstDerivedClass : public TstThreadArgs
{
	virtual void TstVirtual()
	{
		cout << "TstDerivedClass::TstVirtual()" << endl;
	}
};


void TstThreadProc(void* pArgs);
float TstGlobal()
{
	cout << "TstGlobal() called !" << endl;
	return PI;
}

typedef float (TstThreadArgs::*tstFunc)(void);

void CallFuncPtr(FunctionPointer<int, int&> funcPtr)
{
	int val;
	funcPtr(val);
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1);

	//Timer timer;
	//double totalTime = 0;
	//Vec3f tst(1, 2, 3);
	//Vec3f tst1(1, 0, 0);

	//Vec4f vec1(1, 2, 3, 4);
	//Vec4f vec2(1, 0, 0, 0);
	//float result;

	//tstFunc testThis;
	//testThis = &TstThreadArgs::TstMethod;

	TstThreadArgs* pTstArgs = new TstThreadArgs;

	//(pTstArgs->*testThis)();
	//(((TstThreadArgs*)nullptr)->*testThis)();

	//auto func = FunctionPtr(&TstThreadArgs::TstMethod);
	//func.SetInvokingObject(pTstArgs);
	//int retVal = func(250);

	//ThreadPool tstThreadPool;//s[10];
	//tstThreadPool.Init(20);

	//CallFuncPtr(TestThisBitch);

	int tstVal = 250;

	//std::function<int(int)> memFunc;
	//memFunc = std::bind(&TstThreadArgs::TstMethod, pTstArgs, std::placeholders::_1);
	//FunctionPointer<int, int&> memFunc;
	//memFunc = CreateFunctionPointer(pTstArgs, &TstThreadArgs::TstMethod);
	//memFunc(tstVal);

	std::function<int(int&)> globalFunc = TestThisBitch;
	//StaticFunctionPointer<int, int&> globalFunc = TestThisBitch;
	//memFunc = globalFunc;
	//memFunc(250);
	//globalFunc(tstVal);

	//StaticFunctionPointer<int, int&> tmp = TestThisBitch;
	//tmp(tstVal);

	//(pTstArgs->*memFunc)(tstVal);

	//std::function<float(void)> pStdFunc;
	//pStdFunc = std::bind(TstGlobal);
	//pStdFunc();

	//TstThreadArgs* pVirtualTst = new TstDerivedClass;
	//pVirtualTst->TstVirtual();
	//delete pVirtualTst;

	/*for(int i = 0; i < 20; ++i)
	{
		Sleep(rand() % 1000);
		for(int j = 0; j < 1000; ++j)
		{
			tstThreadPool.AddWork(TstThreadProc, pTstArgs);
		}
	}*/

	/*Mat44V convertTst(INIT_IDENTITY);
	Mat44 convTst1;
	convTst1 = MAT44V_TO_MAT44(convertTst);

int TstThreadProc(void* pArgs)
{
	TstThreadArgs* pThreadArgs = (TstThreadArgs*)pArgs;

	Vec3V vectst(2, 4, 6, 8);
	Vec3V vectst2(2, 4, 6, 8);
	vectst2 *= vectst;

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

	cout << vectst2.GetX() << ' ' << vectst2.GetY() << ' ' << vectst2.GetZ() << ' ' << vectst2.GetW() << endl;*/

	//tstThreadPool.Shutdown();
	delete pTstArgs;
	return 0;
}

void TstThreadProc(void* pArgs)
{
	TstThreadArgs* pTstThreadArgs = (TstThreadArgs*)pArgs;

	unsigned int finalVal = 0;

	for(int i = 0; i < 256; ++i)
	{
		switch(pTstThreadArgs->operation[i])
		{
			case 0:
			{
				finalVal += pTstThreadArgs->vals1[i] + pTstThreadArgs->vals2[i];
			}
			break;

			case 1:
			{
				finalVal += pTstThreadArgs->vals1[i] - pTstThreadArgs->vals2[i];
			}
			break;

			case 2:
			{
				finalVal += pTstThreadArgs->vals1[i] * pTstThreadArgs->vals2[i];
			}
			break;

			case 3:
			{
				finalVal += pTstThreadArgs->vals1[i] / pTstThreadArgs->vals2[i];
			}
			break;
		}

	}

	cout << "Final Val: " << finalVal << endl;
}
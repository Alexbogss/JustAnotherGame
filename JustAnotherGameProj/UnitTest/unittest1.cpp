#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\JustAnotherGameProj\main.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Level level;
			Assert::IsTrue(level.LoadFromFile("JAGfirst.tmx"), L"+");
			int tileSize = level.GetTileSize().x;

		}

		TEST_METHOD(TestMethod2)
		{
			Assert::IsTrue(true, L"+");
		}
	};
}
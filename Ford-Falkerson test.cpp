#include "pch.h"
#include "CppUnitTest.h"
#include "../Ford-Fulkerson algorithm/Network.h"
#include "../Ford-Fulkerson algorithm/Network.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FordFalkersontest
{
	TEST_CLASS(FordFalkersontest)
	{
	public:
		
		TEST_METHOD(readFrom_success_test_1)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test1.txt";
			network.readFrom(filepath);
			auto result = network.getInputData();
			List<std::string*> expected;

			auto str = new std::string[INPUT_DATA_AMOUNT] {"S", "A", "2"};
			expected.push_back(str);

			str = new std::string[INPUT_DATA_AMOUNT]{ "S", "B", "3" };
			expected.push_back(str);

			str = new std::string[INPUT_DATA_AMOUNT]{ "A", "T", "4" };
			expected.push_back(str);

			str = new std::string[INPUT_DATA_AMOUNT]{ "B", "T", "6" };
			expected.push_back(str);

			for (size_t i = 0; i < result.getSize(); ++i)
				for (int j = 0; j < INPUT_DATA_AMOUNT; ++j)
					Assert::AreEqual(expected.at(i)[j], result.at(i)[j]);
		}

		TEST_METHOD(readFrom_exception_test_1)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test2.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch(std::logic_error& e)
			{
				Assert::AreEqual(e.what(), "loops are not allowed");
			}
		}

		TEST_METHOD(readFrom_exception_test_2)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test3.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch (std::logic_error& e)
			{
				Assert::AreEqual(e.what(), "ways to the source can not exist");
			}
		}

		TEST_METHOD(readFrom_exception_test_3)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test4.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch (std::logic_error& e)
			{
				Assert::AreEqual(e.what(), "ways from the runoff can not exist");
			}
		}

		TEST_METHOD(readFrom_exception_test_4)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test5.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch (std::logic_error& e)
			{
				Assert::AreEqual(e.what(), "source is missing");
			}
		}

		TEST_METHOD(readFrom_exception_test_5)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test6.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch (std::logic_error& e)
			{
				Assert::AreEqual(e.what(), "runoff is missing");
			}
		}

		TEST_METHOD(readFrom_exception_test_6)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test7.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch (std::invalid_argument& e)
			{
				Assert::AreEqual(e.what(), "file was empty");
			}
		}

		TEST_METHOD(readFrom_exception_test_7)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/testEMPTY.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch (std::exception& e)
			{
				Assert::AreEqual(e.what(), "could not open the file");
			}
		}

		TEST_METHOD(readFrom_exception_test_8)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test9.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch (std::invalid_argument& e)
			{
				Assert::AreEqual(e.what(), "wrong input format: vertex name");
			}
		}

		TEST_METHOD(readFrom_exception_test_9)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test10.txt";
			try
			{
				network.readFrom(filepath);
			}
			catch (std::invalid_argument& e)
			{
				Assert::AreEqual(e.what(), "wrong input format: bandwidth");
			}
		}

		TEST_METHOD(ford_fulkerson_success_test_1)
		{
			Network network;
			std::string filepath = "../UNIT_TEST/test8.txt";
			network.readFrom(filepath);
			network.create();
			uint64_t f = network.ford_fulkerson();
			Assert::AreEqual(uint64_t(60), f);
		}

		TEST_METHOD(ford_fulkerson_success_test_2)
		{
			std::string filepath = "../UNIT_TEST/test8.txt";
			Network network(filepath);
			uint64_t f = network.ford_fulkerson();
			Assert::AreEqual(uint64_t(60), f);
		}
	};
}

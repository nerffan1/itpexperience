#include "catch.hpp"
#include "SrcMain.h"
#include <string>
#include <chrono>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

//TEST_CASE("Basic tests (Part 1)", "[student]")
//{
//	SECTION("basic01")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/basic01.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/basic01-log.txt");
//		REQUIRE(result);
//	}
//	SECTION("basic02")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/basic02.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/basic02-log.txt");
//		REQUIRE(result);
//	}
//	SECTION("basic03")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/basic03.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/basic03-log.txt");
//		REQUIRE(result);
//	}
//	SECTION("basic04")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/basic04.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/basic04-log.txt");
//		REQUIRE(result);
//	}
//}
//
//TEST_CASE("Stack tests (Part 2)", "[student]")
//{
//	SECTION("stack01")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/stack01.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/stack01-log.txt");
//		REQUIRE(result);
//		bool resultStack = CheckTextFilesSame("stack.txt", "expected/stack01-stack.txt");
//		REQUIRE(resultStack);
//	}
//	SECTION("fibonacci")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/fibonacci.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/fibonacci-log.txt");
//		REQUIRE(result);
//		bool resultStack = CheckTextFilesSame("stack.txt", "expected/fibonacci-stack.txt");
//		REQUIRE(resultStack);
//	}
//}
//
//TEST_CASE("Drawing tests (Part 3)", "[student]")
//{
//	SECTION("draw01")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/draw01.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/draw01-log.txt");
//		REQUIRE(result);
//		bool resultImg = CheckFileMD5("output.bmp", "a0ab3ebfce71b6900ee918f7f5af723f");
//		REQUIRE(resultImg);
//	}
//	SECTION("draw02")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/draw02.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/draw02-log.txt");
//		REQUIRE(result);
//		bool resultImg = CheckFileMD5("output.bmp", "0b5a78c514378533d05bc74e8a7dc359");
//		REQUIRE(resultImg);
//	}
//	SECTION("draw03")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/draw03.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/draw03-log.txt");
//		REQUIRE(result);
//		bool resultImg = CheckFileMD5("output.bmp", "14665964d737bf6bd4e8d840dcedf1fd");
//		REQUIRE(resultImg);
//	}
//	SECTION("draw04")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/draw04.psm"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("log.txt", "expected/draw04-log.txt");
//		REQUIRE(result);
//		bool resultImg = CheckFileMD5("output.bmp", "3aa5bd1981008a404bea3ff3a13faa12");
//		REQUIRE(resultImg);
//	}
//}

TEST_CASE("Exception tests (Part 4)", "[student]")
{
	SECTION("excep01")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep01.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep01-log.txt");
		REQUIRE(result);
	}
	SECTION("excep02")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep02.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep02-log.txt");
		REQUIRE(result);
	}
	SECTION("excep03")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep03.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep03-log.txt");
		REQUIRE(result);
	}
	SECTION("excep04")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep04.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep04-log.txt");
		REQUIRE(result);
	}
	SECTION("excep05")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep05.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep05-log.txt");
		REQUIRE(result);
	}
	SECTION("excep06")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep06.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep06-log.txt");
		REQUIRE(result);
	}
	SECTION("excep07")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep07.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep07-log.txt");
		REQUIRE(result);
	}
	SECTION("excep08")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep08.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep08-log.txt");
		REQUIRE(result);
	}
	SECTION("excep09")
	{
		const char* argv[] = {
			"tests/tests",
			"input/excep09.psm"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("log.txt", "expected/excep09-log.txt");
		REQUIRE(result);
	}
}

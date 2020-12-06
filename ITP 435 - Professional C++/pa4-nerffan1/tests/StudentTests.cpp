#include "catch.hpp"
#include "SrcMain.h"
#include <string>
#include <chrono>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
TEST_CASE("Function test cases", "[student]")
{

}

 //Provided amino acid tests
 //(Once you pass the first section, comment out the other)
//TEST_CASE("Amino Acids Tests", "[student]")
//{
//	SECTION("TAS2R16")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/TAS2R16_Homo_sapiens.fasta"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/TAS2R16_amino.txt");
//		REQUIRE(result);
//	}
//	SECTION("A1C1")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/A1C1_Homo_sapiens.fasta"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/A1C1_amino.txt");
//		REQUIRE(result);
//	}
//	SECTION("Ebola")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/Zaire_ebolavirus.fasta"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/Zaire_amino.txt");
//		REQUIRE(result);
//	}
//	SECTION("Mouse Chromosome 19 (run this in release!!)")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/Mus_musculus_c19.fasta"
//		};
//		ProcessCommandArgs(2, argv);
//		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/Mus_musculus_c19_amino.txt");
//		REQUIRE(result);
//	}
//	SECTION("Mouse Chromosome 19 - Timed (run this in release!!)")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/Mus_musculus_c19.fasta"
//		};
//		auto start = std::chrono::high_resolution_clock::now();
//		ProcessCommandArgs(2, argv);
//		auto end = std::chrono::high_resolution_clock::now();
//		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
//		float seconds = duration / 1000000000.0f;
//
//		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/Mus_musculus_c19_amino.txt");
//		REQUIRE(result);
//		WARN("****Mouse Chromosome 19 timed test took: " << seconds << "s****");
//		REQUIRE(seconds < 1.25f);
//	}
//}

// Provided pairwise match tests
// (Once you pass the first section, comment out the other)
TEST_CASE("Pairwise Match Tests", "[student]")
{
	/*SECTION("Small Comparison")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Small_Test1.fasta",
			"input/Small_Test2.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.result", "sampleoutput/Small_Test_Comparison.result");
		REQUIRE(result);
	}
	SECTION("TAS2R16 Comparison")
	{
		const char* argv[] = {
			"tests/tests",
			"input/TAS2R16_Homo_sapiens.fasta",
			"input/TAS2R16_Pan_troglodytes.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.result", "sampleoutput/TAS2R16_Comparison.result");
		REQUIRE(result);
	}*/
	SECTION("Ebola Comparison (run this in release!!)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Zaire_ebolavirus.fasta",
			"input/Reston_ebolavirus.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.result", "sampleoutput/Ebola_Zaire_vs_Reston_Comparison.result");
		REQUIRE(result);
	}
	/*
	SECTION("Ebola Comparison - Timed (run this in release!!)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Zaire_ebolavirus.fasta",
			"input/Reston_ebolavirus.fasta"
		};
		auto start = std::chrono::high_resolution_clock::now();
		ProcessCommandArgs(3, argv);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float seconds = duration / 1000000000.0f;

		bool result = CheckTextFilesSame("match.result", "sampleoutput/Ebola_Zaire_vs_Reston_Comparison.result");
		REQUIRE(result);
		WARN("****Zaire timed test took: " << seconds << "s****");
		REQUIRE(seconds < 4.0f);
	}*/
}

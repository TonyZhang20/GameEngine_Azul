//----------------------------------------------------------------------------
// Copyright 2008-2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
//
//	Framework - for Keenan's C++ classes
//
//    One header to rule them all, 
//        one header to find them, 
//        one header to bring them all 
//        and in the Linker bind them.   <thank you Tolkien>
//
//	C++ Framework:
//
//	* Memory Tracker - dynamic memory leak tracking
//	* C++ Unit Test - light weight unit testing with teardown
//	* Performance Timer - hardware cycle count timer for performance timing
//	* Printing support - outputs to Visual Studio Output window 
//	* File logging - Printing data to an external logging file
//	* CRC32 - binary hash function
//	* Memory Alignment Base class - keeping alignment to 16 bytes for SIMD
//	* SIMD support detection - testing SIMD functionality
//
//  Thread Framework:
//
//  * Thread Naming - Async/Threads names in Thread Watch window
//  * Thread start/end - printing entering / exiting threads
//  * Thread name registry - tracks all the active threads
//  * Banner class - base class for threads/async threads for names
//  * Debug printing - adding thread name/id with indentation
//  * Deferred vs Async - detection
//  * C++ Framework functionality included
//
//----------------------------------------------------------------------------
//	Legal Notice
//---------------------------------------------------------------------------- 
//
// C++ Framework
// Copyright 2008-2025 Ed Keenan All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Ed Keenan nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author/Architect: Ed Keenan - ekeenan2@cdm.depaul.edu
//
//----------------------------------------------------------------------------
//	Release Notes: - previous notes at bottom of File 
//----------------------------------------------------------------------------
//
//    v.4.00 - one include
//    v.4.01 - DLL exit once with DLL_Count.bin
//    v.4.02 - remove meyers singletons
//    v.4.03 - removed ProjectSettings.h
//    v.4.04 - unified DLL memory tracking
//    v.4.05 - release mode fixes, repeat run MemTracker.bin
//    v.4.06 - VERBOSE_DLL_INFO
//    v.4.07 - added std::set() for MemTrackerData needed for static libs
//    v.4.08 - reduced prints issues in release
//    v.4.09 - remove \Logs directory, needed "..." double quotes full path
//    v.4.10 - remove flashing on run (mkdir Logs)
//    v.4.11 - cleanup release Logs
//    v.4.12 - keep MemTrackingLog.txt
//    v.4.13 - rough new line after banner
//    v.4.14 - SOLUTION_LOGS_DIR for printing log files
//    v.4.15 - gamePhysics DLL_Count.bin needs to be relative
//    v.4.16 - Azul sub directory
//    v.4.17 - instructions pre-link
//    v.4.18 - baseline spring 2025
//    v.4.19 - if exist for rmdir (suppresses rough missing dir statement)
//    v.4.20 - chrono_literals
//    v.4.21 - privTrace(\n) in release after banner  
//    v.4.22 - more new line format with banner (with memory off)
//    v.4.23 - x64 padding
//    v.4.24 - FileIO mkdir
//    v.4.25 - Azul mkdir SOLUTION_DIR relative
//    v.4.26 - memtracking file issues
//    v.4.27 - new line in release mode
//    v.4.28 - dll file verification
//    v.4.29 - warning scrub
//    v.4.30 - analysis scrub and suppression
//    v.4.31 - too aggressive casting... convert back to static cast
//    v.4.32 - footer at end of output window
//    v.4.33 - environment var
//    v.4.34 - dll print newline
// ---------------------------------------------------------------------------

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// -------------------------------
// Set the versions here:
// -------------------------------

constexpr const char *FRAMEWORK_VER = "4.34";
constexpr const char *THREAD_FRAMEWORK_VER = "1.31";

// ---------------------------------------------------------------------------
// General Guidelines
//
// Using C-Strings for the project
//       You need to set your project settings
//       Properties/General/Advanced/Character Set->Use Multi-Byte Character Set
//
// Forced include this this header
//		Properties/C++/Advanced/Forced Include File -> "Framework.h"
//      Make sure Framework.h is any project include directory
// 
// Build Events --> Pre-Link Event
//		if exist "$(SolutionDIr)Azul" rmdir /S /Q "$(SolutionDir)Azul"
//
// Add to your defines:
//      Properties/C++/Preprocessor/Preprocessor Definition 
//           -> add WINDOWS_TARGET_PLATFORM="$(TargetPlatformVersion)";
//           -> add SOLUTION_DIR=R"($(SolutionDir))";
//           -> add TOOLS_VERSION=R"($(VCToolsVersion))";
//           -> add LOCAL_WORKING_DIR=R"($(LocalDebuggerWorkingDirectory))"
//
// ---------------------------------------------------------------------------

// Enable Thread_Framework here <search>: THREAD_FRAMEWORK_START
//#define USE_THREAD_FRAMEWORK

// Comment out this line to turn off memory tracking
//#define MEM_TRACKER_ENABLED

// Enable for verbose DLL headers (to insure each lib is using same framework)
//#define VERBOSE_DLL_INFO

// Enable this to reduce Test Prints (suppresses summary)
//#define REDUCED_TEST_PRINTS

// Enable SSE support <search>: SIMD_SUPPORT_START
//#define SIMD_SUPPORT_PRINTS

// Teaching fiasco 
//#define TEACHING_PC

// default warning... please setup correctly
#ifndef WINDOWS_TARGET_PLATFORM
#error  WINDOWS_TARGET_PLATFORM --- MISSING ---
#endif

#ifndef TOOLS_VERSION
#error  TOOLS_VERSION --- MISSING ---
#endif

#ifndef SOLUTION_DIR
#error SOLUTION_DIR --- MISSING ---
#endif

#ifndef LOCAL_WORKING_DIR
#error LOCAL_WORKING_DIR --- MISSING ---
#endif

// Build Mode:
#ifdef _DEBUG
#ifdef _M_X64
constexpr const char *BUILD_CONFIG_MODE = "x64 Debug  ";
#else
constexpr const char *BUILD_CONFIG_MODE = "x86 Debug  ";
#endif
#else
#ifdef _M_X64
constexpr const char *BUILD_CONFIG_MODE = "x64 Release";
#else
#ifdef MR_FAST   // Only used in optimized class
constexpr const char *BUILD_CONFIG_MODE = "MR_FAST    ";
#else
constexpr const char *BUILD_CONFIG_MODE = "x86 Release";
#endif
#endif
#endif

// -----------------------------------------------------------------------------
// Warning levels turned on for Wall... that Should be supressed globally
//     Set here to avoid surpressing them in the settings
// -----------------------------------------------------------------------------

#pragma warning( disable : 4100 ) // unreferenced formal parameter (protocol buff)
#pragma warning( disable : 4127 ) // conditional expression is constant
#pragma warning( disable : 4201 ) // nameless struct/union
#pragma warning( disable : 4251 ) // 'type1' needs dll-interface (Protocol buff)
#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable : 4514 ) // unreferenced inline has been removed
#pragma warning( disable : 4571 ) // catch(...) changed (SEH) are no longer caught
#pragma warning( disable : 4625 ) // 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
#pragma warning( disable : 4626 ) // 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
#pragma warning( disable : 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directive'
#pragma warning( disable : 4587 ) // 'anonymous_structure': behavior change: constructor is no longer implicitly called
#pragma warning( disable : 4588 ) // 'anonymous_structure': behavior change: destructor is no longer implicitly called
#pragma warning( disable : 4710 ) // 'function': function not inlined
#pragma warning( disable : 4711 ) // function 'function' selected for automatic inline expansion
#pragma warning( disable : 4820 ) // 'bytes' bytes padding added after construct 'member_name'
#pragma warning( disable : 4291 ) // 'declaration' : no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning( disable : 5025 ) // 'type': move assignment operator was implicitly defined as deleted
#pragma warning( disable : 5026 ) // 'type': move constructor was implicitly defined as deleted
#pragma warning( disable : 5027 ) // 'type': move assignment operator was implicitly defined as deleted
#pragma warning( disable : 5039 ) // function': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception.
#pragma warning( disable : 5045 ) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning( disable : 6011 ) // dereferencing NULL pointer <name>
#pragma warning( disable : 5204 ) // 'type-name': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
#pragma warning( disable : 5220 ) // 'name': a non-static data member with a volatile qualified type no longer implies that compiler generated copy/move constructors and copy/move assignment operators are not trivial
#pragma warning( disable : 26812 ) // Prefer 'enum class' over 'enum' (Enum.3)
#pragma warning( disable : 4296 ) // >=: expression always true (handle.GetIndex() >= 0)
#pragma warning( disable : 5205 ) // delete of an abstract class 'IXAudio2VoiceCallback' that has a non - virtual destructor results in undefined behavior	GameEngine	C : \code\keenan2022spring_csc588\instructor\Lecture\Week_05\6.3_Snd_App - Voice\src\Voice\Voice.cpp	55
#pragma warning( disable : 4738 ) // storing 32 - bit float result in memory, possible loss of performance
#pragma warning( disable : 4619 ) // warning: there is no warning number
#pragma warning( disable : 5264 ) // 'variable-name': 'const' variable is not used
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union
#pragma warning( disable : 5246 ) // the initialization of a subobject should be wrapped in braces
#pragma warning( disable : 4459 ) // declaration of 'identifier' hides global declaration

// -------------------------------------------
// Prevent Analysis in case it was left on
// -------------------------------------------

#include <codeanalysis\warnings.h>
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )

// --------------------------------------
// Prevent Console from opening
// --------------------------------------

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

// --------------------------------------
// Environment check
// --------------------------------------

// minimum versions - matches school's lab machines
constexpr const char *TEST_WINSDK_VER = "10.0.22621.0";
constexpr const char *TEST_TOOLS_VER = "14.37.32822";
constexpr const char *TEST_COMPILER_VER =  "193732822";

#ifdef TEACHING_PC
#define IDEWinSDKCheck { CHECK(true); }

#define IDEToolsCheck { CHECK(true); }

#define IDECompilerCheck { CHECK(true); }
#else
#define IDEWinSDKCheck { char  winSDK_ver[13]; \
					sprintf_s(winSDK_ver, 13, "%s", WINDOWS_TARGET_PLATFORM); \
					CHECK(strcmp(winSDK_ver, TEST_WINSDK_VER) >= 0); }

#define IDEToolsCheck { char tools_ver[12]; \
					sprintf_s(tools_ver, 12, "%s", TOOLS_VERSION); \
					CHECK(strcmp(tools_ver, TEST_TOOLS_VER) >= 0); }

#define IDECompilerCheck { char compiler_ver[10]; \
					sprintf_s(compiler_ver, 10, "%d", _MSC_FULL_VER); \
					CHECK(strcmp(compiler_ver, TEST_COMPILER_VER) >= 0); }
#endif

// --------------------------------------
// General includes
// --------------------------------------

#include <assert.h>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>  
#include <crtdbg.h>  

// -----------------------------------------------------
// Many standard includes have serious warnings 
//     need to wrap for Wall warnings
//
// Todo: reverify with major compiler releases... 
//       Come on Microsoft.. clean up your code
// -----------------------------------------------------

#pragma warning( push )
#pragma warning( disable : 4820 ) // bytes padding added after 'member_name'
#define NOMINMAX
#include <Windows.h>
#pragma warning( pop ) 

#pragma warning( push )
#pragma warning( disable : 4365 )   
#pragma warning( disable : 4774 )
#include <string>
#pragma warning( pop ) 

#pragma warning( push )
#pragma warning( disable : 4365 ) // 'classname': class has virtual functions, but destructor is not virtual\n instances of this class may not be destructed correctly
#pragma warning( disable : 4623 ) // 'derived class': default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
#pragma warning( disable : 4625 ) // 'derived class': copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
#pragma warning( disable : 4626 ) // 'derived class': assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
#pragma warning( disable : 4820 ) // 'bytes' bytes padding added after construct 'member_name'
#pragma warning( disable : 5026 ) // 'type': move constructor was implicitly defined as deleted
#pragma warning( disable : 5027 ) // 'type': move assignment operator was implicitly defined as deleted
#include <mutex>
#pragma warning( pop ) 

#pragma warning( push )
#pragma warning( disable : 4365 )  // 'classname': class has virtual functions, but destructor is not virtual\n instances of this class may not be destructed correctly
#pragma warning( disable : 4625 ) // 'derived class': copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
#pragma warning( disable : 4626 ) // 'derived class': assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
#pragma warning( disable : 5026 ) // 'type': move constructor was implicitly defined as deleted
#pragma warning( disable : 5027 ) // 'type': move assignment operator was implicitly defined as deleted
#include <atomic>
#pragma warning( pop ) 

#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <intrin.h>

#define AZUL_REPLACE_ME_STUB(x) (x)
#define AZUL_REPLACE_ME(x) (void(x))
constexpr const char *AZUL_INSERT_CODE_HERE = nullptr;          // do nothing
#define AZUL_UNUSED_VAR(x) (void(x))


// -----------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//
// SIMD support detection
//
// -----------------------------------------------------

#ifdef SIMD_SUPPORT_PRINTS

#define SIMD_SUPPORT_START
	// InstructionSet.cpp
	// Compile by using: cl /EHsc /W4 InstructionSet.cpp
	// processor: x86, x64
	// Uses the __cpuid intrinsic to get information about
	// CPU extended instruction set support.

class SIMDInstructionSet
{
public:
	// getters
	std::string Vendor(void) { return this->vendor_; }
	std::string Brand(void) { return this->brand_; }

	bool SSE3(void) { return this->f_1_ECX_[0]; }
	bool PCLMULQDQ(void) { return this->f_1_ECX_[1]; }
	bool MONITOR(void) { return this->f_1_ECX_[3]; }
	bool SSSE3(void) { return this->f_1_ECX_[9]; }
	bool FMA(void) { return this->f_1_ECX_[12]; }
	bool CMPXCHG16B(void) { return this->f_1_ECX_[13]; }
	bool SSE41(void) { return this->f_1_ECX_[19]; }
	bool SSE42(void) { return this->f_1_ECX_[20]; }
	bool MOVBE(void) { return this->f_1_ECX_[22]; }
	bool POPCNT(void) { return this->f_1_ECX_[23]; }
	bool AES(void) { return this->f_1_ECX_[25]; }
	bool XSAVE(void) { return this->f_1_ECX_[26]; }
	bool OSXSAVE(void) { return this->f_1_ECX_[27]; }
	bool AVX(void) { return this->f_1_ECX_[28]; }
	bool F16C(void) { return this->f_1_ECX_[29]; }
	bool RDRAND(void) { return this->f_1_ECX_[30]; }

	bool MSR(void) { return this->f_1_EDX_[5]; }
	bool CX8(void) { return this->f_1_EDX_[8]; }
	bool SEP(void) { return this->f_1_EDX_[11]; }
	bool CMOV(void) { return this->f_1_EDX_[15]; }
	bool CLFSH(void) { return this->f_1_EDX_[19]; }
	bool MMX(void) { return this->f_1_EDX_[23]; }
	bool FXSR(void) { return this->f_1_EDX_[24]; }
	bool SSE(void) { return this->f_1_EDX_[25]; }
	bool SSE2(void) { return this->f_1_EDX_[26]; }

	bool FSGSBASE(void) { return this->f_7_EBX_[0]; }
	bool BMI1(void) { return this->f_7_EBX_[3]; }
	bool HLE(void) { return this->isIntel_ && this->f_7_EBX_[4]; }
	bool AVX2(void) { return this->f_7_EBX_[5]; }
	bool BMI2(void) { return this->f_7_EBX_[8]; }
	bool ERMS(void) { return this->f_7_EBX_[9]; }
	bool INVPCID(void) { return this->f_7_EBX_[10]; }
	bool RTM(void) { return this->isIntel_ && this->f_7_EBX_[11]; }
	bool AVX512F(void) { return this->f_7_EBX_[16]; }
	bool RDSEED(void) { return this->f_7_EBX_[18]; }
	bool ADX(void) { return this->f_7_EBX_[19]; }
	bool AVX512PF(void) { return this->f_7_EBX_[26]; }
	bool AVX512ER(void) { return this->f_7_EBX_[27]; }
	bool AVX512CD(void) { return this->f_7_EBX_[28]; }
	bool SHA(void) { return this->f_7_EBX_[29]; }

	bool PREFETCHWT1(void) { return this->f_7_ECX_[0]; }

	bool LAHF(void) { return this->f_81_ECX_[0]; }
	bool LZCNT(void) { return this->isIntel_ && this->f_81_ECX_[5]; }
	bool ABM(void) { return this->isAMD_ && this->f_81_ECX_[5]; }
	bool SSE4a(void) { return this->isAMD_ && this->f_81_ECX_[6]; }
	bool XOP(void) { return this->isAMD_ && this->f_81_ECX_[11]; }
	bool TBM(void) { return this->isAMD_ && this->f_81_ECX_[21]; }

	bool SYSCALL(void) { return this->isIntel_ && this->f_81_EDX_[11]; }
	bool MMXEXT(void) { return this->isAMD_ && this->f_81_EDX_[22]; }
	bool RDTSCP(void) { return this->isIntel_ && this->f_81_EDX_[27]; }
	bool _3DNOWEXT(void) { return this->isAMD_ && this->f_81_EDX_[30]; }
	bool _3DNOW(void) { return this->isAMD_ && this->f_81_EDX_[31]; }

public:
	SIMDInstructionSet()
		:
		nIds_{0},
		nExIds_{0},
		isIntel_{false},
		isAMD_{false},
		f_1_ECX_{0},
		f_1_EDX_{0},
		f_7_EBX_{0},
		f_7_ECX_{0},
		f_81_ECX_{0},
		f_81_EDX_{0},
		data_{},
		extdata_{}
	{

		std::array<int, 4> cpui;

		// Calling __cpuid with 0x0 as the function_id argument
		// gets the number of the highest valid function ID.
		__cpuid(cpui.data(), 0);
		nIds_ = cpui[0];

		for(int i = 0; i <= nIds_; ++i)
		{
			__cpuidex(cpui.data(), i, 0);
			data_.push_back(cpui);
		}

		// Capture vendor string
		char vendor[0x20];
		memset(vendor, 0, sizeof(vendor));
		*reinterpret_cast<int *>(vendor) = data_[0][1];
		*reinterpret_cast<int *>(vendor + 4) = data_[0][3];
		*reinterpret_cast<int *>(vendor + 8) = data_[0][2];
		vendor_ = vendor;
		if(vendor_ == "GenuineIntel")
		{
			isIntel_ = true;
		}
		else if(vendor_ == "AuthenticAMD")
		{
			isAMD_ = true;
		}

		// load bitset with flags for function 0x00000001
		if(nIds_ >= 1)
		{
			f_1_ECX_ = (unsigned int)data_[1][2];
			f_1_EDX_ = (unsigned int)data_[1][3];
		}

		// load bitset with flags for function 0x00000007
		if(nIds_ >= 7)
		{
			f_7_EBX_ = (unsigned int)data_[7][1];
			f_7_ECX_ = (unsigned int)data_[7][2];
		}

		// Calling __cpuid with 0x80000000 as the function_id argument
		// gets the number of the highest valid extended ID.
		__cpuid(cpui.data(), 0x80000000);
		nExIds_ = cpui[0];

		char brand[0x40];
		memset(brand, 0, sizeof(brand));

		for(int i = 0x80000000; i <= nExIds_; ++i)
		{
			__cpuidex(cpui.data(), i, 0);
			extdata_.push_back(cpui);
		}

		// load bitset with flags for function 0x80000001
		if(nExIds_ >= 0x80000001)
		{
			f_81_ECX_ = (unsigned int)extdata_[1][2];
			f_81_EDX_ = (unsigned int)extdata_[1][3];
		}

		// Interpret CPU brand string if reported
		if(nExIds_ >= 0x80000004)
		{
			memcpy(brand, extdata_[2].data(), sizeof(cpui));
			memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
			memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
			brand_ = brand;
		}


	};

	int nIds_;
	int nExIds_;
	std::string vendor_;
	std::string brand_;
	bool isIntel_;
	bool isAMD_;
	std::bitset<32> f_1_ECX_;
	std::bitset<32> f_1_EDX_;
	std::bitset<32> f_7_EBX_;
	std::bitset<32> f_7_ECX_;
	std::bitset<32> f_81_ECX_;
	std::bitset<32> f_81_EDX_;
	std::vector<std::array<int, 4>> data_;
	std::vector<std::array<int, 4>> extdata_;

};



#endif

// -----------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//
// CPP Unit Test
//      Unit Test in C++ without exceptions (that was hard)
//      Includes TearDown.. so you can cleanup allocations even if test fails
//      Test aborts on first failed CHECK() in unit test, then continues to next
//
//      Used to be stand alone files... now all one include
//
// -----------------------------------------------------------------------

#ifndef UNIT_TEST_CPP_H
#define UNIT_TEST_CPP_H


#define UNIT_TEST_MEMORYCHECK_BEGIN   
#define UNIT_TEST_MEMORYCHECK_DISABLE MemTrace::UnitTest_MemLeakCheck_Disable_Proxy(); 
#define UNIT_TEST_MEMORYCHECK_ENABLE  MemTrace::UnitTest_MemLeakCheck_Enable_Proxy();	
#define UNIT_TEST_MEMORYCHECK_END     } \
                                              { \
											  CHECK(MemTrace::UnitTest_MemLeakCheck_End_Proxy()); \
											  }										  
#define UnitTest_SetName(A,B)		  { \
												MemTrace &rTrace = MemTrace::privGetRefInstance();\
												std::lock_guard<std::mutex> loc(rTrace.mtxMemLeakCount);\
												\
												Debug::SetName(A,B); \
												rTrace.MemLeakCount_Start += 1;\
											  }

class UnitTrace
{
private:
	static const unsigned int UnitTraceBuffSize = 512;

public:
	// displays a printf to the output window
	static void out(const char *const fmt, ...)
	{
		UnitTrace *pTrace = UnitTrace::privGetInstance();
		assert(pTrace);

		std::lock_guard<std::mutex> lock(pTrace->mtx);

		va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
		va_start(args, fmt);
#pragma warning( pop )

		vsprintf_s(&pTrace->privBuff[0], UnitTraceBuffSize, fmt, args);
		OutputDebugString(&pTrace->privBuff[0]);

		//va_end(args);
		args = static_cast<va_list> (nullptr);

	}

	static void out2(const char *const fmt, ...)
	{
		UnitTrace *pTrace = UnitTrace::privGetInstance();
		assert(pTrace);

		std::lock_guard<std::mutex> lock(pTrace->mtx);

		va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
		va_start(args, fmt);
#pragma warning( pop )

		vsprintf_s(&pTrace->privBuff[0], UnitTraceBuffSize, fmt, args);
		vprintf(fmt, args);
		OutputDebugString(&pTrace->privBuff[0]);

		//va_end(args);
		args = static_cast<va_list> (nullptr);

	}

	// Big Six
	UnitTrace() noexcept
	{
		memset(&privBuff[0], 0x0, UnitTraceBuffSize);
	}
	UnitTrace(const UnitTrace &) = delete;
	UnitTrace(UnitTrace &&) = delete;
	UnitTrace &operator = (const UnitTrace &) = delete;
	UnitTrace &operator = (UnitTrace &&) = delete;
	~UnitTrace() = default;

private:
	static UnitTrace *privGetInstance() noexcept
	{
		static UnitTrace helper;
		return &helper;
	}
	char privBuff[UnitTraceBuffSize];
	std::mutex mtx;
};

#pragma warning(push)
#pragma warning( disable : 4738 )
class UnitUtility
{
public:
	static bool AreEqual(const float a, const float b, const float epsilon = 0.001f) noexcept
	{
		return fabs(a - b) < epsilon;
	}

	static bool AreEqual(const double a, const double b, const double epsilon = 0.001) noexcept
	{
		return (abs(a - b) < epsilon);
	}
};

#pragma warning(pop)

struct UnitStats
{
	// Big six
	UnitStats() noexcept
		:testCount(0),
		testPass(0),
		testFail(0),
		testDisabled(0),
		indvAsserts(0),
		pad { 0 }
	{
	}
	UnitStats(const UnitStats &) = default;
	UnitStats(UnitStats &&) = default;
	UnitStats &operator = (const UnitStats &) = default;
	UnitStats &operator = (UnitStats &&) = default;
	~UnitStats() = default;

	// data: ------------------
	int testCount;
	int testPass;
	int testFail;
	int testDisabled;
	int indvAsserts;
	char pad[4]; // for x64 padding
};
struct UnitData
{
	static const unsigned int UnitDataBuffErrorSize = 512;

	// Big six
	UnitData() noexcept
		: pMemberName(nullptr),
		pSourceFilePath(nullptr),
		sourceLineNumber(0),
		result(false),
		pad0(0),
		pad1(0),
		pad2(0),
		pErrorBuffer{0},
		pErrorBuffer2{0}

	{
	}
	UnitData(const UnitData &) = delete;
	UnitData(UnitData &&) = delete;
	UnitData &operator = (const UnitData &) = delete;
	UnitData &operator = (UnitData &&) = delete;
	~UnitData() = default;

	// data: -----------------
	const char *pMemberName;
	const char *pSourceFilePath;
	int sourceLineNumber;
	bool result;
	char pad0;
	char pad1;
	char pad2;

	char pErrorBuffer[UnitData::UnitDataBuffErrorSize];
	char pErrorBuffer2[UnitData::UnitDataBuffErrorSize];
};
class UnitSLink
{
public:
	// Big Six
	UnitSLink() noexcept
		:_pNext(nullptr)
	{
	}
	UnitSLink(const UnitSLink &) = delete;
	UnitSLink(UnitSLink &&) = delete;
	UnitSLink &operator = (const UnitSLink &) = delete;
	UnitSLink &operator = (UnitSLink &&) = delete;
	virtual ~UnitSLink() = default;

	static void AddToFront(UnitSLink *&pRoot, UnitSLink &rNode) noexcept
	{
		if(pRoot == nullptr)
		{
			pRoot = &rNode;
			assert(rNode._pNext == nullptr);
		}
		else
		{
			UnitSLink *pTmp = pRoot;
			pRoot = &rNode;
			rNode._pNext = pTmp;
		}
	}
	static void AddToEnd(UnitSLink *&pRoot, UnitSLink *pNode) noexcept
	{
		if(nullptr == pRoot)
		{
			pRoot = pNode;

			assert(pNode != nullptr);
			assert(pNode->_pNext == nullptr);
		}
		else
		{
			UnitSLink *pTmpX = pRoot;

			while(pTmpX != nullptr)
			{
				if(pTmpX->_pNext == nullptr)
				{
					// at the end
					pTmpX->_pNext = pNode;
					pNode->_pNext = nullptr;
				}

				pTmpX = pTmpX->_pNext;
			}
		}
	}

public:
	// Data
	UnitSLink *_pNext;
};
class TestRegistry
{
public:
	// Big four
	TestRegistry(const TestRegistry &) = delete;
	TestRegistry(TestRegistry &&) = delete;
	TestRegistry &operator = (const TestRegistry &) = delete;
	TestRegistry &operator = (TestRegistry &&) = delete;
	~TestRegistry() = default;

	void AddTest(UnitSLink *pTest) noexcept
	{
		// add to End	
		UnitSLink::AddToEnd(this->_pRoot, pTest);
	}
	UnitStats &GetStats() noexcept
	{
		return this->_UnitStats;
	}
	UnitData &GetData() noexcept
	{
		return this->_UnitData;
	}
	UnitSLink *GetRoot() noexcept
	{
		return this->_pRoot;
	}
	static TestRegistry &GetInstance() noexcept
	{
		static TestRegistry tRegistry;
		return tRegistry;
	}
	static UnitStats Stats() noexcept
	{
		TestRegistry &reg = TestRegistry::GetInstance();
		return reg._UnitStats;
	}

private:
	TestRegistry() noexcept
	{
		this->_pRoot = nullptr;
	}

	// Data: ------------------------
	UnitData _UnitData;
	UnitStats _UnitStats;
	UnitSLink *_pRoot;
};
class Test : public UnitSLink
{
public:

	Test(const char *const pTestName, bool _enable) noexcept
		:UnitSLink(),
		pName(pTestName),
		testFunc(nullptr),
		enabled(_enable),
		pad { 0 }
	{
		// functional pointer
		this->testFunc = this;

		// register it
		TestRegistry &tR = TestRegistry::GetInstance();
		tR.AddTest(this);
	}

	// Big six
	Test() = delete;
	Test(const Test &) = delete;
	Test & operator = (const Test &) = delete;
	~Test() = default;
	Test(Test &&) = delete;
	Test & operator = (Test &&) = delete;

	virtual void run(UnitData &, UnitStats &) = 0;

	// For Tests with NO Teardowns... do nothing
	virtual void teardown() noexcept
	{
	};
	// For Tests with NO setup... do nothing
	virtual void setup() noexcept
	{
	};
	static void RunTests()
	{
#ifdef _DEBUG

#ifndef REDUCED_TEST_PRINTS
		UnitTrace::out2("------ Testing DEBUG ----------\n");
		UnitTrace::out2("\n");
#endif

#ifdef _M_X64
		const char *const mode = "x64 Debug";
		AZUL_UNUSED_VAR(mode);
#else
		const char *const mode = "x86 Debug";
		AZUL_UNUSED_VAR(mode);
#endif
#else
#ifdef _M_X64
		const char *const mode = "x64 Release";
		AZUL_UNUSED_VAR(mode);
#else
		const char *const mode = "x86 Release";
		AZUL_UNUSED_VAR(mode);
#endif

#ifdef MR_FAST   // Only used in optimized class
		UnitTrace::out2("------ Testing MR_FAST ---------\n");
#else
		UnitTrace::out2("------ Testing RELEASE ---------\n");
		UnitTrace::out2("\n");
#endif
#endif


		TestRegistry &rRegistry = TestRegistry::GetInstance();
		UnitSLink *pTmp = rRegistry.GetRoot();

		UnitStats &unitStats = rRegistry.GetStats();
		UnitData &unitData = rRegistry.GetData();

		while(pTmp != nullptr)
		{
			unitStats.testCount++;

			// downcast to the test
			Test *pTest = static_cast<Test *>(pTmp);

			assert(pTest);

			// Needed to be added - for fencing issues between tests
			// Release rearranges.. and affects timing

			// Forces a Fence... 
			atomic_thread_fence(std::memory_order_acq_rel);

			// Always call the setup
			pTest->testFunc->setup();

			// Forces a Fence... 
			atomic_thread_fence(std::memory_order_acq_rel);

			// run the test
			unitData.result = true;

			assert(pTest->testFunc != nullptr);
			pTest->testFunc->run(unitData, unitStats);

			// Forces a Fence... 
			atomic_thread_fence(std::memory_order_acq_rel);

			// Always call the teardown
			pTest->testFunc->teardown();

			// Forces a Fence... 
			atomic_thread_fence(std::memory_order_acq_rel);

			if(pTest->enabled)
			{
				if(unitData.result)
				{
					unitStats.testPass++;
					UnitTrace::out2(" PASSED: %s \n", pTest->pName);
				}
				else
				{
					unitStats.testFail++;
					UnitTrace::out2("-FAILED: %s \n", pTest->pName);
					UnitTrace::out2("%s", unitData.pErrorBuffer);
					UnitTrace::out2("\t%s", unitData.pErrorBuffer2);
				}
			}
			else
			{
				unitStats.testDisabled++;
				UnitTrace::out2("-IGNORE: %s \n", pTest->pName);
			}

			// next test
			pTmp = pTmp->_pNext;
		}

		if(unitStats.testFail)
		{
			UnitTrace::out2("\n");
		}

		UnitTrace::out2("\n");

#ifndef REDUCED_TEST_PRINTS
		UnitTrace::out2("  --- Tests Results ---    \n");
		UnitTrace::out2("\n");
		UnitTrace::out2("[%s] Ignored: %d\n", mode, unitStats.testDisabled);
		UnitTrace::out2("[%s]  Passed: %d\n", mode, unitStats.testPass);
		UnitTrace::out2("[%s]  Failed: %d\n", mode, unitStats.testFail);
		UnitTrace::out2("\n");
		UnitTrace::out2("   Test Count: %d\n", unitStats.testCount);
		UnitTrace::out2(" Indiv Checks: %d\n", unitStats.indvAsserts);
		UnitTrace::out2("         Mode: %s \n", mode);
		UnitTrace::out2("\n");
		UnitTrace::out2("-----------------\n");
		UnitTrace::out2("\n");
#endif

	}

public:
	const char *const pName;
	Test *testFunc;
	bool enabled;
	char pad[7];   // x64 padding
};

// a trick to create a c-string
#define STRING_ME(s) #s

// Increments total check count
// Creates a clickable format in the output window for failure
// Abort test on first fail
#define CHECK( condition ) \
		{ \
					atomic_thread_fence(std::memory_order_acq_rel); \
			_UnitStats.indvAsserts++;\
			if( !( condition ) ) \
			{ \
				_UnitData.result = false;  \
				_UnitData.pMemberName = this->pName; \
				_UnitData.pSourceFilePath = __FILE__;  \
				_UnitData.sourceLineNumber = __LINE__; \
				sprintf_s(_UnitData.pErrorBuffer,UnitData::UnitDataBuffErrorSize,"\tcheck '%s' fail\n", STRING_ME(condition) );\
				sprintf_s(_UnitData.pErrorBuffer2,UnitData::UnitDataBuffErrorSize,"\t%s(%d): <double-click> \n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber ); \
				return; \
			} \
			else\
			{\
			}\
		}

#define CHECK_EQUAL( value1, value2 ) \
		{ \
					atomic_thread_fence(std::memory_order_acq_rel); \
			_UnitStats.indvAsserts++;\
			if( !( (value1) == (value2) ) ) \
			{ \
				_UnitData.result = false;  \
				_UnitData.pMemberName = this->pName; \
				_UnitData.pSourceFilePath = __FILE__; \
				_UnitData.sourceLineNumber = __LINE__; \
				/*UnitTrace::out2("\tFAILED on equality check: '%s' not equal to '%s'\n", STRING_ME(value1), STRING_ME(value2) ); */ \
				/*UnitTrace::out2("\t%s(%d): %s \n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName ); */ \
				sprintf_s(_UnitData.pErrorBuffer,UnitData::UnitDataBuffErrorSize,"\tcheck '%s' not equal '%s' fail\n", STRING_ME(value1), STRING_ME(value2) );\
				sprintf_s(_UnitData.pErrorBuffer2,UnitData::UnitDataBuffErrorSize,"\t%s(%d): <double-click> \n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber ); \
				return; \
			} \
			else\
			{\
			}\
		}


		// -----------------------------------------------------------------------------
		// Here is the unit test magic
		//
		// 1) it creates a class, <TestName>_Test
		//    it is derived from Test class
		// 2) it defines the constructor and calls the base constructor with the name
		//    it registers the name of the test to a single class (TestRegistry)
		// 3) It also overloads the Test::run() method with the body of the macro
		//
		// -----------------------------------------------------------------------------

class TestConfig
{
public:
	enum Flag  // not an enum class since we can bitwise or values in input params
	{
		MEMORY_CHECK = 0x1,   // turn on memory check
		VERSION_CHECK = 0x2,  // turn on version check
		ALL = (unsigned int)TestConfig::Flag::MEMORY_CHECK | (unsigned int)TestConfig::Flag::VERSION_CHECK,
		NONE = 0
	};

	TestConfig() noexcept
	{
		this->_f = TestConfig::Flag::ALL;
	}

	TestConfig::Flag operator | (const TestConfig::Flag fb) noexcept
	{
		TestConfig::Flag flag;
		flag = static_cast<TestConfig::Flag>(static_cast<int>(this->_f) | static_cast<int>(fb));

		return flag;
	}

	TestConfig::Flag  _f;
};

#ifdef USE_THREAD_FRAMEWORK
#define MULTITHREADED_DELAY           std::this_thread::sleep_for(1s);
#else
#define MULTITHREADED_DELAY           
#endif

#define TEST_END \
				 UNIT_TEST_MEMORYCHECK_END \
				 } \
				 }

#define TEST_TEARDOWN( TestEnableFlag ) \
		void TestEnableFlag##_Test::teardown() noexcept

#define TEST_SETUP( TestEnableFlag ) \
		void TestEnableFlag##_Test::setup() noexcept

// TestEnableFlag is both a string name and a flag
#define TEST( TestEnableFlag, TestConfigFlag  ) \
		class TestEnableFlag##_Test : public Test \
		{ \
			public: \
				TestEnableFlag##_Test() noexcept : \
				Test( STRING_ME(TestEnableFlag##_Test), TestEnableFlag) \
				{ \
				}; \
				TestEnableFlag##_Test & operator = (const TestEnableFlag##_Test &) = delete;\
				TestEnableFlag##_Test(const TestEnableFlag##_Test &) = delete;\
			\
			void run(UnitData &_UnitData, UnitStats &_UnitStats) override;\
		} TestEnableFlag##_instance; \
		\
		void TestEnableFlag##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) \
		{ \
			if(!TestEnableFlag) \
			{ \
			} \
			else \
			{ \
				MULTITHREADED_DELAY\
				if(((int)TestConfigFlag & (int)TestConfig::Flag::VERSION_CHECK)) \
				{ \
					IDEWinSDKCheck \
					IDEToolsCheck \
					IDECompilerCheck \
				} \
				UNIT_TEST_MEMORYCHECK_BEGIN \
				if(((int)TestConfigFlag & (int)TestConfig::Flag::MEMORY_CHECK)) \
                { \
					UNIT_TEST_MEMORYCHECK_ENABLE \
				} \
				else \
				{ \
					UNIT_TEST_MEMORYCHECK_DISABLE \
				} \
				{


#define TEST_WITH_TEARDOWN( TestEnableFlag, TestConfigFlag  ) \
		class TestEnableFlag##_Test : public Test \
		{ \
			public: \
				TestEnableFlag##_Test() noexcept : \
				Test( STRING_ME(TestEnableFlag##_Test), TestEnableFlag) \
				{ \
				}; \
				TestEnableFlag##_Test & operator = (const TestEnableFlag##_Test &) = delete;\
				TestEnableFlag##_Test(const TestEnableFlag##_Test &) = delete;\
			\
			void run(UnitData &_UnitData, UnitStats &_UnitStats) override;\
			void teardown() noexcept override;\
		} TestEnableFlag##_instance; \
		\
		void TestEnableFlag##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) \
		{ \
			if(!TestEnableFlag) \
			{ \
			} \
			else \
			{ \
				MULTITHREADED_DELAY\
				if(((int)TestConfigFlag & (int)TestConfig::Flag::VERSION_CHECK)) \
				{ \
					IDEWinSDKCheck \
					IDEToolsCheck \
					IDECompilerCheck \
				} \
				UNIT_TEST_MEMORYCHECK_BEGIN \
				if(((int)TestConfigFlag & (int)TestConfig::Flag::MEMORY_CHECK)) \
                { \
					UNIT_TEST_MEMORYCHECK_ENABLE \
				} \
				else \
				{ \
					UNIT_TEST_MEMORYCHECK_DISABLE \
				} \
				{


#define TEST_WITH_SETUP_TEARDOWN( TestEnableFlag, TestConfigFlag  ) \
		class TestEnableFlag##_Test : public Test \
		{ \
			public: \
				TestEnableFlag##_Test() noexcept : \
				Test( STRING_ME(TestEnableFlag##_Test), TestEnableFlag) \
				{ \
				}; \
				TestEnableFlag##_Test & operator = (const TestEnableFlag##_Test &) = delete;\
				TestEnableFlag##_Test(const TestEnableFlag##_Test &) = delete;\
			\
			void run(UnitData &_UnitData, UnitStats &_UnitStats) override;\
			void teardown() noexcept override;\
			void setup() noexcept override;\
		} TestEnableFlag##_instance; \
		\
		void TestEnableFlag##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) \
		{ \
			if(!TestEnableFlag) \
			{ \
			} \
			else \
			{ \
				MULTITHREADED_DELAY\
				if(((int)TestConfigFlag & (int)TestConfig::VERSION_CHECK)) \
				{ \
					IDEWinSDKCheck \
					IDEToolsCheck \
					IDECompilerCheck \
				} \
				UNIT_TEST_MEMORYCHECK_BEGIN \
				if(((int)TestConfigFlag & (int)TestConfig::MEMORY_CHECK)) \
                { \
					UNIT_TEST_MEMORYCHECK_ENABLE \
				} \
				else \
				{ \
					UNIT_TEST_MEMORYCHECK_DISABLE \
				} \
				{


#endif  UNIT_TEST_CPP_H

// -----------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//
// Performance timer
//      Quick hardware timer
//      Uses the hardware clock for timing
//      Returns time as float (easier than Abstract Data Types for students)
//      Not designed for timing event... but instead for measuring time
//
//      Used to be stand alone files... now all one include
//
// -----------------------------------------------------------------------

#ifndef PERFORMANCE_TIMER_H
#define PERFORMANCE_TIMER_H

class PerformanceTimer
{
public:
	// big six
	PerformanceTimer() noexcept
		:
		ticTime(),
		tocTime(),
		ticGlobalTime(),
		tocGlobalTime(),
		deltaTime(),
		SecondsPerCycle(0.0f),
		timeSeconds(0.0f)
	{
		this->privInitTimer();
		this->Reset();
		this->ticGlobalTime = this->privGetTimer();
	}
	PerformanceTimer(const PerformanceTimer &) = delete;
	PerformanceTimer(PerformanceTimer &&) = delete;
	PerformanceTimer &operator= (const PerformanceTimer &) = delete;
	PerformanceTimer &operator= (PerformanceTimer &&) = delete;
	~PerformanceTimer() = default;

	void Tic() noexcept
	{
		// Forces a Fence... 
		atomic_thread_fence(std::memory_order_acq_rel);

		this->ticTime = this->privGetTimer();

		// Forces a Fence...
		atomic_thread_fence(std::memory_order_acq_rel);
	}
	void Toc() noexcept
	{
		// Forces a Fence... 
		atomic_thread_fence(std::memory_order_acq_rel);

		this->tocTime = this->privGetTimer();
		assert(this->tocTime.QuadPart >= this->ticTime.QuadPart);
		this->deltaTime.QuadPart = this->tocTime.QuadPart - this->ticTime.QuadPart;

		// Forces a Fence... 
		atomic_thread_fence(std::memory_order_acq_rel);
	}
	void Reset() noexcept
	{
		this->ticTime.QuadPart = 0;
		this->tocTime.QuadPart = 0;
		this->deltaTime.QuadPart = 0;
	}
	double TimeInSeconds() const noexcept 
	{
		double floatTime;
		floatTime = static_cast<double>(this->deltaTime.QuadPart);
		floatTime = floatTime * this->SecondsPerCycle;

		return floatTime;
	}

	float GetGlobalTime() noexcept
	{
		// Forces a Fence... 
		atomic_thread_fence(std::memory_order_acq_rel);

		LARGE_INTEGER	deltaGlobalTime{{0}};

		this->tocGlobalTime = this->privGetTimer();
		assert(this->tocGlobalTime.QuadPart >= this->ticGlobalTime.QuadPart);

		deltaGlobalTime.QuadPart = this->tocGlobalTime.QuadPart - this->ticGlobalTime.QuadPart;

		float floatTime;
		floatTime = static_cast<float>(deltaGlobalTime.QuadPart);
		floatTime *= this->SecondsPerCycle;

		// Forces a Fence... 
		atomic_thread_fence(std::memory_order_acq_rel);

		return floatTime;
	}

private:

	void privInitTimer() noexcept
	{
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		this->SecondsPerCycle = 1.0f / static_cast<float>(Frequency.QuadPart);
	}
	LARGE_INTEGER privGetTimer() noexcept
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return time;
	}

	// ------------------------------------------
	// data
	// ------------------------------------------

	LARGE_INTEGER	ticTime;
	LARGE_INTEGER	tocTime;
	LARGE_INTEGER	ticGlobalTime;
	LARGE_INTEGER	tocGlobalTime;
	LARGE_INTEGER	deltaTime;
	float			SecondsPerCycle;
	float			timeSeconds;
};

#endif PERFORMANCE_TIMER_H

// -----------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//
// Output Debug print
//      Same interface as printf(...) but outputs to the debug window
//      Also protected with mutex for race conditions
//
//      Used to be stand alone files... now all one include
//
// -----------------------------------------------------------------------

#ifndef DEBUG_OUTPUT_H
#define DEBUG_OUTPUT_H

class Trace
{
private:
	static const unsigned int TraceBuffSize = 1024;

public:

	// Big six
	Trace() noexcept
	{
		memset(&privBuff[0], 0x0, TraceBuffSize);
	}
	Trace(const Trace &) = delete;
	Trace(Trace &&) = delete;
	Trace &operator = (const Trace &) = delete;
	Trace &operator = (Trace &&) = delete;
	~Trace() = default;

	// displays a printf to the output window
	static void out(const char *const fmt, ...)
	{
		Trace *pTrace = Trace::privGetInstance();
		assert(pTrace);

		std::lock_guard<std::mutex> lock(pTrace->mtx);

		va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
		va_start(args, fmt);
#pragma warning( pop )

		vsprintf_s(&pTrace->privBuff[0], TraceBuffSize, fmt, args);
		OutputDebugString(&pTrace->privBuff[0]);

		//va_end(args);
		args = static_cast<va_list> (nullptr);

	}

	static void out2(const char *const fmt, ...)
	{
		Trace *pTrace = Trace::privGetInstance();
		assert(pTrace);

		std::lock_guard<std::mutex> lock(pTrace->mtx);

		va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
		va_start(args, fmt);
#pragma warning( pop )

		vsprintf_s(&pTrace->privBuff[0], TraceBuffSize, fmt, args);
		vprintf(fmt, args);
		OutputDebugString(&pTrace->privBuff[0]);

		//va_end(args);
		args = static_cast<va_list> (nullptr);

	}

	static const char *GetAzulPath()
	{
		HKEY hKey;
		static char path[256] { 0 };
		DWORD size = sizeof(path);
		LSTATUS status;

		status = RegOpenKeyExA(HKEY_CURRENT_USER, "Environment", 0, KEY_READ, &hKey);
		assert(status == ERROR_SUCCESS);

		status = RegQueryValueExA(hKey, "AZUL_PATH", nullptr, nullptr, (LPBYTE)path, &size);
		assert(status == ERROR_SUCCESS);

		RegCloseKey(hKey);

		return path;
	}

private:
	static Trace *privGetInstance() noexcept
	{
		static Trace helper;
		return &helper;
	}

	char privBuff[TraceBuffSize];
	std::mutex mtx;
};

#endif DEBUG_OUTPUT_H

// -----------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//
// File I/O for logging
//      Need a clean an easy way to create and log to a file in ascii
//      used in course for logging...
//
//      Used to be stand alone files... now all one include
//
// -----------------------------------------------------------------------

#ifndef FILE_IO_H
#define FILE_IO_H


#ifdef _DEBUG
#ifdef _M_X64
const char *const pFileio_mode = "x64_Debug";
#else
const char *const pFileio_mode = "x86_Debug";
#endif
#else
#ifdef _M_X64
const char *const pFileio_mode = "x64_Release";
#else
const char *const pFileio_mode = "x86_Release";
#endif
#endif

class FileIO
{
public:
	static const unsigned int StringSize = 512;

public:

	static void Open(const char *const pFirstName, const char *const pLastName) noexcept
	{
		assert(pFirstName);
		assert(pLastName);
		FileIO::privGetInstance()->privOpen(pFirstName, pLastName);
	}
	static void Close() noexcept
	{
		FileIO::privGetInstance()->privClose();
	}
	static FILE *GetHandle() noexcept
	{
		return FileIO::privGetInstance()->privGetHandle();
	}

private:

	void privOpen(const char *const pFirstName, const char *const pLastName) noexcept
	{
		const char *const pFile_extension = ".txt";
		char pFileName[FileIO::StringSize]{0};
		const char *pAzulPath = Trace::GetAzulPath();

		strcat_s(pFileName, FileIO::StringSize, pAzulPath);
		strcat_s(pFileName, FileIO::StringSize, "\\Logs\\");

		errno_t fileError(0);

		// is there enough of space?
		assert((strlen(pFileName)
				+ strlen(pFirstName)
				+ strlen(pLastName)
				+ strlen("-")
				+ strlen(pFileio_mode)
				+ strlen(pFile_extension)
				+ strlen("/0")) < FileIO::StringSize);

		strcat_s(pFileName, FileIO::StringSize, pFirstName);
		strcat_s(pFileName, FileIO::StringSize, pLastName);
		strcat_s(pFileName, FileIO::StringSize, "_");
		strcat_s(pFileName, FileIO::StringSize, pFileio_mode);
		strcat_s(pFileName, FileIO::StringSize, pFile_extension);

		fileError = fopen_s(&pFileHandle, pFileName, "wt");

		if(fileError != 0)
		{
			char pTmp[FileIO::StringSize]{0};
			strcat_s(pTmp, FileIO::StringSize, "mkdir \"");
			strcat_s(pTmp, FileIO::StringSize, pAzulPath);
			strcat_s(pTmp, FileIO::StringSize, "\\Logs\"");
			// Create the sub directory
			system(pTmp);

			fileError = fopen_s(&pFileHandle, pFileName, "wt");
			assert(pFileHandle);
		}

		if(pFileHandle != nullptr)
		{
			fprintf(this->pFileHandle, "-------------------------------------------------\n");
			fprintf(this->pFileHandle, "\n");
			fprintf(this->pFileHandle, "Log File \n");
			fprintf(this->pFileHandle, "\n");
			fprintf(this->pFileHandle, "Name: %s %s\n", pFirstName, pLastName);
			fprintf(this->pFileHandle, "Mode: %s\n", pFileio_mode);
			fprintf(this->pFileHandle, "\n");
			fprintf(this->pFileHandle, "-------------------------------------------------\n");
			fprintf(this->pFileHandle, "\n");
		}
	}
	FILE *privGetHandle() noexcept
	{
		assert(pFileHandle);
		return this->pFileHandle;
	}
	static FileIO *privGetInstance() noexcept
	{
		static FileIO instance;
		return &instance;
	}
	void privClose() noexcept
	{
		errno_t fileError(0);
		assert(pFileHandle);

		fileError = fflush(this->pFileHandle);
		assert(!fileError);

		fileError = fclose(this->pFileHandle);
		this->pFileHandle = nullptr;
		assert(!fileError);
	}

	// big six  
	constexpr FileIO() noexcept
		:pFileHandle(nullptr)
	{

	};
	FileIO(const FileIO &) = delete;
	FileIO(FileIO &&) = delete;
	FileIO &operator=(const FileIO &) = delete;
	FileIO &operator=(FileIO &&) = delete;
	~FileIO()
	{
		if(nullptr != this->pFileHandle)
		{
			this->privClose();
		}
	};

	// ------------------------------------------
	// data
	// ------------------------------------------

	FILE *pFileHandle;
};

#endif FILE_IO_H


// -----------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//
// CRC32 for File CRC32
//      Easy way to determine that a file has changed 1 or more bytes
//
// -----------------------------------------------------------------------

#ifndef FILE_CRC32_H
#define FILE_CRC32_H

	// Based on Bjorn Samuelsson - fast CRC32 for little endian processors
	// http://home.thep.lu.se/~bjorn/
	// public domain implementation of the standard CRC32 checksum.

#if !GRADER
#define FILE_MODIFIED_CHECK(A,B,C) { uint32_t file_crc; \
								   CHECK(CRC32::GetCRC(A, file_crc) == true); \
								   CHECK(file_crc != C); }
#else
#define FILE_MODIFIED_CHECK(A,B,C) { uint32_t file_crc; \
								   CHECK(CRC32::GetCRC(B, file_crc) == true); \
								   CHECK(file_crc != C); }
#endif

#define FILE_EXIST(A) 	CRC32::FileExist(A)

#define FILE_GET_CRC32(A) CRC32::GetCRC(A)

class CRC32
{
public:
	// Any unsigned integer type with at least 32 bits may be used as
	// accumulator type for fast crc32-calulation, but unsigned long is
	// probably the optimal choice for most systems. 
	typedef unsigned long CRC32_accum_t;
	static const unsigned int CRC32_accum_SIZE = sizeof(CRC32_accum_t);

public:
	// Standard CRC32 checksum: fast public domain implementation for
	// little-endian architectures.  Written for compilation with an
	// optimizer set to perform loop unwinding.  Outputs the checksum for
	// each file given as a command line argument.  Invalid file names and
	// files that cause errors are silently skipped.  The program reads
	// from stdin if it is called with no arguments. 
	static uint32_t GetCRC(const void *pBuff, size_t numBytes) noexcept
	{
		// Get instance
		const CRC32 *pCRC = CRC32::privGetInstance();
		assert(pCRC);

		uint32_t CRC32_result = 0;
		pCRC->privFile(pBuff, numBytes, CRC32_result);

		return CRC32_result;
	}
	static uint32_t GetCRC(const char *const pFileName)
	{
		uint32_t file_crc = 0;
		bool status;
		status = CRC32::GetCRC(pFileName, file_crc);
		assert(status == true);

		return file_crc;
	}
	static bool FileExist(const char *const pFileName) noexcept
	{
		FILE *pFile(nullptr);
		bool Status(false);

		fopen_s(&pFile, pFileName, "rb");
		if(pFile == nullptr)
		{
			Status = false;
		}
		else
		{
			Status = true;
			fclose(pFile);
		}
		return Status;
	}
	static bool GetCRC(const char *const pFileName, uint32_t &crc)
	{
		FILE *pFile(nullptr);
		bool Status(false);
		constexpr unsigned int CRCBuffSize = 1L << 15; // 32K size
		char *pBuff = new char[CRCBuffSize]();
		assert(pBuff);
		crc = 0;			 // set to 0

		// Get instance
		const CRC32 *pCRC = CRC32::privGetInstance();
		assert(pCRC);

		// Open file
		fopen_s(&pFile, pFileName, "rb");
		if(pFile == nullptr)
		{
			Status = false;
		}
		else
		{
			while(!feof(pFile) && !ferror(pFile))
			{
				pCRC->privFile(pBuff, fread(pBuff, 1, CRCBuffSize, pFile), crc);
			}

			if(!ferror(pFile))
			{
				Status = true;
			}
			else
			{
				Status = false;
			}

			fclose(pFile);
		}
		delete[] pBuff;
		pBuff = nullptr;

		return Status;
	}

private:
	CRC32(const CRC32 &) = delete;
	CRC32 &operator = (const CRC32 &) = delete;
	CRC32(CRC32 &&) = delete;
	CRC32 &operator = (CRC32 &&) = delete;
	~CRC32()
	{

	}
	CRC32() noexcept
	{
		this->privInitTables();
	}
	void privFile(const void * const data, const size_t n_bytes, uint32_t &crc) const noexcept
	{
		// get at fresh crc
		crc = 0;

		assert(data);

		const size_t n_accum = n_bytes / sizeof(CRC32_accum_t);

		for(size_t i = 0; i < n_accum; ++i)
		{
			CRC32_accum_t a = crc ^ ((CRC32_accum_t *)data)[i];

			for(size_t j = crc = 0; j < CRC32_accum_SIZE; ++j)
			{
				crc ^= wtable[(j << 8) + (uint8_t)(a >> 8 * j)];
			}
		}

		for(size_t i = n_accum * sizeof(CRC32_accum_t); i < n_bytes; ++i)
		{
			crc = table[(uint8_t)crc ^ ((uint8_t *)data)[i]] ^ crc >> 8;
		}
	}
	void privInitTables() noexcept
	{
		for(uint32_t i = 0; i < 0x100; ++i)
		{
			this->table[i] = privForByte(i);
		}

		for(size_t k = 0; k < CRC32_accum_SIZE; ++k)
		{
			for(uint32_t w, i = 0; i < 0x100; ++i)
			{
				for(size_t j = w = 0; j < CRC32_accum_SIZE; ++j)
				{
					w = this->table[(uint8_t)(j == k ? w ^ i : w)] ^ w >> 8;
				}

				this->wtable[(k << 8) + i] = w ^ (k ? this->wtable[0] : 0);
			}
		}
	}
	uint32_t privForByte(uint32_t r) noexcept
	{
		for(int j = 0; j < 8; ++j)
		{
			r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
		}
		return r ^ (uint32_t)0xFF000000L;
	}
	static CRC32 *privGetInstance() noexcept
	{
		static CRC32 instance;
		return &instance;
	}

	uint32_t table[0x100];
	uint32_t wtable[0x100 * sizeof(CRC32_accum_t)];

};


#endif

// -----------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//
// Align16
//      Needed a clean way to allocate aligned data for Matrix, Quat, Vector
//      Data is align by inheriting this class
//      
//
//      Used to be stand alone files... now all one include
//
// -----------------------------------------------------------------------


#ifndef ALIGN_16_H
#define ALIGN_16_H

#define ALIGN_UNUSED_VAR(x) (void(x))

#pragma push_macro("new")
#undef new

class Align16
{
public:

	// Placement new for align16
	void *operator new(size_t, void *p) noexcept
	{
		ALIGN_UNUSED_VAR(p);
		return p;
	}

	void *operator new(size_t size) noexcept
	{
		void *p = _aligned_malloc(size, 16);
		return p;
	}

	void operator delete(void *p)
	{
		_aligned_free(p);
	}

	void *operator new[](size_t size) noexcept
	{
		void *p = _aligned_malloc(size, 16);
		return p;
	}

	void operator delete[](void *p)
	{
		_aligned_free(p);
	}

	void *operator new(size_t      size,
					   int         _BlockUse,
					   char const *_FileName,
					   int         _LineNumber) noexcept
	{
		ALIGN_UNUSED_VAR(_BlockUse);
		ALIGN_UNUSED_VAR(_FileName);
		ALIGN_UNUSED_VAR(_LineNumber);

		void *p = _aligned_malloc_dbg(size, 16, _FileName, _LineNumber);
		return p;
	}

	void operator delete(void *p,
						 int         _BlockUse,
						 char const *_FileName,
						 int         _LineNumber)
	{
		ALIGN_UNUSED_VAR(_BlockUse);
		ALIGN_UNUSED_VAR(_FileName);
		ALIGN_UNUSED_VAR(_LineNumber);
		_aligned_free_dbg(p);
	}

	void *operator new[](size_t        size,
						 int         _BlockUse,
						 char const *_FileName,
						 int         _LineNumber)  noexcept
	{
		ALIGN_UNUSED_VAR(_BlockUse);
		ALIGN_UNUSED_VAR(_FileName);
		ALIGN_UNUSED_VAR(_LineNumber);

		void *p = _aligned_malloc_dbg(size, 16, _FileName, _LineNumber);
		return p;
	}

	void operator delete[](void *p,
						   int         _BlockUse,
						   char const *_FileName,
						   int         _LineNumber)
	{
		ALIGN_UNUSED_VAR(_BlockUse);
		ALIGN_UNUSED_VAR(_FileName);
		ALIGN_UNUSED_VAR(_LineNumber);
		_aligned_free_dbg(p);
	}

};

#pragma pop_macro("new")

#endif //ALIGN_16_H


// -----------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//
// Memory Tracker
//      Homemade Memory Tracker sitting on top of CRTDEBUG
//      Works by macro trick.. replacing everyone's new with macro
//      Problems with placement new() need to follow instructions
//      If using aligned allocations... need to overload new/delete operator
//      see Align16 for example
//      Only works in Debug and with MEM_TRACKER_ENABLE is defined not in RELEASE
//      Works in x86/64 and with external libraries that are leaking as well
//
//      Used to be stand alone files... now all one include
//
// -----------------------------------------------------------------------

#ifndef MEM_TRACKER_H
#define MEM_TRACKER_H

		// ---------------------------------------------------------------------------
		// One instructions to rule them all:
		//
		//   To Enable:
		//           #define MEM_TRACKER_ENABLED
		//   To Disable:
		//           Comment out #define MEM_TRACKER_ENABLED
		//
		//      Include Framework.h to every file:
		//           Change your project settings
		//               Go to C / C++ tab
		//               Go to Advanced
		//                      set Forced Include : Framework.h
		//               This will add this file automagically to all files (great!)
		//
		//  placement new() label:
		//
		//      No memory tracking __SHOULD__ take place on placement new
		//      So you need to tell the Memory Tracking system to ignore this placement new
		//      Add 3 lines around placement new to allow it to compile
		//
		//      example:  Dog *pDog = new(address) Dog();  // original (new way below)
		//
		//      AZUL_PLACEMENT_NEW_BEGIN   <----- Add 1/3 --------------
		//      #undef new                 <----- Add 2/3 --------------
		//
		//              Dog *pDog = new(address) Dog();
		//
		//      AZUL_PLACEMENT_NEW_END     <----- Add 3/3 --------------
		//
		//      Note:
		//           Need to have matching BEGIN/END macros or an assert will happen
		//
		//   Issue with new(std::nothrow)
		//      This needs to be tracked... but the macro trick is a problem
		//
		// ---------------------------------------------------------------------------

#include <set>

#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
#define AllocRefTracker  { _CrtMemState state{0}; \
		_CrtMemCheckpoint(&state); \
		MemTrace::_CrtMemBlockHeader *pTmp; \
		pTmp = reinterpret_cast<MemTrace::_CrtMemBlockHeader *> (state.pBlockHeader); \
		Trace::out("\nref:%d size:%d \n \t%s(%d):\n\n", pTmp->lRequest,pTmp->nDataSize, __FILE__, __LINE__); }
#else
#define AllocRefTracker
#endif

enum class MemTracker
{
	Disabled
};

#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
constexpr const char *MEMORY_TRACKING_ENABLE_STRING = "enabled         ";
#else
constexpr const char *MEMORY_TRACKING_ENABLE_STRING = "--> DISABLED <--";
#endif

class MemTrace
{
private:
	static const unsigned int MemTraceBuffSize = 256;
	static const unsigned int MemStringSize = 512;
	static const int MemoryCheck_Disable_Magic_num = -1581;

public:
	// -----------------------------------------------------
	// Only found this structure in documentation
	// created a shadow structure to allow error reporting
	// -----------------------------------------------------
	typedef struct _CrtMemBlockHeader
	{
		//static const unsigned int nNoMansLandSize = 4; // said in doc that its 4

		struct _CrtMemBlockHeader *pBlockHeaderNext;
		struct _CrtMemBlockHeader *pBlockHeaderPrev;
		char *szFileName;    // File name
		int nLine;           // Line number
		int nBlockUse;       // Type of block
		size_t nDataSize;    // Size of user block
		long lRequest;       // Allocation number
		//unsigned char gap[nNoMansLandSize];
		char pad[4]; // x64 padding

		// ---------------------------------------------------
		// In an actual memory block in the debug heap,
		//    this structure is followed by:
		//       unsigned char data[nDataSize];
		//       unsigned char anotherGap[nNoMansLandSize];
		// ---------------------------------------------------

	} _CrtMemBlockHeader;


	struct _MemTrackingHeader
	{
		char szFileName[MemTraceBuffSize];    // File name
		int nLine;           // Line number
		int nBlockUse;       // Type of block
		size_t nDataSize;    // Size of user block
		long lRequest;       // Allocation number

		void Clear() noexcept
		{
			memset(this, 0, sizeof(_MemTrackingHeader));
		}

		void Set(const _CrtMemBlockHeader *pTmp) noexcept
		{
			assert(pTmp);
			if(pTmp->szFileName != nullptr)
			strcpy_s(this->szFileName, MemTraceBuffSize, pTmp->szFileName);
			this->nLine = pTmp->nLine;
			this->nBlockUse = pTmp->nBlockUse;
			this->nDataSize = pTmp->nDataSize;
			this->lRequest = pTmp->lRequest;
		}
	};

	struct _MemTrackingHeader_comp
	{
		bool operator()(const _MemTrackingHeader &lhs, const _MemTrackingHeader &rhs) const noexcept
		{
			const int strResult = strcmp(lhs.szFileName, rhs.szFileName);
			if(strResult < 0)
			{
				return true;
			}
			else if(strResult > 0)
			{
				return false;
			}
			else if(lhs.nLine < rhs.nLine)
			{
				return true;
			}
			else if(lhs.nLine > rhs.nLine)
			{
				return false;
			}
			else if(lhs.nBlockUse < rhs.nBlockUse)
			{
				return true;
			}
			else if(lhs.nBlockUse > rhs.nBlockUse)
			{
				return false;
			}
			else if(lhs.nDataSize < rhs.nDataSize)
			{
				return true;
			}
			else if(lhs.nDataSize > rhs.nDataSize)
			{
				return false;
			}
			else if(lhs.lRequest < rhs.lRequest)
			{
				return true;
			}
			else if(lhs.lRequest > rhs.lRequest)
			{
				return false;
			}
			else
			{
				return false;
			}
		}
	};



public:
	// Big Six
	MemTrace() noexcept
		:
		mtx(),
		mtxMemLeakCount(),
		MemLeakCount_Start{0},
		PlacementNew_Count{0},
		PlacementNew_mtx()
	{
		memset(&privBuff[0], 0, MemTraceBuffSize);
		this->PlacementNew_Count = 0;
	}
	MemTrace(const MemTrace &) = delete;
	MemTrace(MemTrace &&) = delete;
	MemTrace &operator = (const MemTrace &) = delete;
	MemTrace &operator = (MemTrace &&) = delete;
	~MemTrace() = default;

private:
	// displays a printf to the output window
	void privOut(const char *const fmt, ...) noexcept
	{
		MemTrace &rTrace = MemTrace::privGetRefInstance();

		std::lock_guard<std::mutex> lck(rTrace.mtx);

		va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
		va_start(args, fmt);
#pragma warning( pop )

		vsprintf_s(&rTrace.privBuff[0], MemTraceBuffSize, fmt, args);
		OutputDebugString(&rTrace.privBuff[0]);

		// va_end(args); - original.. below to new code
		args = static_cast<va_list> (nullptr);
	}

	char *privStripDir(const char *const pInName) noexcept
	{
		char *pReturn = (char *)(pInName);

		const char *pch = pInName;

		while(pch != nullptr)
		{
			pch = strstr(pch, "\\");
			if(pch != nullptr)
			{
				pch += 1;
				pReturn = (char *)pch;
			}
		}

		return pReturn;
	}

	bool privSSEMessage(const char *const pName, bool flag) noexcept
	{
		this->privOut("** %12s %13s   **\n", pName, flag ?
					  " ready             " :
					  " --> DISABLED <--  ");
		return flag;
	};

public:
	static int ApplicationMemLeakCount() noexcept
	{
#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
		FILE *pMailBoxFile = nullptr;
		int val = 0;
		char pStringMailBox[MemTrace::MemStringSize]{0};
		const char *pAzulPath = Trace::GetAzulPath();

		strcat_s(pStringMailBox, MemTrace::MemStringSize, pAzulPath);
		strcat_s(pStringMailBox, MemTrace::MemStringSize, "\\Azul\\MemMailBox.bin");
		fopen_s(&pMailBoxFile, pStringMailBox, "rb");
		if(pMailBoxFile != nullptr)
		{
			fread_s(&val, sizeof(int), sizeof(int), 1, pMailBoxFile);
			fclose(pMailBoxFile);
		}
#else
		int val = 0;
#endif
		return val;
	}

	static int LeakCount() noexcept
	{
		MemTrace &rTrace = MemTrace::privGetRefInstance();
		std::lock_guard<std::mutex> loc(rTrace.mtxMemLeakCount);

		const int NormBlockLeakCount = rTrace.privLeakCount();

		return NormBlockLeakCount;
	}

	int privLeakCount() noexcept
	{
#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
		_CrtMemState state{nullptr};
		_CrtMemCheckpoint(&state);

		_CrtMemBlockHeader *pTmp;
		pTmp = reinterpret_cast<MemTrace::_CrtMemBlockHeader *> (state.pBlockHeader);

		int NormBlockLeakCount = 0;

		// Walk to the end of list && see if there is any leaks
		while(pTmp != nullptr)
		{
			if(pTmp->nBlockUse == _NORMAL_BLOCK)
			{
				NormBlockLeakCount++;
			}

			if(pTmp->pBlockHeaderNext == nullptr)
			{
				break;
			}
			pTmp = pTmp->pBlockHeaderNext;
		}
#else
		int NormBlockLeakCount = 0;
#endif

		return NormBlockLeakCount;
	}

	static void ProcessEnd() noexcept
	{
		// -------------------------------
		// DLL_COUNT.bin
		// -------------------------------

		FILE *pFile;

		char pFileName[MemTrace::MemStringSize]{0};

		const char *pAzulPath = Trace::GetAzulPath();

		strcat_s(pFileName, MemTrace::MemStringSize, pAzulPath);
		strcat_s(pFileName, MemTrace::MemStringSize, "\\Azul\\DLL_COUNT.bin");

		// -------------------------------
		// Is this the last time?
		// -------------------------------
		int count = 0xabcd;

		errno_t fError = fopen_s(&pFile, pFileName, "rb");
		assert(fError == 0);

		size_t CountRead;
		CountRead = fread_s(&count, sizeof(int), sizeof(int), 1, pFile);
		assert(CountRead == 1);

		int fCloseError;
		fCloseError = fclose(pFile);
		assert(fCloseError == 0);

		bool LastTime;

		// -----------------------------------------------
		// Write decrement to DLL_COUNT.bin
		// -----------------------------------------------

		fError = fopen_s(&pFile, pFileName, "wb");
		assert(pFile);
		assert(fError == 0);

		count--;

		size_t CountWrite;
		CountWrite = fwrite(&count, sizeof(int), 1, pFile);
		assert(CountWrite == 1);

		fCloseError = fclose(pFile);
		assert(fCloseError == 0);

		if(count == 0)
		{
			LastTime = true;

			// delete DLL_COUNT.bin 
			char pTmp[MemTrace::MemStringSize] = {0};
			strcat_s(pTmp, MemTrace::MemStringSize, "del ");
			strcat_s(pTmp, MemTrace::MemStringSize, pFileName);
			system(pTmp);
		}
		else
		{
			LastTime = false;
		}

		// -------------------------------------------------------------------------
		// RECORD memory tracking data to File for each dll or main application
		// -------------------------------------------------------------------------
		{

			FILE *pMemTrackerFile = nullptr;

			char pStringMemTracker[MemTrace::MemStringSize]{0};
			strcat_s(pStringMemTracker, MemTrace::MemStringSize, pAzulPath);
			strcat_s(pStringMemTracker, MemTrace::MemStringSize, "\\Azul\\MemTracker.bin");
			errno_t erro;
			erro = fopen_s(&pMemTrackerFile, pStringMemTracker, "ab");
			assert(erro == 0);

			fseek(pMemTrackerFile, 0, SEEK_END);

			_CrtMemState state{0};
			_CrtMemCheckpoint(&state);

			_CrtMemBlockHeader *pTmp = reinterpret_cast<MemTrace::_CrtMemBlockHeader *> (state.pBlockHeader);

			//_MemTrackingHeader MemTrackerHeaderTmp;

			while(pTmp != nullptr)
			{
				_MemTrackingHeader MemTrackerHeaderTmp { 0 };
				MemTrackerHeaderTmp.Clear();
				MemTrackerHeaderTmp.Set(pTmp);
				fwrite(&MemTrackerHeaderTmp, sizeof(_MemTrackingHeader), 1, pMemTrackerFile);
				fflush(pMemTrackerFile);

				pTmp = pTmp->pBlockHeaderNext;
			}

			if(pMemTrackerFile != nullptr)
			{
				fclose(pMemTrackerFile);
			}
		}

#ifdef VERBOSE_DLL_INFO
	//	LastTime = true;
#endif
		// -------------------------------------------------------------------------
		// Last DLL or Application - print summary of leaks
		// -------------------------------------------------------------------------
		if(LastTime)
		{

			MemTrace &rTrace = MemTrace::privGetRefInstance();
			//rTrace.privOut("exit count: %d \n", count);

			// This is included so you can have one universal include
			std::call_once(rTrace.ProcessEndFlag, [&rTrace, pAzulPath]() noexcept
						   {
							   FILE *pFile = nullptr;
							   FILE *pMailBoxFile = nullptr;
							   FILE *pMemTrackerFile = nullptr;

							   char pString2[MemTrace::MemStringSize]{0};
							   strcat_s(pString2, MemTrace::MemStringSize, pAzulPath);
							   strcat_s(pString2, MemTrace::MemStringSize, "\\Azul\\MemTrackerLog.txt");
							   errno_t err = fopen_s(&pFile, pString2, "wt");
							   assert(err == 0);

							   char pStringMailBox[MemTrace::MemStringSize]{0};
							   strcat_s(pStringMailBox, MemTrace::MemStringSize, pAzulPath);
							   strcat_s(pStringMailBox, MemTrace::MemStringSize, "\\Azul\\MemMailBox.bin");
							   err = fopen_s(&pMailBoxFile, pStringMailBox, "wb");
							   assert(err == 0);

							   char pStringMemTracker[MemTrace::MemStringSize]{0};
							   strcat_s(pStringMemTracker, MemTrace::MemStringSize, pAzulPath);
							   strcat_s(pStringMemTracker, MemTrace::MemStringSize, "\\Azul\\MemTracker.bin");
							   err = fopen_s(&pMemTrackerFile, pStringMemTracker, "rb");
							   assert(err == 0);

							   assert(pFile);
							   if(pFile != nullptr)
							   {
								   fprintf(pFile, "\n");
								   fprintf(pFile, "****************************************\n");
								   fprintf(pFile, "\n");
								   fprintf(pFile, "  MemTrackerLog \n");
								   fprintf(pFile, "\n");
								   fprintf(pFile, "       %s  \n", __DATE__);
								   fprintf(pFile, "       %s  \n", __TIME__);
								   fprintf(pFile, "\n");
								   fprintf(pFile, "****************************************\n");
								   fprintf(pFile, "\n");
								   fprintf(pFile, "External Libs: \n");
								   fprintf(pFile, "\n");
							   }

							  // rTrace.privOut("\n");

							   fseek(pMemTrackerFile, 0, SEEK_END);
							   const size_t fileSize = (size_t)ftell(pMemTrackerFile);
							   fseek(pMemTrackerFile, 0, SEEK_SET);
							   unsigned char *poBuff = new unsigned char[fileSize]();
							   fread(poBuff, fileSize, 1, pMemTrackerFile);

							   const _MemTrackingHeader *pMemTrackerHeaderTmp = reinterpret_cast<_MemTrackingHeader *>(poBuff);
							   const size_t count = fileSize / (sizeof(_MemTrackingHeader));


							   unsigned int i = 0;

							   size_t appMaxIndex = 0;
							   size_t appMaxSize = 0;
							   size_t appCount = 0;
							   size_t appTotalSize = 0;
							   _MemTrackingHeader *pAppMaxTmp = nullptr;

							   size_t extMaxIndex = 0;
							   size_t extMaxSize = 0;
							   size_t extCount = 0;
							   size_t extTotalSize = 0;
							   _MemTrackingHeader *pExtMaxTmp = nullptr;

							   std::set<_MemTrackingHeader, _MemTrackingHeader_comp> MemTrackingHeaderSet;

							   int NormBlockLeakCount = 0;

							   // Walk to the end of list && see if there is any leaks
							   for(size_t j = 0; j < count; j++)
							   {
								   MemTrackingHeaderSet.insert(pMemTrackerHeaderTmp[j]);
							   }

							   for(auto &it : MemTrackingHeaderSet)
							   {
								   if(it.nBlockUse == _NORMAL_BLOCK)
								   {
									   NormBlockLeakCount++;
								   }
							   }

							   if(MemTrace::GetPlacementNewCount() != 0)
							   {
								   rTrace.privOut("--------------------------------\n");
								   printf("--------------------------------\n");

								   rTrace.privOut(">>> CRASH: placement new macro mismatch <<<<<<\n");
								   printf(">>> CRASH: placement new macro mismatch <<<<<<\n");
								   rTrace.privOut("--------------------------------\n");
								   printf("--------------------------------\n");

								   assert(MemTrace::GetPlacementNewCount() == 0);
							   }

							   if(NormBlockLeakCount > 0)
							   {
								   rTrace.privOut("------------------------------------------------------\n");
								   rTrace.privOut(">>>   Memory Tracking: Leaks detected              <<<\n");
								   rTrace.privOut(">>>   Double-click on file string to Leak location <<<\n");
								   rTrace.privOut("------------------------------------------------------\n");
								   rTrace.privOut("\n");

								   for(auto &it : MemTrackingHeaderSet)
								   {
									   _MemTrackingHeader *pTmp = (_MemTrackingHeader * )&it;

									   if(pTmp->nBlockUse == _NORMAL_BLOCK)
									   {
										   // treat allocation with/without names as different groups
										   if(pTmp->szFileName == nullptr)
										   {
											   extTotalSize += pTmp->nDataSize;
											   extCount++;
											   if(extMaxSize < pTmp->nDataSize)
											   {
												   extMaxSize = pTmp->nDataSize;
												   extMaxIndex = i;
												   pExtMaxTmp = pTmp;
											   }
											   if(pFile != nullptr)
											   {
												   fprintf(pFile, "Leak(%d)  %d bytes ref:%d \n", (int)i, (int)pTmp->nDataSize, (int)pTmp->lRequest);
											   }
										   }
										   else
										   {
											   appTotalSize += pTmp->nDataSize;
											   appCount++;
											   if(appMaxSize < pTmp->nDataSize)
											   {
												   appMaxSize = pTmp->nDataSize;
												   appMaxIndex = i;
												   pAppMaxTmp = pTmp;
											   }

											   if(appCount < 101)
											   {
												   rTrace.privOut("Leak(%d)  %d bytes ref:%d %s \n", i, pTmp->nDataSize, pTmp->lRequest, rTrace.privStripDir(pTmp->szFileName));
												   rTrace.privOut("   %s(%d) : <double-click> \n", &pTmp->szFileName[0], pTmp->nLine);
												   rTrace.privOut("\n");
											   }
											   if(appCount == 101)
											   {
												   rTrace.privOut("--> ...Leak exceeds 100... <-- \n");
												   rTrace.privOut("\n");
											   }
										   }

										   i++;
									   }

								   }

								   rTrace.privOut("Memory Tracking statistics \n");
								   rTrace.privOut("\n");
								   rTrace.privOut("     Application files: \n");
								   rTrace.privOut("                num of leaks: %d \n", appCount);
								   rTrace.privOut("          total bytes leaked: %d \n", appTotalSize);
								   if(pAppMaxTmp != nullptr)
								   {
									   rTrace.privOut("          largest individual: Leak(%d) size: %d \n", appMaxIndex, pAppMaxTmp->nDataSize);
								   }
								   rTrace.privOut("\n");
								   rTrace.privOut("     External Libs: \n");
								   rTrace.privOut("                num of leaks: %d \n", extCount);
								   rTrace.privOut("          total bytes leaked: %d \n", extTotalSize);

								   if(pExtMaxTmp != nullptr)
								   {
									   rTrace.privOut("          largest individual: Leak(%d) size: %d \n", extMaxIndex, pExtMaxTmp->nDataSize);
								   }
								   if(pFile != nullptr)
								   {
									   fprintf(pFile, "\n");
									   fprintf(pFile, "Memory Tracking Stats:\n");
									   fprintf(pFile, "\n");
									   fprintf(pFile, "     Application files: \n");
									   fprintf(pFile, "                num of leaks: %d \n", (int)appCount);
									   fprintf(pFile, "          total bytes leaked: %d \n", (int)appTotalSize);
									   if(pAppMaxTmp != nullptr)
									   {
										   fprintf(pFile, "          largest individual: Leak(%d) size: %d \n", (int)appMaxIndex, (int)pAppMaxTmp->nDataSize);
									   }
									   fprintf(pFile, "\n");
									   fprintf(pFile, "     External Libs: \n");
									   fprintf(pFile, "                num of leaks: %d \n", (int)extCount);
									   fprintf(pFile, "          total bytes leaked: %d \n", (int)extTotalSize);

									   if(pExtMaxTmp != nullptr)
									   {
										   fprintf(pFile, "          largest individual: Leak(%d) size: %d \n", (int)extMaxIndex, (int)pExtMaxTmp->nDataSize);
									   }
								   }
								

								   char pTmp5[MemTrace::MemStringSize]{0};
								   strcat_s(pTmp5, MemTrace::MemStringSize, pAzulPath);
								   strcat_s(pTmp5, MemTrace::MemStringSize, "\\Azul\\MemTrackerLog.txt");

								   rTrace.privOut("\n");
								   rTrace.privOut("     MemTrackerLog Link: \n");
								   rTrace.privOut("          %s(1) : <double-click>\n", pTmp5);
								   rTrace.privOut("\n");

							   }

							   rTrace.privOut("\n");
							   rTrace.privOut("--------------------------------\n");
							   printf("--------------------------------\n");
							   if(appCount)
							   {
								   rTrace.privOut(">>> Memory Tracking: FAIL <<<<<<\n");
								   printf(">>> Memory Tracking: FAIL <<<<<<\n");
							   }
							   else
							   {
								   rTrace.privOut("    Memory Tracking: passed \n");
								   printf("    Memory Tracking: passed \n");
							   }
							   rTrace.privOut("--------------------------------\n");
							   rTrace.privOut("    Memory Tracking: end()      \n");
							   rTrace.privOut("--------------------------------\n");
							   printf("--------------------------------\n");
							   printf("    Memory Tracking: end()      \n");
							   printf("--------------------------------\n");
							   printf("\n");


							   assert(pMailBoxFile);
							   if(pMailBoxFile != nullptr)
							   {
								   fwrite(&appCount, sizeof(int), 1, pMailBoxFile);
							   }
							   if(pMailBoxFile != nullptr)
							   {
								   fclose(pMailBoxFile);
							   }
							   if(pMemTrackerFile != nullptr)
							   {
								   fclose(pMemTrackerFile);
							   }
							   if(pFile != nullptr)
							   {
								   fclose(pFile);
							   }

							   // remove DLL_COUNT.bin + MemTracker.bin directory
							   char pStringTmp2[MemTrace::MemStringSize]{0};
							   strcat_s(pStringTmp2, MemTrace::MemStringSize, pAzulPath);
							   strcat_s(pStringTmp2, MemTrace::MemStringSize, "\\Azul\\*.bin ");

							   char pTmp2[MemTrace::MemStringSize]{0};
							   strcat_s(pTmp2, MemTrace::MemStringSize, "del \"");
							   strcat_s(pTmp2, MemTrace::MemStringSize, pStringTmp2);
							   strcat_s(pTmp2, MemTrace::MemStringSize, "\"");
							   system(pTmp2);



							   printf("\n");
							   rTrace.privOut("\n");

							   printf("****************************************\n");
							   rTrace.privOut("****************************************\n");

						   }
			);
		}
		else
		{
			//MemTrace &rTrace = MemTrace::privGetRefInstance();
			//rTrace.privOut("exit count: %d \n", count);
		}
	};

	static void ProcessEnd_Release() noexcept
	{
		// -------------------------------
		// DLL_COUNT.bin
		// -------------------------------

		FILE *pFile;

		char pFileName[MemTrace::MemStringSize]{0};
		const char *pAzulPath = Trace::GetAzulPath();

		strcat_s(pFileName, MemTrace::MemStringSize, pAzulPath);
		strcat_s(pFileName, MemTrace::MemStringSize, "\\Azul\\DLL_COUNT.bin");

		// -------------------------------
		// Is this the last time?
		// -------------------------------
		int count = 0xabcd;

		errno_t fError = fopen_s(&pFile, pFileName, "rb");
		assert(fError == 0);

		size_t CountRead;
		CountRead = fread_s(&count, sizeof(int), sizeof(int), 1, pFile);
		assert(CountRead == 1);

		int fCloseError;
		fCloseError = fclose(pFile);
		assert(fCloseError == 0);

		bool LastTime;

		// -----------------------------------------------
		// Write decrement to DLL_COUNT.bin
		// -----------------------------------------------

		fError = fopen_s(&pFile, pFileName, "wb");
		assert(pFile);

		count--;

		size_t CountWrite;
		CountWrite = fwrite(&count, sizeof(int), 1, pFile);
		assert(CountWrite == 1);

		fCloseError = fclose(pFile);
		assert(fCloseError == 0);

		//Trace::out("count:%d\n", count);
		if(count == 0)
		{
			LastTime = true;
		}
		else
		{
			LastTime = false;
		}

		// -------------------------------------------------------------------------
		// Last DLL or Application - print summary of leaks
		// -------------------------------------------------------------------------
		if(LastTime)
		{

			MemTrace &rTrace = MemTrace::privGetRefInstance();
			//rTrace.privOut("exit count: %d \n", count);

			// This is included so you can have one universal include
			std::call_once(rTrace.ProcessEndFlag, [&rTrace, pAzulPath]() noexcept
						   {
							   AZUL_UNUSED_VAR(rTrace);

							   // remove DLL_COUNT.bin + MemTracker.bin directory
							   char pStringTmp2[MemTrace::MemStringSize]{0};
							   strcat_s(pStringTmp2, MemTrace::MemStringSize, pAzulPath);
							   strcat_s(pStringTmp2, MemTrace::MemStringSize, "\\Azul\\*.bin ");

							   char pTmp2[MemTrace::MemStringSize]{0};
							   strcat_s(pTmp2, MemTrace::MemStringSize, "del \"");
							   strcat_s(pTmp2, MemTrace::MemStringSize, pStringTmp2);
							   strcat_s(pTmp2, MemTrace::MemStringSize, "\"");
							   system(pTmp2);

							   printf("\n");
							   rTrace.privOut("\n");

							   printf("****************************************\n");
							   rTrace.privOut("****************************************\n");
						   }
			);
		}
		else
		{
			//MemTrace &rTrace = MemTrace::privGetRefInstance();
			//rTrace.privOut("exit count: %d \n", count);
		}
	};

	static void ProcessBegin() noexcept
	{
		FILE *pFile;

		char pFileName[MemTrace::MemStringSize]{0};

		const char *pAzulPath = Trace::GetAzulPath();

		strcat_s(pFileName, MemTrace::MemStringSize, pAzulPath);
		strcat_s(pFileName, MemTrace::MemStringSize, "\\Azul\\DLL_COUNT.bin");

		// -------------------------------
		// Is this the first time?
		// -------------------------------
		int count = 0xabcd;
		errno_t fError = fopen_s(&pFile, pFileName, "rb");

		bool FirstTime;

		if(fError != 0)
		{
			FirstTime = true;

			char pTmp[MemTrace::MemStringSize]{0};
			strcat_s(pTmp, MemTrace::MemStringSize, "mkdir \"");
			strcat_s(pTmp, MemTrace::MemStringSize, pAzulPath);
			strcat_s(pTmp, MemTrace::MemStringSize, "\\Azul\"");

			// Create the sub directory
			system(pTmp);

			// Now add the DLL_Count.bin
			fError = fopen_s(&pFile, pFileName, "wb");
			assert(fError == 0);

			count = 0;

			size_t CountWrite;
			CountWrite = fwrite(&count, sizeof(int), 1, pFile);
			assert(CountWrite == 1);

			int fCloseError;
			fCloseError = fclose(pFile);
			assert(fCloseError == 0);

		}
		else
		{
			FirstTime = false;

			size_t CountRead;

			// Read the DLL_Count.bin
			CountRead = fread_s(&count, sizeof(int), sizeof(int), 1, pFile);
			assert(CountRead == 1);

			int fCloseError;
			fCloseError = fclose(pFile);
			assert(fCloseError == 0);
		}

		// ---------------------------------------
		// Write incremented to DLL_COUNT.bin
		// ---------------------------------------

		fError = fopen_s(&pFile, pFileName, "wb");
		assert(fError == 0);

		count++;

		size_t CountWrite;
		CountWrite = fwrite(&count, sizeof(int), 1, pFile);
		assert(CountWrite == 1);

		int fCloseError;
		fCloseError = fclose(pFile);
		assert(fCloseError == 0);

#ifdef VERBOSE_DLL_INFO
		FirstTime = true;
#endif

		if(FirstTime)
		{
			MemTrace &rTrace = MemTrace::privGetRefInstance();
			//rTrace.privOut("count: %d \n", count);

			// This is included so you can have one universal include
			std::call_once(rTrace.ProcessBeginFlag, [&rTrace]() noexcept
						   {

							   rTrace.privOut("\n");
							   rTrace.privOut("****************************************\n");
							   rTrace.privOut("**      Framework: %s               **\n", FRAMEWORK_VER);
							   rTrace.privOut("**   C++ Compiler: %d          **\n", _MSC_FULL_VER);
							   rTrace.privOut("**  Tools Version: %s        **\n", TOOLS_VERSION);
							   rTrace.privOut("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM);
							   rTrace.privOut("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING);
							   rTrace.privOut("**           Mode: %s        **\n", BUILD_CONFIG_MODE);
							   rTrace.privOut("****************************************\n");
#ifdef TEACHING_PC
							   rTrace.privOut("**    TEACHING PC: --> ENABLED <--    **\n");
							   rTrace.privOut("****************************************\n");

#endif
#ifdef USE_THREAD_FRAMEWORK
							   rTrace.privOut("**         Thread: %s               **\n", THREAD_FRAMEWORK_VER);
							   rTrace.privOut("****************************************\n");
#endif

#ifdef SIMD_SUPPORT_PRINTS

							   //	InstructionSet *InstructionSet::poInstance = nullptr;

							   SIMDInstructionSet *pInst = new SIMDInstructionSet();
							   rTrace.privOut("**  -- SIMD SUPPORT -------------     **\n");
							   rTrace.privSSEMessage("MMX", pInst->MMX());
							   rTrace.privSSEMessage("SSE", pInst->SSE());
							   rTrace.privSSEMessage("SSE2", pInst->SSE2());
							   rTrace.privSSEMessage("SSE3", pInst->SSE3());
							   rTrace.privSSEMessage("SSE4.1", pInst->SSE41());
							   rTrace.privSSEMessage("SSE4.2", pInst->SSE42());
							   rTrace.privSSEMessage("SSSE3", pInst->SSSE3());
							   rTrace.privSSEMessage("AVX", pInst->AVX());
							   rTrace.privSSEMessage("AVX2", pInst->AVX2());
							   rTrace.privSSEMessage("FMA", pInst->FMA());
							   rTrace.privSSEMessage("AVX512CD", pInst->AVX512CD());
							   rTrace.privSSEMessage("AVX512ER", pInst->AVX512ER());
							   rTrace.privSSEMessage("AVX512F", pInst->AVX512F());
							   rTrace.privSSEMessage("AVX512PF", pInst->AVX512PF());

							   delete pInst;
							   rTrace.privOut("****************************************\n");

#endif
							   rTrace.privOut("\n");
							   printf("\n");
							   printf("****************************************\n");
							   printf("**      Framework: %s               **\n", FRAMEWORK_VER);
							   printf("**   C++ Compiler: %d          **\n", _MSC_FULL_VER);
							   printf("**  Tools Version: %s        **\n", TOOLS_VERSION);
							   printf("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM);
							   printf("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING);
							   printf("**           Mode: %s        **\n", BUILD_CONFIG_MODE);
							   printf("****************************************\n");
#ifdef TEACHING_PC
							   printf("**    TEACHING PC: --> ENABLED <--    **\n");
							   printf("****************************************\n");
#endif
#ifdef USE_THREAD_FRAMEWORK
							   printf("**         Thread: %s               **\n", THREAD_FRAMEWORK_VER);
							   printf("****************************************\n");
#endif
							   rTrace.privOut("--------------------------------\n");
							   rTrace.privOut("    Memory Tracking: start()    \n");
							   rTrace.privOut("--------------------------------\n");
							   rTrace.privOut("\n");
							   printf("--------------------------------\n");
							   printf("    Memory Tracking: start()    \n");
							   printf("--------------------------------\n");
							   printf("\n");

						   }
			);
		}
		else
		{
			//MemTrace &rTrace = MemTrace::privGetRefInstance();
			//rTrace.privOut("count: %d \n", count);
		}

	}

	static void ProcessBegin_Release()
	{
		FILE *pFile;

		char pFileName[MemTrace::MemStringSize]{0};
		const char *pAzulPath = Trace::GetAzulPath();

		strcat_s(pFileName, MemTrace::MemStringSize, pAzulPath);
		strcat_s(pFileName, MemTrace::MemStringSize, "\\Azul\\DLL_COUNT.bin");

		// -------------------------------
		// Is this the first time?
		// -------------------------------
		int count = 0xabcd;
		errno_t fError = fopen_s(&pFile, pFileName, "rb");

		bool FirstTime;

		if(fError != 0)
		{
			FirstTime = true;
			
			char pTmp[MemTrace::MemStringSize]{0};
			strcat_s(pTmp, MemTrace::MemStringSize, "mkdir \"");
			strcat_s(pTmp, MemTrace::MemStringSize, pAzulPath);
			strcat_s(pTmp, MemTrace::MemStringSize, "\\Azul\"");

			// Create the sub directory
			system(pTmp);

			fError = fopen_s(&pFile, pFileName, "wb");
			assert(fError == 0);

			count = 0;

			size_t CountWrite;
			CountWrite = fwrite(&count, sizeof(int), 1, pFile);
			assert(CountWrite == 1);

			int fCloseError;
			fCloseError = fclose(pFile);
			assert(fCloseError == 0);

		}
		else
		{
			FirstTime = false;

			size_t CountRead;
			CountRead = fread_s(&count, sizeof(int), sizeof(int), 1, pFile);
			assert(CountRead == 1);

			int fCloseError;
			fCloseError = fclose(pFile);
			assert(fCloseError == 0);
		}

		// -------------------------------
		// Write incremented to DLL_COUNT.bin
		// -----------------------------------------------

		fError = fopen_s(&pFile, pFileName, "wb");
		assert(fError == 0);

		count++;

		size_t CountWrite;
		CountWrite = fwrite(&count, sizeof(int), 1, pFile);
		assert(CountWrite == 1);

		int fCloseError;
		fCloseError = fclose(pFile);
		assert(fCloseError == 0);

#ifdef VERBOSE_DLL_INFO
		FirstTime = true;
#endif

		if(FirstTime)
		{
			MemTrace &rTrace = MemTrace::privGetRefInstance();

			// This is included so you can have one universal include
			std::call_once(rTrace.ProcessBeginFlag, [&rTrace]() noexcept
						   {
							   rTrace.privOut("\n");
							   rTrace.privOut("****************************************\n");
							   rTrace.privOut("**      Framework: %s               **\n", FRAMEWORK_VER);
							   rTrace.privOut("**   C++ Compiler: %d          **\n", _MSC_FULL_VER);
							   rTrace.privOut("**  Tools Version: %s        **\n", TOOLS_VERSION);
							   rTrace.privOut("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM);
							   rTrace.privOut("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING);
							   rTrace.privOut("**           Mode: %s        **\n", BUILD_CONFIG_MODE);
							   rTrace.privOut("****************************************\n");
#ifdef TEACHING_PC
							   rTrace.privOut("**    TEACHING PC: --> ENABLED <--    **\n");
							   rTrace.privOut("****************************************\n");

#endif
#ifdef USE_THREAD_FRAMEWORK
							   rTrace.privOut("**         Thread: %s               **\n", THREAD_FRAMEWORK_VER);
							   rTrace.privOut("****************************************\n");
#endif
						
							   printf("\n");
							   printf("****************************************\n");
							   printf("**      Framework: %s               **\n", FRAMEWORK_VER);
							   printf("**   C++ Compiler: %d          **\n", _MSC_FULL_VER);
							   printf("**  Tools Version: %s        **\n", TOOLS_VERSION);
							   printf("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM);
							   printf("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING);
							   printf("**           Mode: %s        **\n", BUILD_CONFIG_MODE);
							   printf("****************************************\n");
#ifdef TEACHING_PC
							   printf("**    TEACHING PC: --> ENABLED <--    **\n");
							   printf("****************************************\n");
#endif
#ifdef USE_THREAD_FRAMEWORK
							   printf("**         Thread: %s               **\n", THREAD_FRAMEWORK_VER);
							   printf("****************************************\n");
#endif
							   printf("\n");
							   rTrace.privOut("\n");
						   }
			);
		}
	}


	static int GetPlacementNewCount() noexcept
	{
		const MemTrace &rTrace = MemTrace::privGetRefInstance();
		return rTrace.PlacementNew_Count;
	}

	static void IncrementPlacementNewCount()
	{
		MemTrace &rTrace = MemTrace::privGetRefInstance();
		std::lock_guard<std::mutex> lock(rTrace.PlacementNew_mtx);
		rTrace.PlacementNew_Count++;
	}

	static void UnitTest_MemLeakCheck_Disable_Proxy()
	{
		MemTrace &rTrace = MemTrace::privGetRefInstance();
		std::lock_guard<std::mutex> loc(rTrace.mtxMemLeakCount);

		rTrace.MemLeakCount_Start = MemoryCheck_Disable_Magic_num;
	}

	static void UnitTest_MemLeakCheck_Enable_Proxy()
	{
		MemTrace &rTrace = MemTrace::privGetRefInstance();
		std::lock_guard<std::mutex> loc(rTrace.mtxMemLeakCount);

		rTrace.MemLeakCount_Start = rTrace.privLeakCount();
	}

	static bool UnitTest_MemLeakCheck_End_Proxy()
	{
		MemTrace &rTrace = MemTrace::privGetRefInstance();
		std::lock_guard<std::mutex> loc(rTrace.mtxMemLeakCount);

		const int End = rTrace.privLeakCount();
		const int Start = rTrace.MemLeakCount_Start;
		//rTrace.privOut("    start:%d  end:%d \n\n", Start,End);
		bool flag(false);
		if(rTrace.MemLeakCount_Start != MemoryCheck_Disable_Magic_num)
		{
			flag = (Start >= End);
		}
		else
		{
			// disabled - so the test shouldn't crash, return true
			flag = true;
		}

		return flag;
	}


	static void DecrementPlacementNewCount()
	{
		MemTrace &rTrace = MemTrace::privGetRefInstance();
		std::lock_guard<std::mutex> lock(rTrace.PlacementNew_mtx);
		rTrace.PlacementNew_Count--;
	}

	std::once_flag ProcessBeginFlag;
	std::once_flag ProcessEndFlag;

public:
	static MemTrace *psInstance;

	static MemTrace &privGetRefInstance() noexcept
	{
		static MemTrace helper;
		return helper;
	}

private:

	char privBuff[MemTraceBuffSize];
	std::mutex mtx;

public:
	std::mutex mtxMemLeakCount;
	int MemLeakCount_Start;

	int PlacementNew_Count;
	std::mutex PlacementNew_mtx;
};

//============================================
//			Fixed
//============================================
#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)

#define AZUL_PLACEMENT_NEW_BEGIN	assert(MemTrace::GetPlacementNewCount() == 0); \
												__pragma(push_macro("new")) \
												MemTrace::IncrementPlacementNewCount();

#define AZUL_PLACEMENT_NEW_END      __pragma(pop_macro("new")) \
												MemTrace::DecrementPlacementNewCount(); \
												assert(MemTrace::GetPlacementNewCount() == 0);
#else
#define AZUL_PLACEMENT_NEW_BEGIN	__pragma(push_macro("new"))

#define AZUL_PLACEMENT_NEW_END __pragma(pop_macro("new"))
#endif


//============================================
//			Original
//============================================

#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
#define _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define malloc(s)          _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define free(p)            _free_dbg(p, _NORMAL_BLOCK)
#define calloc(c, s)       _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s)      _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)


#else
#pragma warning( push )
#pragma warning( disable : 4820 )
#pragma warning( disable : 4577 )
#pragma warning( disable : 4987 )
#include <new>
#pragma warning( pop )
#endif

#ifdef MEM_TRACKER_ENABLED
#ifdef _DEBUG
#define MEM_TRACKER_PROCESS_BEGIN	\
						MemTrace::ProcessBegin();

#define MEM_TRACKER_PROCESS_END \
						MemTrace::ProcessEnd();
#else
#define MEM_TRACKER_PROCESS_BEGIN		\
						MemTrace::ProcessBegin_Release();

#define MEM_TRACKER_PROCESS_END \
						MemTrace::ProcessEnd_Release();
#endif
#else
#define MEM_TRACKER_PROCESS_BEGIN		\
						MemTrace::ProcessBegin_Release();

#define MEM_TRACKER_PROCESS_END \
						MemTrace::ProcessEnd_Release();

#endif

// MEM_TRACKER_BEGIN:
static struct _StaticMem
{
	_StaticMem() noexcept
	{
		MEM_TRACKER_PROCESS_BEGIN
	}
	~_StaticMem()
	{
		MEM_TRACKER_PROCESS_END
	}
	_StaticMem(const _StaticMem &) = delete;
	_StaticMem(_StaticMem &&) = delete;
	_StaticMem &operator = (const _StaticMem &) = delete;
	_StaticMem &operator = (_StaticMem &&) = delete;
} _StaticMemInst;


#endif  //MEM_TRACKER_H

// undo trick
#undef THREAD_FRAMEWORK_H

#endif //FRAMEWORK_H

// ------------------------------------
// Force include ThreadFramework.h
// ------------------------------------

#ifdef USE_THREAD_FRAMEWORK

#define THREAD_FRAMEWORK_START

//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
//
// Thread Framework - for Keenan's C++ classes
//
//    One header to rule them all, 
//        one header to find them, 
//        one header to bring them all 
//        and in the Linker bind them.   <thank you Tolkien>
//
//    Don't try to understand it... its very complicated
//
//    Systems:
//        Dictionary - hold info {name,threadID,tabs} per thread
//        Banner - thread banner (begin/end) of thread
//        ThreadHelper - Front door
//    
// ---------------------------------------------------------------------------
//    v.1.00  Merge files / start
//    v.1.01 - Start Banner
//    v.1.02 - Fix banner printing
//    v.1.03 - Version control
//    v.1.04 - Remove entry from map when thread ended
//    v.1.05 - Deep copy of name in Banner
//    v.1.06 - new name -async defer- label
//    v.1.07 - problem with names on uninitialized thread names
//    v.1.08 - Fixed async/setName bug
//    v.1.09 - Cleanup
//    v.1.10 - remove exception trick
//    v.1.11 - ThreadID, MainBanner(),Create(), Destroy()
//    v.1.12 - THREAD_NAME_SIZE, ChangeCurrentName(), detection of async vs deferred
//    v.1.13 - Unified START_BANNER macro for derived or non-derived, poor rtti
//    v.1.14 - ChangeName() by std::thread
//    v.1.15 - helper privGetID(), privPrintTabs()
//    v.1.16 - Support calling numTabs, #THREAD_FRAMEWORK_H disable prints
//    v.1.17 - watermark 2021
//    v.1.18 - bugs with new compiler
//    v.1.19 - namespace scoping on defines START_MAIN_BANNER, START_BANNER
//    v.1.20 - SimpleBanner
//    v.1.21 - const fixes
//    v.1.22 - stl Map switched to insert from emplace
//    v.1.23 - using namespace ThreadFramework, chrono_literals
//    v.1.24 - using namespace chronos
//    v.1.25 - baseline S2023
//    v.1.26 - Thread end() on simple banner not removing name (accidently gets recycled)
//    v.1.27 - Race condtion between Main() SetName and Thread's SimpleBanner
//    v.1.28 - double check needed for getName in SimpleBanner
//    v.1.29 - increase buffer size DebugBuffSize
//    v.1.30 - Dictionary get Size()
//    v.1.31 - chrono_literals, simple_banner = delete missing
//----------------------------------------------------------------------------- 

#ifndef THREAD_FRAMEWORK_H
#define THREAD_FRAMEWORK_H

// Comment this out to turn off all thread-debugging i.e. prints
#define THREAD_FRAMEWORK_ENABLED

// -----------------------------------------------
//
// To Enable:
//      define THREAD_FRAMEWORK_ENABLED 
//
// In Compiler:
//      preprocessor define:
//           USE_THREAD_FRAMEWORK
//      forced include:
//           ThreadFramework.h
// 
// -----------------------------------------------
//  SUMMARY
// -----------------------------------------------
//
//   class Debug
//   {
//   public:
//		given threadName
//			static bool GetID(ThreadID &outID, const char *const threadName)
//		given std::Thread
//			static const char *const GetName(const std::thread &t0)
//			static ThreadID GetID(const std::thread &t0)
//			static bool SetName(const std::thread &t0, const char *threadName, int numTabs = 1)
//			static bool ChangeName(const std::thread &t0, const char *threadName)
//		given ThreadID
//			static const char *const GetName(const DWORD threadId)
//		inside Current thread
//			static const char *const GetCurrentName()
//			static ThreadID GetCurrentID()
//			static int GetCurrentNumTabs()
//			static bool SetCurrentName(const char *const threadName, int numTabs = 1)
//			static bool ChangeCurrentName(const char *const threadName)
//		same as Trace::out() but honor's tabs for specific threads
//			static void out(const char *const fmt, ...)
//   };
//
//   class BannerBase  <--- Functor Base for Thread Names and calling threads num tabs
//   {
//        BannerBase(const char *const _pName)
//   };
//
//   class Banner  <--- RAII for ThreadBanner
//   {
//   public:
//       Banner(void *_pBannerBase)  <-- used for all non-main banner
//       Banner(class BannerName *pBannerName)  <-- used for main banner
//       ~Banner();
//   }
//
//   class MainBanner : public Banner
//   {
//	 public:
//       MainBanner(const char *const pName)  <-- used for main banner
//   }
//
// Useful Macros
//
//   #define START_BANNER_MAIN(name) 
//   #define START_BANNER		
//


namespace ThreadFramework
{
#define THREAD_UNUSED_VAR(x) (void(x))

#ifdef THREAD_FRAMEWORK_ENABLED
#define START_BANNER_MAIN(x)    ThreadFramework::MainBanner mainbanner(x);  
#define START_BANNER			ThreadFramework::Banner banner(this);
#else
#define START_BANNER_MAIN(x)    ThreadFramework::MainBanner mainbanner(x);
#define START_BANNER			ThreadFramework::Banner banner(this);
#endif
}


#pragma warning( push )
#pragma warning( disable : 4625 )
#pragma warning( disable : 4626 )
#pragma warning( disable : 4774 )
#include <iostream>
#include <thread>
#include <sstream> 
#include <map>
#include <mutex>
#include <future>
#include <assert.h>
#pragma warning( pop ) 

#include <codecvt>
#include <string>

// --------------------------------------------------------------
// DICTIONARY
// --------------------------------------------------------------

namespace ThreadFramework
{
	typedef unsigned int ThreadID;

	class Dictionary
	{
	public:
		static const unsigned int THREAD_NAME_SIZE = 64;

		struct Data
		{
			char name[THREAD_NAME_SIZE];  // maximum name length 63+null
			int  numTabs;         // number of tabs for printing
			bool async;           // does thread have an active active deferred
			bool finalized;       // thread started with name
			char pad[2];          // alignment
		};

	public:
		Dictionary()
			: poThreadNames(new std::map<ThreadID, Data>()),
			mtx()
		{
		}
		Dictionary(const Dictionary &) = delete;
		Dictionary &operator = (const Dictionary &) = delete;
		~Dictionary()
		{
			const std::lock_guard<std::mutex> lock(this->mtx);

			delete this->poThreadNames;
		}

		size_t GetMapSize()
		{
			return this->poThreadNames->size();
		}

		void Clear()
		{
			std::lock_guard<std::mutex> lock(this->mtx);
			assert(this->poThreadNames);
			this->poThreadNames->clear();
		}

		bool SetFinalized(ThreadID threadId)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			auto it = poThreadNames->find(threadId);
			bool s = false;

			if(it == poThreadNames->end())
			{
				s = false;
			}
			else
			{
				it->second.finalized = true;
				s = true;
			}

			return s;
		}
		const Data *GetData(ThreadID threadId)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			auto it = poThreadNames->find(threadId);
			Data *s = nullptr;

			if(it == poThreadNames->end())
			{
				s = nullptr;
			}
			else
			{
				s = &it->second;
			}

			return s;
		}
		bool SetData(ThreadID threadId, const Data *data)
		{
			std::lock_guard<std::mutex> lock(this->mtx);
			bool status = false;

			auto it = poThreadNames->find(threadId);

			if(it == poThreadNames->end())
			{
				status = false;
			}
			else
			{
				it->second = *data;
				status = true;
			}

			return status;
		}

		bool GetID(ThreadID &threadId, const char *const threadName)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			bool status = false;
			threadId = 0;
			for(auto it = poThreadNames->begin(); it != poThreadNames->end(); ++it)
			{
				if(strcmp(it->second.name, threadName) == 0)
				{
					// found it.
					status = true;
					threadId = it->first;
					break;
				}
			}

			return status;
		}

		char *GetName(ThreadID threadId)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			auto it = poThreadNames->find(threadId);
			char *s = nullptr;

			if(it == poThreadNames->end())
			{
				s = nullptr;
			}
			else
			{
				s = it->second.name;
			}

			return s;
		}
		int GetNumTabs(ThreadID threadId)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			auto it = poThreadNames->find(threadId);
			int numTabs = 0;

			if(it == poThreadNames->end())
			{
				numTabs = 0;
			}
			else
			{
				numTabs = it->second.numTabs;
			}

			return numTabs;
		}
		void SetNumTabs(ThreadID threadId, int count)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			auto it = poThreadNames->find(threadId);

			if(it == poThreadNames->end())
			{
				assert(0);
			}
			else
			{
				it->second.numTabs = count;
			}
		}
		void ClearAsync(ThreadID threadId)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			auto it = poThreadNames->find(threadId);

			if(it == poThreadNames->end())
			{
				assert(0);
			}
			else
			{
				it->second.async = false;
			}
		}
		void SetAsync(ThreadID threadId)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			auto it = poThreadNames->find(threadId);

			if(it == poThreadNames->end())
			{
				assert(0);
			}
			else
			{
				it->second.async = true;
			}
		}
		bool GetAsync(ThreadID threadId)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			auto it = poThreadNames->find(threadId);
			bool flag = false;

			if(it == poThreadNames->end())
			{
				flag = false;
				//assert(0);
			}
			else
			{
				flag = it->second.async;
			}

			return flag;
		}

		bool AddToMap(const char *const threadName, ThreadID threadId, int _numTabs)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			bool status(true);

			assert(threadName);

			// is the string name too big?
			const size_t N = strlen(threadName) + 1;
			assert(N <= THREAD_NAME_SIZE);

			// Fill the data struct
			Dictionary::Data data;

			memset(data.name, 0x0, THREAD_NAME_SIZE);
			strcpy_s(data.name, N, threadName);
			data.numTabs = _numTabs;
			data.async = false;
			data.finalized = false;

			// The name is now OWNED by the Map
			//auto it = poThreadNames->emplace(threadId, data);
			auto it = poThreadNames->insert(std::pair<ThreadID, Data>(threadId, data));

			// is that threadId already in the map?
			if(it.second == false)
			{
				// duplicate entry... no transfer

				// hits here for deferred async
				//auto it2 = poThreadNames->find(threadId);
				//Trace::out("--> warning AddToMap(%d) repeated threadID using %s  \n", threadId, it2->second.name);

				status = false;
			}
			return status;
		}
		bool RemoveFromMap(ThreadID threadId)
		{
			std::lock_guard<std::mutex> lock(this->mtx);

			bool status = false;
			auto it = poThreadNames->find(threadId);

			if(it == poThreadNames->end())
			{
				Trace::out("ERROR:(%5d) <---- RemoveFromMap()\n", threadId);
				assert(false);
				status = false;
			}
			else
			{
				// it->second.name;
				// do something
				status = true;
				poThreadNames->erase(it);
			}

			return status;
		}

	private:
		std::map<ThreadID, Data> *poThreadNames;
		std::mutex mtx;
	};
}

// ---------------------------------------------------------------
// Locally static section
//----------------------------------------------------------------

namespace ThreadFramework
{

#ifdef THREAD_FRAMEWORK_ENABLED
	class Debug
	{
	public:
		static const unsigned int DebugBuffSize = 1024;

	public:
		Debug(const Debug &) = delete;
		Debug &operator = (const Debug &) = delete;
		~Debug()
		{
			delete this->poDictionary;
		}

		static void Create()
		{
			Dictionary *pDict = new Dictionary();
			assert(pDict);

			privInstance(pDict, true);
		}
		static void Destroy()
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			delete pDebug->poDictionary;
			pDebug->poDictionary = nullptr;

		}

		// given threadName
		static bool GetID(ThreadID &outID, const char *const threadName)
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			// Find entry in dictionary
			return pDebug->poDictionary->GetID(outID, threadName);
		}

		// given std::Thread
		static const char *GetName(const std::thread &t0)
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			const ThreadID threadId = pDebug->privGetID(t0);

			return pDebug->poDictionary->GetName(threadId);
		}
		static ThreadID GetID(const std::thread &t0)
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			const ThreadID threadId = pDebug->privGetID(t0);

			return threadId;
		}
		static bool SetName(const std::thread &t0, const char *threadName, int numTabs = 1)
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			const ThreadID threadId = pDebug->privGetID(t0);

			// register the name
			const bool status = pDebug->poDictionary->AddToMap(threadName, threadId, numTabs);
			if(status)
			{
				std::wstring wsTmp(&threadName[0], &threadName[strlen(threadName)]);

				HANDLE tHandle = static_cast<HANDLE>((const_cast<std::thread &>(t0)).native_handle());
				const HRESULT hr = SetThreadDescription(tHandle, wsTmp.c_str());
				AZUL_UNUSED_VAR(hr);
				assert(FAILED(hr) == S_OK);
			}

			return status;
		}

		static bool ChangeName(const std::thread &t0, const char *threadName)
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			const ThreadID threadId = pDebug->privGetID(t0);
			bool status = false;

			const Dictionary::Data *pData = pDebug->poDictionary->GetData(threadId);
			if(pData != nullptr)
			{
				Dictionary::Data data = *pData;
				strcpy_s(&data.name[0], Dictionary::THREAD_NAME_SIZE, threadName);

				status = pDebug->poDictionary->SetData(threadId, &data);

				std::wstring wsTmp(&threadName[0], &threadName[strlen(threadName)]);

				HANDLE tHandle = static_cast<HANDLE>((const_cast<std::thread &>(t0)).native_handle());
				const HRESULT hr = SetThreadDescription(tHandle, wsTmp.c_str());
				AZUL_UNUSED_VAR(hr);
				assert(FAILED(hr) == S_OK);
			}
			else
			{
				status = false;
			}

			return status;

		}

		// given ThreadID
		static const char *GetName(const DWORD threadId)
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			return pDebug->poDictionary->GetName(threadId);
		}

		// inside Current thread...
		static const char *GetCurrentName()
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			// Get id of the current thread
			const ThreadID threadId = ::GetThreadId(GetCurrentThread());

			return pDebug->poDictionary->GetName(threadId);
		}
		static ThreadID GetCurrentID()
		{
			// Get the string ID in an Integer form
			std::stringstream ss;

			// extracts the id
			ss << std::this_thread::get_id();

			// convert from string to number
			ThreadID id = std::stoul(ss.str());

			return id;
		}
		static int GetCurrentNumTabs()
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			// Get id of the current thread
			const ThreadID threadId = ::GetThreadId(GetCurrentThread());

			return pDebug->poDictionary->GetNumTabs(threadId);
		}
		static bool SetCurrentName(const char *const threadName, int numTabs = 1)
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			// Get id of the current thread

			const ThreadID threadId = ::GetThreadId(GetCurrentThread());

			// register the name
			const bool status = pDebug->poDictionary->AddToMap(threadName, threadId, numTabs);

			if(status)
			{
				std::wstring wsTmp(&threadName[0], &threadName[strlen(threadName)]);

				const HRESULT hr = SetThreadDescription(GetCurrentThread(), wsTmp.c_str());
				AZUL_UNUSED_VAR(hr);
				assert(FAILED(hr) == S_OK);
			}

			return status;
		}
		static bool ChangeCurrentName(const char *const threadName)
		{
			bool status = false;

			Debug *pDebug = privInstance();
			assert(pDebug);

			// Get id of the current thread
			const ThreadID threadId = ::GetThreadId(GetCurrentThread());

			// Get the Name... change it.
			char *pOldName = pDebug->poDictionary->GetName(threadId);
			if(pOldName != nullptr)
			{
				strcpy_s(pOldName, Dictionary::THREAD_NAME_SIZE, threadName);

				std::wstring wsTmp(&threadName[0], &threadName[strlen(threadName)]);

				const HRESULT hr = SetThreadDescription(GetCurrentThread(), wsTmp.c_str());
				AZUL_UNUSED_VAR(hr);
				assert(FAILED(hr) == S_OK);

				status = true;
			}
			else
			{
				assert(pOldName);
				status = false;
			}
			return status;
		}


		// Same as Debug::privOut() but honor's tabs for specific threads
		static void out(const char *const fmt, ...)
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			std::lock_guard<std::mutex> lock(pDebug->debugMtx);

			// indent + 1 more 
			const size_t numTabs = (size_t)Debug::GetCurrentNumTabs() + 1;

			va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
			va_start(args, fmt);
#pragma warning( pop )

			char s[Debug::DebugBuffSize];
			char *p = Debug::privAddTabs(s, numTabs);

			if(pDebug->poDictionary->GetAsync(Debug::GetCurrentID()))
			{
				const char *pThreadName = Debug::GetCurrentName();
				if(pThreadName)
				{
					// async deferred with name
					sprintf_s(p, DebugBuffSize - numTabs, "(%s): -async defer-: ", Debug::GetCurrentName());
				}
				else
				{
					// async deferred with no name... print threadID
					sprintf_s(p, DebugBuffSize - numTabs, "(%5d): -async defer-: ", Debug::GetCurrentID());
				}
			}
			else
			{
				const char *pThreadName = Debug::GetCurrentName();
				if(pThreadName)
				{
					// thread or async(launch async) with name
					sprintf_s(p, DebugBuffSize - numTabs, "(%s): ", Debug::GetCurrentName());
				}
				else
				{
					// if no name... print threadID
					sprintf_s(p, DebugBuffSize - numTabs, "(%5d): ", Debug::GetCurrentID());
				}
			}
			strcat_s(s, 256, fmt);

			char sB[Debug::DebugBuffSize] = {0};

			vsprintf_s(sB, 256, s, args);

			pDebug->privOut(sB);

			//va_end(args);
			args = static_cast<va_list> (nullptr);

		}
	private:
		friend class Banner;

	public:  // HACK
		// displays a printf to the output window
		static void privOut(const char *const fmt, ...)
		{
			// Assumes its locked before this is called.
			Debug *pDebug = Debug::privInstance();
			assert(pDebug);

			std::lock_guard<std::mutex> lock(pDebug->printMtx);

			va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
			va_start(args, fmt);
#pragma warning( pop )

			vsprintf_s(&pDebug->privBuff[0], DebugBuffSize, fmt, args);
			OutputDebugString(&pDebug->privBuff[0]);

			//va_end(args);
			args = static_cast<va_list> (nullptr);
		}

		static char *privAddTabs(char *s, size_t numTabs)
		{
			// Add the number of tabs into the banner
			//    A real PAIN we cannot tear Debug::privOut()
			//    Create a buffer... print into that, submit buff to Debug::privOut()

			char *p;

			// clear buffer;
			memset(s, 0x0, Debug::DebugBuffSize);
			p = s;

			// add number of tabs and move pointer
			size_t count = numTabs;
			while(count--)
			{
				*p++ = 0x9; // "\t"
			}

			return p;

		}
		ThreadID privGetID(const std::thread &t0)
		{
			// OMG - a const cast?
			// reason... if you have a thread reference... I want no way of modifying the data
			// but to get the name for the debugger, I need to call native_handle which is mutable

			const ThreadID threadId = ::GetThreadId(
				static_cast<HANDLE>(
					(const_cast<std::thread &>(t0)).native_handle()
					)
			);

			return threadId;
		}

		static Dictionary *GetDictionary()
		{
			Debug *pDebug = privInstance();
			assert(pDebug);

			return pDebug->poDictionary;
		}

		Debug()
			: poDictionary(nullptr),
			debugMtx()
		{
		}
		static Debug *privInstance(Dictionary *pDict = nullptr, bool flag = false)
		{
			static Debug debug;

			if(flag)
			{
				debug.poDictionary = pDict;
			}

			return &debug;
		}

		Dictionary *poDictionary;
		std::mutex   debugMtx;
		std::mutex   printMtx;

		char privBuff[DebugBuffSize];
	};

#else
	class Debug
	{
	public:
		static const unsigned int DebugBuffSize = 256;

	public:
		Debug() = default;
		Debug(const Debug &) = delete;
		Debug &operator = (const Debug &) = delete;
		~Debug() = default;

		// given threadName
		static bool GetID(ThreadID &, const char *const)
		{
			return false;
		}
		// given std::Thread
		static const char *GetName(const std::thread &)
		{
			return nullptr;
		}
		static Dictionary *GetDictionary()
		{
			return nullptr;
		}

		static void privOut(const char *const, ...)
		{

		}
		static ThreadID GetID(const std::thread &)
		{
			return (ThreadID)-1;
		}
		static bool SetName(const std::thread &, const char *, int numTabs = 1)
		{
			THREAD_UNUSED_VAR(numTabs);
			return false;
		}
		static bool ChangeName(const std::thread &, const char *)
		{
			return false;
		}
		// given ThreadID
		static const char *GetName(const DWORD)
		{
			return nullptr;
		}
		// inside Current thread
		static const char *GetCurrentName()
		{
			return nullptr;
		}
		static ThreadID GetCurrentID()
		{
			return (ThreadID)-1;
		}
		static int GetCurrentNumTabs()
		{
			return 33333;
		}
		static bool SetCurrentName(const char *const, int numTabs = 1)
		{
			THREAD_UNUSED_VAR(numTabs);
			return false;
		}
		static bool ChangeCurrentName(const char *const)
		{
			return false;
		}
		// same as Trace::out() but honor's tabs for specific threads
		static void out(const char *const, ...)
		{

		}
	};

#endif

}

// --------------------------------------------------------------
// Banner
// --------------------------------------------------------------

namespace ThreadFramework
{
#ifdef THREAD_FRAMEWORK_ENABLED
	class BannerBase
	{
	public:
	public:
		BannerBase(const char *const _pName)
			: IsBannerBaseID{0x7890abcd},
			BannerName{0},
			CallingThreadNumTabs{Debug::GetCurrentNumTabs()}
		{
			strncpy_s(this->BannerName, Dictionary::THREAD_NAME_SIZE, _pName, strlen(_pName) + 1);
		}
		BannerBase() = delete;
		BannerBase(const BannerBase &) = default;
		BannerBase &operator=(const BannerBase &) = default;
		virtual ~BannerBase() = default;

		const char *GetBannerName()
		{
			return this->BannerName;
		}
		uint32_t GetBannerBaseID() const
		{
			return this->IsBannerBaseID;
		}
		int GetCallingThreadNumTabs()
		{
			return this->CallingThreadNumTabs;
		}
	private:
		uint32_t IsBannerBaseID;  // poor man's rtti
		char BannerName[Dictionary::THREAD_NAME_SIZE];
		int CallingThreadNumTabs;
	};
#else
	class BannerBase
	{
	public:
		BannerBase(const char *const) {}

		BannerBase() = delete;
		BannerBase(const BannerBase &) = default;
		BannerBase &operator=(const BannerBase &) = default;
		virtual ~BannerBase() = default;
	};
#endif
}

namespace ThreadFramework
{
	class SimpleBanner
	{
	public:
		SimpleBanner()
		{
			this->pName = Debug::GetCurrentName();
			this->tabs = Debug::GetCurrentNumTabs();
			this->tID = Debug::GetCurrentID();

#ifdef THREAD_FRAMEWORK_ENABLED
			privPrint();
#endif
		}

		SimpleBanner(const SimpleBanner &) = delete;
		SimpleBanner &operator = (const SimpleBanner &) = delete;

		~SimpleBanner()
		{
#ifdef THREAD_FRAMEWORK_ENABLED
			privPrintEnd();
#endif
		}

	private:
		void privPrint()
		{
			const ThreadID _ThreadID = this->tID;
			char s[Debug::DebugBuffSize];

			// RACE condition HACK...
			// Main threads set's name... but thread gets launched faster
			// than Main can finish... spin unit its finish
			// count is always one... no need for while loop

			int YieldMax = 5000;
			int count = 0;
			while((Debug::GetDictionary()->GetName(_ThreadID)) == nullptr)
			{
				std::this_thread::yield();
				count++;

				// infinite loop safety exit
				if(count >= YieldMax)
				{
					break;
				}
			}

			if(pName == nullptr)
			{
				this->pName = Debug::GetDictionary()->GetName(_ThreadID);
				this->tabs = Debug::GetDictionary()->GetNumTabs(_ThreadID);

				//Trace::out("Cx:%d shit %s\n", count, Debug::GetDictionary()->GetName(_ThreadID));
			}

			char *p = this->privAddTabs(s, (size_t)this->tabs);

			sprintf_s(p, Debug::DebugBuffSize - this->tabs, "thread(%5d) %s: begin()\n",
					  _ThreadID,
					  this->pName);

			Debug::privOut(s);
		}

		void privPrintEnd()
		{
			const ThreadID _ThreadID = this->tID;
			char s[Debug::DebugBuffSize];

			char *p = this->privAddTabs(s, (size_t)this->tabs);

			sprintf_s(p, Debug::DebugBuffSize - this->tabs,
					  "thread(%5d) %s: end()\n",
					  _ThreadID,
					  this->pName);

			Dictionary *pDict = Debug::GetDictionary();
			assert(pDict);
			AZUL_UNUSED_VAR(pDict);

			// for non-named async threads
			if(this->pName != nullptr)
			{
				bool status = pDict->RemoveFromMap(_ThreadID);
				assert(status);
				AZUL_UNUSED_VAR(status);
			}

			Debug::privOut(s);
		}

		char *privAddTabs(char *s, size_t numTabs)
		{
			// Add the number of tabs into the banner
			//    A real PAIN we cannot tear Debug::privOut()
			//    Create a buffer... print into that, submit buff to Debug::privOut()

			char *p;

			// clear buffer;
			memset(s, 0x0, Debug::DebugBuffSize);
			p = s;

			// add number of tabs and move pointer
			size_t count = numTabs;
			while(count--)
			{
				*p++ = 0x9; // "\t"
			}

			return p;

		}

		ThreadID tID;
		const char *pName;
		int tabs;
	};
}

namespace ThreadFramework
{
#ifdef THREAD_FRAMEWORK_ENABLED
	class Banner
	{
	public:
		Banner(void *_pBannerBase)
			: bannerMtx(),
			isThread(true),
			isMainBanner(false),
			pDict(Debug::GetDictionary())
		{
			std::lock_guard<std::mutex> lock(this->bannerMtx);

			// poor man's rtti
			// I buried a magic number after the vptr
			// check the magic number to indicate if derived from base or not
			BannerBase *pBannerBase = (BannerBase *)_pBannerBase;
			if(0x7890abcd == pBannerBase->GetBannerBaseID())
			{
				// Derived from BannerBase
				const ThreadID _ThreadID = Debug::GetCurrentID();

				const Dictionary::Data *pData = pDict->GetData(_ThreadID);

				if(pData)
				{
					if(pData->finalized == false)
					{
						// previously defined
						char name[Dictionary::THREAD_NAME_SIZE];
						strncpy_s(name, Dictionary::THREAD_NAME_SIZE, pData->name, strlen(pData->name));

						const bool RemoveStatus = pDict->RemoveFromMap(_ThreadID);
						AZUL_UNUSED_VAR(RemoveStatus);
						assert(RemoveStatus);

						const bool status = Debug::SetCurrentName(name, pBannerBase->GetCallingThreadNumTabs() + 1);
						this->isThread = status;
						this->isThread = false;

						const bool FinalizedStatus = pDict->SetFinalized(_ThreadID);
						AZUL_UNUSED_VAR(FinalizedStatus);
						assert(FinalizedStatus);

					}
					else
					{
						// Previous defined and finalized --> ASYNC
						const bool status = Debug::SetCurrentName(pBannerBase->GetBannerName(), pBannerBase->GetCallingThreadNumTabs() + 1);
						this->isThread = status;
					}
				}
				else
				{
					// First time	
					const bool status = Debug::SetCurrentName(pBannerBase->GetBannerName(), pBannerBase->GetCallingThreadNumTabs() + 1);
					this->isThread = status;

					const bool FinalizedStatus = pDict->SetFinalized(_ThreadID);
					AZUL_UNUSED_VAR(FinalizedStatus);
					assert(FinalizedStatus);
				}

				const size_t numTabs = (size_t)this->pDict->GetNumTabs(_ThreadID);

				char s[Debug::DebugBuffSize];
				char *p = Debug::privAddTabs(s, numTabs);

				if(this->isThread)
				{
					sprintf_s(p, Debug::DebugBuffSize - numTabs, "thread(%5d) %s: begin()\n",
							  _ThreadID,
							  this->pDict->GetName(_ThreadID));

					Debug::privOut(s);
				}
				else
				{
					this->pDict->SetAsync(_ThreadID);

					const int _tabCount = this->pDict->GetNumTabs(_ThreadID) + 1;
					this->pDict->SetNumTabs(_ThreadID, _tabCount);

					Debug::out("start()\n");

				}
			}
			else
			{
				// _NOT_ Derived from BannerBase
				const ThreadID _ThreadID = Debug::GetCurrentID();

				const size_t numTabs = (size_t)this->pDict->GetNumTabs(_ThreadID);

				char s[Debug::DebugBuffSize];
				char *p = Debug::privAddTabs(s, numTabs);

				sprintf_s(p, Debug::DebugBuffSize - numTabs, "thread(%5d) %s: begin()\n",
						  _ThreadID,
						  this->pDict->GetName(_ThreadID));

				Debug::privOut(s);
			}
		}

		Banner(const char *const pName)
			: bannerMtx(),
			isThread(true),
			isMainBanner(true),
			pDict(nullptr)
		{
			Debug::Create();
			Debug::SetCurrentName(pName, 0);

			pDict = Debug::GetDictionary();
			std::lock_guard<std::mutex> lock(this->bannerMtx);

			const ThreadID _ThreadID = Debug::GetCurrentID();

			const size_t numTabs = (size_t)this->pDict->GetNumTabs(_ThreadID);

			char s[Debug::DebugBuffSize];
			char *p = Debug::privAddTabs(s, numTabs);

			sprintf_s(p, Debug::DebugBuffSize - numTabs, "thread(%5d) %s: begin()\n",
					  _ThreadID,
					  this->pDict->GetName(_ThreadID));

			Debug::privOut(s);
		}

		Banner() = delete;
		Banner(const Banner &) = default;
		Banner &operator = (const Banner &) = default;
		virtual ~Banner()
		{
			const std::lock_guard<std::mutex> lock(this->bannerMtx);

			const ThreadID _ThreadID = Debug::GetCurrentID();

			const size_t numTabs = (size_t)this->pDict->GetNumTabs(_ThreadID);

			char s[Debug::DebugBuffSize];
			char *p = Debug::privAddTabs(s, numTabs);

			if(this->isThread)
			{
				sprintf_s(p, Debug::DebugBuffSize - numTabs, "thread(%5d) %s: end()\n",
						  _ThreadID,
						  this->pDict->GetName(_ThreadID));
				Debug::privOut(s);

				const bool status = this->pDict->RemoveFromMap(_ThreadID);
				assert(status);
				AZUL_UNUSED_VAR(status);
			}
			else
			{
				Debug::out("end()\n");

				this->pDict->ClearAsync(_ThreadID);
				const int _tabCount = this->pDict->GetNumTabs(_ThreadID) - 1;
				this->pDict->SetNumTabs(_ThreadID, _tabCount);
			}

			if(this->isMainBanner)
			{
				Debug::Destroy();
			}
		};

		// Data ------------------
		std::mutex		 bannerMtx;
		bool             isThread;
		bool		     isMainBanner;
		Dictionary *pDict;

	};

	class MainBanner : public Banner
	{
	public:
		MainBanner(const char *const pName)
			: Banner(pName)
		{
			std::lock_guard<std::mutex> lock(this->bannerMtx);

			const ThreadID _ThreadID = Debug::GetCurrentID();

			const bool FinalizedStatus = pDict->SetFinalized(_ThreadID);
			AZUL_UNUSED_VAR(FinalizedStatus);
			assert(FinalizedStatus);
		}

		MainBanner(const MainBanner &) = delete;
		MainBanner &operator = (const MainBanner &) = delete;
		~MainBanner()
		{
			//Debug::Destroy();
		}
	};
#else
	class Banner
	{
	public:
		Banner(void *) {}
		Banner(const char *const) {}

		Banner() = default;
		Banner(const Banner &) = default;
		Banner &operator = (const Banner &) = default;
		virtual ~Banner() {}
	};

	class MainBanner : public Banner
	{
	public:
		MainBanner(const char *const) {}

		MainBanner(const MainBanner &) = delete;
		MainBanner &operator = (const MainBanner &) = delete;
		~MainBanner() = default;

	};
#endif
}

using namespace ThreadFramework;
#include <chrono>
using namespace std::chrono_literals;
using namespace std::chrono;

#endif

//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
//
// Framework - Release Notes
//
// ---------------------------------------------------------------------------
//    v.2.5.0 
//    v.2.6   - Baseline
//    v.2.6.1 - fixed warning TestRegistry
//    v.2.7   - Baseline
//    v.2.8   - warning 5039 windows.h
//    v.2.9   - fence
//    v.2.10  - msc_ver 1914, win 10.0.17
//    v.2.11  - warning fixes 
//    v.2.12  - mutex on all trace::out
//    v.2.13  - fixed warnings on mutex and default copy const / assignment
//    v.2.14  - FileIO remove new... so you can overload new/delete
//    v.2.15  - number tests pass reformat
//    v.2.16  - fixing static analysis issues
//    v.2.18  - teardown issue
//    v.2.19  - WinSDK, Compiler, Framework version reporting
//    v.2.20  - WinSDK, Compiler, Framework version reporting in Release mode
//    v.2.21  - x64 info header
//    v.2.22  - Timer renamed to PerformanceTimer 
//    v.2.23  - Mem Tracking now works with placement and alignment new
//    v.2.24  - new tracking with file names and line numbers
//    v.2.25  - fixing collision with unused_var() macros in align16
//    v.2.26  - any order for the link, removed FileIO.cpp and PerformanceTimer.cpp
//    v.2.27  - atomics in Timer 
//    v.2.28  - remove dynamic cast, warnings on atomic
//    v.2.29  - placement new on align16
//    v.2.30  - HUGE rework of UnitTest.h removed UnitTest.cpp
//    v.3.00  - Flatten into 2 files
//    v.3.01  - reduce includes
//    v.3.02  - removed framework main... std::call_once() now only one include <period>
//    v.3.03  - MemoryTracking reporting improvements
//    v.3.04  - Clean up of framework
//    v.3.05  - consolidate warnings
//    v.3.06  - Tweaked several config 8 in total {dbg/rel,x86/64, mem_ena/dis}
//    v.3.07  - nothrow is a problem
//    v.3.08  - application vs external memory tracking
//    v.3.09  - added log file
//    v.3.10  - modes printing
//    v.3.11  - banner before leak print out
//    v.3.12  - project settings
//    v.3.13  - UnitStats, mkdir only it doesn't exist
//    v.3.14  - file open errors... need a cleaner way to deal with them
//    v.3.15  - log directory - fixed for Memory:  FileIO
//    v.3.16  - added default if MEMORY_LOGS_DIR is missing
//    v.3.17  - copyright
//    v.3.18  - warnings around string include
//    v.3.19  - removed string class include (it was system leaking)
//    v.3.20  - MR_FAST mode display string
//    v.3.21  - Trace::out2() Print to debug and stdio printf
//    v.3.22  - Added which test was runned
//    v.3.23  - MemTrace::LeakCount()
//    v.3.24  - placement new
//    v.3.25  - REPLACE_ME_STUB()
//    v.3.26  - bogus compiler warning in compiler C6011 
//    v.3.27  - INSERT_CODE_HERE
//    v.3.28  - code analysis warning
//    v.3.29  - warning for new 19.25 compiler that's not honored in earlier
//    v.3.30  - macro collison... prefix macros with AZUL
//    v.3.31  - support malloc(),calloc(),realloc(),free() and placement new macros
//    v.3.32  - fix warnings new compiler 10.0.19041.0
//    v.3.33  - UnitTestMemoryCheck macros
//    v.3.34  - added ApplicationMemLeakCount() and MemMailBox.bin
//    v.3.35  - warning fix on MemMailBox.bin
//    v.3.36  - lock_guard cleanup
//    v.3.37  - ThreadFramework - first pass
//    v.3.38  - issues with abs() function, state not initialized
//    v.3.39  - Thread version banner
//    v.3.40  - issues with abs() function, state not initialized
//    v.3.41  - xmemory issue with <string>
//    v.3.42  - global timer
//    v.3.43  - baseline merge from multithreaded
//    v.3.44  - watermark 2021
//    v.3.45  - placement new bug, fence to check
//    v.3.46  - floating point precision bug in release
//    v.3.47  - CHECK_EQUAL 
//    v.3.48  - Fix ThreadFramework.h in release new compiler
//    v.3.49  - improve printing on fail
//    v.3.50  - Magic
//    v.3.51  - SOLUTION_DIR replaces MEMORY_LOGS_DIR
//    v.3.52  - Rework shutdown to match new TEST format
//    v.3.53  - verified all configurations
//    v.3.54  - CRC32 added functionality
//    v.3.55  - FILE_MODIFIED_CHECK, FILE_EXIST, FILE_GET_CRC32
//    v.3.56  - cleanup warnings in release
//    v.3.60  - flag on CHECKSUM grader
//    v.3.60.1 - unit test mode
//    v.3.61  - error,leaks full pathname
//    v.3.62  - PASSED print out
//    v.3.63  - framework enum cleanup
//    v.3.64  - bitwise or for testing flags
//    v.3.65  - limit memory leaks to 100
//    v.3.66  - print hacks for 588
//    v.3.67  - ReduceTestPrints
//    v.3.671 - tweak on 588 prints
//    v.3.68  - Increase Trace::out buff size to 1K
//    v.3.69  - merger of different framework.h 
//    v.3.70  - SIMD support
//    v.3.71  - CRC on raw buffer
//    v.3.72  - Warning fix, teaching pc
//    v.3.73  - Thread Unit issue, TEST_WITH_SETUP_TEARDOWN
//    v.3.74  - Race condition UnitTest_SetName() added mtx refactor
//    v.3.75  - adjust UnitTest_SetName()
//    v.3.76  - MultiThreaded_Delay
//    v.3.77 - ThreadFramework: using namespace ThreadFramework, chronos
//    v.3.78 - unit test buffer increase
//    v.3.79 - 2022 vs rework
//    v.3.80 - TimeInSeconds() - double issue
//    v.3.81 - ThreadFramework: using namespace chronos
//    v.3.82 - Fall 2023
//    v.3.83 - ThreadFramework race condition SetName
//    v.3.84 - ThreadFramework 1.28 getName() issue
//    v.3.85 - ThreadFramework 1.29 buffer size
//    v.3.86 - warning suppression procol buf (dll) 
//    v.3.87 - LocalDebuggerWorkingDirectory
//    v.3.88 - Unit Test newline format changes
//    v.3.89 - crc reference wasn't initialize to 0
//    v.3.90 - AllocRefTracker()
//    v.3.91 - ThreadFramework: Dictionary size
//    v.3.92 - release mode AllocRefTracker()

#endif

// ---  End of File ---

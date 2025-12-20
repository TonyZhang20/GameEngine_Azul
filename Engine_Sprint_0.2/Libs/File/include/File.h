//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef FILE_EXPORTAPI_H
#define FILE_EXPORTAPI_H

#ifdef FILE_USE_DLL
#ifdef FILELIBRARY_EXPORTS
#define FILELIBRARY_API __declspec(dllexport)
#else
#define FILELIBRARY_API __declspec(dllimport)
#endif // MATHLIBRARY_EXPORTS
#else
#define FILE_LIBRARY_API
#endif // MATH_USE_DLL

#endif 

#ifndef FILE_H
#define FILE_H

// Make the assumption of c-char strings, not UNICODE
namespace Azul
{

	class File
	{
	public:

		// Constants for the library
		static const uint32_t MAJOR_VERSION = 4;
		static const uint32_t MINOR_VERSION = 0;
		// hide the base directory.. in File.cpp
		static const uint32_t BASE_DIR_SIZE = 128;

		typedef void* Handle;

		enum class Mode : uint32_t
		{
			READ,
			WRITE,
			READ_WRITE
		};

		enum class Position : uint32_t
		{
			BEGIN,
			CURRENT,
			END
		};

		enum class Error : uint32_t
		{
			SUCCESS,
			BASE_DIR_FAIL,
			BASE_DIR_NULLPTR_FAIL,
			BASE_DIR_INSUFFICIENT_SPACE_FAIL,
			OPEN_FAIL,
			OPEN_FILENAME_FAIL,
			OPEN_BASE_DIR_FAIL,
			CLOSE_FAIL,
			WRITE_FAIL,
			READ_FAIL,
			SEEK_FAIL,
			TELL_FAIL,
			FLUSH_FAIL,
			UNDEFINED
		};

	public:
		FILELIBRARY_API static File::Error GetBaseDir(char * const pDirOut, size_t DirOutSize) noexcept;
		FILELIBRARY_API static File::Error SetBaseDir(const char *const pDir) noexcept;
		FILELIBRARY_API static File::Error ClearBaseDir() noexcept;

		FILELIBRARY_API static File::Error Open(File::Handle &fh, const char* const fileName, File::Mode mode, bool UseBaseAddr = false) noexcept;
		FILELIBRARY_API static File::Error Close(File::Handle &fh) noexcept;
		FILELIBRARY_API static File::Error Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept;
		FILELIBRARY_API static File::Error Read(File::Handle fh, void* const _buffer, const DWORD _size) noexcept;
		FILELIBRARY_API static File::Error Seek(File::Handle fh, File::Position location, int offset) noexcept;
		FILELIBRARY_API static File::Error Tell(File::Handle fh, DWORD& offset) noexcept;
		FILELIBRARY_API static File::Error Flush(File::Handle fh) noexcept;
		FILELIBRARY_API static bool IsHandleValid(File::Handle fh) noexcept;

		FILELIBRARY_API static File::Error GetFileAsBuffer(const char* const pFileName, unsigned char*& pBuff, unsigned int& numBytes) noexcept;


		// add methods and data if you want
	};

}

#endif

// --- End of File ---

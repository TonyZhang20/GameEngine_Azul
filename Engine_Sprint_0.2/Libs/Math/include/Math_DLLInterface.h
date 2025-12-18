//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_DLL_INTERFACE_H
#define ENGINE_MATH_DLL_INTERFACE_H

#ifdef MATH_USE_DLL
#ifdef MATH_LIBRARY_EXPORTS
#define MATH_LIBRARY_API __declspec(dllexport)
#else
#define MATH_LIBRARY_API __declspec(dllimport)
#endif
#else
#define MATH_LIBRARY_API
#endif

#endif

//--- End of File ---

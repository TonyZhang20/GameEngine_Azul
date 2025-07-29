//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef LAYER_EXPORTAPI_H
#define LAYER_EXPORTAPI_H

#ifdef LAYER_USE_DLL
#ifdef LAYERLIBRARY_EXPORTS
#define LAYERLIBRARY_API __declspec(dllexport)
#else
#define LAYERLIBRARY_API __declspec(dllimport)
#endif // MATHLIBRARY_EXPORTS
#else
#define LAYER_LIBRARY_API
#endif // MATH_USE_DLL

#endif 

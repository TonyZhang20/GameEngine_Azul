//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef PROTOBUF_ENGINE_DLL_INTERFACE_H
#define PROTOBUF_ENGINE_DLL_INTERFACE_H

#ifdef PROTOBUF_ENGINE_USE_DLL
#ifdef PROTOBUF_ENGINE_LIBRARY_EXPORTS
#define PROTOBUF_ENGINE_LIBRARY_API __declspec(dllexport)
#else
#define PROTOBUF_ENGINE_LIBRARY_API __declspec(dllimport)
#endif
#else
#define PROTOBUF_ENGINE_LIBRARY_API
#endif

#endif

// --- End of File ---

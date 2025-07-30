#ifndef MYSTL_DLL_INTERFACE_H
#define MYSTL_DLL_INTERFACE_H

#ifdef MYSTL_USE_DLL
#ifdef MYSTL_LIBRARY_EXPORTS
#define MYSTL_LIBRARY_API __declspec(dllexport)
#else
#define MYSTL_LIBRARY_API __declspec(dllimport)
#endif
#else
#define MYSTL_LIBRARY_API
#endif

#endif
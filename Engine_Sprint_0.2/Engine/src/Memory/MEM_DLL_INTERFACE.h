#ifndef MEM_EXPORTAPI_H
#define MEM_EXPORTAPI_H

#ifdef MEMORY_USE_DLL
#ifdef MEMORY_LIBRARY_EXPORTS
#define MEM_LIBRARY_API __declspec(dllexport)
#else
#define MEM_LIBRARY_API __declspec(dllimport)
#endif // MATHLIBRARY_EXPORTS
#else
#define MEM_LIBRARY_API
#endif // MATH_USE_DLL

#endif 
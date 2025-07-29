#ifndef EVENTS_EXPORTAPI_H
#define EVENTS_EXPORTAPI_H

#ifdef EVENTS_USE_DLL
#ifdef EVENTSLIBRARY_EXPORTS
#define EVENTSLIBRARY_API __declspec(dllexport)
#else
#define EVENTSLIBRARY_API 
//__declspec(dllimport)
#endif
#else
#define EVENTS_LIBRARY_API

#endif

#endif

#define BIT(x) (1 << x)
//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <unordered_set>

namespace Azul
{
	class Registration final
	{
	public:
		static const unsigned int ReserveSize = 256;

	public:
		Registration();
		Registration(const Registration &) = delete;
		Registration &operator = (const Registration &) = delete;
		Registration(Registration &&) = delete;
		Registration &operator = (Registration &&) = delete;
		~Registration();

		inline void Clear() { set.clear(); };
		void RegisterPointer(void*);
		bool FindPointer(void*);
		void UnRegister(void*);

		// Data:
		std::unordered_set< void * > set;
	};
}

#endif

// --- End of File ---

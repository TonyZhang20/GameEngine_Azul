//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			Trace::out("DLL_PROCESS_ATTACHED --> ProtoBufEngineDLL\n");
			break;
			//case DLL_THREAD_ATTACH:
			//	Trace::out("DLL_THREAD_ATTACHED()\n");
			//	break;
			//case DLL_THREAD_DETACH:
			//	Trace::out("DLL_THREAD_DETACHED()\n");
			//	break;
		case DLL_PROCESS_DETACH:
			Trace::out("ProtoBufEngineDLL <-- DLL_PROCESS_DETACHED\n");
			break;
	}
	return TRUE;
}

// --- End of File ---

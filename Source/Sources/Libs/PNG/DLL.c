
//<<>-<>>---------------------------------------------------------------------()
/*
	DLL libpng
									      */
//()-------------------------------------------------------------------<<>-<>>//

// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Donn�es								  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

#include "_Global.h"
#include "PNG.h"


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Fonctions							  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason)
		{
		case DLL_PROCESS_ATTACH:
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;
		}

	return(TRUE);
}

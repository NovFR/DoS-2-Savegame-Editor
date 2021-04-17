
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des icônes du jeu
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Game.h"
#include "Definitions.h"
#include "Locale.h"
#include "Requests.h"
#include "Taskbar.h"
#include "Utils.h"
#include "Debug.h"
#include "Texts.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Atlas des icônes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la base de données ««««««««««««««««««««««««««««««««««»

void Game_IconsRebuild(HWND hWnd)
{
	defs_Rebuild(hWnd,IDM_DB_REBUILDICONS,FALSE,szDefsIconsPath,(LPTHREAD_START_ROUTINE)Game_IconsRebuildThread);
	return;
}


// «»»» Tâche de création «««««««««««««««««««««««««««««««««««««««««««««««»

DWORD WINAPI Game_IconsRebuildThread(DEFSCONTEXT *pDefs)
{
	defs_CreateMsgQueue();
	Debug_Log(0,NULL);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildStarts);

	pDefs->db.uId = sqlite3_open_v2(szIconsDataBasePath,&pDefs->db.pSql,SQLITE_OPEN_READWRITE,NULL);
	if (pDefs->db.uId == SQLITE_OK)
		{
		sqlite3_exec(pDefs->db.pSql,"PRAGMA journal_mode = MEMORY",NULL,NULL,NULL);
		defs_SQLWipeTables(&pDefs->db,pDefs->bWipe,2,"IconUVList","TextureAtlasInfo");
		if ((pDefs->iResult = defs_CreateFileList(szDefsIconsPath,pDefs)) == DEFS_RESULT_OK)
			{
			DEFSFILE*	pFile;
			float		i,iMax;

			i = 0.0f;
			iMax = (float)List_EntryCount(&pDefs->files);

			for (pFile = (DEFSFILE *)pDefs->files.next; pFile != NULL; pFile = (DEFSFILE *)pFile->node.next)
				{
				if ((pDefs->iResult = defs_ThreadAbort())) break;
				if ((pDefs->iResult = defs_LoadData(pFile))) break;
				if ((pDefs->iResult = defs_LoadXML(pFile))) break;
				if ((pDefs->iResult = defs_UpdateTextureAtlas(&pDefs->db,pFile))) break;
				if ((pDefs->iResult = defs_UpdateIconsUV(&pDefs->db,pFile))) break;
				xml_ReleaseAll(&pFile->xmlRoot);
				pDefs->iPos[0] = (int)(++i/iMax*100.0f);
				}
			}
		}
	else
		{
		defs_SQLErrorMsg(pDefs->db.pSql);
		pDefs->iResult = DEFS_RESULT_FAILED;
		}
	sqlite3_close(pDefs->db.pSql);

	defs_DoneMessage(pDefs);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildEnds);
	App.hThread = NULL;
	return(0);
}

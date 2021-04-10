
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Objets
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "XML.h"
#include "Texts.h"
#include "Utils.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modèle								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Vérification d'un modèle ««««««««««««««««««««««««««««««««««««««««»

int Game_ItemTemplateCheck(DOS2ITEM *pItem, WCHAR *pszNewTemplate, UINT uNewTemplateType)
{
	WCHAR*	pszOriginalTemplate;
	WCHAR*	pszCurrentTemplate;
	WCHAR*	pszTemplateType;
	UINT	uOriginalTemplateType;
	UINT	uCurrentTemplateType;
	GUID	uidOriginalTemplate;
	GUID	uidCurrentTemplate;
	GUID	uidNewTemplate;
	GUID	uidEmpty;
	WCHAR	szUID[40];

	ZeroMemory(&uidOriginalTemplate,sizeof(GUID));
	ZeroMemory(&uidCurrentTemplate,sizeof(GUID));
	ZeroMemory(&uidNewTemplate,sizeof(GUID));
	ZeroMemory(&uidEmpty,sizeof(GUID));

	//--- Original values ---

	pszCurrentTemplate = xml_GetAttrValue(xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,szXMLattribute,szXMLid,L"CurrentTemplate"),szXMLvalue);
	pszOriginalTemplate = xml_GetAttrValue(xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,szXMLattribute,szXMLid,L"OriginalTemplate"),szXMLvalue);
	pszTemplateType = xml_GetAttrValue(xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,szXMLattribute,szXMLid,L"CurrentTemplateType"),szXMLvalue);
	uCurrentTemplateType = pszTemplateType?wcstol(pszTemplateType,NULL,10):0;
	pszTemplateType = xml_GetAttrValue(xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,szXMLattribute,szXMLid,L"OriginalTemplateType"),szXMLvalue);
	uOriginalTemplateType = pszTemplateType?wcstol(pszTemplateType,NULL,10):0;
	if (pszCurrentTemplate)
		{
		Misc_Printf(szUID,40,szUIDFmt,pszCurrentTemplate);
		IIDFromString(szUID,&uidCurrentTemplate);
		}
	if (pszOriginalTemplate)
		{
		Misc_Printf(szUID,40,szUIDFmt,pszOriginalTemplate);
		IIDFromString(szUID,&uidOriginalTemplate);
		}

	//--- Vérifications ---

	if (!pszNewTemplate) return(ITEM_TEMPLATE_INVALID);						// No UID

	Misc_Printf(szUID,40,szUIDFmt,pszNewTemplate);
	if (IIDFromString(szUID,&uidNewTemplate) != S_OK) return(ITEM_TEMPLATE_INVALID);		// Invalid UID
	if (!memcmp(&uidNewTemplate,&uidEmpty,sizeof(GUID))) return(ITEM_TEMPLATE_INVALID);		// Empty UID

	if (!memcmp(&uidNewTemplate,&uidCurrentTemplate,sizeof(GUID)))					// UID == CurrentUID
		{
		if (uCurrentTemplateType != uNewTemplateType) return(ITEM_TEMPLATE_TYPEMISMATCH);
		return(ITEM_TEMPLATE_ALREADYDEFINED);
		}
	if (!memcmp(&uidNewTemplate,&uidOriginalTemplate,sizeof(GUID)))					// UID == OriginalUID
		{
		if (uOriginalTemplateType != uNewTemplateType) return(ITEM_TEMPLATE_TYPEMISMATCH);
		return(ITEM_TEMPLATE_ALREADYDEFINED);
		}

	return(ITEM_TEMPLATE_OK);
}


// «»»» Affichage d'erreur ««««««««««««««««««««««««««««««««««««««««««««««»

int Game_ItemTemplateMsg(HWND hWnd, int iMsg)
{
	UINT	uLocaleId = 0;

	switch(iMsg)
		{
		case ITEM_TEMPLATE_INVALID:
			uLocaleId = TEXT_ERR_TEMPLATE_INVALID;
			break;
		case ITEM_TEMPLATE_TYPEMISMATCH:
			uLocaleId = TEXT_ERR_TEMPLATE_TYPEMISMATCH;
			break;
		}

	if (uLocaleId) MessageBox(hWnd,Locale_GetText(uLocaleId),NULL,MB_ICONERROR|MB_OK);
	return(uLocaleId?0:1);
}

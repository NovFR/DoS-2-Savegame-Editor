
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des fichiers PNG
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Files.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Lecture d'une image PNG «««««««««««««««««««««««««««««««««««««««««»

HBITMAP png_LoadImage(WCHAR *pszPath)
{
	HBITMAP		hResult;
	HANDLE		hFile;
	DWORD		dwFileSize;
	DWORD		dwBytes;
	BYTE*		pFileBuffer;

	hResult = NULL;
	hFile = INVALID_HANDLE_VALUE;
	pFileBuffer = NULL;

	//--- Lecture de l'image ---

	hFile = CreateFile(pszPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (hFile == INVALID_HANDLE_VALUE) goto Done;

	dwFileSize = GetFileSize(hFile,NULL);
	if (dwFileSize == 0xFFFFFFFF) goto Done;

	pFileBuffer = HeapAlloc(App.hHeap,0,dwFileSize);
	if (!pFileBuffer) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }

	ReadFile(hFile,pFileBuffer,dwFileSize,&dwBytes,NULL);
	if (dwBytes != dwFileSize) goto Done;

	//--- Conversion ---

	hResult = png_Load(pFileBuffer);

	//--- Terminé ---

Done:	if (pFileBuffer) HeapFree(App.hHeap,0,pFileBuffer);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	return(hResult);
}


// «»»» Lecture d'une ressource PNG «««««««««««««««««««««««««««««««««««««»

HBITMAP png_LoadRessource(UINT uResID)
{
	HGLOBAL	hPngRes;
	HRSRC	hRes;
	void*	pImage;

	hRes = FindResource(NULL,MAKEINTRESOURCE(uResID),RT_RCDATA);
	if (hRes)
		{
		hPngRes = LoadResource(NULL,hRes);
		if (hPngRes)
			{
			pImage = LockResource(hPngRes);
			if (pImage) return(png_Load(pImage));
			}
		}

	return(NULL);
}


// «»»» Conversion d'une image PNG en HBITMAP «««««««««««««««««««««««««««»

HBITMAP png_Load(void *pImage)
{
	PNGIMAGEINFO	Info;
	HBITMAP		hResult;
	BYTE*		pImageData;
	png_struct*	pPngStruct;
	png_info*	pPngInfo;
	png_byte**	pRowPointers;
	int		r;

	//--- Initialisations ---

	hResult = NULL;
	pImageData = NULL;
	pPngStruct = NULL;
	pPngInfo = NULL;
	pRowPointers = NULL;

	pPngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if (!pPngStruct) return(NULL);

	pPngInfo = png_create_info_struct(pPngStruct);
	if (!pPngInfo) goto Done;

	if (setjmp(png_jmpbuf(pPngStruct)))
		{
		#if _DEBUG
		MessageBox(App.hWnd,L"Something went wrong...",NULL,MB_ICONERROR|MB_OK);
		#endif
		goto Done;
		}

	ZeroMemory(&Info,sizeof(PNGIMAGEINFO));
	Info.pImage = pImage;

	png_set_read_fn(pPngStruct,&Info,png_Read);

	//--- Set parameters ---

	png_read_info(pPngStruct,pPngInfo);
	png_get_IHDR(pPngStruct,pPngInfo,(UINT *)&Info.iWidth,(UINT *)&Info.iHeight,&Info.iBitDepth,&Info.iColorType,NULL,NULL,NULL);

	png_set_scale_16(pPngStruct);
	if (Info.iColorType == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(pPngStruct);
	if (Info.iColorType == PNG_COLOR_TYPE_GRAY && Info.iBitDepth < 8) png_set_expand_gray_1_2_4_to_8(pPngStruct);
	if (png_get_valid(pPngStruct,pPngInfo,PNG_INFO_tRNS) != 0) png_set_tRNS_to_alpha(pPngStruct);
	if ((Info.iColorType&PNG_COLOR_MASK_COLOR) != 0) png_set_bgr(pPngStruct);
	png_set_alpha_mode(pPngStruct,PNG_ALPHA_OPTIMIZED,PNG_DEFAULT_sRGB);
	png_set_filler(pPngStruct,0xffff,PNG_FILLER_AFTER);

	//--- Read image ---

	pImageData = HeapAlloc(App.hHeap,0,Info.iWidth*Info.iHeight*4);
	if (!pImageData) goto Done;

	pRowPointers = HeapAlloc(App.hHeap,0,Info.iHeight*sizeof(png_byte *));
	if (!pRowPointers) goto Done;

	for (r = 0; r < Info.iHeight; r++) pRowPointers[r] = pImageData+r*(Info.iWidth*4);

	png_read_image(pPngStruct,pRowPointers);
	png_read_end(pPngStruct,pPngInfo);

	hResult = CreateBitmap(Info.iWidth,Info.iHeight,1,32,pImageData);

	//--- Done ---

Done:	if (pRowPointers) HeapFree(App.hHeap,0,pRowPointers);
	if (pImageData) HeapFree(App.hHeap,0,pImageData);
	png_destroy_read_struct(&pPngStruct,&pPngInfo,NULL);
	return(hResult);
}

// --- Fonction de lecture ---

void png_Read(png_struct *pPng, png_byte *pData, png_size_t uSize)
{
	PNGIMAGEINFO*	pInfo;
	png_byte*	pImage;

	pInfo = (PNGIMAGEINFO *)png_get_io_ptr(pPng);
	pImage = pInfo->pImage+pInfo->uOffset;

	CopyMemory(pData,pImage,uSize);
	pInfo->uOffset += uSize;
	return;
}

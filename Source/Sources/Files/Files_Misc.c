
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines communes pour tous les types de fichiers
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Files.h"
#include "LZ4.h"
#include "LZ4frame.h"
#include "zlib.h"
#include "Texts.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Décompression «««««««««««««««««««««««««««««««««««««««««««««««««««»

UINT lsa_Decompress(BYTE bCompressionFlags, BYTE *pSrc, DWORD dwSrcSize, BYTE *pDest, DWORD dwDstSize, DWORD dwVersion)
{
	switch(bCompressionFlags&COMPRESSION_METHOD)
		{
		case COMPRESSION_METHOD_LZ4: {
			if (dwVersion >= 3)
				{
				LZ4F_decompressionContext_t	dctx;
				LZ4F_errorCode_t		result;
				BYTE*				input;
				BYTE*				output;
				size_t				inputAvailable;
				size_t				inputOffset;
				size_t				outputOffset;
				size_t				outputSize;
				size_t				outputFree;

				result = LZ4F_createDecompressionContext(&dctx,LZ4F_VERSION);
				if (LZ4F_isError(result)) return(TEXT_ERR_LSA_UNPACK);

				input = pSrc;
				output = NULL;
				inputOffset = outputOffset = outputSize = 0;
				while (inputOffset < dwSrcSize)
					{
					outputFree = outputSize-outputOffset;
					if (outputFree < 65535)
						{
						BYTE* outputTmp = HeapAlloc(App.hHeap,0,outputSize+(65535-outputFree));
						if (output)
							{
							CopyMemory(outputTmp,output,outputSize);
							HeapFree(App.hHeap,0,output);
							}
						output = outputTmp;
						outputSize += 65535-outputFree;
						outputFree = outputSize-outputOffset;
						}
					inputAvailable = dwSrcSize-inputOffset;
					result = LZ4F_decompress(dctx,output+outputOffset,&outputFree,input+inputOffset,&inputAvailable,NULL);
					if (LZ4F_isError(result))
						{
						LZ4F_freeDecompressionContext(dctx);
						HeapFree(App.hHeap,0,output);
						return(TEXT_ERR_LSA_UNPACK);
						}
					inputOffset += inputAvailable;
					outputOffset += outputFree;
					if (inputAvailable == 0)
						{
						LZ4F_freeDecompressionContext(dctx);
						HeapFree(App.hHeap,0,output);
						return(TEXT_ERR_LSA_UNPACK);
						}
					}

				LZ4F_freeDecompressionContext(dctx);

				// Assuming that the header gives the correct length from the beginning...
				// ...we just paste decompressed data into the original buffer.
				//
				if (outputOffset != dwDstSize) return(TEXT_ERR_LSA_UNPACK);
				CopyMemory(pDest,output,dwDstSize);
				HeapFree(App.hHeap,0,output);
				}
			else
				{
				int iDecompressed = LZ4_decompress_safe((const char *)pSrc,(char *)pDest,dwSrcSize,dwDstSize);
				if (iDecompressed != dwDstSize) return(TEXT_ERR_LSA_UNPACK);
				}
			} break;

		case COMPRESSION_METHOD_ZLIB: {
			long unsigned int iDestLen = dwDstSize;
			int result = uncompress(pDest,&iDestLen,pSrc,dwSrcSize);
			if (result != Z_OK) return(TEXT_ERR_LSA_UNPACK);
			} break;

		default:return(TEXT_ERR_LSA_METHOD);
		}

	return(0);
}

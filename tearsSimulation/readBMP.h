#ifndef READ_BMP
#define READ_BMP

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////////////////////////
// This function opens the "bitmap" file given (szFileName), verifies that it is
// a 24bit .BMP file and loads the bitmap bits needed so that it can be used
// as a texture. The width and height of the bitmap are returned in nWidth and
// nHeight. The memory block allocated and returned must be deleted with free();
// The returned array is an 888 BGR texture
// These structures match the layout of the equivalent Windows specific structs 
// used by Win32

#pragma pack(1)
struct RGB { 
	GLbyte blue;
	GLbyte green;
	GLbyte red;
	GLbyte alpha;
};

struct BMPInfoHeader {
	GLuint	size;
	GLuint	width;
	GLuint	height;
	GLushort  planes;
	GLushort  bits;
	GLuint	compression;
	GLuint	imageSize;
	GLuint	xScale;
	GLuint	yScale;
	GLuint	colors;
	GLuint	importantColors;
};

struct BMPHeader {
	GLushort	type; 
	GLuint	size; 
	GLushort	unused; 
	GLushort	unused2; 
	GLuint	offset; 
}; 

struct BMPInfo {
	BMPInfoHeader		header;
	RGB				colors[1];
};
#pragma pack(8)


GLubyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);
//{
//	FILE*	pFile;
//	BMPInfo *pBitmapInfo = NULL;
//	unsigned long lInfoSize = 0;
//	unsigned long lBitSize = 0;
//	GLubyte *pBits = NULL;					// Bitmaps bits
//	BMPHeader	bitmapHeader;
//
//	// Attempt to open the file
//	pFile = fopen(szFileName, "rb");
//	if(pFile == NULL)
//		return NULL;
//
//	// File is Open. Read in bitmap header information
//	fread(&bitmapHeader, sizeof(BMPHeader), 1, pFile);
//
//	// Read in bitmap information structure
//	lInfoSize = bitmapHeader.offset - sizeof(BMPHeader);
//	pBitmapInfo = (BMPInfo *) malloc(sizeof(GLbyte)*lInfoSize);
//	if(fread(pBitmapInfo, lInfoSize, 1, pFile) != 1)
//	{
//		free(pBitmapInfo);
//		fclose(pFile);
//		return false;
//	}
//
//	// Save the size and dimensions of the bitmap
//	*nWidth = pBitmapInfo->header.width;
//	*nHeight = pBitmapInfo->header.height;
//	lBitSize = pBitmapInfo->header.imageSize;
//
//	// If the size isn't specified, calculate it anyway	
//	if(pBitmapInfo->header.bits != 24)
//	{
//		free(pBitmapInfo);
//		return false;
//	}
//
//	if(lBitSize == 0)
//		lBitSize = (*nWidth *
//		pBitmapInfo->header.bits + 7) / 8 *
//		abs(*nHeight);
//
//	// Allocate space for the actual bitmap
//	free(pBitmapInfo);
//	pBits = (GLubyte*)malloc(sizeof(GLubyte)*lBitSize);
//
//	// Read in the bitmap bits, check for corruption
//	if(fread(pBits, lBitSize, 1, pFile) != 1)
//	{
//		free(pBits);
//		pBits = NULL;
//	}
//
//	// Close the bitmap file now that we have all the data we need
//	fclose(pFile);
//
//	return pBits;
//}

#endif
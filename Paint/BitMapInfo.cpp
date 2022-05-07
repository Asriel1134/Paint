#include "pch.h"
#include "BitMapInfo.h"

BitMapInfo::BitMapInfo(BITMAP bitmap, long sizeBits, PBYTE pBits)
{
	this->bitmap = bitmap;
	this->sizeBits = sizeBits;
	this->pBits = pBits;
}
#pragma once
class BitMapInfo
{
public:
	BITMAP bitmap;
	long sizeBits;
	PBYTE pBits;

	BitMapInfo(BITMAP bitmap, long sizeBits, PBYTE pBits);
};


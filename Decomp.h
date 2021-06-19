#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>

class MPPCUnpack
{
public:
    BYTE packedOffset = 0;
    std::vector<byte> Unpack(byte packedByte);
    void Unpack(std::vector<byte>& compressedBytes);

public:
    int code1 = 0;
    int code2 = 0;
    int code3 = 0;
    int code4 = 0;

    std::vector<byte> packedBytes;
    std::vector<byte> unpackedBytes;
    std::vector<byte> unpackedChunk;

    void Copy(int shift, int size, std::vector<byte>& unpackedChunkData);
    unsigned int GetPackedBits(int bitCount);
    bool HasBits(int count);
};

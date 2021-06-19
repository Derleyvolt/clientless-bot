#include "stdafx.h"
#include "Decomp.h"

// ESSE ARQUIVO ESPECÍFICO NÃO É DE MINHA AUTORIA.. NAO HÁ NADA SOBRE ESSE ALGORITMO PRIMITIVO DE COMPACTAÇÃO NA NET, É BEM ANTIGO..
// E EU NÃO TAVA MT AFIM DE PASSAR ALGUMAS SEMANAS CONVERTENDO ASSEMBLY EM C++ .. 2018

std::vector<byte>  MPPCUnpack::Unpack(byte packedByte)
{
    packedBytes.push_back(packedByte);

    unpackedChunk.clear();

    if (unpackedBytes.size() >= 10240)
        unpackedBytes.erase(unpackedBytes.begin(), unpackedBytes.begin() + 2048);

    for (; ; )
    {
        if (code3 == 0)
        {
            if (HasBits(4))
            {
                if (GetPackedBits(1) == 0)
                {
                    // 0-xxxxxxx
                    code1 = 1;
                    code3 = 1;
                }
                else
                {
                    if (GetPackedBits(1) == 0)
                    {
                        // 10-xxxxxxx
                        code1 = 2;
                        code3 = 1;
                    }
                    else
                    {
                        if (GetPackedBits(1) == 0)
                        {
                            // 110-xxxxxxxxxxxxx-*
                            code1 = 3;
                            code3 = 1;
                        }
                        else
                        {
                            if (GetPackedBits(1) == 0)
                            {
                                // 1110-xxxxxxxx-*
                                code1 = 4;
                                code3 = 1;
                            }
                            else
                            {
                                // 1111-xxxxxx-*
                                code1 = 5;
                                code3 = 1;
                            }
                        }
                    }
                }
            }
            else
                break;
        }
        else if (code3 == 1)
        {
            if (code1 == 1)
            {
                if (HasBits(7))
                {
                    auto outB = (byte)GetPackedBits(7);
                    unpackedChunk.push_back(outB);
                    unpackedBytes.push_back(outB);
                    code3 = 0;
                }
                else
                    break;
            }
            else if (code1 == 2)
            {
                if (HasBits(7))
                {
                    auto outB = (byte)(GetPackedBits(7) | 0x80);
                    unpackedChunk.push_back(outB);
                    unpackedBytes.push_back(outB);
                    code3 = 0;
                }
                else
                    break;
            }
            else if (code1 == 3)
            {
                if (HasBits(13))
                {
                    code4 = (int)GetPackedBits(13) + 0x140;
                    code3 = 2;
                }
                else
                    break;
            }
            else if (code1 == 4)
            {
                if (HasBits(8))
                {
                    code4 = (int)GetPackedBits(8) + 0x40;
                    code3 = 2;
                }
                else
                    break;
            }
            else if (code1 == 5)
            {
                if (HasBits(6))
                {
                    code4 = (int)GetPackedBits(6);
                    code3 = 2;
                }
                else
                    break;
            }
        }
        else if (code3 == 2)
        {
            if (code4 == 0)
            {
                // Guess !!!
                if (packedOffset != 0)
                {
                    packedOffset = 0;
                    packedBytes.erase(packedBytes.begin());
                }
                code3 = 0;
                continue;
            }
            code2 = 0;
            code3 = 3;
        }
        else if (code3 == 3)
        {
            if (HasBits(1))
            {
                if (GetPackedBits(1) == 0)
                {
                    code3 = 4;
                }
                else
                {
                    code2++;
                }
            }
            else
                break;
        }
        else if (code3 == 4)
        {
            int copySize;

            if (code2 == 0)
            {
                copySize = 3;
            }
            else
            {
                auto size = code2 + 1;

                if (HasBits(size))
                {
                    copySize = (int)GetPackedBits(size) + (1 << size);
                }
                else
                    break;
            }

            Copy(code4, copySize, unpackedChunk);
            code3 = 0;
        }
    }

    return unpackedChunk;
}

void MPPCUnpack::Unpack(std::vector<byte>& compressedBytes)
{
    std::vector<byte> list;

    for (auto i : compressedBytes) 
    {
        auto p = Unpack(i);
        list.insert(list.end(), p.begin(), p.end());
    }

    compressedBytes = list;
}

void MPPCUnpack::Copy(int shift, int size, std::vector<byte>& unpackedChunkData)
{
    for (auto i = 0; i < size; i++)
    {
        int pIndex = unpackedBytes.size() - shift;

        if (pIndex < 0)
            return;

        auto b = unpackedBytes[pIndex];
        unpackedBytes.push_back(b);
        unpackedChunkData.push_back(b);
    }
}

unsigned int MPPCUnpack::GetPackedBits(int bitCount)
{
    if (bitCount > 16)
        return 0;

    if (!HasBits(bitCount))
        std::cout << "Unpack bit stream overflow" << std::endl;

    auto alBitCount = bitCount + packedOffset;
    auto alByteCount = (alBitCount + 7) / 8;

    unsigned int v = 0;

    for (auto i = 0; i < alByteCount; i++)
    {
        v |= (unsigned int)(packedBytes[i]) << (24 - i * 8);
    }

    v <<= packedOffset;
    v >>= 32 - bitCount;

    packedOffset += (byte)bitCount;
    auto freeBytes = packedOffset / 8;

    if (freeBytes != 0)
        packedBytes.erase(packedBytes.begin(), packedBytes.begin() + freeBytes);

    packedOffset %= 8;
    return v;
}

bool MPPCUnpack::HasBits(int count)
{
    return (packedBytes.size() * 8 - packedOffset) >= count;
}

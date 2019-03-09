// ConcatenateArrays.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include "pch.h"
#include <iostream>


// Bazı tanımlamalar yapalım.
#define BLOCK_LENGTH 16
#define BYTE_SIZE 8
#define BYTE_SIZE_DOUBLE 8.0

void concatenateArrays(unsigned char lastBlock[BLOCK_LENGTH], unsigned char previousChunk[BLOCK_LENGTH], unsigned char result[BLOCK_LENGTH]) {
	int i;
	//Mesaj bloğu uzunluklarının bilindiklerini varsayalım..
	int lastBlockSize = 105;
	int prevChunkSize = BLOCK_LENGTH * BYTE_SIZE - lastBlockSize;
	//Boş bytelar kadar sola kaydırılması gerekiyor.
	int byteShift = BLOCK_LENGTH - (int)ceil(lastBlockSize / BYTE_SIZE_DOUBLE);

	//boş bitler kadar da sola kaydırılması lazım.
	int bitShift = BYTE_SIZE - (lastBlockSize % BYTE_SIZE);

	//Once result arrayini sıfırlayalım.
	for (i = 0; i < BLOCK_LENGTH; i++)
		result[i] = 0x0;

	//Byle olarak sola kaydırıyoruz
	for (i = 0; i < BLOCK_LENGTH - byteShift; i++)
		result[i] = lastBlock[i + byteShift];

	//Ayrrayi bit olarak sola kaydırırken sağdaki byte tan gelecek bitler için mask hazırlıyoruz.
	unsigned char mask = 0x0;
	for (i = 0; i < bitShift; i++) {
		mask <<= 1;
		mask ^= 0x01;
	}

	//Şimdi ilk byte ın başındaki gereksiz bitlerden kurtulup herşeyi sola kaydıırıyoruz
	for (i = 0; i < BLOCK_LENGTH; i++) {
		result[i] <<= bitShift;
		if (i != BLOCK_LENGTH - 1)
			result[i] ^= (result[i + 1] >> (BYTE_SIZE - bitShift))&mask;
	}

	for (i = 0; i < BLOCK_LENGTH; i++)
		result[i] ^= previousChunk[i];

}

int main()
{
	unsigned char lastBlock[BLOCK_LENGTH] = { 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E }; //105 bits
	unsigned char previousChunk[BLOCK_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xEE, 0xEF };//23 bits
	unsigned char resultArr[BLOCK_LENGTH];

	concatenateArrays(lastBlock, previousChunk, resultArr);

	for (int i = 0; i < BLOCK_LENGTH; ++i)
		printf("%02X", resultArr[i]);
	
}
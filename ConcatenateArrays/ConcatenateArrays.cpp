// ConcatenateArrays.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include "pch.h"
#include <iostream>


// Bazı tanımlamalar yapalım.
#define BLOCK_LENGTH 16
#define BYTE_SIZE 8
#define BYTE_SIZE_DOUBLE 8.0

void concatenateArrays(unsigned char lastBlock[BLOCK_LENGTH], int lastBlockSize,  unsigned char previousCtext[BLOCK_LENGTH], unsigned char result[BLOCK_LENGTH]) {
	int i;

	//Son bloğa eklenecek kısım
	unsigned char stolenCiphertext[BLOCK_LENGTH];

	int stolenCtextSize = BLOCK_LENGTH * BYTE_SIZE - lastBlockSize;
	int stolenByteSize = (int)ceil(stolenCtextSize / BYTE_SIZE_DOUBLE);
	int stolenBitChunkSize = stolenCtextSize % BYTE_SIZE;

	//Son mesaj bloğunun boş bytelar kadar sola kaydırılması gerekiyor.
	int messageByteShift = BLOCK_LENGTH - (int)ceil(lastBlockSize / BYTE_SIZE_DOUBLE);

	//Son mesaj bloğunun boş bitler kadar da sola kaydırılması lazım.
	int messageBitShift = BYTE_SIZE - (lastBlockSize % BYTE_SIZE);


	//Once arrayleri sıfırlayalım.
	for (i = 0; i < BLOCK_LENGTH; i++)
		result[i] = stolenCiphertext[i] =0x0;

	//Byle olarak sola kaydırıyoruz
	for (i = 0; i < BLOCK_LENGTH - messageByteShift; i++)
		result[i] = lastBlock[i + messageByteShift];

	//Mesaj ayrrayini bit olarak sola kaydırırken sağdaki byte tan gelecek bitler için mask hazırlıyoruz.
	unsigned char mask = 0x0;
	for (i = 0; i < messageBitShift; i++) {
		mask <<= 1;
		mask ^= 0x01;
	}

	//Şimdi ilk byte ın başındaki gereksiz bitlerden kurtulup herşeyi sola kaydıırıyoruz
	for (i = 0; i < BLOCK_LENGTH; i++) {
		result[i] <<= messageBitShift;
		if (i != BLOCK_LENGTH - 1)
			result[i] ^= (result[i + 1] >> (BYTE_SIZE - messageBitShift))&mask;
	}


	//Son ciphertextten gerekli yerleri alıyoruz. Şimdilik fazla bit alacağız.
	for (i = BLOCK_LENGTH - stolenByteSize; i < BLOCK_LENGTH; i++)
		stolenCiphertext[i] = previousCtext[i];

	mask = 0x0;
	for (i = 0; i < stolenBitChunkSize; i++) {
		mask <<= 1;
		mask ^= 0x01;
	}
	//Fazla aldığımız bitleri atıyoruz
	stolenCiphertext[BLOCK_LENGTH - stolenByteSize] = (stolenCiphertext[BLOCK_LENGTH - stolenByteSize] & mask);

	for (i = 0; i < BLOCK_LENGTH; i++)
		result[i] ^= stolenCiphertext[i];

}

int main()
{
	//Son mesaj bloğunu sağa dayalı bir şekilde alıyoruz. 
	unsigned char lastMessageBlock[BLOCK_LENGTH] = { 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0A }; //105 bits
	unsigned char previousCtext[BLOCK_LENGTH] = { 0xAA, 0x0c, 0xd0, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB, 0xF3, 0xEE, 0xEF };//son 23 bit alınacak
	unsigned char resultArr[BLOCK_LENGTH];

	//Mesaj bloğu uzunluklarının bilindiklerini varsayalım..
	int lastBlockSize = 105;
	concatenateArrays(lastMessageBlock, lastBlockSize, previousCtext, resultArr);

	for (int i = 0; i < BLOCK_LENGTH; ++i)
		printf("%02X", resultArr[i]);
	
}
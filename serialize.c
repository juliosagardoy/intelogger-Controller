/*
 *
 */
#include "serialize.h"

/*
 * Takes a byte an pushes it to the end of bufctl. bufctl grows 1
 */
void serialize_byte(const byte in, bufctl *out)
{
	memcpy(((byte*)out->data) + out->datasize, &in, sizeof(byte));
	
	out->datasize += sizeof(byte);
}

/*
 * Takes a word an pushes it to the end of bufctl. bufctl grows 2
 */
void serialize_word(const word in, bufctl *out)
{
	byte lsB, msB;
	
	lsB = (byte)in;
	msB = (byte)(in >> 8);
	memcpy(((byte*)out->data) + out->datasize, &msB, sizeof(byte));
	memcpy(((byte*)out->data) + out->datasize + 1, &lsB, sizeof(byte));
	out->datasize += sizeof(word);			
}
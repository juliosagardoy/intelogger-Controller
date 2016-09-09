/*
 *
 */
#include "serialize.h"

/**
 * 
 */
void init_serializer(void)
{
    memset(&RxMsg, 0x00, sizeof (RxMsg));
    memset(&BufCtl, 0x00, sizeof (BufCtl));
}

/**
 * Takes a byte an pushes it to the end of bufctl. bufctl grows 1
 * @param in The byte to be appended
 * @param out Pointer to bufctl
 */
void serialize_byte_pushback(const byte in, vector *out)
{
	memcpy(((byte*)out->data) + out->datasize, &in, sizeof(byte));
	
	out->datasize += sizeof(byte);
}

/*
 * Takes a word (2-bytes) an pushes it to the end of bufctl. bufctl grows 2
 * @param in The word to be appended
 * @param out Pointer to bufctl
 */
void serialize_word_pushback(const word in, vector *out)
{
	byte lsB, msB;
	
	lsB = (byte)in;
	msB = (byte)(in >> 8);
	memcpy(((byte*)out->data) + out->datasize, &msB, sizeof(byte));
	memcpy(((byte*)out->data) + out->datasize + 1, &lsB, sizeof(byte));
	out->datasize += sizeof(word);			
}

/**
 * Transmit the current contents of BufCtl
 * @param start Sentence start byte
 * @param datatype Specifies what kind of data the datagram holds
 * @param datasize Specifies the datagram size in bytes, without including header
 * @param pointer to datagram itself
 * @return Nothing
 */
void transmit_sentence(byte start, byte datatype, byte datasize, const byte *data) {
    EUSART_Write_1Byte(start);
    EUSART_Write_1Byte(datatype);
    EUSART_Write_1Byte(datasize);
    EUSART_WriteCString(data);
}
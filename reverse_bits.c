#include <stdio.h>

unsigned char reverse_bits(unsigned char octet)
{
	unsigned char res = 0;
	int i = 0;
	while (i < 8)
	{
		res <<= 1;
		res |= (octet & 1);
		octet >>= 1;
		i++;
	}
	return res;
}
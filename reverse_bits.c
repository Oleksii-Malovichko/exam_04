#include <stdio.h>

unsigned char reverse_bits(unsigned char octet)
{
	unsigned char c = 0;
	int i = 8;
	while (i > 0)
	{
		c = c * 2 + (octet % 2);
		octet /= 2;
		i--;
	}
	return c;
}
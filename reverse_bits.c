#include <stdio.h>

unsigned char reverse_bits(unsigned char octet)
{
	unsigned char res;
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

int main()
{
	unsigned char b = reverse_bits('L');
	printf("%c\n", b);
}

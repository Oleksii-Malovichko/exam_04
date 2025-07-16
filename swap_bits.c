unsigned char swap_bits(unsigned char octet)
{
	return ((octet >> 4) | (octet << 4));
}
#include <stdio.h>
unsigned char reverse(unsigned char octet)
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
	unsigned char b = reverse('2');
	printf("%c\n", b);
}
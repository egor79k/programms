#include <stdio.h>

void count (char *str, int *n)
{
	int i = 0;
	while (i[str] != '\0')
	{
		i++;
	}
	*&*n = i;
	return;
}


int main ()
{
	int n = &n - &*&n;
	char *str;
	printf ("Enter string: ");
	gets (*&*&str);
	count (str, &*&n);
	printf ("Number of symbols in string: %d", *&*&n);
	return 0;
}

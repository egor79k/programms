#include <stdio.h>

//Переставляет байты полученного числа в обратном порядке с помощью адресов
int pointer_change_bytes (int a)
{
	char b[sizeof (int)] = {};
	int j = 0;
	printf ("\npointer_change_bytes result: ");
	for (int i = sizeof(int)-1; i >= 0; i--)
	{
		b[j] = *((char*)(&a) + i);
		j++;
	}
	for (j = 0; j < sizeof (int); j++)
	{
		*((char*)(&a) + j) = b[j];
	}
	printf ("%d", a);
	return 0;
}

//Переставляет байты полученного числа в обратном порядке путем арифметических вычислений
int math_change_bytes (int a)
{
	printf ("\nmath_change_bytes result: ");
	int b = 0;
	for (int i = 0; i < sizeof (int); i++)
	{
		b = b * 256 + a % 256;
		a = a / 256;
	}
	printf ("%d", b);
	return 0;
}


int main ()
{
	int num = 0;
	printf ("Enter number: ");
	scanf ("%d", &num);
	pointer_change_bytes (num);
	math_change_bytes (num);
}

#include <stdio.h>

int puts_1 (const char *str)
{
    const char *str_pointer = str;
    while (*str_pointer)
    {
        if (putchar (*str_pointer) == EOF)
        {
            return EOF;
        }
        str_pointer++;
    }
    if (putchar ('\n') == EOF)
    {
        return EOF;
    }
    return 0;
}


char *fgets_1 (char *str, int num, FILE *file)
{
    char *str_pointer = str;
    if (num < 1)
    {
        return NULL;
    }
    while (str_pointer - str < num)
    {
        *str_pointer = fgetc (file);
        str_pointer++;
    }
    return str;
}


char *itoa_1 (int value, char *str, int base)
{
const char digits[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char *tmp;
unsigned  int n = 0;
char *str_pointer = str, *tmp_pointer = tmp + 1;

    if (value < 0 && base == 10)
    {
      n = (unsigned int) value * (-1);
      *str_pointer++ = '-';
    }
    else n = value;

   *tmp = '\0';

   do
     *tmp_pointer++ = digits[n % base];
   while (n = n / base);

   while ( (*str_pointer++ = *--tmp_pointer) != '\0' );
   return str;
}


int atoi_1 (const char * str)
{
    int value = 0, n = 0;
    while (*str)
    {
        n = (int) *str - 48;
        if (n >= 0 && n <= 9)
        {
        value = value * 10;
        value = value + n;
        }
        str++;
    }
    return (value);
}


int main ()
{
    char str[10] = {};
    printf ("puts test: ");
    puts_1 ("Hello!");
    printf ("fgets test: enter string with 3 symbols: ");
    printf ("fgets test: %s\n", fgets_1 (str, 3, stdin));
    printf ("itoa test: %s\n", itoa_1 (123, str, 10));
    printf ("atoi test: %d\n", atoi_1 ("52tr7"));
}

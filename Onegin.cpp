#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

//Возвращает количество символов в исходном файле (NChars)
int count_chars (char *file_name)
{
	assert (file_name);
	FILE *in = fopen (file_name, "r");
	int NChars = 0;
	fseek (in, 0, SEEK_END);
	NChars = ftell (in);
	fclose (in);
	return NChars;
}

//Записывает текст из исходного файла в массив text и возвращает количество строк (NLines)
int input (char *file_name, int NChars, char *text)
{
	int NLines = 0;
	FILE *in = fopen (file_name, "r");
	fread (text, sizeof (char), NChars, in);
	fclose (in);
	if (text[NChars - 1] != '\n')
	{
		text[NChars] = '\n';
	}
	for (int i = 0; i < NChars; i++)
	{
		if (text[i] == '\n')
		{
			NLines++;
		}
	}
	return NLines;
}

//Записывает адреса начала строк в массив index и сортирует их по алфавиту
void sort (char **index, int NLines, int NChars, char *text)
{
	char *tmp;
	int j = 0;
	index[0] = &text[0];
	for (int i = 1; i <= NChars; i++)
	{
		if (text[i-1] == '\n')
		{
			text[i-1] = '\0';
			j++;
			index[j] = &text[i];
		}
	}
	for (int i = 0; i < NLines; i++)
	{
		for (int k = 1; k < NLines-i; k++)
		{
			if (strcmp (index[k], index[k-1]) < 0)
			{
				tmp = index[k];
				index[k] = index[k-1];
				index[k-1] = tmp;
			}
		}

	}
}

//Записывает отсортированный текст в файл вывода
void output (char *file_name, int NLines, char **index)
{
	assert (file_name);
	assert (index);
	FILE *out = fopen (file_name, "w");
	for (int i = 0; i < NLines; i++)
	{
		fprintf(out, "%s\n", index[i]);
	}
}


int main ()
{
	int NChars = 0, NLines = 0;
	char file_name[20];

	printf ("Enter input file name: ");
	scanf ("%s", file_name);
	NChars = count_chars (file_name);
	printf("|About input file:\n||Number of symbols: %d\n", NChars);
	char text[NChars];
	NLines = input (file_name, NChars, text);
	printf("||Number of strings: %d\n", NLines);
	char *index[NLines];
	sort (index, NLines, NChars, text);
	printf ("Enter output file name: ");
	scanf ("%s", file_name);
	output (file_name, NLines, index);
}

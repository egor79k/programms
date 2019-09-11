#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

int count_chars (char *file_name);

int input (char *file_name, int NChars, char *text);

void find_strings (char **start_index, char **end_index, char **original_index, int NChars, char *text);

void sort (char **start_index, char **end_index, int NLines, int NChars);

void output (char *file_name, int NLines, char **start_index, char **end_index, char **original_index);


int main ()
{
	int NChars = 0, NLines = 0;
	char file_name[20];

	printf ("Enter input file name: ");
	scanf ("%s", file_name);
	NChars = count_chars (file_name);
	printf("|About input file:\n||Number of symbols: %d\n", NChars);
	char *text = (char *) calloc (NChars, sizeof (char));
	NLines = input (file_name, NChars, text);
	printf("||Number of strings: %d\n", NLines);
	char *start_index[NLines];
	char *end_index[NLines];
	char *original_index[NLines];
	find_strings (start_index, end_index, original_index, NChars, text);
	sort (start_index, end_index, NLines, NChars);
	printf ("Enter output file name: ");
	scanf ("%s", file_name);
	output (file_name, NLines, start_index, end_index, original_index);
}

//Возвращает количество символов в исходном файле (NChars)
int count_chars (char *file_name)
{
	assert (file_name);
	FILE *in = fopen (file_name, "r");
	int NChars = 0;
	fseek (in, 0, SEEK_END);
	NChars = ftell (in);
	fclose (in);
	return NChars + 1;
}

//Записывает текст из исходного файла в массив text и возвращает количество строк (NLines)
int input (char *file_name, int NChars, char *text)
{
	assert (file_name);
	assert (NChars != 0);
	assert (text);
	int NLines = 0;
	FILE *in = fopen (file_name, "r");
	fread (text, sizeof (char), NChars, in);
	fclose (in);
	if (text[NChars - 1] != '\n')
	{
		text[NChars] = '\n';
	}
	for (int i = 0; i <= NChars; i++)
	{
		if (text[i] == '\n')
		{
			NLines++;
		}
	}
	return NLines;
}

//Записывает адреса начала строк в массив index и адреса концов строк в массив end_index
void find_strings (char **start_index, char **end_index, char **original_index, int NChars, char *text)
{
	assert (start_index);
	assert (end_index);
	assert (NChars != 0);
	assert (text);
	int j = 0;
	start_index[0] = &text[0];
	original_index[0] = &text[0];
	for (int i = 1; i <= NChars; i++) //Поиск начал строк
	{
		if (text[i-1] == '\n')
		{
			j++;
			start_index[j] = &text[i];
			original_index[j] = &text[i];
		}
	}
	j = 0;
	for (int i = 1; i <= NChars; i++) //Поиск концов строк
	{
		if (text[i] == '\n')
		{
			text[i] = '\0';
			end_index[j] = &text[i-1];
			j++;
		}
	}
}

//Сортирует строки по алфаивту
void sort (char **start_index, char **end_index, int NLines, int NChars)
{
	assert (start_index);
	assert (end_index);
	assert (NLines != 0);
	assert (NChars != 0);
	char *tmp = 0;
	char *end_tmp1 = 0;
	char *end_tmp2 = 0;
	for (int i = 0; i < NLines; i++)
	{
		for (int k = 1; k < NLines-i; k++)
		{
			if (strcmp (start_index[k], start_index[k - 1]) < 0)
			{
				tmp = start_index[k];
				start_index[k] = start_index[k-1];
				start_index[k-1] = tmp;
			}
			end_tmp1 = end_index[k];
			end_tmp2 = end_index[k - 1];
			while (*end_tmp1 == ',' || *end_tmp1 == ';' || *end_tmp1 == '.' || *end_tmp1 == ':' || *end_tmp1 == '!' || *end_tmp1 == '?' || *end_tmp1 == '"') end_tmp1--;
			while (*end_tmp2 == ',' || *end_tmp2 == ';' || *end_tmp2 == '.' || *end_tmp2 == ':' || *end_tmp2 == '!' || *end_tmp2 == '?' || *end_tmp2 == '"') end_tmp2--;
			while (*end_tmp1 == *end_tmp2)
			{
				end_tmp1--;
				end_tmp2--;
			}
			if ((int)*end_tmp1 - (int)*end_tmp2 < 0)
			{
				tmp = end_index[k];
				end_index[k] = end_index[k - 1];
				end_index[k - 1] = tmp;
			}
		}

	}
	
}

//Записывает отсортированный текст в файл вывода
void output (char *file_name, int NLines, char **start_index, char **end_index, char **original_index)
{
	assert (file_name);
	assert (NLines != 0);
	assert (start_index);
	assert (end_index);
	FILE *out = fopen (file_name, "w");
	fprintf(out, "-----------\nStarts sort\nin-----------\n");
	for (int i = 0; i < NLines; i++)
	{
		fprintf(out, "%s\n", start_index[i]);
	}
	fprintf(out, "\n---------\nEnds sort\n---------\n");
	for (int i = 0; i < NLines; i++)
	{
		while (*end_index[i] != '\0')
		{
		end_index[i]--;
		}
		end_index[i]++;
		fprintf(out, "%s\n", end_index[i]);
	}
	fprintf(out, "\n-------------\nOriginal text\n-------------\n");
	for (int i = 0; i < NLines; i++)
	{
		fprintf(out, "%s\n", original_index[i]);
	}
}

#include <stdio.h>
#include <locale.h>
#include <cstring>

const int NChars = 70;
const int NLines = 50;

void input (char text[NLines][NChars], char *index[NLines])
{
	FILE*in = fopen ("onegin.txt", "r");
	for (int i = 0; i < NLines; i++)
	{
		fgets (text[i], NChars, in);
		index[i] = text[i];
	}
}


void sort (char *index[NLines])
{
	char *tmp;
	for (int i = 1; i < NLines; i++)
	{
		for (int k = 1; k < NLines-i; k++)	 
		if (strcmp (index[k], index[k-1]) < 0)
		{
			tmp = index[k];
			index[k] = index[k-1];
			index[k-1] = tmp;
		}

	}
}


void output (char *index[NLines])
{
	FILE*out = fopen ("output.txt", "w");
	for (int i = 0; i < NLines; i++)
	{
		fprintf(out, "%s", index[i]);
	}
}

int main ()
{
	setlocale (LC_ALL, "Rus");
	char *index[NLines] = {};
	char text[NLines][NChars] = {};
	input (text, index); 
	sort (index);
	output (index);
}

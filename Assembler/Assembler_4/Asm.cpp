#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"

#define GET_PARAMETR 													\
		k = 0;															\
		while (text[i] != '\n' && text[i] != '\0' && text[i] != ' ')	\
		{																\
			parametr[k] = text[i];										\
			k++;														\
			i++;														\
		}																\
		i++;															\


const int MAX_PARAMETR_LENGTH = 7;
const int MAX_NAME_LENGTH = 100;
const int MAX_BIN_SIZE = 250;
const char *initial = {"ELKI"};
const int version = 4;

void Get_File_Name (char *file_name, const char *mode);
char *Get_Buffer (const char *file_name);
char *Write_File_Initial (char *bin);
char *Compile (char *text, char *bin);


int main ()
{
	char file_name[MAX_NAME_LENGTH] = {};
	//Processing input file...
	printf ("\nEnter input file name: ");
	Get_File_Name (file_name, "r");
	char *text = Get_Buffer (file_name);
	char *bin = (char *) calloc (MAX_BIN_SIZE, sizeof (char));
	char *bin_start = bin;
	//Compiling...
	bin = Write_File_Initial (bin);
	bin = Compile (text, bin);
	if (bin == nullptr)
	{
		printf("\nCompilation error\n");
		return 1;
	}
	else printf("\nSuccesful compilation\n");
	//Processing output file...
	printf ("\nEnter output file name: ");
	Get_File_Name (file_name, "wb");
	FILE *out = fopen (file_name, "wb");
	fwrite (bin_start, sizeof (char), bin - bin_start, out);
	return 0;
}


void Get_File_Name (char *file_name, const char *mode)
{
	bool check_result = false;
	while (check_result == false)
	{
		scanf ("%s", file_name);
		FILE *file = fopen (file_name, mode);
		if (file == 0) check_result = false;
		else check_result = true;
		if (check_result == false) printf ("\nWrong name of file. Please enter correct: ");
	}
	return;
}


char *Get_Buffer (const char *file_name)
{
	assert (file_name);

	int NChars = 0;
	FILE *in = fopen (file_name, "r");
	fseek (in, 0, SEEK_END);
	NChars = ftell (in);
	fseek (in, 0, SEEK_SET);
	char *text = (char *) calloc (NChars, sizeof (char));
	fread (text, sizeof (char), NChars, in);
	fclose (in);
	
	return text;
}


char *Write_File_Initial (char *bin)
{
	int j = 0;
	for (j = 0; j < 4; j++) *bin++ = initial[j];
	for (j = 0; j < sizeof (int); j++) *bin++ = *(((char *) &version) + j);
	return bin;
}


char *Compile (char *text, char *bin)
	{
		int i = 0, j = 0,  k = 0;
		char parametr[MAX_PARAMETR_LENGTH] = {};

		while (text[i] != '\0')
		{
			GET_PARAMETR
			if (strcmp (parametr, push) == 0)
			{
				memset (parametr, '\0', sizeof (char) * MAX_PARAMETR_LENGTH);
				GET_PARAMETR
				*bin++ = push_num;
				int number = atoi (parametr);
				for (j = 0; j < sizeof (int); j++) *bin++ = *(((char *) &number) + j);
			}
			else  if (strcmp (parametr, add) == 0) *bin++ = add_num;
			else  if (strcmp (parametr, sub) == 0) *bin++ = sub_num;
			else  if (strcmp (parametr, mul) == 0) *bin++ = mul_num;
			else  if (strcmp (parametr, din) == 0) *bin++ = din_num;
			else  if (strcmp (parametr, end) == 0) *bin++ = end_num;
			else return nullptr;

			memset (parametr, '\0', sizeof (char) * MAX_PARAMETR_LENGTH);
		}
		return bin;
	}

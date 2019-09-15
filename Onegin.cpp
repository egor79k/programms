/**
* Program reads text from input file and write it in output file sorted by starts and by ends
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//Testing functions_________________________________
void Write_test_file ();
void NChars_test ();
void NLines_test ();
void find_strings_test ();
//__________________________________________________

int count_chars (const char *file_name);

int input (const char *file_name, int NChars, char *text);

void find_strings (char **start_index, char **end_index, char **original_index, int NChars, char *text);

void sort (char **start_index, char **end_index, int NLines, int NChars);

void output (const char *file_name, int NLines, char **start_index, char **end_index, char **original_index);

//Comparators functions______________________________
void start_sort (char **start_index, int k);
void end_sort (char **end_index, int k);
//___________________________________________________

int main ()
{
	//Testing...
	printf ("\n|Testing:\n");
	Write_test_file ();
	NChars_test ();
	NLines_test ();
	find_strings_test ();

	//Getting input file name...
	int NChars = 0, NLines = 0;
	char file_name[20];
	printf ("\nEnter input file name: ");
	scanf ("%s", file_name);

	//Getting information about input file...
	NChars = count_chars (file_name) + 1;
	printf("\n|About input file:\n||Number of symbols: %d\n", NChars);
	char *text = (char *) calloc (NChars, sizeof (char));
	NLines = input (file_name, NChars, text);
	printf("||Number of strings: %d\n", NLines);

	//Declaring pointers massives and filling them...
	char **start_index = (char **) calloc (NLines, sizeof (int));
	char **end_index = (char **) calloc (NLines, sizeof (int));
	char **original_index = (char **) calloc (NLines, sizeof (int));
	find_strings (start_index, end_index, original_index, NChars, text);

	//Sorting...
	sort (start_index, end_index, NLines, NChars);
	
	//Writing result in pointed file...
	printf ("\nEnter output file name: ");
	scanf ("%s", file_name);
	output (file_name, NLines, start_index, end_index, original_index);
}

//Returns number of chars in input file (NChars)
int count_chars (const char *file_name)
{
	assert (file_name);
	FILE *in = fopen (file_name, "r");
	int NChars = 0;
	fseek (in, 0, SEEK_END);
	NChars = ftell (in);
	fclose (in);
	return NChars;
}

//Puts text from input file in massive 'text' and returns number of lines (NLines)
int input (const char *file_name, int NChars, char *text)
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

//Puts pointers on strings starts in 'start_index' and 'original_index' and pointers on strings ends in 'end_index'
void find_strings (char **start_index, char **end_index, char **original_index, int NChars, char *text)
{
	assert (start_index);
	assert (end_index);
	assert (NChars != 0);
	assert (text);
	int j = 0;
	start_index[0] = &text[0];
	original_index[0] = &text[0];
	for (int i = 1; i <= NChars; i++) //Finding strings starts
	{
		if (text[i-1] == '\n')
		{
			j++;
			start_index[j] = &text[i];
			original_index[j] = &text[i];
		}
	}
	j = 0;
	for (int i = 1; i <= NChars; i++) //Finding strings ends
	{
		if (text[i] == '\n')
		{
			text[i] = '\0';
			end_index[j] = &text[i-1];
			j++;
		}
	}
	return;
}

//Sorts strings in alphabet order
void sort (char **start_index, char **end_index, int NLines, int NChars)
{
	assert (start_index);
	assert (end_index);
	assert (NLines != 0);
	assert (NChars != 0);
	for (int i = 0; i < NLines; i++)
	{
		for (int k = 1; k < NLines-i; k++)
		{
			start_sort (start_index, k);
			end_sort (end_index, k);
		}

	}
	return;
}

//Writes sorted texts in output file
void output (const char *file_name, int NLines, char **start_index, char **end_index, char **original_index)
{
	assert (file_name);
	assert (NLines != 0);
	assert (start_index);
	assert (end_index);
	FILE *out = fopen (file_name, "w");
	fprintf(out, "-----------\nStarts sort\n-----------\n"); //Запись строк в алфавитном порядке
	for (int i = 0; i < NLines; i++)
	{
		fprintf(out, "%s\n", start_index[i]);
	}
	fprintf(out, "\n---------\nEnds sort\n---------\n"); //Запись строк в алфавитном порядке по окончаниям
	for (int i = 0; i < NLines; i++)
	{
		while (*end_index[i] != '\0')
		{
		end_index[i]--;
		}
		end_index[i]++;
		fprintf(out, "%s\n", end_index[i]);
	}
	fprintf(out, "\n-------------\nOriginal text\n-------------\n"); //Запись изначального варианта текста
	for (int i = 0; i < NLines; i++)
	{
		fprintf(out, "%s\n", original_index[i]);
	}
	fclose (out);
	return;
}


//Comparators__________________________________________________

//Starts sorter
void start_sort (char **start_index, int k)
{
	if (strcmp (start_index[k], start_index[k - 1]) < 0) //Sorting strings by starts
	{
		char *tmp = 0;
		tmp = start_index[k];
		start_index[k] = start_index[k-1];
		start_index[k-1] = tmp;
	}
}


void end_sort (char **end_index, int k)
{
	char *tmp = 0;
	char *end_tmp1 = 0;
	char *end_tmp2 = 0;
	end_tmp1 = end_index[k];
	end_tmp2 = end_index[k - 1];
	while (*end_tmp1 == ',' || *end_tmp1 == ';' || *end_tmp1 == '.' || *end_tmp1 == ':' || *end_tmp1 == '!' || *end_tmp1 == '?' || *end_tmp1 == '"') end_tmp1--;
	while (*end_tmp2 == ',' || *end_tmp2 == ';' || *end_tmp2 == '.' || *end_tmp2 == ':' || *end_tmp2 == '!' || *end_tmp2 == '?' || *end_tmp2 == '"') end_tmp2--;
	while (*end_tmp1 == *end_tmp2)
	{											//  ^ Skipping punctuation marks
		end_tmp1--;
		end_tmp2--;
	}
	if ((int)*end_tmp1 - (int)*end_tmp2 < 0) //Sorting strings by ends
	{
		tmp = end_index[k];
		end_index[k] = end_index[k - 1];
		end_index[k - 1] = tmp;
	}
}

//Testing functions________________________________________________________________

//Writing file for test functions
void Write_test_file ()
{
	FILE *Test_file = fopen ("Test_file.txt", "w");
	fprintf (Test_file, "1234567890\nqwerty\n[Test]\n[Test]\n[Test]\n");
	fclose (Test_file);
}

//Testing chars counting
void NChars_test ()
{
	int NChars = count_chars ("Test_file.txt");
	if (NChars != 44)
	{
		printf ("||NChars_test: error: NChars = %d, should be 44\n", NChars);
	}
	else printf ("||NChars_test: OK\n");
	return;
}

//Testing lines counting
void NLines_test ()
{
	char test_text[44];
	int NLines = input ("Test_file.txt", 44, test_text);
	if (NLines != 6)
	{
		printf ("||NLines_test: error: NLines = %d, should be 6\n", NLines);
	}
	else printf ("||NLines_test: OK\n");
	return;
}

//Testing find_strings function
void find_strings_test ()
{
	char *test_start_index[5];
	char *test_end_index[5];
	char *test_original_index[5];
	char test_text[48];
	char starts[5] = {'1', 'q', '[', '[', '['};
	char ends[5] = {'0', 'y', ']', ']', ']'};
	FILE *in = fopen ("Test_file.txt", "r");
	fread (test_text, sizeof (char), 45, in);
	fclose (in);
	find_strings (test_start_index, test_end_index, test_original_index, 45, test_text);
	for (int i = 0; i < 5; i++)
	{
		if (*test_start_index[i] != starts[i])
		{
			printf ("||find_strings_test: error: test_start_index[%d] = '%c', should be '%c'\n", i, test_start_index[i], starts[i]);
			return;
		} 
		if (*test_original_index[i] != starts[i])
		{
			printf ("||find_strings_test: error: test_original_index[%d] = '%c', should be '%c'\n", i, test_original_index[i], starts[i]);
			return;
		} 
		if (*test_end_index[i] != ends[i])
		{
			printf ("||find_strings_test: error: test_end_index[%d] = '%c', should be '%c'\n", i, test_end_index[i], ends[i]);
			return;
		} 
	}
	printf ("||find_strings_test: OK\n");
	return;
}

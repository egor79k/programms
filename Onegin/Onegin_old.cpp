/**
* Program reads text from input file and write it in output file sorted by starts and by ends
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//Testing functions_________________________________
void Write_test_file ();
void count_chars_test ();
void input_test ();
void find_strings_test ();
void start_sort_test ();
void end_sort_test ();
//__________________________________________________

void get_file_name (char *file_name, const char *mode);

int count_chars (const char *file_name);

int input (const char *file_name, const int NChars, char *text);

void find_strings (char **start_index, char **end_index, char **original_index, const int NChars, char *text);

void start_sort (char **start_index, const int NLines, const int NChars);

void end_sort (char **end_index, const int NLines, const int NChars);

char *skip_marks (char *pointer);

void change (char **first, char **second);

void find_starts_by_ends (char **end_index, int NLines, char *begin);

void output (const char *file_name, const int NLines, char **index, char *header);


int main ()
{
	//Testing...
	printf ("\n|Testing:\n");
	Write_test_file ();
	count_chars_test ();
	input_test ();
	find_strings_test ();
	start_sort_test ();
	end_sort_test ();

	//Getting input file name...
	const unsigned char max_name_length = 100;
	char file_name[max_name_length];
	printf ("\nEnter input file name: ");
	get_file_name (file_name, "r");
	
	//Getting information about input file...
	int NChars = 0, NLines = 0;
	NChars = count_chars (file_name) + 2;
	printf("\n|About input file:\n||Number of symbols: %d\n", NChars);
	char *text = (char *) calloc (NChars, sizeof (char));
	NLines = input (file_name, NChars, text);
	printf("||Number of strings: %d\n", NLines);

	//Declaring massives of pointers and filling them...
	char **start_index    = (char **) calloc (NLines, sizeof (int));
	char **end_index      = (char **) calloc (NLines, sizeof (int));
	char **original_index = (char **) calloc (NLines, sizeof (int));
	find_strings (start_index, end_index, original_index, NChars, text);

	//Sorting...
	start_sort (start_index, NLines, NChars);
	end_sort (end_index, NLines, NChars);
	find_starts_by_ends (end_index, NLines, original_index[0] - 1);
	
	//Writing result in pointed file...
	printf ("\nEnter output file name: ");
	get_file_name (file_name, "w");
	output (file_name, NLines, start_index, "-----------\nStarts sort\n-----------\n");
	output (file_name, NLines, end_index, "\n-----------\nEnds sort\n-----------\n");
	output (file_name, NLines, original_index, "\n-----------\nOriginal text\n-----------\n");
}

//Reads filename and checks it
void get_file_name (char *file_name, const char *mode)
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
int input (const char *file_name, const int NChars, char *text)
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
void find_strings (char **start_index, char **end_index, char **original_index, const int NChars, char *text)
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
void start_sort (char **start_index, const int NLines, const int NChars)
{
	assert (start_index);
	assert (NLines != 0);
	assert (NChars != 0);
	for (int i = 0; i < NLines; i++)
	{
		for (int k = 1; k < NLines-i; k++)
		{
			if (strcmp (start_index[k], start_index[k - 1]) < 0) //Sorting strings by starts
			{
				change (&start_index[k], &start_index[k-1]);
			}
		}

	}
	return;
}


void end_sort (char **end_index, const int NLines, const int NChars)
{
	assert (end_index);
	assert (NLines != 0);
	assert (NChars != 0);
	char *end_tmp1 = 0;
	char *end_tmp2 = 0;
	for (int i = 0; i < NLines; i++)
	{
		for (int k = 1; k < NLines-i; k++)
		{
			end_tmp1 = skip_marks (end_index[k]);		//Skipping punctuation marks
			end_tmp2 = skip_marks (end_index[k - 1]);
			while (*end_tmp1 == *end_tmp2)
			{											
				end_tmp1--;
				end_tmp2--;
			}
			if ((int)*end_tmp1 - (int)*end_tmp2 < 0) //Sorting strings by ends
			{
				change (&end_index[k], &end_index[k-1]);
			}
		}

	}
	return;
}


char *skip_marks (char *pointer)
{
	while (*pointer == ',' || *pointer == ';' || *pointer == '.' || *pointer == ':' || *pointer == '!' || *pointer == '?' || *pointer == '"') pointer--;
	return pointer;
}

//Swaps two pointers
void change (char **first, char **second)
{
	char *tmp;
	tmp = *first;
	*first = *second;
	*second = tmp;
	return;
}

//Finds start index of string by its end index
void find_starts_by_ends (char **end_index, int NLines, char *begin)
{
	for (int i = 0; i < NLines; i++)
	{
		while (*end_index[i] != '\0' && end_index[i] != begin)
		{
			end_index[i]--;
		}
		end_index[i]++;
	}
}

//Writes sorted texts in output file
void output (const char *file_name, const int NLines, char **index, char *header)
{
	assert (file_name);
	assert (NLines != 0);
	assert (index);
	FILE *out = fopen (file_name, "a");
	fprintf(out, "%s", header); //Запись строк в алфавитном порядке
	for (int i = 0; i < NLines; i++)
	{
		fprintf(out, "%s\n", index[i]);
	}
	fclose (out);
	return;
}


//Testing functions________________________________________________________________

//Writing file for test functions
void Write_test_file ()
{
	FILE *Test_file = fopen ("Test_file.txt", "w");
	fprintf (Test_file, "1234567890\nqwerty\ntest program,\nonegin!\nasd fg.\nno errors\n");
	fclose (Test_file);
}

//Testing chars counting
void count_chars_test ()
{
	int NChars = count_chars ("Test_file.txt");
	if (NChars != 64)
	{
		printf ("||count_chars_test: error: NChars = %d, should be 64\n", NChars);
		exit (1);
	}
	else printf ("||count_chars_test: %3s\n", "OK");
	return;
}

//Testing lines counting
void input_test ()
{
	char test_text[64];
	int NLines = input ("Test_file.txt", 64, test_text);
	if (NLines != 7)
	{
		printf ("||input_test: error: NLines = %d, should be 7\n", NLines);
		exit (1);
	}
	else printf ("||input_test: %9s\n", "OK");
	return;
}

//Testing find_strings function
void find_strings_test ()
{
	char *test_start_index[6];
	char *test_end_index[6];
	char *test_original_index[6];
	char test_text[64];
	char starts[6] = {'1', 'q', 't', 'o', 'a', 'n'};
	char ends[6] = {'0', 'y', ',', '!', '.', 's'};
	FILE *in = fopen ("Test_file.txt", "r");
	fread (test_text, sizeof (char), 64, in);
	fclose (in);
	find_strings (test_start_index, test_end_index, test_original_index, 64, test_text);
	for (int i = 0; i < 6; i++)
	{
		if (*test_start_index[i] != starts[i])
		{
			printf ("||find_strings_test: error: test_start_index[%d] = '%c', should be '%c'\n", i, test_start_index[i], starts[i]);
			exit (1);
		} 
		if (*test_original_index[i] != starts[i])
		{
			printf ("||find_strings_test: error: test_original_index[%d] = '%c', should be '%c'\n", i, test_original_index[i], starts[i]);
			exit (1);
		} 
		if (*test_end_index[i] != ends[i])
		{
			printf ("||find_strings_test: error: test_end_index[%d] = '%c', should be '%c'\n", i, test_end_index[i], ends[i]);
			exit (1);
		} 
	}
	printf ("||find_strings_test: OK\n");
	return;
}


void start_sort_test ()
{
	char *test_start_index[6];
	char *test_end_index[6];
	char *test_original_index[6];
	char test_text[64];
	char sorted_starts[6] = {'1', 'a', 'n', 'o', 'q', 't'};
	FILE *in = fopen ("Test_file.txt", "r");
	fread (test_text, sizeof (char), 64, in);
	fclose (in);
	find_strings (test_start_index, test_end_index, test_original_index, 64, test_text);
	start_sort (test_start_index, 6, 64);
	for (int i = 0; i < 6; i++)
	{
		if (*test_start_index[i] != sorted_starts[i])
		{
			printf ("||start_sort_test: error: test_start_index[%d] = '%c', should be '%c'\n", i, *test_start_index[i], sorted_starts[i]);
			exit (1);
		}  
	}
	printf ("||start_sort_test: %4s\n", "OK");
	return;
}


void end_sort_test ()
{
	char *test_start_index[6];
	char *test_end_index[6];
	char *test_original_index[6];
	char test_text[64];
	char sorted_ends[6] = {'0', '.', ',', '!', 's', 'y'};
	FILE *in = fopen ("Test_file.txt", "r");
	fread (test_text, sizeof (char), 64, in);
	fclose (in);
	find_strings (test_start_index, test_end_index, test_original_index, 64, test_text);
	end_sort (test_end_index, 6, 64);
	for (int i = 0; i < 6; i++)
	{
		if (*test_end_index[i] != sorted_ends[i])
		{
			printf ("||end_sort_test: error: test_end_index[%d] = '%c', should be '%c'\n", i, *test_end_index[i], sorted_ends[i]);
			exit (1);
		}  
	}
	printf ("||end_sort_test: %6s\n", "OK");
	return;
}

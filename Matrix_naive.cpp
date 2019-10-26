#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *Get_Buff_From_File (char *file_name);

char *Get_Matrix (int *matrix, char *text, int area);

void Mul (int *matrix_1, int *matrix_2, int matrix_size, FILE *out);


int main ()
{
	char *text = Get_Buff_From_File ("input.txt");

	int matrix_size = atoi (text);
	while (*text != '\n') text++;
	text++;

//Getting matrixes
	int area = matrix_size * matrix_size;
	int A[area] = {};
	int B[area] = {};

	text = Get_Matrix (A, text, area);
	Get_Matrix (B, text, area);

	FILE *out = fopen ("output.txt", "w");

//Multiplication
	Mul (A, B, matrix_size, out);
	
	return 0;
}


char *Get_Buff_From_File (char *file_name)
{
	FILE *in = fopen (file_name, "r");
	fseek (in, 0, SEEK_END);
	int NChars = ftell (in);
	fseek (in, 0, SEEK_SET);
	char *text = (char *) calloc (NChars, sizeof (char));
	fread (text, sizeof (char), NChars, in);
	fclose (in);
	return text;
}

//Getting matrix of specified size from buffer
char *Get_Matrix (int *matrix, char *text, int area)
{
	int i = 0, k = 0;
	while (i < area)
	{
		matrix[i++] = atoi (text);
		while (*text != ' ' && *text != '\n' && *text != '\0') text++;
		text++;
	}
	return text;
}


void Mul (int *matrix_1, int *matrix_2, int matrix_size, FILE *out)
{
	for (int i = 0; i < matrix_size; i++)
	{
		for (int j = 0; j < matrix_size; j++)
		{
			int sum = 0;
			for (int r = 0; r < matrix_size; r++)
			{
				sum += matrix_1[i * matrix_size +r] * matrix_2[r * matrix_size +j];
			}
			fprintf(out, "%d ", sum);
		}
		fprintf(out, "\n");
	}
	return;
}
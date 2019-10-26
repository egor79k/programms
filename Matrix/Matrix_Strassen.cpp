#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *Get_Buff_From_File (char *file_name);

char *Get_Matrix (int *matrix, char *text, int matrix_size, int matrix_size_old);

void Split (int *matrix, int *splitted_matrix, int matrix_size, int FROMstring, int FROMcolumn);

void Add (int *A, int *B, int *C, int size);

void Sub (int *A, int *B, int *C, int size);

void Unite (int *united_matrix, int *matrix, int matrix_size, int FROMstring, int FROMcolumn);

void Strassen_mul (int *A, int *B, int *C, int matrix_size);


int main ()
{
	char *text = Get_Buff_From_File ("input.txt");

	int matrix_size = atoi (text);
	while (*text != '\n') text++;
	text++;

	int matrix_size_old = matrix_size;
//Expansion matrix size to make it a power of two (if it wasn't it)
	float num = (float)matrix_size;
	while (num > 1) num /= 2;
	if (num != 1)
	{
		int new_size = 1;
		while (new_size < matrix_size) new_size *= 2;
		matrix_size = new_size;
	}

//Getting matrixes
	int area = matrix_size * matrix_size;
	int A[area] = {};
	int B[area] = {};
	int C[area] = {};

	text = Get_Matrix (A, text, matrix_size, matrix_size_old);
	Get_Matrix (B, text, matrix_size, matrix_size_old);

//Multiplication
	Strassen_mul (A, B, C, matrix_size);

//Printing result matrix in output file
	FILE *out = fopen ("output.txt", "w");

	for (int i = 0; i < matrix_size_old; i++)
	{
		for (int j = 0; j < matrix_size_old; j++)
		{
			fprintf(out, "%d ", C[i * matrix_size + j]);
		}
		fprintf(out, "\n");
	}
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
char *Get_Matrix (int *matrix, char *text, int matrix_size, int matrix_size_old)
{
	int i = 0, k = 0;
	while (k < matrix_size_old)
	{
		matrix[k * matrix_size + i++] = atoi (text);
		while (*text != ' ' && *text != '\n' && *text != '\0') text++;
		if (*text == '\n' || *text == '\0')
		{
			i = 0;
			k++;
		}
		text++;
	}
	return text;
}

//Extraction a quater-part of matrix
void Split (int *matrix, int *splitted_matrix, int matrix_size, int FROMstring, int FROMcolumn)
{
	int half = matrix_size / 2;
	int TOstring = FROMstring + half;
	int TOcolumn = FROMcolumn + half;
	int n = 0;

	for (int i = FROMstring; i < TOstring; i++)
	{
		for (int j = FROMcolumn; j < TOcolumn; j++)
		{
			splitted_matrix[n++] = matrix[i * matrix_size + j];
		}
	}
}

// C = A + B
void Add (int *A, int *B, int *C, int size)
{
	for (int i = 0; i < size; i++) C[i] = A[i] + B[i];
	return;
}

// C = A - B
void Sub (int *A, int *B, int *C, int size)
{
	for (int i = 0; i < size; i++) C[i] = A[i] - B[i];
	return;
}

//Inclusion a quater-part matrix in the full one
void Unite (int *united_matrix, int *matrix, int matrix_size, int FROMstring, int FROMcolumn)
{
	int half = matrix_size / 2;
	int TOstring = FROMstring + half;
	int TOcolumn = FROMcolumn + half;
	int n = 0;

	for (int i = FROMstring; i < TOstring; i++)
	{
		for (int j = FROMcolumn; j < TOcolumn; j++)
		{
			united_matrix[i * matrix_size + j] = matrix[n++];
		}
	}
}

//Strassen matrix multiplication algorithm
void Strassen_mul (int *A, int *B, int *C, int matrix_size)
{
	if (matrix_size <= 1)
	{
		C[0] = A[0] * B[0];
		return;
	}

	int half = matrix_size / 2;
	int quater = half * half;

	int A11[quater] = {};
	int A12[quater] = {};
	int A21[quater] = {};
	int A22[quater] = {};

	Split (A, A11, matrix_size, 0,    0   );
	Split (A, A12, matrix_size, 0,    half);
	Split (A, A21, matrix_size, half, 0   );
	Split (A, A22, matrix_size, half, half);

	int B11[quater] = {};
	int B12[quater] = {};
	int B21[quater] = {};
	int B22[quater] = {};

	Split (B, B11, matrix_size, 0,    0   );
	Split (B, B12, matrix_size, 0,    half);
	Split (B, B21, matrix_size, half, 0   );
	Split (B, B22, matrix_size, half, half);

	int TMP1[quater] = {};
	int TMP2[quater] = {};
	int P1[quater] = {};
	int P2[quater] = {};
	int P3[quater] = {};
	int P4[quater] = {};
	int P5[quater] = {};
	int P6[quater] = {};
	int P7[quater] = {};

	//P1 = (A11 + A22)(B11 + B22)
	Add (A11, A22, TMP1, quater);
	Add (B11, B22, TMP2, quater);
	Strassen_mul (TMP1, TMP2, P1, half);

	//P2 = (A21 + A22)B11
	Add (A21, A22, TMP1, quater);
	Strassen_mul (TMP1, B11, P2, half);

	//P3 = A11(B12 - B22)
	Sub (B12, B22, TMP1, quater);
	Strassen_mul (A11, TMP1, P3, half);

	//P4 = A22(B21 - B11)
	Sub (B21, B11, TMP1, quater);
	Strassen_mul (A22, TMP1, P4, half);

	//P5 = (A11 + A12)B22
	Add (A11, A12, TMP1, quater);
	Strassen_mul (TMP1, B22, P5, half);

	//P6 = (A21 - A11)(B11 + B12)
	Sub (A21, A11, TMP1, quater);
	Add (B11, B12, TMP2, quater);
	Strassen_mul (TMP1, TMP2, P6, half);

	//P7 = (A12 - A22)(B21 + B22)
	Sub (A12, A22, TMP1, quater);
	Add (B21, B22, TMP2, quater);
	Strassen_mul (TMP1, TMP2, P7, half);

	int C11[quater] = {};
	int C12[quater] = {};
	int C21[quater] = {};
	int C22[quater] = {};

	//C11 = P1 + P4 - P5 + P7
	Add (P1, P4, TMP1, quater);
	Sub (TMP1, P5, TMP2, quater);
	Add (TMP2, P7, C11, quater);
	
	//C12 = P3 + P5
	Add (P3, P5, C12, quater);
	
	//C21 = P2 + P4
	Add (P2, P4, C21, quater);
	
	//C22 = P1 - P2 + P3 + P6
	Sub (P1, P2, TMP1, quater);
	Add (TMP1, P3, TMP2, quater);
	Add (TMP2, P6, C22, quater);

	Unite (C, C11, matrix_size, 0,    0   );
	Unite (C, C12, matrix_size, 0,    half);
	Unite (C, C21, matrix_size, half, 0   );
	Unite (C, C22, matrix_size, half, half);
	return;
}
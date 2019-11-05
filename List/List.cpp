#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
#define DEFAULT __FILE__, __LINE__, __FUNCTION__

#ifdef DEBUG
	#define ON_DEBUG(code) code
#else
	#define ON_DEBUG(code) ;
#endif

const int MAX_LIST_LENGTH = 20;
const int MAX_PNG_NAME = 20;


struct list
{
	int data[MAX_LIST_LENGTH] = {};
	int next[MAX_LIST_LENGTH] = {};
	int prev[MAX_LIST_LENGTH] = {};
	int head = 0;
	int tail = 0;
	int free_elem = 1;
	char *name = "";
};


void ListConstruct (list *lst, char *name);
int InsertAfter (list *lst, int prev_num, int value);
int InsertBefore (list *lst, int next_num, int value);
bool Delete (list *lst, int num);
void Dump (list *lst, const char *reason = "Dump", const char *file = "Not pointed file", const int line = -1, const char *function = "Not pointed function");
void GraphDump (list *lst, char png_file[MAX_PNG_NAME]);
int ListOk (list *lst);
void CheckList (list *lst);



int main ()
{
	list lst1 = {};
	ListConstruct (&lst1, "lst1");
	
	InsertBefore (&lst1, 0, 35);
	InsertAfter (&lst1, 4, 35);
	InsertAfter (&lst1, 0, 65);
	InsertBefore (&lst1, 1, 55);
	InsertAfter (&lst1, 2, 60);
	InsertAfter (&lst1, 1, 75);
	Delete (&lst1, 2);
	InsertAfter (&lst1, 0, 35);
	InsertBefore (&lst1, 3, 55);
	GraphDump (&lst1, "out.png");
}


void ListConstruct (list *lst, char *name)
{
	for (int i = 1; i < MAX_LIST_LENGTH; i++)
	{
		lst->next[i] = i + 1;
	}

	for (int i = 1; i < MAX_LIST_LENGTH; i++)
	{
		lst->prev[i] = -1;
	}
	lst->name = name;

	ON_DEBUG (CheckList (lst);)
	return;
}


int InsertAfter (list *lst, int prev_num, int value)
{
	int new_num = lst->free_elem;
	if (prev_num == 0 && lst->head == 0) lst->head = new_num;
	else if (prev_num == 0 && lst->head != 0) return 0;
	else if (prev_num != 0 && lst->head == 0) return 0;
	lst->free_elem = lst->next[lst->free_elem];
	lst->data[new_num] = value;
	lst->next[new_num] = lst->next[prev_num];
	lst->prev[new_num] = prev_num;
	if (lst->next[new_num] != 0) lst->prev[lst->next[new_num]] = new_num;
	else lst->tail = new_num;
	if (prev_num != 0) lst->next[prev_num] = new_num;

	ON_DEBUG (CheckList (lst);)
	return new_num;
}


int InsertBefore (list *lst, int next_num, int value)
{
	if (next_num == 0) return 0;
	int new_num = lst->free_elem;
	lst->free_elem = lst->next[lst->free_elem];
	lst->data[new_num] = value;
	lst->next[new_num] = next_num;
	lst->prev[new_num] = lst->prev[next_num];
	if (lst->prev[next_num] != 0) lst->next[lst->prev[next_num]] = new_num;
	else lst->head = new_num;
	lst->prev[next_num] = new_num;

	ON_DEBUG (CheckList (lst);)
	return new_num;
}


bool Delete	(list *lst, int num)
{
	if (num == lst->head) lst->head = lst->next[num];
	if (lst->prev[num] != 0) lst->next[lst->prev[num]] = lst->next[num];
	if (lst->prev[lst->next[num]] != 0) lst->prev[lst->next[num]] = lst->prev[num];
	lst->next[num] = lst->free_elem;
	lst->free_elem = num;
	lst->data[num] = 0;
	lst->prev[num] = -1;

	ON_DEBUG (CheckList (lst);)
	return true;
}


void Dump (list *lst, const char *reason, const char *file, const int line, const char *function)
{
	int i = 0;
	printf("\n============================================================\n");
	printf("%s from %s (%d) %s()", reason, file, line, function);

	printf ("\n%c", 218);
	for (i = 0; i < MAX_LIST_LENGTH * 5 + 6; i++)
	{
		putchar (196);
	}

	printf ("%c\n%cnum:  ", 191, 179);
	for (i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("%5d", i);
	}

	printf ("%c\n%c", 179, 195);
	for (i = 0; i < MAX_LIST_LENGTH * 5 + 6; i++)
	{
		putchar (196);
	}

	printf ("%c\n%cdata: ", 180, 179);
	for (i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("%5d", lst->data[i]);
	}

	printf ("%c\n%cnext: ", 179, 179);
	for (i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("%5d", lst->next[i]);
	}

	printf ("%c\n%cprev: ", 179, 179);
	for (i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("%5d", lst->prev[i]);
	}

	printf("%c\n%c", 179, 192);
	for (i = 0; i < MAX_LIST_LENGTH * 5 + 6; i++)
	{
		putchar (196);
	}
	printf("%c\n|head: %d", 217, lst->head);
	printf("\n|free: %d\n", lst->free_elem);
	printf("============================================================\n");
	return;
}


void GraphDump (list *lst, char png_file[MAX_PNG_NAME])
{
	int i = lst->head;
	int elem = 0;
	FILE *out = fopen ("list.dot", "w");

	fprintf (out, "digraph {\nedge[color = \"blue\"];\nnode[color = \"green4\", shape = record];");

	while (i != 0)
	{
		fprintf (out, "%d[label = \"{%d | {prev\\n%d | num\\n%d | next\\n%d}}\"];\n", elem, lst->data[i], lst->prev[i], i, lst->next[i]);
		i = lst->next[i];
		elem++;
	}

	fprintf(out, "name[shape = doubleoctagon, label = %s, color = \"goldenrod\", fontsize = 20];\n", lst->name);
	fprintf(out, "params[shape = record, label = \" head\\n%d | free_elem\\n%d | tail\\n%d \", color = \"goldenrod\"];\n", lst->head, lst->free_elem, lst->tail);
	fprintf (out, "name->params->0[style = \"dashed\", arrowhead = \"none\"];\n");
	for (i = 0; i < elem - 1; i++)
	{
		fprintf(out, "%d->", i);
	}
	fprintf (out, "%d;\n", i);

	fprintf (out, "}");

	fclose (out);

	char command[53] = "dot list.dot -T png -o ";
	strcpy (&command[23], png_file);
	system (command);
	return;
}

//Errors: 0 - No errors, 1 - Empty value in prev in full element, 2 - List order is broken 
int ListOk (list *lst)
{
	int i = lst->head;
	while (lst->next[i] != 0)
	{
		if (lst->prev[i] < 0) return 1;
		i = lst->next[i];
	}
	if (lst->prev[i] < 0) return 1;
	if (i != lst->tail) return 3;
	while (lst->prev[i] > 0) i = lst->prev[i];
	if (i != lst->head) return 2;
	return 0;
}


void CheckList (list *lst)
{
	int err = ListOk (lst);
	if (err != 0)
	{
		switch (err)
		{
			case 1:
				printf("Error: Empty value of previous of used element\n");
				break;
			case 2:
				printf("Error: List order is broken");
				break;
			case 3:
				printf("Error: Wrong pointed tail\n");
				break;
		}
		Dump (lst, "Error", DEFAULT);
	}
}

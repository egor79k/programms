#include <stdio.h>

const int MAX_LIST_LENGTH = 20;


struct list
{
	int data[MAX_LIST_LENGTH] = {};
	int next[MAX_LIST_LENGTH] = {};
	int prev[MAX_LIST_LENGTH] = {};
	int head = 0;
	int free_elem = 1;
	char *name = "";
};


void ListConstruct (list *lst, char *name);
int InsertAfter (list *lst, int prev_num, int value);
int InsertBefore (list *lst, int next_num, int value);
bool Delete (list *lst, int num);
void Dump (list *lst);
int ListOk (list *lst);



int main ()
{
	list lst1 = {};
	ListOk (&lst1);
	ListConstruct (&lst1, "lst1");
	Dump (&lst1);
	ListOk (&lst1);
	int n =InsertBefore (&lst1, 0, 35);
	Dump (&lst1);
	printf("befn = %d\n", n);
	ListOk (&lst1);
	n = InsertAfter (&lst1, 4, 35);
	Dump (&lst1);
	ListOk (&lst1);
	printf("afn = %d\n", n);
	InsertAfter (&lst1, 0, 65);
	Dump (&lst1);
	InsertBefore (&lst1, 1, 55);
	Dump (&lst1);
	ListOk (&lst1);
	InsertAfter (&lst1, 2, 60);
	Dump (&lst1);
	InsertAfter (&lst1, 1, 75);
	Dump (&lst1);
	ListOk (&lst1);
	//InsertBefore (1, 60);
	//Dump ();
	Delete (&lst1, 2);
	Dump (&lst1);
	n = InsertAfter (&lst1, 0, 35);
	Dump (&lst1);
	printf("afn = %d\n", n);
	InsertBefore (&lst1, 3, 55);
	Dump (&lst1);
	ListOk (&lst1);
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
	if (lst->prev[lst->next[new_num]] != 0) lst->prev[lst->next[new_num]] = new_num;
	if (prev_num != 0) lst->next[prev_num] = new_num;
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
	return new_num;
}


bool Delete	(list *lst, int num)
{
	if (lst->prev[num] != 0) lst->next[lst->prev[num]] = lst->next[num];
	if (lst->prev[lst->next[num]] != 0) lst->prev[lst->next[num]] = lst->prev[num];
	lst->next[num] = lst->free_elem;
	lst->free_elem = num;
	lst->data[num] = 0;
	lst->prev[num] = -1;
	return true;
}


void Dump (list *lst)
{
	printf ("\nDump from %s", lst->name);
	printf ("\n_______");
	for (int i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("____");
	}
	printf ("\n|num:  ");
	for (int i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("%4d", i);
	}
	printf ("\n|------");
	for (int i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("----");
	}
	printf ("\n|data: ");
	for (int i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("%4d", lst->data[i]);
	}
	printf ("\n|next: ");
	for (int i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("%4d", lst->next[i]);
	}
	printf ("\n|prev: ");
	for (int i = 0; i < MAX_LIST_LENGTH; i++)
	{
		printf ("%4d", lst->prev[i]);
	}
	printf("\n|\n|head: %d", lst->head);
	printf("\n|free: %d\n", lst->free_elem);
}

int ListOk (list *lst)
{
	int i = lst->head;
	while (lst->next[i] != 0)
	{
		if (lst->prev[i] < 0) return 1;
		i = lst->next[i];
	}
	if (lst->prev[i] < 0) return 1;
	while (lst->prev[i] > 0) i = lst->prev[i];
	if (i != lst->head) printf("Error! i = %d must be %d\n", i, lst->head);
	else printf("List Ok!\n");
	return 0;
}
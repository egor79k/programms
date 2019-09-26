#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 0
#define DEAFULT __FILE__, __LINE__, __FUNCTION__
#define ASSERT_OK 														\
if (StackOK (stk) == false)												\
{																		\
	Dump (stk, "Assertion error", DEAFULT);	\
}																		\


typedef int Elem_t;

const int RESIZE_VALUE = 5;
const int HYSTERESIS = 2;
const int FREE_PLACE = 0;

struct stack_t
{
	Elem_t *data;
	int full_size;
	int size;
	int err;
#ifdef DEBUG
	char *name;
#endif
};

//Work with stack:
bool StackConstruct (stack_t *stk, const int start_size = 1);
bool StackPush (stack_t *stk, Elem_t value);
Elem_t StackPop (stack_t *stk);
//Check stack:
void Dump (stack_t *stk, const char *reason = "Dump", const char *file = "Not pointed file", const int line = -1, const char *function = "Not pointed function");
bool StackOK (stack_t *stk);


bool StackConstruct (stack_t *stk, const int start_size)
{
	stk->data = (Elem_t *) calloc (start_size, sizeof (Elem_t));
	stk->full_size = start_size;
	stk->size = 0;
	stk->err = 0;
	return true;
}


bool StackResize (stack_t *stk, int resize_val)
{
	stk->full_size = stk->full_size + resize_val;
	stk->data = (Elem_t *) realloc (stk->data, stk->full_size * sizeof (Elem_t));
	if (resize_val > 0)
	{
		for (int i = stk->full_size - resize_val; i < stk->full_size; i++)
		{
			stk->data[i] = FREE_PLACE;
		}
	}
	ASSERT_OK
	return true;
}


bool StackPush (stack_t *stk, Elem_t value)
{
	ASSERT_OK
	if (stk->size < stk->full_size)
	{
		stk->data[stk->size++] = value;
	}
	else
	{
		StackResize (stk, RESIZE_VALUE);
		stk->data[stk->size++] = value;
	}
	ASSERT_OK
	return true;
}

//Errors: 0 - No errors, 1 - Stack Owerflow, 2 - Stack Underflow,
Elem_t StackPop (stack_t *stk)
{
	ASSERT_OK
	if (stk->size == 0)
	{
		stk->err = 2;
		ASSERT_OK
		return stk->data[stk->size];
	}

	Elem_t value = stk->data[--stk->size];
	stk->data[stk->size] = FREE_PLACE;
	ASSERT_OK

	if (stk->size < stk->full_size - RESIZE_VALUE - HYSTERESIS && stk->full_size > RESIZE_VALUE)
	{
		StackResize (stk, -RESIZE_VALUE);
	}
	return value;
}


void Dump (stack_t *stk, const char *reason, const char *file, const int line, const char *function)
{
	printf("\n==============================\n");

	printf("%s from %s (%d) %s()\n\n|Stack_t \"%s\" [%p] ", reason, file, line, function, stk->name, stk);
	if (StackOK (stk) == true) printf("(Ok)\n");
	else printf("(Error)\n");
	printf("||size = %d\n||data[%d] = [%p]\n\n", stk->size, stk->full_size, &stk->data[stk->full_size]);

	for (int i = 0; i < stk->size; i++)
	{
		printf ("*[%d] = %d", i, stk->data[i]);
		if (stk->data[i] == -666) printf(" [poison]");
		printf("\n");
	}

	for (int i = stk->size; i < stk->full_size; i++) printf (" [%d] = %d\n", i, stk->data[i]);

	if (stk->err == 0) printf ("\nerror = %d %s\n", stk->err, "(Ok)");
	else if (stk->err == 1) printf("\nerror = %d %s\n", stk->err, "(Stack Owerflow)");
	else printf("\nerror = %d %s\n", stk->err, "(Stack Underflow)");

	printf("==============================\n");
	return;
}


bool StackOK (stack_t *stk)
{
	if (stk == nullptr) return false;
	if (stk->size > stk->full_size || stk->size < 0) return false;
	if (stk->data == nullptr) return false;
	if (stk->err != 0) return false;
	return true;
}
#include "Stack_Dynamic.h"

bool StackConstruct (stack_t *stk, const int start_size)
{
	stk->data = (Elem_t *) calloc (start_size, sizeof (Elem_t));
	stk->full_size = start_size;
	stk->size = 0;
	stk->err = 0;
	stk->name = "Unnamed stack";
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
	ON_DEBUG(CheckHash (stk, PushHash, value);)
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

	if (stk->size < stk->full_size - RESIZE_VALUE - HYSTERESIS && stk->full_size > RESIZE_VALUE)
	{
		StackResize (stk, -RESIZE_VALUE);
	}
	ON_DEBUG(CheckHash (stk, PopHash, value);)
	ASSERT_OK
	return value;
}


bool StackDestruct (stack_t *stk)
{
	ASSERT_OK
	stk->size = NULL;
	stk->full_size = NULL;
  	free (stk->data);

#ifdef DEBUG
	stk->name = nullptr;
#endif

  return true;
}


void Dump (stack_t *stk, const char *reason, const char *file, const int line, const char *function)
{
	printf("\n==============================\n");

	printf("%s from %s (%d) %s()\n\n|Stack_t \"%s\" [%p] ", reason, file, line, function, stk->name, stk);
	if (StackOK (stk) == true) printf("(Ok)\n");
	else printf("(Error)\n");
	printf("||size = %d\n||data[%d] = [%p]\n\n", stk->size, stk->full_size, &stk->data[stk->full_size]);

	for (int i = 0; i < stk->size; i++)	printf ("*[%d] = %d\n", i, stk->data[i]);

	for (int i = stk->size; i < stk->full_size; i++) printf (" [%d] = %d\n", i, stk->data[i]);

	if (stk->err == 0) printf ("\nerror = %d %s\n", stk->err, "(Ok)");
	else if (stk->err == 1) printf("\nerror = %d %s\n", stk->err, "(Stack Owerflow)");
	else if (stk->err == 2) printf("\nerror = %d %s\n", stk->err, "(Stack Underflow)");
	else printf("\nerror = %d %s\n", stk->err, "(Stack Attacked)");

	ON_DEBUG(printf("Hash = %d\n", stk->hash);)

	printf("==============================\n");
	return;
}


bool StackOK (stack_t *stk)
{
	if (stk == nullptr) return false;
	if (stk->size > stk->full_size) stk->err = 1; 
	if (stk->size < 0) stk->err = 2;
	if (stk->data == nullptr) return false;
	if (stk->err != 0) return false;
	ON_DEBUG(if (stk->Canary1 != Canary1_value) {stk->err = 3; return false;})
	ON_DEBUG(if (stk->Canary2 != Canary2_value) {stk->err = 3; return false;})
	for (int i = stk->size; i < stk->full_size; i++)
	{
		if (stk->data[i] != FREE_PLACE) return false;
	}
	return true;
}


bool CheckHash (stack_t *stk, int (*HashType) (stack_t *stk, Elem_t value), Elem_t value)
{
	int hash = 0;
	for (int i = 0; i < stk->size; i++)
	{
		hash = ~(hash ^ stk->data[i]);
	}
	if (HashType (stk, value) == hash)
	{
		stk->hash = hash;
		return true;
	}
	else 
	{
		stk->hash = hash;
		stk->err = 3;
		return false;
	}
}


int PushHash (stack_t *stk, Elem_t value)
{
	return ~(stk->hash ^ (int) value);
}


int PopHash (stack_t *stk, Elem_t value)
{
	return ~stk->hash ^ (int) value;
}

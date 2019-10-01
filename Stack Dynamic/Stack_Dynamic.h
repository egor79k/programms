#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 0
#define DEFAULT __FILE__, __LINE__, __FUNCTION__

#ifdef DEBUG
	#define ON_DEBUG(code) code
	#define ASSERT_OK 														\
	if (StackOK (stk) == false)												\
	{																		\
		Dump (stk, "Assertion error", DEFAULT);								\
	}																		
#else
	#define ON_DEBUG(code) ;
	#define ASSERT_OK ;
#endif

typedef int Elem_t;

const int RESIZE_VALUE = 5;
const int HYSTERESIS = 2;
const int FREE_PLACE = 0;
ON_DEBUG(const int Canary1_value = 1234567890;)
ON_DEBUG(const int Canary2_value = 1876543290;)


struct stack_t
{
	ON_DEBUG(int Canary1 = Canary1_value;)
	Elem_t *data;
	int full_size;
	int size;
	int err;
	const char *name;
	ON_DEBUG(int hash;)
	ON_DEBUG(int Canary2 = Canary2_value;)
};

//Work with stack:
bool StackConstruct (stack_t *stk, const int start_size = 1);
bool StackPush (stack_t *stk, Elem_t value);
Elem_t StackPop (stack_t *stk);
bool StackDestruct (stack_t *stk);
//Check stack:
void Dump (stack_t *stk, const char *reason = "Dump", const char *file = "Not pointed file", const int line = -1, const char *function = "Not pointed function");
bool StackOK (stack_t *stk);
bool CheckHash (stack_t *stk, int (*HashType) (stack_t *stk, Elem_t value), Elem_t value);
int PushHash (stack_t *stk, Elem_t value);
int PopHash (stack_t *stk, Elem_t value);


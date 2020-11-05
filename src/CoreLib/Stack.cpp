#include <CoreLib/Stack.h>

Stack LT_StackCreate(uint64 size, uint64 typeSize) {
  Stack stack = {.array = LT_ArrayCreate(size, typeSize),
                 .elements_count = 0,
                 .isEmpty = LT_TRUE};

  return stack;
}

void LT_StackDestroy(Stack *stack) { LT_ArrayDestroy(stack); }

void *LT_StackPop(Stack *stack) {
  if (!stack->elements_count)
    stack->isEmpty = LT_TRUE;
  else
    stack->elements_count--;
  return LT_ArrayGetElement(stack, stack->elements_count);
}

void *LT_StackTop(Stack *stack) {
  return LT_ArrayGetElement(stack, stack->elements_count - 1);
}

void LT_StackPush(Stack *stack, void *element) {
  LT_ArraySetElement(stack, stack->elements_count, element);
  stack->elements_count++;
  stack->isEmpty = LT_FALSE;
}

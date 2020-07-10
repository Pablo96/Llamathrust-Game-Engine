static void TestArray() {
  Array array = LT_ArrayCreate(8 * 10, 8);
  
  uint64 count = LT_ArrayCount(&array);
  log_info("Count %u", count);
  
  for (uint64 i = 0; i < count; i++) {
    uint64 element = 9 * i;
    LT_ArraySetElement(&array, i, &element);
  }

  for (uint64 i = 0; i < count; i++) {
    uint64 element = *(uint64*) LT_ArrayGetElement(&array, i);
    log_info("number: %u", element);
  }

  LT_ArrayDestroy(&array);
}


static void TestArrayOutRange() {
  Array array = LT_ArrayCreate(8 * 10, 8);
  uint64 count = LT_ArrayCount(&array);
  
  for (uint64 i = count; i < count + 5; i++) {
    uint64 element = 9 * i;
    LT_ArraySetElement(&array, i, &element);
  }

  LT_ArrayDestroy(&array);
}

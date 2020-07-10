#include "test_framework.h"
#include "CoreLib.h"

int main(int argc, const char** argv) {
    LT_ADD_TEST(TestArray);
    LT_ADD_TEST(TestArrayOutRange);

    LT_TestRun();

    return 0;
}
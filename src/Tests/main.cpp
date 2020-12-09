#include <ErrorCodes.hpp>

#include "CoreLib.hpp"
#include "Networking.hpp"
#include "Threading.hpp"
#include "framework/test_framework.hpp"

int main(int argc, const char **argv) {
  log_set_level(LOG_ERROR);

  LT_ADD_TEST(TestArrayCount, TEST_SUCCESS);
  LT_ADD_TEST(TestArray, TEST_SUCCESS);
  LT_ADD_TEST(TestArrayOutRange,
              SET_EXPECTED_RESULT(ERROR_INDEX_OUT_OF_BOUNDS));

#ifdef LT_WINDOWS
  LT_ADD_TEST(TestStack, TEST_SUCCESS);
#endif
  LT_ADD_TEST(TestQueue, TEST_SUCCESS);

#ifdef LT_WINDOWS
  LT_ADD_TEST(TestThreadSpawn, TEST_SUCCESS);
  LT_ADD_TEST(TestThreadExitCode, TEST_SUCCESS);
  LT_ADD_TEST(TestThreadLock, TEST_SUCCESS);
  LT_ADD_TEST(TestThreadPool, TEST_SUCCESS);

  LT_ADD_TEST(TestNetworking, TEST_SUCCESS);
#endif

  return LT_TestRun();
}

#include "test_framework.h"
#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


typedef struct _TestNode {
  int (*func)(void*);
  const char* name;
  struct _TestNode* next;
} TestNode;

TestNode* test_list = NULL;
uint64 test_count = 0; 

static void CreateNode(TestNode* in_mem, unsigned long (*func)(void*), const char* testName) {
    TestNode test = {
        .func = func,
        .name = testName,
        .next = NULL
    };
    memcpy(in_mem, &test, sizeof(TestNode));
    test_count++;
}

void __TestAdd(unsigned long (*func)(void*), const char* testName) {
    TestNode* mem;
    if (test_list != NULL) {
        TestNode* current = test_list;
        while (current->next != NULL) {
            current = current->next;
        }

        mem = current->next = (TestNode*) malloc(sizeof(TestNode));
    } else {
        mem = test_list = (TestNode*) malloc(sizeof(TestNode));
    }

    CreateNode(mem, func, testName);
}

static Prepare(TestNode* list) {
    TestNode* current = test_list;
    for (uint64 i = 0; current != NULL; i++) {
        memcpy(&list[i], current, sizeof(TestNode));
        current = current->next;
    }

    return list;
}

void LT_TestRun() {
    log_log(LOG_INFO,  "LT TESTS", 0, "Preparing %u Tests", test_count);

    TestNode list[test_count];
    Prepare(list);
    HANDLE threads[test_count];

    log_log(LOG_INFO,  "LT TESTS", 0, "Spawning threads");
    // Create a thread for every test
    for (uint64 i = 0; i < test_count; i++) {
        threads[i] = CreateThread(
            NULL,
            0,
            list[i].func,
            NULL,
            CREATE_SUSPENDED,
            NULL
        );
    }

    // Execute tests
    for (uint64 i = 0; i < test_count; i++) {
        const char* name =  list[i].name;
        log_log(LOG_INFO,  "LT TESTS", 0, "Running test %s", name);

        HANDLE thread = threads[i];
        
        ResumeThread(thread);
        
        // Wait for test to finish
        WaitForSingleObject(thread, INFINITE);

        DWORD exitCode;
        GetExitCodeThread(thread, &exitCode);   

        log_log(LOG_INFO,  "LT TESTS", 0, "TEST '%s' %s", list[i].name, (exitCode == TEST_SUCCESS) ? "SUCCESS" : "FAILED");
    }
}

/**
 * @file thread.h
 * @author Pablo Narvaja
 * @brief Here is everything related to threading
 **/
#pragma once
#include <Common.h>

#define MAX_RESERVED_SIZE 8 * 2

#ifdef LT_DEBUG
#define ASSERT_RESERVED_SIZE(size) \
if (size > MAX_RESERVED_SIZE) {\
log_fatal("Size of platform object(%u) is greater than MAX_SIZE(%u)", size, MAX_RESERVED_SIZE);\
exit(36);}
#else
#define ASSERT_RESERVED_SIZE(size)
#endif


typedef uint64 (*ThreadFuncWrapper)(void* name);

/**
 * @struct Thread
 * @brief represents a thread within the engine.
 * @note Called by the platform layer only!!
 * @field reserved:
 *	@type byte pointer
 *	@brief reserved for platform representation.
 * @field name:
 *	@type const char pointer
 *	@brief optional name for the thread.
 * @field ID
 *  @type const uint64
 *  @brief the ID of this thread.
 * @field reservedSize
 *  @type const uint64
 *  @brief the size used by the platform representation
 **/
typedef struct _Thread {
    byte reserved[MAX_RESERVED_SIZE];
    const char* name;
    const uint64 ID;
    const uint16 reservedSize;
} Thread;


/**
 * @func ConstructThread
 * @brief Create a new thread on the heap.
 * @param platformObj:
 *	@type const void pointer
 *	@brief the pointer to the thread object must not be null
 *  @note data will be copied to the thread obj.
 * @param size:
 *	@type const uint64
 *	@brief size of the platform obj.
 * @return Thread pointer
 *  @brief pointer to the created thread obj.
 **/
extern Thread* ConstructThread(const void* platformObj, const uint64 size, const char* name);

extern Thread* LT_Thread_Create(ThreadFuncWrapper func, const char* name);
extern void LT_Thread_Run(void);
extern void LT_Thread_Join(void);
extern void LT_Thread_Delete(void);

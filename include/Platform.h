#ifdef _WIN64
    #define LT_WINDOWS
#elif defined(_WIN32)
    #error "Windows 32bit is not supported!"
#elif defined(__ANDROID__)
    #define LT_ANDROID
#elif defined(__linux__)
    #define LT_LINUX
#elif defined(__APPLE__)
     #error "APPLE is not supported!"
#endif

// Creates a window (width, height, title)
static void (*create_window)(int, int, const char*);

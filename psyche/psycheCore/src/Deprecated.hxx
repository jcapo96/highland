#ifndef deprecated_hxx
#define deprecated_hxx

#ifdef __GNUC__
#define DEPRECATED_MUTE(func) func __attribute__ ((deprecated))
#define DEPRECATED_VERBOSE(func, message) func __attribute__ ((deprecated(message)))
#elif defined(_MSC_VER)
#define DEPRECATED_MUTE(func) __declspec(deprecated) func
#define DEPRECATED_VERBOSE(func, message) __declspec(deprecated(message)) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED_MUTE(func) func
#define DEPRECATED_VERBOSE(func, message) func
#endif

#define GET_DEPRECATED_MACRO(_1, _2, NAME, ...)  NAME
#define DEPRECATED(...) GET_DEPRECATED_MACRO(__VA_ARGS__, DEPRECATED_VERBOSE,\
    DEPRECATED_MUTE)(__VA_ARGS__)



#endif

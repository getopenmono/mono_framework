
#ifndef deprecated_h
#define deprecated_h

#if defined(__GNUC__) && !defined(__clang__)
#undef __DEPRECATED
#define __DEPRECATED(msg,replace) __attribute__((deprecated))

#elif defined(_MSC_VER)
#define __DEPRECATED(msg,replace) __declspec(deprecated)

#elif defined(__clang__)
#undef __DEPRECATED
#define __DEPRECATED(msg,replace) __attribute__((deprecated((msg), (replace))))

#else
#pragma message("WARNING: deprecated.h need to implement DEPRECATED for this compiler")
#define __DEPRECATED void
#endif

#endif /* deprecated_h */

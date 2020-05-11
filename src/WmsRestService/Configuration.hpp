// WmsRestService (info@francescodirienzo.it)
// Copyright Francesco Di Rienzo 2020
// MIT License

#pragma once

#if defined(_MSC_VER)
#define WMSRESTSERVICE_HAS_INT64 1
#else
#define WMSRESTSERVICE_HAS_INT64 0
#endif

#if __cplusplus >= 201103L
#define WMSRESTSERVICE_HAS_LONG_LONG 1
#define WMSRESTSERVICE_HAS_NULLPTR 1
#define WMSRESTSERVICE_HAS_RVALUE_REFERENCES 1
#else
#define WMSRESTSERVICE_HAS_LONG_LONG 0
#define WMSRESTSERVICE_HAS_NULLPTR 0
#define WMSRESTSERVICE_HAS_RVALUE_REFERENCES 0
#endif

// Small or big machine?
#ifndef WMSRESTSERVICE_EMBEDDED_MODE
#if defined(ARDUINO)                /* Arduino*/                 \
    || defined(__IAR_SYSTEMS_ICC__) /* IAR Embedded Workbench */ \
    || defined(__XC)                /* MPLAB XC compiler */      \
    || defined(__ARMCC_VERSION)     /* Keil ARM Compiler */      \
    || defined(__AVR)               /* Atmel AVR8/GNU C Compiler */
#define WMSRESTSERVICE_EMBEDDED_MODE 1
#else
#define WMSRESTSERVICE_EMBEDDED_MODE 0
#endif
#endif

// Auto enable std::stream if the right headers are here and no conflicting
// macro is defined
#if !defined(WMSRESTSERVICE_ENABLE_STD_STREAM) && defined(__has_include)
#if __has_include(<istream>) && \
    __has_include(<ostream>) && \
    !defined(min) && \
    !defined(max)
#define WMSRESTSERVICE_ENABLE_STD_STREAM 1
#else
#define WMSRESTSERVICE_ENABLE_STD_STREAM 0
#endif
#endif

// Auto enable std::string if the right header is here and no conflicting
// macro is defined
#if !defined(WMSRESTSERVICE_ENABLE_STD_STRING) && defined(__has_include)
#if __has_include(<string>) && !defined(min) && !defined(max)
#define WMSRESTSERVICE_ENABLE_STD_STRING 1
#else
#define WMSRESTSERVICE_ENABLE_STD_STRING 0
#endif
#endif

#if WMSRESTSERVICE_EMBEDDED_MODE

// Store floats by default to reduce the memory usage (issue #134)
#ifndef WMSRESTSERVICE_USE_DOUBLE
#define WMSRESTSERVICE_USE_DOUBLE 0
#endif

// Store longs by default, because they usually match the size of a float.
#ifndef WMSRESTSERVICE_USE_LONG_LONG
#define WMSRESTSERVICE_USE_LONG_LONG 0
#endif

// Embedded systems usually don't have std::string
#ifndef WMSRESTSERVICE_ENABLE_STD_STRING
#define WMSRESTSERVICE_ENABLE_STD_STRING 0
#endif

// Embedded systems usually don't have std::stream
#ifndef WMSRESTSERVICE_ENABLE_STD_STREAM
#define WMSRESTSERVICE_ENABLE_STD_STREAM 0
#endif

// Limit nesting as the stack is likely to be small
#ifndef WMSRESTSERVICE_DEFAULT_NESTING_LIMIT
#define WMSRESTSERVICE_DEFAULT_NESTING_LIMIT 10
#endif

#else  // WMSRESTSERVICE_EMBEDDED_MODE

// On a computer we have plenty of memory so we can use doubles
#ifndef WMSRESTSERVICE_USE_DOUBLE
#define WMSRESTSERVICE_USE_DOUBLE 1
#endif

// Use long long when available
#ifndef WMSRESTSERVICE_USE_LONG_LONG
#if WMSRESTSERVICE_HAS_LONG_LONG || WMSRESTSERVICE_HAS_INT64
#define WMSRESTSERVICE_USE_LONG_LONG 1
#else
#define WMSRESTSERVICE_USE_LONG_LONG 0
#endif
#endif

// On a computer, we can use std::string
#ifndef WMSRESTSERVICE_ENABLE_STD_STRING
#define WMSRESTSERVICE_ENABLE_STD_STRING 1
#endif

// On a computer, we can assume std::stream
#ifndef WMSRESTSERVICE_ENABLE_STD_STREAM
#define WMSRESTSERVICE_ENABLE_STD_STREAM 1
#endif

// On a computer, the stack is large so we can increase nesting limit
#ifndef WMSRESTSERVICE_DEFAULT_NESTING_LIMIT
#define WMSRESTSERVICE_DEFAULT_NESTING_LIMIT 50
#endif

#endif  // WMSRESTSERVICE_EMBEDDED_MODE

#ifdef ARDUINO

#include <Arduino.h>

// Enable support for Arduino's String class
#ifndef WMSRESTSERVICE_ENABLE_ARDUINO_STRING
#define WMSRESTSERVICE_ENABLE_ARDUINO_STRING 1
#endif

// Enable support for Arduino's Stream class
#ifndef WMSRESTSERVICE_ENABLE_ARDUINO_STREAM
#define WMSRESTSERVICE_ENABLE_ARDUINO_STREAM 1
#endif

// Enable support for Arduino's Print class
#ifndef WMSRESTSERVICE_ENABLE_ARDUINO_PRINT
#define WMSRESTSERVICE_ENABLE_ARDUINO_PRINT 1
#endif

#else  // ARDUINO

// Disable support for Arduino's String class
#ifndef WMSRESTSERVICE_ENABLE_ARDUINO_STRING
#define WMSRESTSERVICE_ENABLE_ARDUINO_STRING 0
#endif

// Disable support for Arduino's Stream class
#ifndef WMSRESTSERVICE_ENABLE_ARDUINO_STREAM
#define WMSRESTSERVICE_ENABLE_ARDUINO_STREAM 0
#endif

// Disable support for Arduino's Print class
#ifndef WMSRESTSERVICE_ENABLE_ARDUINO_PRINT
#define WMSRESTSERVICE_ENABLE_ARDUINO_PRINT 0
#endif

#endif  // ARDUINO

#ifndef WMSRESTSERVICE_ENABLE_PROGMEM
#ifdef PROGMEM
#define WMSRESTSERVICE_ENABLE_PROGMEM 1
#else
#define WMSRESTSERVICE_ENABLE_PROGMEM 0
#endif
#endif

// Convert unicode escape sequence (\u0123) to UTF-8
#ifndef WMSRESTSERVICE_DECODE_UNICODE
#define WMSRESTSERVICE_DECODE_UNICODE 0
#endif

// Ignore comments in input
#ifndef WMSRESTSERVICE_ENABLE_COMMENTS
#define WMSRESTSERVICE_ENABLE_COMMENTS 0
#endif

// Support NaN in JSON
#ifndef WMSRESTSERVICE_ENABLE_NAN
#define WMSRESTSERVICE_ENABLE_NAN 0
#endif

// Support Infinity in JSON
#ifndef WMSRESTSERVICE_ENABLE_INFINITY
#define WMSRESTSERVICE_ENABLE_INFINITY 0
#endif

// Control the exponentiation threshold for big numbers
// CAUTION: cannot be more that 1e9 !!!!
#ifndef WMSRESTSERVICE_POSITIVE_EXPONENTIATION_THRESHOLD
#define WMSRESTSERVICE_POSITIVE_EXPONENTIATION_THRESHOLD 1e7
#endif

// Control the exponentiation threshold for small numbers
#ifndef WMSRESTSERVICE_NEGATIVE_EXPONENTIATION_THRESHOLD
#define WMSRESTSERVICE_NEGATIVE_EXPONENTIATION_THRESHOLD 1e-5
#endif

#ifndef WMSRESTSERVICE_LITTLE_ENDIAN
#if defined(_MSC_VER) ||                                                      \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
    defined(__LITTLE_ENDIAN__) || defined(__i386) || defined(__x86_64)
#define WMSRESTSERVICE_LITTLE_ENDIAN 1
#else
#define WMSRESTSERVICE_LITTLE_ENDIAN 0
#endif
#endif

#ifndef WMSRESTSERVICE_ENABLE_ALIGNMENT
#if defined(__AVR)
#define WMSRESTSERVICE_ENABLE_ALIGNMENT 0
#else
#define WMSRESTSERVICE_ENABLE_ALIGNMENT 1
#endif
#endif

#ifndef WMSRESTSERVICE_TAB
#define WMSRESTSERVICE_TAB "  "
#endif

#ifndef WMSRESTSERVICE_STRING_BUFFER_SIZE
#define WMSRESTSERVICE_STRING_BUFFER_SIZE 32
#endif

#ifndef WMSRESTSERVICE_DEBUG
#ifdef __PLATFORMIO_BUILD_DEBUG__
#define WMSRESTSERVICE_DEBUG 1
#else
#define WMSRESTSERVICE_DEBUG 0
#endif
#endif

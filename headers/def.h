#ifndef DEF
#define DEF

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#define true	1
#define false	0

typedef enum datastruct_err {Success , Allocation_err , Invalid_Input , Found , Not_Found , No_Space , Already_Exists} datastruct_err;

typedef __uint128_t u128;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef __int128_t i128;
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;
typedef char* str;
typedef void(free_func) (void * obj_ptr);

#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB
#define TB 1024 * GB

#ifdef __cplusplus
}
#endif

#endif
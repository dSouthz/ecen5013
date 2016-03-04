#include <stdint.h>
#include <stdio.h>
#include "timer.h"

//Enable this definition to use standard library functions instead of custom
//You will also need to disable -fno-builtin options under Project->Properties->C/C++ Build->
//                          Settings -> Cross ARM C Compiler->Miscellaneous

//Instead of using the undef, you can, remove the macro definition in Project->Properties->
//  Cross ARM C Compiler-> Preprocessor -> PRJ_MEM_FUNC

//#define PRJ_DMA_FUNC
#define PRJ_DMA_FUNC //TODO: make this a project wide define
#ifdef PRJ_MEM_FUNC
    #ifdef PRJ_DMA_FUNC
        #include "dma_memory.h"
    #else
        #include "memory.h"
    #endif
#else
    #include <string.h>
    #error "-fno-builtin -fno-builtin-memmove and -fno-builtin memcpy must be removed from the compiler options. Then disable this error"
#endif


// Memory test definitions
#define ALPHABET_MOD    25
#define ASCII_OFFSET    65
#define MAX_BUF_SIZE    5000
#define TEST_SIZE_10    10
#define TEST_SIZE_100   100
#define TEST_SIZE_1000  1000
#define TEST_SIZE_5000  5000
#define MEMSET_VALUE            0
// Array accessors for test cases
#define NUM_TEST_CASES          4
#ifdef PRJ_MEM_FUNC
    #define NUM_TEST_FUNCTIONS  3
#else
    #define NUM_TEST_FUNCTIONS  4
#endif
#define TEST_CASE_10            0
#define TEST_CASE_100           1
#define TEST_CASE_1000          2
#define TEST_CASE_5000          3
#define TEST_FUNC_MEMMOVE       0
#define TEST_FUNC_MEMZERO       1
#define TEST_FUNC_REVERSE       2
#define TEST_FUNC_MEMSET        1
#define TEST_FUNC_STRCPY        2
#define TEST_FUNC_STRNCPY       3

//printf test variables. Arbitrary.
#define STR1                    "20 character string!"
#define VAR1                    80085
#define VAR2                    98.7654f
#define VAR3                    -8675309

uint32_t test_memmove(uint8_t *src, uint8_t *dst, uint32_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = profile_timer_count;
    memmove(src, dst, len);
    clk_end = profile_timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}

#ifdef PRJ_MEM_FUNC
uint32_t test_memzero(uint8_t *src, uint32_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = profile_timer_count;
    memzero(src, len);
    clk_end = profile_timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}

#ifndef PRJ_DMA_FUNC
uint32_t test_reverse(uint8_t *src, uint32_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = profile_timer_count;
    reverse(src, len);
    clk_end = profile_timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}
#endif //PRJ_DMA_FUNC
#endif //PRJ_MEM_FUNC

#ifndef PRJ_MEM_FUNC
uint32_t test_memset(uint8_t *src, uint32_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = profile_timer_count;
    memset(src, MEMSET_VALUE, len);
    clk_end = profile_timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}

uint32_t test_strcpy(uint8_t *src, uint8_t *dst)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = profile_timer_count;
    strcpy((char *)dst, (char *)src);
    clk_end = profile_timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}

uint32_t test_strncpy(uint8_t *src, uint8_t *dst, size_t len)
{
    uint32_t clk_start;
    uint32_t clk_end;

    clk_start = profile_timer_count;
    strncpy((char *)dst, (char *)src, len);
    clk_end = profile_timer_count;

    //Account for 32 bit wraparound
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return clk_end - clk_start;
}
#endif //PRJ_MEM_FUNC

void test_memory()
{
    uint8_t buffer_src[MAX_BUF_SIZE];
    uint8_t buffer_dst[MAX_BUF_SIZE];

    uint32_t i;

    uint32_t results[NUM_TEST_FUNCTIONS][NUM_TEST_CASES];

    //Initialize the array with junk data
    for (i=0; i < MAX_BUF_SIZE; i++)
    {
        buffer_src[i] = i % 8 + 1;          //Arbitrary
        buffer_dst[i] = (i * 3) % 8 + 1;    //Arbitrary
    }

    // Test cases: memmove() is tested for both library and custom.
    //             Others are dependent on compilation options

    //Test memmove
    results[TEST_FUNC_MEMMOVE][TEST_CASE_10] = test_memmove(buffer_src, buffer_dst, TEST_SIZE_10);
    results[TEST_FUNC_MEMMOVE][TEST_CASE_100] = test_memmove(buffer_src, buffer_dst, TEST_SIZE_100);
    results[TEST_FUNC_MEMMOVE][TEST_CASE_1000] = test_memmove(buffer_src, buffer_dst, TEST_SIZE_1000);
    results[TEST_FUNC_MEMMOVE][TEST_CASE_5000] = test_memmove(buffer_src, buffer_dst, TEST_SIZE_5000);

#ifdef PRJ_MEM_FUNC
    //Test memzero
    results[TEST_FUNC_MEMZERO][TEST_CASE_10] = test_memzero(buffer_src, TEST_SIZE_10);
    results[TEST_FUNC_MEMZERO][TEST_CASE_100] = test_memzero(buffer_src, TEST_SIZE_100);
    results[TEST_FUNC_MEMZERO][TEST_CASE_1000] = test_memzero(buffer_src, TEST_SIZE_1000);
    results[TEST_FUNC_MEMZERO][TEST_CASE_5000] = test_memzero(buffer_src, TEST_SIZE_5000);

    #ifndef PRJ_DMA_FUNC
    //Test reverse
    results[TEST_FUNC_REVERSE][TEST_CASE_10] = test_reverse(buffer_src, TEST_SIZE_10);
    results[TEST_FUNC_REVERSE][TEST_CASE_100] = test_reverse(buffer_src, TEST_SIZE_100);
    results[TEST_FUNC_REVERSE][TEST_CASE_1000] = test_reverse(buffer_src, TEST_SIZE_1000);
    results[TEST_FUNC_REVERSE][TEST_CASE_5000] = test_reverse(buffer_src, TEST_SIZE_5000);
    #endif //PRJ_DMA_FUNC
#else
    //Test memset
    results[TEST_FUNC_MEMSET][TEST_CASE_10] = test_memset(buffer_src, TEST_SIZE_10);
    results[TEST_FUNC_MEMSET][TEST_CASE_100] = test_memset(buffer_src, TEST_SIZE_100);
    results[TEST_FUNC_MEMSET][TEST_CASE_1000] = test_memset(buffer_src, TEST_SIZE_1000);
    results[TEST_FUNC_MEMSET][TEST_CASE_5000] = test_memset(buffer_src, TEST_SIZE_5000);

    //Test strcpy
    //strcpy will stop on a '\0' char. Ensure everything but the last character is non-null
    //Initialize the source buffer all non-null characters
    for (i=0; i < MAX_BUF_SIZE; i++)
    {
        buffer_src[i] = i % ALPHABET_MOD + ASCII_OFFSET; //Letters A - Z
    }

    //Set the last char to null, test, and restore to non-null after each test
    buffer_src[TEST_SIZE_10 -1] = '\0';
    results[TEST_FUNC_STRCPY][TEST_CASE_10] = test_strcpy(buffer_src, buffer_dst);
    buffer_src[TEST_SIZE_10 -1] = ASCII_OFFSET;

    buffer_src[TEST_SIZE_100 -1] = '\0';
    results[TEST_FUNC_STRCPY][TEST_CASE_100] = test_strcpy(buffer_src, buffer_dst);
    buffer_src[TEST_SIZE_100 -1] = ASCII_OFFSET;

    buffer_src[TEST_SIZE_1000 -1] = '\0';
    results[TEST_FUNC_STRCPY][TEST_CASE_1000] = test_strcpy(buffer_src, buffer_dst);
    buffer_src[TEST_SIZE_1000 -1] = ASCII_OFFSET;

    buffer_src[TEST_SIZE_5000 -1] = '\0';
    results[TEST_FUNC_STRCPY][TEST_CASE_5000] = test_strcpy(buffer_src, buffer_dst);
    buffer_src[TEST_SIZE_5000 -1] = ASCII_OFFSET;

    //Test strncpy
    results[TEST_FUNC_STRNCPY][TEST_CASE_10] = test_strncpy(buffer_src, buffer_dst, TEST_SIZE_10);
    results[TEST_FUNC_STRNCPY][TEST_CASE_100] = test_strncpy(buffer_src, buffer_dst, TEST_SIZE_100);
    results[TEST_FUNC_STRNCPY][TEST_CASE_1000] = test_strncpy(buffer_src, buffer_dst, TEST_SIZE_1000);
    results[TEST_FUNC_STRNCPY][TEST_CASE_5000] = test_strncpy(buffer_src, buffer_dst, TEST_SIZE_5000);

#endif //PRJ_MEM_FUNC

    //Add a breakpoint here and view the contents of the array to get times
    //In theory, we could printf the results, but we haven't initialized the UART. So nothing comes out.

    buffer_src[0] = i++; //Does nothing. Only here to serve as a breakpoint, since eclipse sucks at life.
}

uint32_t adjust_clock(uint32_t clk_start, uint32_t clk_end)
{
    if (clk_end < clk_start)
    {
        return (UINT32_MAX - clk_start + clk_end);
    }

    return (clk_start - clk_end);
}

void test_printf()
{
    uint32_t clk_start;
    uint32_t clk_end;
    int32_t  var1 = VAR1;
    float    var2 = VAR2;
    int32_t  var3 = VAR3;

    uint32_t printf_20_char;
    uint32_t printf_str_var1;
    uint32_t printf_str_var1_var2;
    uint32_t printf_str_var1_var2_var3;

    clk_start = profile_timer_count;
    printf(STR1);
    clk_end = profile_timer_count;
    printf_20_char = adjust_clock(clk_start, clk_end);

    clk_start = profile_timer_count;
    printf(STR1 "%d", var1);
    clk_end = profile_timer_count;
    printf_str_var1 = adjust_clock(clk_start, clk_end);

    clk_start = profile_timer_count;
    printf(STR1 "%d %f", var1, var2);
    clk_end = profile_timer_count;
    printf_str_var1_var2 = adjust_clock(clk_start, clk_end);

    clk_start = profile_timer_count;
    printf(STR1 " %d %f %d", var1, var2, var3);
    clk_end = profile_timer_count;
    printf_str_var1_var2_var3 = adjust_clock(clk_start, clk_end);

    printf("eclipse sucks!"); //Junk instruction. Only here to make it easier to set breakpoints
                             // because eclipse sucks at breakpoints at the end of functions.
}

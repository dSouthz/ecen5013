#ifndef _TEST_MEMORY_H_
#define _TEST_MEMORY_H_

/* Test cases for memory functionality */

typedef enum _testResult
{
    TEST_FAIL = 0,
    TEST_SUCCESS = 1
} TestResult_t;


/*
 * Tests memmove on non-overlapping regions of memory
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memmove_non_overlap();

/*
 * Tests memmove on left overlapping regions of memory
 *
 *    src       [xxxxxxxxxxx]
 *    dst    [xxxxxxxxxxx]
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memmove_overlap_left();

/*
 * Tests memmove on right overlapping regions of memory
 *
 *    src [xxxxxxxxxxx]
 *    dst      [xxxxxxxxxxx]
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memmove_overlap_right();

/*
 * Tests passing in null pointers to memmove
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memmove_null_ptr();

/*
 * Tests using zero for length during memmove
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memmove_zero_length();

/*
 * Tests memcpy on non-overlapping regions of memory
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memcpy_non_overlap();

/*
 * Tests memcpy on left overlapping regions of memory
 *
 *    src        [xxxxxxxxxxx]
 *    dst    [xxxxxxxxxxx]
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memcpy_overlap_left();

/*
 * Tests memcpy on right overlapping regions of memory
 *
 *    src [xxxxxxxxxxx]
 *    dst       [xxxxxxxxxxx]
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memcpy_overlap_right();

/*
 * Tests passing in null pointers to memcpy
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memcpy_null_ptr();

/*
 * Tests using zero for length during memmove
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memcpy_zero_length();

/*
 * Tests memzero on a valid pointer and length
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memzero_valid_ptr();

/*
 * Tests memzero on a null pointer
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memzero_null_ptr();

/*
 * Tests memzero on a zero length
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_memzero_zero_length();

/*
 * Tests reverse on a valid pointer and length
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_reverse_valid_ptr();

/*
 * Tests reverse on a null pointer
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_reverse_null_ptr();

/*
 * Tests memzero on a zero length
 *
 * returns TEST_SUCCESS on success, TEST_FAIL otherwise
 */
TestResult_t test_reverse_zero_length();

#endif

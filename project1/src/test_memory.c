#include <stdio.h>
#include <stdint.h>
#include "memory.h"
#include "test_memory.h"

#define MEM_SIZE        100
#define MOVE_AMT        10
#define INIT_VALUE      0xAB
#define TEST_VALUE      0xDE

void Initialize(uint8_t *pMemory, uint32_t length, uint8_t value)
{
    uint32_t i;

    for (i=0; i < length; i++)
    {
        pMemory[i] = value;
    }
}

//See test_memory.h for documentation
TestResult_t test_memmove_non_overlap()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory;
    uint8_t *pRegion2 = memory + MEM_SIZE - MOVE_AMT;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    for (i=0; i < MOVE_AMT; i++)
    {
        pRegion1[i] = TEST_VALUE;
    }

    if (memmove(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: Failed to move memory\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MOVE_AMT; i++)
    {
        if (memory[MEM_SIZE - 1 - i] != TEST_VALUE)
        {
            printf("%s: memory[%d] was not the test value\n", __FUNCTION__, (MEM_SIZE - 1 - i));
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memmove_overlap_left()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory + MOVE_AMT / 2;
    uint8_t *pRegion2 = pRegion1 - 2;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    for (i=0; i < MOVE_AMT; i++)
    {
        pRegion1[i] = TEST_VALUE + i;
    }

    if (memmove(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: Failed to move memory\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MOVE_AMT; i++)
    {
        if (pRegion2[i] != TEST_VALUE + i)
        {
            printf("%s: memory[%d] was not the test value\n", __FUNCTION__, i);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memmove_overlap_right()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory;
    uint8_t *pRegion2 = memory + MOVE_AMT - 2;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    for (i=0; i < MOVE_AMT; i++)
    {
        pRegion1[i] = TEST_VALUE + i;
    }

    if (memmove(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: Failed to move memory\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MOVE_AMT; i++)
    {
        if (pRegion2[i] != TEST_VALUE + i)
        {
            printf("%s: memory[%d] was not the test value\n", __FUNCTION__, i);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memmove_null_ptr()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory;
    uint8_t *pRegion2 = NULL;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    if(!memmove(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: failed to detect a null pointer for dst\n", __FUNCTION__);
        return TEST_FAIL;
    }

    pRegion1 = NULL;
    pRegion2 = memory;

    if(!memmove(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: failed to detect a null pointer for src\n", __FUNCTION__);
        return TEST_FAIL;
    }

    pRegion2 = NULL;
    if(!memmove(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: allowed a memcpy with both pointers = null\n", __FUNCTION__);
        return TEST_FAIL;
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memmove_zero_length()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory;
    uint8_t *pRegion2 = memory + MEM_SIZE / 2;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    for (i=0; i < MOVE_AMT; i++)
    {
        pRegion1[i] = TEST_VALUE + i;
    }

    if (memmove(pRegion1, pRegion2, 0))
    {
        printf("%s: memmove returned failure for length = 0\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MOVE_AMT; i++)
    {
        if ( (pRegion1[i] != TEST_VALUE + i) || (pRegion2[i] != INIT_VALUE) )
        {
            printf("%s: memmove changed data when it shouldn't have\n", __FUNCTION__);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memcpy_non_overlap()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory;
    uint8_t *pRegion2 = memory + MEM_SIZE - MOVE_AMT;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    for (i=0; i < MOVE_AMT; i++)
    {
        pRegion1[i] = TEST_VALUE;
    }

    if (memcpy(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: Failed to move memory\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MOVE_AMT; i++)
    {
        if (memory[MEM_SIZE - 1 - i] != TEST_VALUE)
        {
            printf("%s: memory[%d] was not the test value\n", __FUNCTION__, (MEM_SIZE - 1 - i));
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memcpy_overlap_left()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory + MOVE_AMT / 2;
    uint8_t *pRegion2 = pRegion1 - 2;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    //Fill pRegion1 with some test data, but make sure pRegion2 does not get filled
    for (i=0; i < MOVE_AMT; i++)
    {
        pRegion2[i] = TEST_VALUE + i;
    }

    if (!memcpy(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: memcpy returned success when it shouldn't have\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MOVE_AMT; i++)
    {
        if (pRegion2[i] != TEST_VALUE + i)
        {
            printf("%s: copied memory when it should not have\n", __FUNCTION__);
            return TEST_FAIL;
        }
    }
    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memcpy_overlap_right()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory;
    uint8_t *pRegion2 = memory + MOVE_AMT / 2;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    for (i=0; i < MOVE_AMT; i++)
    {
        pRegion2[i] = TEST_VALUE + i;
    }

    if (!memcpy(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: copied memory when it should not have\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MOVE_AMT; i++)
    {
        if (pRegion2[i] != TEST_VALUE + i)
        {
            printf("%s: copied memory when it should not have\n", __FUNCTION__);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memcpy_null_ptr()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory;
    uint8_t *pRegion2 = NULL;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    if(!memcpy(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: failed to detect a null pointer for dst\n", __FUNCTION__);
        return TEST_FAIL;
    }

    pRegion1 = NULL;
    pRegion2 = memory;

    if(!memcpy(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: failed to detect a null pointer for src\n", __FUNCTION__);
        return TEST_FAIL;
    }

    pRegion2 = NULL;
    if(!memcpy(pRegion1, pRegion2, MOVE_AMT))
    {
        printf("%s: allowed a memcpy with both pointers = null\n", __FUNCTION__);
        return TEST_FAIL;
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memcpy_zero_length()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion1 = memory;
    uint8_t *pRegion2 = memory + MEM_SIZE / 2;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    for (i=0; i < MOVE_AMT; i++)
    {
        pRegion1[i] = TEST_VALUE + i;
    }

    if (memcpy(pRegion1, pRegion2, 0))
    {
        printf("%s: memcpy returned failure for length = 0\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MOVE_AMT; i++)
    {
        if ( (pRegion1[i] != TEST_VALUE + i) || (pRegion2[i] != INIT_VALUE) )
        {
            printf("%s: memcpy changed data when it shouldn't have\n", __FUNCTION__);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memzero_valid_ptr()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion = memory;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    if (memzero(pRegion, MEM_SIZE))
    {
        printf("%s: memzero returned failure\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MEM_SIZE; i++)
    {
        if (memory[i] != 0)
        {
            printf("%s: memory was not initialized to zero\n", __FUNCTION__);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memzero_null_ptr()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion = NULL;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    if (!memzero(pRegion, MEM_SIZE))
    {
        printf("%s: memzero returned success when it shouldn't have\n", __FUNCTION__);
        return TEST_FAIL;
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_memzero_zero_length()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion = memory;
    uint32_t i;

    Initialize(memory, MEM_SIZE, INIT_VALUE);

    if (memzero(pRegion, 0))
    {
        printf("%s: memzero returned failure when it shouldn't have\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MEM_SIZE; i++)
    {
        if (memory[i] == 0)
        {
            printf("%s: zero'd memory when it shouldn't have\n", __FUNCTION__);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_reverse_valid_ptr()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion = memory;
    uint32_t i;

    for (i=0; i < MEM_SIZE; i++)
    {
        memory[i] = i;
    }

    if (reverse(pRegion, MEM_SIZE))
    {
        printf("%s: reverse returned failure when it shouldn't have\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MEM_SIZE; i++)
    {
        if (memory[MEM_SIZE - i - 1] != i)
        {
            printf("%s: did not reverse memory\n", __FUNCTION__);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_reverse_null_ptr()
{
    uint8_t *pRegion = NULL;

    if (!reverse(pRegion, MEM_SIZE))
    {
        printf("%s: reverse returned success on a null pointer\n", __FUNCTION__);
        return TEST_FAIL;
    }

    return TEST_SUCCESS;
}

//See test_memory.h for documentation
TestResult_t test_reverse_zero_length()
{
    uint8_t memory[MEM_SIZE];
    uint8_t *pRegion = memory;
    uint32_t i;

    for (i=0; i < MEM_SIZE; i++)
    {
        memory[i] = i;
    }

    if (reverse(pRegion, 0))
    {
        printf("%s: reverse returned failure when it shouldn't have\n", __FUNCTION__);
        return TEST_FAIL;
    }

    for (i=0; i < MEM_SIZE; i++)
    {
        if (memory[i] != i)
        {
            printf("%s: memory was changed when it shoudn't have been\n", __FUNCTION__);
            return TEST_FAIL;
        }
    }

    return TEST_SUCCESS;
}

int main()
{
    uint32_t fails = 0;
    uint32_t successes = 0;

    printf("Executing test cases. Errors will be reported as encountered\n");

    test_memmove_non_overlap()      == TEST_SUCCESS ? successes++ : fails++;
    test_memmove_overlap_left()     == TEST_SUCCESS ? successes++ : fails++;
    test_memmove_overlap_right()    == TEST_SUCCESS ? successes++ : fails++;
    test_memmove_null_ptr()         == TEST_SUCCESS ? successes++ : fails++;
    test_memmove_zero_length()      == TEST_SUCCESS ? successes++ : fails++;

    test_memcpy_non_overlap()       == TEST_SUCCESS ? successes++ : fails++;
    test_memcpy_overlap_left()      == TEST_SUCCESS ? successes++ : fails++;
    test_memcpy_overlap_right()     == TEST_SUCCESS ? successes++ : fails++;
    test_memcpy_null_ptr()          == TEST_SUCCESS ? successes++ : fails++;
    test_memcpy_zero_length()       == TEST_SUCCESS ? successes++ : fails++;

    test_memzero_valid_ptr()        == TEST_SUCCESS ? successes++ : fails++;
    test_memzero_null_ptr()         == TEST_SUCCESS ? successes++ : fails++;
    test_memzero_zero_length()      == TEST_SUCCESS ? successes++ : fails++;

    test_reverse_valid_ptr()        == TEST_SUCCESS ? successes++ : fails++;
    test_reverse_null_ptr()         == TEST_SUCCESS ? successes++ : fails++;
    test_reverse_zero_length()      == TEST_SUCCESS ? successes++ : fails++;

    printf("Summary\n");
    printf("    Passed: %u\n", successes);
    printf("    Failed: %u\n", fails);

    return 0;
}

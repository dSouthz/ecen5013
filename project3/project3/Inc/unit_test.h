#ifndef SOURCES_UNIT_TEST_H_
#define SOURCES_UNIT_TEST_H_

#define TEST_FAILURE 0
#define TEST_SUCCESS 1

// Tests filling and emptying the buffer
// Returns: TEST_SUCCESS on SUCCESS, TEST_FAILURE on failure
int Test_fill_and_empty();

// Tests boundary conditions for the buffer
// Returns: TEST_SUCCESS on SUCCESS, TEST_FAILURE on failure
int Test_boundary();

// Tests pointers are incremented correctly for buffer additions
// Returns: TEST_SUCCESS on SUCCESS, TEST_FAILURE on failure
int Test_pointer_length();

// Runs the entire test suite, prints results
void RunTestSuite();

#endif /* SOURCES_UNIT_TEST_H_ */

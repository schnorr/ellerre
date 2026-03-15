# Helper script for ctest: runs BINARY < INPUT and compares output to EXPECTED.
# Usage: cmake -DBINARY=<path> -DINPUT=<path> -DEXPECTED=<path> -P run_test.cmake

execute_process(
  COMMAND ${BINARY}
  INPUT_FILE ${INPUT}
  OUTPUT_VARIABLE ACTUAL
  RESULT_VARIABLE RC
)

if(NOT RC EQUAL 0)
  message(FATAL_ERROR "Binary exited with code ${RC} for input ${INPUT}")
endif()

file(READ ${EXPECTED} EXPECTED_CONTENT)

if(NOT ACTUAL STREQUAL EXPECTED_CONTENT)
  message(FATAL_ERROR "Output mismatch for ${INPUT}\nExpected:\n${EXPECTED_CONTENT}\nActual:\n${ACTUAL}")
endif()

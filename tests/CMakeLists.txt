set(TEST_LIBRARIES meiose-static)
include_directories("include/")

#add_custom_target(coverage_individual)

set(TEST_BUILD_FLAGS "")
set(TEST_LINK_FLAGS -lgcov --coverage)

set(TESTS_LIST "")
set(TESTS_LIST_RUN "")
set(TESTS_LCOV_MERGE_LIST "")
list(APPEND TESTS_LCOV_MERGE_LIST "-d" "${CMAKE_BINARY_DIR}/CMakeFiles/meiose-static.dir")
function(test)
  set(TEST_NAME ${ARGV0})
  set(TEST_SOURCES "")
  foreach(argn RANGE 1 ${ARGC})
    list(APPEND TEST_SOURCES "${ARGV${argn}}")
  endforeach(argn)
  list(APPEND TESTS_LIST "${TEST_NAME}")
  list(APPEND TESTS_LIST_RUN "${TEST_NAME}_run")
  list(APPEND TESTS_LCOV_MERGE_LIST "-d" "${CMAKE_BINARY_DIR}/tests/CMakeFiles/${TEST_NAME}.dir") # "-t" "${TEST_NAME}")
  set(TESTS_LIST "${TESTS_LIST}" PARENT_SCOPE)
  set(TESTS_LIST_RUN "${TESTS_LIST_RUN}" PARENT_SCOPE)
  set(TESTS_LCOV_MERGE_LIST "${TESTS_LCOV_MERGE_LIST}" PARENT_SCOPE)
  add_executable(${TEST_NAME} ${TEST_SOURCES})
  target_link_libraries(${TEST_NAME} ${TEST_LIBRARIES} ${TEST_LINK_FLAGS})
  target_compile_options(${TEST_NAME} PRIVATE ${TEST_BUILD_FLAGS})
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  #setup_target_for_coverage(NAME "${TEST_NAME}_cov" EXECUTABLE ${TEST_NAME})
  #add_dependencies(coverage_individual "${TEST_NAME}_cov")
  add_custom_target("${TEST_NAME}_run" COMMAND "${TEST_NAME}" DEPENDS "${TEST_NAME}")
endfunction(test)

test(JsonReadTest JsonReadTest.cpp)
test(JsonWriteReadTest JsonWriteReadTest.cpp)
test(JsonWriteTest JsonWriteTest.cpp)

test(MsgpackWriteReadTest MsgpackWriteReadTest.cpp)

test(VariantAssignPrimitiveTest VariantAssignPrimitiveTest.cpp)
test(VariantAssignStructuresTest VariantAssignStructuresTest.cpp)
test(VariantCastTest VariantCastTest.cpp)
test(VariantCtorPrimitiveTest VariantCtorPrimitiveTest.cpp)
test(VariantCopyMovePrimitiveTest VariantCopyMovePrimitiveTest.cpp)
test(VariantCtorStructuresTest VariantCtorStructuresTest.cpp)
test(VariantCopyMoveStructuresTest VariantCopyMoveStructuresTest.cpp)
test(VariantIncorrectCastTest VariantIncorrectCastTest.cpp)
test(VariantIsTypeTest VariantIsTypeTest.cpp)
test(VariantTest VariantTest.cpp)
test(VariantUtilitiesTest VariantUtilitiesTest.cpp)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(COV_TARGET_NAME coverage)
add_custom_target(${COV_TARGET_NAME}
  # Capturing lcov counters and generating report
  COMMAND ${LCOV_PATH} -c ${TESTS_LCOV_MERGE_LIST} -o ${COV_TARGET_NAME}.info
  COMMAND ${LCOV_PATH} --remove ${COV_TARGET_NAME}.info ${COVERAGE_EXCLUDES} "${PROJECT_SOURCE_DIR}/tests/\\*" -o ${COV_TARGET_NAME}.info.cleaned
  COMMAND ${LCOV_PATH} --extract ${COV_TARGET_NAME}.info.cleaned "${PROJECT_SOURCE_DIR}/\\*" -o ${COV_TARGET_NAME}.info.extracted
  COMMAND ${GENHTML_PATH} -s --demangle-cpp -o ${COV_TARGET_NAME} ${COV_TARGET_NAME}.info.extracted
  #COMMAND ${CMAKE_COMMAND} -E remove ${COV_TARGET_NAME}.info ${COV_TARGET_NAME}.info.cleaned ${COV_TARGET_NAME}.info.extracted

  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
  DEPENDS ${TESTS_LIST_RUN}
  COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
  COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET ${COV_TARGET_NAME} PRE_BUILD
  # Cleanup lcov
  COMMAND ${LCOV_PATH} --directory tests --zerocounters
  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
)

# Show info where to find the report
add_custom_command(TARGET ${COV_TARGET_NAME} POST_BUILD
  COMMAND ;
  COMMENT "Open ./${COV_TARGET_NAME}/index.html in your browser to view the coverage report."
)

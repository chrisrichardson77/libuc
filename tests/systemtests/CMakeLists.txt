project(systemtests CXX)

include_directories("../../src" "../../include" ${CMAKE_CURRENT_BINARY_DIR} ${GTEST_BOTH_INCLUDE_DIRS})

FILE (GLOB_RECURSE test_SRCS *.cxx)

ADD_EXECUTABLE(${PROJECT_NAME} ${test_SRCS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(${PROJECT_NAME} googlemock)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_LIBS} ${GMOCK_LIBRARY} pthread)

if (BUILD_GTEST_FILTER)
    SET(GTEST_FILTER --gtest_filter=${BUILD_GTEST_FILTER}) 
endif ()

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/SystemTestConstantsGenerated.h.in" "${CMAKE_CURRENT_BINARY_DIR}/SystemTestConstantsGenerated.h")

ADD_CUSTOM_TARGET(systemtest "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "--gtest_output=xml" ${GTEST_FILTER}
    DEPENDS ${PROJECT_NAME} 
    COMMENT "Executing system tests..."
    VERBATIM SOURCES ${test_SRCS})

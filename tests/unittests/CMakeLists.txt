project(unittests CXX)

# Set the target name policy to allow the reserved target name "test" to be used
if (POLICY CMP0037)
    message(STATUS "Setting target name policy to OLD (allowing use of reserved target names)")
    cmake_policy(SET CMP0037 OLD)
endif ()

IF (NOT CMAKE_CROSSCOMPILING)    
    include_directories("../../src/" "../../include" ${GTEST_BOTH_INCLUDE_DIRS})
    
    FILE (GLOB_RECURSE test_SRCS *.cxx)

    ADD_EXECUTABLE(${PROJECT_NAME} ${test_SRCS})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    add_dependencies(${PROJECT_NAME} googlemock)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_LIBS} ${GMOCK_LIBRARY} pthread
    )
   
    if (BUILD_GTEST_FILTER)
        SET(GTEST_FILTER --gtest_filter=${BUILD_GTEST_FILTER}) 
    endif ()
   
    ADD_CUSTOM_TARGET(test "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}" 
        --gtest_output=xml
        ${GTEST_FILTER}
        DEPENDS ${PROJECT_NAME}
        COMMENT "Executing unit tests..."
        VERBATIM SOURCES ${test_SRCS})
ENDIF (NOT CMAKE_CROSSCOMPILING)

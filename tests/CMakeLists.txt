IF (NOT CMAKE_CROSSCOMPILING)
	if (BUILD_UNIT_TESTS OR BUILD_SYSTEM_TESTS)
		enable_testing()
    	add_subdirectory(gmock)
	endif (BUILD_UNIT_TESTS OR BUILD_SYSTEM_TESTS)
ENDIF (NOT CMAKE_CROSSCOMPILING)

if (BUILD_UNIT_TESTS)
    add_subdirectory(unittests)
endif (BUILD_UNIT_TESTS)

if (BUILD_SYSTEM_TESTS)
    add_subdirectory(systemtests)
endif (BUILD_SYSTEM_TESTS)

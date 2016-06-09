include(ExternalProject)

# Add gmock
ExternalProject_Add(
    googlemock
    GIT_REPOSITORY
        https://github.com/google/googletest.git
        TIMEOUT 30
    #PATCH_COMMAND svn patch ${CMAKE_SOURCE_DIR}/gtest.patch ${CMAKE_BINARY_DIR}/ThirdParty/src/googlemock/gtest
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    #CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
    #           -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
    #           -Dgtest_force_shared_crt=ON
    # Disable install step
    UPDATE_COMMAND "" # Don't keep updating from svn
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    #LOG_DOWNLOAD ON
    #LOG_CONFIGURE ON
    #LOG_BUILD ON
    )
    
ExternalProject_Get_Property(googlemock source_dir binary_dir)

SET(GMOCK_INCLUDE_DIRS "${source_dir}/googlemock/include" CACHE STRING "GMock include directory")
SET(GTEST_INCLUDE_DIRS "${source_dir}/googletest/include" CACHE STRING "GTest include directory")

SET(GTEST_BOTH_INCLUDE_DIRS ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS} CACHE STRING "GTest and GMock include directories")

SET(GMOCK_LIBRARY "${binary_dir}/googlemock/libgmock.a" CACHE STRING "GMock library")

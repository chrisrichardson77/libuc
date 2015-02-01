# Copyright (C) 2015 FourC AS, http://www.fourc.eu/
# All Rights Reserved.
#
# This file is part of PngChecker.
#
# PngChecker is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PngChecker is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with PngChecker.  If not, see <http://www.gnu.org/licenses/>.

# This file configures Google Mocks as an external project to the PngChecker build,
# iff testing is enabled.

include(ExternalProject)

# Add gmock
ExternalProject_Add(
    googlemock
    SVN_REPOSITORY http://googlemock.googlecode.com/svn/trunk/
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

SET(GMOCK_INCLUDE_DIRS "${source_dir}/include" CACHE STRING "GMock include directory")
SET(GTEST_INCLUDE_DIRS "${source_dir}/gtest/include" CACHE STRING "GTest include directory")

SET(GTEST_BOTH_INCLUDE_DIRS ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS} CACHE STRING "GTest and GMock include directories")

SET(GMOCK_LIBRARY "${binary_dir}/libgmock.a" CACHE STRING "GMock library")

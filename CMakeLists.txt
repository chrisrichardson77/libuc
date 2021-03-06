CMAKE_MINIMUM_REQUIRED (VERSION 2.8.2)

PROJECT(Uc CXX)

OPTION(BUILD_EXAMPLES "Build example programs" OFF)
option(BUILD_UNIT_TESTS "Include unit tests" ON)
option(BUILD_SYSTEM_TESTS "Include system tests" OFF)

INCLUDE(version.cmake)

LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

FIND_PACKAGE(MySQL)
FIND_PACKAGE(Sqlite3)
FIND_PACKAGE(FLEX)
FIND_PACKAGE(CURL)

SET(JSON_UCODER_CPP "${CMAKE_BINARY_DIR}/ucoder_json.cpp") # This file is generated by the build

SET(${PROJECT_NAME}_SRCS
	src/buffer.cpp
	src/buffer_util.cpp
	src/string_util.cpp
	src/uccontract.cpp
	src/ucdb.cpp
	src/ucio.cpp
	src/ucoder_bin.cpp
	src/ucoder_ini.cpp
	src/ucontainer.cpp
	src/uc_web.cpp
	flex/json_parser.lex
	${JSON_UCODER_CPP}
)

# Flex
# If we found the flex command, generate ucoder_json.cpp from json_parser.lex
# otherwise, use the fallback (which has (perhaps incorrectly) been under revision control
# throughout project history)
IF (FLEX_FOUND)
  #============================================================
  # FLEX_TARGET_CUSTOM
  # based on original
  # FLEX_TARGET (public macro)
  # with a bugfix for handling spaces in paths
  #============================================================
  #
  MACRO(FLEX_TARGET_CUSTOM Name Input Output)
    SET(FLEX_TARGET_usage "FLEX_TARGET_CUSTOM(<Name> <Input> <Output> [COMPILE_FLAGS <string>]")
    IF(${ARGC} GREATER 3)
      IF(${ARGC} EQUAL 5)
        IF("${ARGV3}" STREQUAL "COMPILE_FLAGS")
          SET(FLEX_EXECUTABLE_opts  "${ARGV4}")
          SEPARATE_ARGUMENTS(FLEX_EXECUTABLE_opts)
        ELSE()
          MESSAGE(SEND_ERROR ${FLEX_TARGET_usage})
        ENDIF()
      ELSE()
        MESSAGE(SEND_ERROR ${FLEX_TARGET_usage})
      ENDIF()
    ENDIF()

    ADD_CUSTOM_COMMAND(OUTPUT ${Output}
      COMMAND ${FLEX_EXECUTABLE}
      ARGS ${FLEX_EXECUTABLE_opts} -o "${Output}" ${Input}
      DEPENDS ${Input}
      COMMENT "[FLEX][${Name}] Building scanner with flex ${FLEX_VERSION}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      VERBATIM)

    SET(FLEX_${Name}_DEFINED TRUE)
    SET(FLEX_${Name}_OUTPUTS ${Output})
    SET(FLEX_${Name}_INPUT ${Input})
    SET(FLEX_${Name}_COMPILE_FLAGS ${FLEX_EXECUTABLE_opts})
  ENDMACRO(FLEX_TARGET_CUSTOM)
  #============================================================


	MESSAGE(STATUS "Flex found: ucoder_json will be generated")
	FLEX_TARGET_CUSTOM(flex_json flex/json_parser.lex ${JSON_UCODER_CPP})
ELSE (FLEX_FOUND)
	MESSAGE(WARNING "Flex not found: will use fallback 'flex/ucoder_json.cpp.fallback'")
	CONFIGURE_FILE(flex/ucoder_json.cpp.fallback ${JSON_UCODER_CPP} COPYONLY)
ENDIF (FLEX_FOUND)

# MySql integration
IF (MYSQL_FOUND)
	MESSAGE(STATUS "Found MySQL: ${MYSQL_LIBRARY}")

	SET(${PROJECT_NAME}_SRCS ${${PROJECT_NAME}_SRCS} src/ucmysql.cpp)

	INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
	SET(${PROJECT_NAME}_LIBS ${${PROJECT_NAME}_LIBS} ${MYSQL_LIBRARIES})
ELSE ()
	MESSAGE(STATUS "MySql not found - skipping support.")
ENDIF ()

# SqLite integration
IF (SQLITE3_FOUND)
#	MESSAGE(STATUS "Found Sqlite3: ${SQLITE3_LIBRARY}") # For some reason the FIND_PACKAGE SQLITE3_FIND_QUIETLY switch doesn't seem to work and this message will already have been displayed

	SET(${PROJECT_NAME}_SRCS ${${PROJECT_NAME}_SRCS} src/ucsqlite.cpp)

	INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
	SET(${PROJECT_NAME}_LIBS ${${PROJECT_NAME}_LIBS} ${SQLITE3_LIBRARY})
ELSE ()
	MESSAGE(STATUS "SqLite not found - skipping support.")
ENDIF ()

# Curl integration
IF (CURL_FOUND)
	SET(${PROJECT_NAME}_SRCS ${${PROJECT_NAME}_SRCS} src/buffer_curl.cpp src/uc_curl.cpp)

	INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
	SET(${PROJECT_NAME}_LIBS ${${PROJECT_NAME}_LIBS} ${CURL_LIBRARIES})
ELSE ()
	MESSAGE(STATUS "Curl not found - skipping support.")
ENDIF ()

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
	VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}"
	OUTPUT_NAME ${PROJECT_NAME}
	CLEAN_DIRECT_OUTPUT 1)
INSTALL(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
INSTALL(DIRECTORY include/uc DESTINATION include FILES_MATCHING PATTERN "*.h")

IF (BUILD_EXAMPLES)
	SET(EXAMPLE_SRCS examples/example.cpp)
	ADD_EXECUTABLE(example ${EXAMPLE_SRCS})
	TARGET_LINK_LIBRARIES(example ${PROJECT_NAME} ${CURL_LIBRARIES})

	SET(CONTRACT_EXAMPLE_SRCS examples/contract_example.cpp)
	ADD_EXECUTABLE(contract_example ${CONTRACT_EXAMPLE_SRCS})
	TARGET_LINK_LIBRARIES(contract_example ${PROJECT_NAME} ${CURL_LIBRARIES})

	CONFIGURE_FILE(examples/contract.json ${CMAKE_BINARY_DIR}/contract.json COPYONLY)
	CONFIGURE_FILE(examples/data.json ${CMAKE_BINARY_DIR}/data.json COPYONLY)
ENDIF()

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR lib/cmake/${PROJECT_NAME} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Add all targets to the build-tree export set
export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE ${PROJECT_NAME})

# Create the UcConfig.cmake and UcConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}")
configure_file(cmake/UcConfig.cmake.in "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${UC_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(cmake/UcConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY)

# ... for both
configure_file(cmake/UcConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)

# Install the UcConfig.cmake and UcConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT ${PROJECT_NAME}Targets DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

ADD_SUBDIRECTORY(tests)


# Copyright (c) 2016 Fabian Schuiki
cmake_minimum_required(VERSION 2.8)
project(libasic)

option(BUILD_LEF_LIB "Build the LEF library" ON)
option(BUILD_LIB_LIB "Build the LIB library" ON)
option(BUILD_DOC "Build the documentation" ON)

# Default to a release build, making the project easier to package. If you plan
# on writing code, call cmake with the -DCMAKE_BUILD_TYPE=debug option.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE release)
endif()
message("## libasic ${CMAKE_BUILD_TYPE} build")


# Versioning
set(ASIC_VERSION_MAJOR 0)
set(ASIC_VERSION_MINOR 1)
set(ASIC_VERSION_PATCH 0)
set(ASIC_VERSION "${ASIC_VERSION_MAJOR}.${ASIC_VERSION_MINOR}.${ASIC_VERSION_PATCH}")
message("## libasic version ${ASIC_VERSION}")


# Compiler Flags
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/common)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror-implicit-function-declaration -Wno-unused-parameter -Wno-unused-label -fvisibility=hidden")


# Load the header prefix and suffix. These contain common parts of the header
# files and shall be used by the individual libraries to assemble their proper
# header.
file(READ include/prefix.h HEADER_PREFIX)
file(READ include/suffix.h HEADER_SUFFIX)


# The following macro is used to generate static libraries with symbols properly
# stripped.
macro(asic_add_library NAME)
	# Create a static library target that will take care of compiling the source
	# files of this library.
	add_library(${NAME}-obj STATIC ${ARGN})

	# Create a custom command that unpacks the above library, partially links
	# all its objects, and localizes all hidden symbols. This yields one object
	# file that contains the entire library.
	add_custom_command(OUTPUT lib${NAME}.o
		COMMAND rm -rf lib${NAME}-objs
		COMMAND mkdir lib${NAME}-objs
		COMMAND cd lib${NAME}-objs && ${CMAKE_AR} x $<TARGET_FILE:${NAME}-obj>
		COMMAND ${CMAKE_LINKER} -r lib${NAME}-objs/*.o -o lib${NAME}.o
		COMMAND objcopy --localize-hidden lib${NAME}.o
	)

	# Create the static library which simply archives the stripped object file
	# created above.
	add_library(${NAME} STATIC lib${NAME}.o)
	set_target_properties(${NAME} PROPERTIES LINKER_LANGUAGE C)
endmacro()


# Compile the common source files.
add_library(common OBJECT
	src/common/util.c
	src/common/util-array.c
	src/common/util-ptrset.c
)


set(INSTALL_TARGETS)
set(INSTALL_HEADERS)


# Build the LEF library
if (BUILD_LEF_LIB)
	configure_file(include/lef.h include/asic/lef.h)
	asic_add_library(asic-lef
		src/lef/lef.c
		src/lef/lef-ast.c
		src/lef/lef-write.c
		$<TARGET_OBJECTS:common>
	)
	list(APPEND INSTALL_TARGETS asic-lef)
	list(APPEND INSTALL_HEADERS ${PROJECT_BINARY_DIR}/include/asic/lef.h)
endif()


# Build the LIB library
if (BUILD_LIB_LIB)
	configure_file(include/lib.h include/asic/lib.h)
	asic_add_library(asic-lib
		src/lib/lib.c
		src/lib/lib-ast.c
		src/lib/lib-lexer.c
		src/lib/lib-parser.c
		$<TARGET_OBJECTS:common>
	)
	list(APPEND INSTALL_TARGETS asic-lib)
	list(APPEND INSTALL_HEADERS ${PROJECT_BINARY_DIR}/include/asic/lib.h)
endif()


# Build the documentation if requested.
if (BUILD_DOC)
	find_package(Doxygen REQUIRED)
	configure_file(Doxyfile.in ${PROJECT_BINARY_DIR}/doc/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} Doxyfile 1> doxygen.log 2> doxygen.err
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc
		COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()


# Configure the installation.
install(
	TARGETS ${INSTALL_TARGETS}
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
install(FILES ${INSTALL_HEADERS} DESTINATION include/asic)

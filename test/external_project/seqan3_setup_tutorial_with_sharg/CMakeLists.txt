# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

# --- helper scripts
include (../find-package-diagnostics.cmake)
# ---

# Copy the cpp so we can use add_executable like in the tutorial
file (COPY "${CMAKE_SOURCE_DIR}/../src/hello_world.cpp" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file (RENAME "${CMAKE_CURRENT_BINARY_DIR}/hello_world.cpp" "${CMAKE_CURRENT_BINARY_DIR}/another_program.cpp")
file (COPY "${CMAKE_SOURCE_DIR}/../src/hello_world.cpp" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
# In the tutorial, the CMAKE_CURRENT_SOURCE_DIR is a sibling directory of the seqan3 checkout
set (CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../../source")
# Cannot replicate a Sharg checkout just like in the tutorial
list (APPEND CMAKE_PREFIX_PATH "${SHARG_ROOT}/build_system")

## [adding_files]
cmake_minimum_required (VERSION 3.4)
project (seqan3_tutorial CXX)

# add seqan3 to search path
list (APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../seqan3/build_system")
# add the Sharg Parser to search path
list (APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../sharg-parser/build_system")

# require seqan3 with a version between >=3.0.0 and <4.0.0
find_package (seqan3 3.0 REQUIRED)
find_package (sharg 1.0 REQUIRED)

# build app with seqan3
add_executable (hello_world hello_world.cpp)
target_link_libraries (hello_world seqan3::seqan3 sharg::sharg)

add_executable (another_program another_program.cpp)
target_link_libraries (another_program seqan3::seqan3 sharg::sharg)
## [adding_files]

if (CMAKE_VERSION VERSION_LESS 3.14)
    install (TARGETS hello_world another_program RUNTIME DESTINATION bin)
else ()
    install (TARGETS hello_world another_program) # RUNTIME DESTINATION not needed anymore since cmake 3.14
endif ()

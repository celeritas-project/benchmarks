#------------------------------------------------------------------------------
# Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
# See the Celeritas top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Set up the project
#------------------------------------------------------------------------------
project(Geant4-Celeritas-KN)

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
cmake_policy(VERSION 3.12...3.16)
set(CMAKE_CXX_STANDARD 14)

#------------------------------------------------------------------------------
# Find packages
#------------------------------------------------------------------------------
find_package(Geant4 REQUIRED vis_all)

#------------------------------------------------------------------------------
# Locate sources and headers of the project
#------------------------------------------------------------------------------
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.hh)

#------------------------------------------------------------------------------
# Add the executable and link it
#------------------------------------------------------------------------------
add_executable(gckn geant4-celeritas-kn.cc ${sources} ${headers})

target_include_directories(gckn PRIVATE
 ${PROJECT_SOURCE_DIR}/src
 ${Geant4_INCLUDE_DIR})

target_link_libraries(gckn PRIVATE
  ${Geant4_LIBRARIES})

#------------------------------------------------------------------------------
# Install the executable in the current directory
#------------------------------------------------------------------------------
install(TARGETS gckn DESTINATION .)

#------------------------------------------------------------------------------
# Create a tables folder and copy vis.mac to the current dir
#------------------------------------------------------------------------------
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/tables)
configure_file(${PROJECT_SOURCE_DIR}/vis.mac ${PROJECT_BINARY_DIR} COPYONLY)

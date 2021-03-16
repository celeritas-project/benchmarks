#---------------------------------*-CMake-*----------------------------------#
# Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
# See the Celeritas top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#----------------------------------------------------------------------------#

# [Temporary] In a conda env, this fixes the cmake issue. make still fails.
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()

project(g4-app)

cmake_minimum_required(VERSION 3.9)
cmake_policy(VERSION 3.12...3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Werror -fcolor-diagnostics")

#----------------------------------------------------------------------------#
# Find packages
#----------------------------------------------------------------------------#
find_package(Geant4 REQUIRED vis_all)
find_package(ROOT REQUIRED)
find_package(nlohmann_json REQUIRED)

#----------------------------------------------------------------------------#
# Generate ROOT dictionary
#----------------------------------------------------------------------------#
include(${ROOT_USE_FILE})

root_generate_dictionary(RootInterface
  ${PROJECT_SOURCE_DIR}/src/Event.hh 
  MODULE event
  LINKDEF ${PROJECT_SOURCE_DIR}/src/RootInterfaceLinkDef.hh)

add_library(event SHARED ${PROJECT_SOURCE_DIR}/src/Event.hh RootInterface.cxx)
target_link_libraries(event PUBLIC ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------#
# Locate sources and headers of the project
#----------------------------------------------------------------------------#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.hh)

#----------------------------------------------------------------------------#
# Add executable
#----------------------------------------------------------------------------#
add_executable(g4app g4-app.cc ${sources} ${headers})

target_include_directories(g4app PRIVATE
 ${PROJECT_SOURCE_DIR}/src
 ${Geant4_INCLUDE_DIR}
 ${ROOT_INCLUDE_DIR}
 ${NLOHMANN_SOURCE_DIR}/include)

target_link_libraries(g4app PRIVATE
  ${Geant4_LIBRARIES}
  ROOT::Core
  nlohmann_json::nlohmann_json
  event)

#----------------------------------------------------------------------------#
# Install the executable in the current directory
#----------------------------------------------------------------------------#
install(TARGETS g4app DESTINATION .)

#----------------------------------------------------------------------------#
# Copy vis.mac and input_example.json to the build directory
#----------------------------------------------------------------------------#
configure_file(${PROJECT_SOURCE_DIR}/vis.mac ${PROJECT_BINARY_DIR} COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/input_example.json 
  ${PROJECT_BINARY_DIR} COPYONLY)

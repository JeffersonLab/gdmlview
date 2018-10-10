#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gdmlview)

# Default install path is the source directory
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(STATUS "    Install-prefix was at default -> forcing it to the /usr/local" )
    message(STATUS "    Use -DCMAKE_INSTALL_PREFIX=/usr/local to set to something else" )
    set (CMAKE_INSTALL_PREFIX "/usr/local"
         CACHE PATH "default install path" FORCE )
endif()

#----------------------------------------------------------------------------
# Find Geant4 package
#
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find Boost libraries
find_package(Boost COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})

#----------------------------------------------------------------------------
# Debugging symbols, warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(gdmlview-bin gdmlview.cc ${sources})
set_target_properties(gdmlview-bin PROPERTIES OUTPUT_NAME gdmlview)
target_link_libraries(gdmlview-bin ${Geant4_LIBRARIES} ${Boost_LIBRARIES})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS gdmlview-bin DESTINATION bin)

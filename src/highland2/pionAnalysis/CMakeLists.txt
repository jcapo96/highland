# -*- cmake -*-
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(pion)

#-------------------------------------------------------
# ----- Set cmake options ------------------------------
#-------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-g -pipe -Wall -ansi -Wnon-virtual-dtor -fpic -Woverloaded-virtual -std=c++17")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

message("CMAKE source dir: ${CMAKE_SOURCE_DIR}")
message("CMAKE binary dir: ${CMAKE_BINARY_DIR}")
message("CMAKE compiler:   ${CMAKE_CXX_COMPILER}")
message("CMAKE_CXX_FLAGS:  ${CMAKE_CXX_FLAGS}")
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

#-------------------------------------------------------
#------- External packages -----------------------------
#-------------------------------------------------------

# Look for the required libraries

#------ ROOT -------------------
find_package(ROOT REQUIRED COMPONENTS Geom EG)
include(${ROOT_USE_FILE})

message("ROOT_USE_FILE:            ${ROOT_USE_FILE}")
message("ROOT include directories: ${ROOT_INCLUDE_DIR}")
message("ROOT libraries:           ${ROOT_LIBRARIES}")


#------ HighLAND -------------------
find_package(HIGHLAND REQUIRED)

message("HighLAND_USE_FILE:            ${HIGHLAND_USE_FILE}")
message("HighLAND include directories: ${HIGHLAND_INCLUDE_DIRS}")
message("HighLAND libraries:           ${HIGHLAND_LIBRARIES}")

#-------------------------------------------------------
#-------------- executable -----------------------------
#-------------------------------------------------------

file(GLOB pion_src  ${CMAKE_SOURCE_DIR}/src/*.cxx)

add_executable(pion.exe ${pion_src} ${CMAKE_SOURCE_DIR}/app/RunPionAnalysis.cxx)
target_include_directories(pion.exe PRIVATE ${HIGHLAND_INCLUDE_DIRS})
target_link_libraries(pion.exe ${ROOT_LIBRARIES} ${HIGHLAND_LIBRARIES})





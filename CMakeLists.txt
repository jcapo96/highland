# -*- cmake -*-
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(HighLAND)

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

# ---- create folders ----------
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/include")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/dict")

set (DATAMCDIR mc)
set (LARSOFTREADERDIR "${CMAKE_SOURCE_DIR}/src/highland2/LArSoftReader")

#-------------------------------------------------------
#------- External packages -----------------------------
#-------------------------------------------------------

# Set where to look for external packages 
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/") 

message("CMAKE_MODULE_PATH source dir: ${CMAKE_MODULE_PATH}")


# Look for the required libraries
find_package(ROOT REQUIRED COMPONENTS Geom EG)

include(${ROOT_USE_FILE})

message("ROOT_USE_FILE: ${ROOT_USE_FILE}")
message("ROOT include directories: ${ROOT_INCLUDE_DIRS}")
message("ROOT libraries: ${ROOT_LIBRARIES}")

#-------------------------------------------------------
#----- include directories -----------------------------
#-------------------------------------------------------

file(GLOB highland_inc ${CMAKE_SOURCE_DIR}/src/*/*/src
                       ${LARSOFTREADERDIR}/src/2ndproduction
                       ${LARSOFTREADERDIR}/src/2ndproduction/${DATAMCDIR}
                       ${CMAKE_BINARY_DIR}/dict)

include_directories(${highland_inc})
message("Include directories: ${INCLUDE_DIRECTORIES}")

#-------------------------------------------------------
#------- ROOT Dictionaries -----------------------------
#-------------------------------------------------------

file(GLOB dict_src ${CMAKE_SOURCE_DIR}/src/*/*/src/*_LinkDef.h)

foreach(file0 ${dict_src})
  string(FIND ${file0} "/"          pos_ini REVERSE)
  string(SUBSTRING ${file0} 0      ${pos_ini}  dir)
  string(REPLACE "${dir}/"     "" file2 ${file0})
  string(REPLACE _LinkDef.h    "" file3 ${file2})  

  root_generate_dictionary(dict/${file3}_Dict ${dir}/${file3}.hxx LINKDEF ${file0}  MODULE ${file3} DEPENDENCIES ${ROOT_LIBRARIES})

  list(APPEND dict_list ${dir}/${file3}.cxx dict/${file3}_Dict.cxx)
  list(APPEND dict_src ${CMAKE_BINARY_DIR}/dict/${file3}_Dict.cxx)

  INSTALL(FILES "${CMAKE_BINARY_DIR}/lib${file3}_rdict.pcm" DESTINATION ${CMAKE_SOURCE_DIR}/lib)
  INSTALL(FILES "${CMAKE_BINARY_DIR}/lib${file3}.rootmap"   DESTINATION ${CMAKE_SOURCE_DIR}/lib)
endforeach(file0)

#---------- root dictionaries for LArSoftReader --------------

  root_generate_dictionary(dict/LArSoftReaderProjectDict
                                   ${LARSOFTREADERDIR}/src/2ndproduction/${DATAMCDIR}/LArSoftReaderProjectHeaders.h
                           LINKDEF ${LARSOFTREADERDIR}/src/2ndproduction/${DATAMCDIR}/LArSoftReaderLinkDef.h
                           MODULE LArSoftReader
                           DEPENDENCIES ${ROOT_LIBRARIES})
  list(APPEND dict_list ${LARSOFTREADERDIR}/src/2ndproduction/data/LArSoftReaderProjectSource.cxx dict/LArSoftReaderProjectDict.cxx)
#  list(APPEND dict_src ${CMAKE_BINARY_DIR}/dict/LArSoftReaderProjectDict.cxx)
  INSTALL(FILES "${CMAKE_BINARY_DIR}/libLArSoftReader_rdict.pcm" DESTINATION ${CMAKE_SOURCE_DIR}/lib)
  INSTALL(FILES "${CMAKE_BINARY_DIR}/libLArSoftReader.rootmap"   DESTINATION ${CMAKE_SOURCE_DIR}/lib)

#-------------------------------------------------------
#-------- the highland library -------------------------
#-------------------------------------------------------

file(GLOB highland_src  ${CMAKE_SOURCE_DIR}/src/psyche/*/src/*.cxx 
                        ${CMAKE_SOURCE_DIR}/src/highland2/highland*/src/*.cxx                                           
                        ${CMAKE_SOURCE_DIR}/src/highland2/baseAnalysis/src/*.cxx
                        ${CMAKE_SOURCE_DIR}/src/highland2/pionAnalysis/src/*.cxx
                        ${CMAKE_SOURCE_DIR}/src/highland2/LArSoftReader/src/2ndproduction/LArSoft*.cxx
                        ${CMAKE_SOURCE_DIR}/src/highland2/LArSoftReader/src/2ndproduction/${DATAMCDIR}/LArSoftReaderProjectSource.cxx)    

add_library(highland SHARED ${dict_src} ${highland_src} ) 
target_link_libraries(highland ${ROOT_LIBRARIES})
INSTALL(TARGETS highland DESTINATION ${CMAKE_SOURCE_DIR}/lib)

#-------------------------------------------------------
#-------------- executable -----------------------------
#-------------------------------------------------------

add_executable(pion.exe  ${CMAKE_SOURCE_DIR}/src//highland2/pionAnalysis/app/RunPionAnalysis.cxx)
target_link_libraries(pion.exe ${ROOT_LIBRARIES} "${CMAKE_BINARY_DIR}/libhighland${CMAKE_SHARED_LIBRARY_SUFFIX}")
INSTALL(TARGETS pion.exe DESTINATION ${CMAKE_SOURCE_DIR}/bin)

#-------------------------------------------------------
#----- highland includes -------------------------------
#-------------------------------------------------------

file(GLOB highland_inc2 ${CMAKE_SOURCE_DIR}/src/*/*/src/*.hxx
                        ${LARSOFTREADERDIR}/src/2ndproduction/*.hxx
                        ${LARSOFTREADERDIR}/src/2ndproduction/${DATAMCDIR}/*.hxx)

foreach(file0 ${highland_inc2})
   INSTALL(FILES ${file0} DESTINATION ${CMAKE_SOURCE_DIR}/include)
endforeach(file0)

#-------------------------------------------------------
#------- custom targets --------------------------------
#-------------------------------------------------------

# target to create dictionaries before libraries
add_custom_target(highland_dicts ALL SOURCES ${dict_list})

# target to change the library name in the rootmap files
add_custom_target(ch_rootmap COMMAND ${CMAKE_SOURCE_DIR}/scripts/chrootmap ${CMAKE_BINARY_DIR})

# target to install header files in the include folder
add_custom_target(include ALL SOURCES ${highland_inc2} ${CMAKE_SOURCE_DIR})

#-------------------------------------------------------
#------- dependencies ----------------------------------
#-------------------------------------------------------

add_dependencies(pion.exe       highland)
add_dependencies(highland       ch_rootmap)
add_dependencies(ch_rootmap     highland_dicts)
add_dependencies(highland_dicts include)


#-------------------------------------------------------
#------- other ------ ----------------------------------
#-------------------------------------------------------

#---Configure and install various files neded later and for clients -----------------------------
include(HighlandConfiguration)

#---To be used from the binary tree--------------------------------------------------------------------------
set(HIGHLAND_INCLUDE_DIR_SETUP "
# HIGHLAND configured for use from the build tree - absolute paths are used.
set(HIGHLAND_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
")
set(HIGHLAND_LIBRARY_DIR_SETUP "
# HIGHLAND configured for use from the build tree - absolute paths are used.
set(HIGHLAND_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
")
set(HIGHLAND_BINARY_DIR_SETUP "
# HIGHLAND configured for use from the build tree - absolute paths are used.
set(HIGHLAND_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
")
set(HIGHLAND_LIBRARIES_SETUP "
# HIGHLAND configured for use from the build tree - absolute paths are used.
set(HIGHLAND_LIBRARIES \"-L${CMAKE_SOURCE_DIR}/lib -lhighland\")
")

configure_file(${CMAKE_SOURCE_DIR}/cmake/scripts/HIGHLANDConfig.cmake.in
               ${CMAKE_SOURCE_DIR}/HIGHLANDConfig.cmake @ONLY NEWLINE_STYLE UNIX)


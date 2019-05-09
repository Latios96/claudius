#-*-cmake-*-
#
# yue.nicholas@gmail.com
#
# This auxiliary CMake file helps in find the MtoA headers and libraries
#
# MTOA_FOUND            set if MtoA is found.
# MTOA_INCLUDE_DIR      MtoA's include directory
# MTOA_mtoa_api_LIBRARY Full path location of libmtoa_api
# MTOA_LIBRARY          Alias for MTOA_mtoa_api_LIBRARY

find_package(PackageHandleStandardArgs)

##
## Obtain MtoA install location
##
set(MTOA_LOCATION $ENV{MTOA_LOCATION})

set(MTOA_LIBRARY_DIR ${MTOA_LOCATION}/lib)
set(MTOA_INCLUDE_DIR ${MTOA_LOCATION}/include)

message(STATUS "MTOA location: ${MTOA_LOCATION}")
message(STATUS "MTOA LIB:      ${MTOA_mtoa_api_LIBRARY}")
message(STATUS "MTOA LIB:      ${MTOA_LIBRARY}")
message(STATUS "MTOA include:  ${MTOA_INCLUDE_DIR}")

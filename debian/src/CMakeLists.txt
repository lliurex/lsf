
#main lsf lib
add_library(lsf-1.0 SHARED filesystem.cpp network.cpp system.cpp utils.cpp)

#n4d lib
add_library(lsf-1.0-n4d SHARED n4d.cpp)


#n4d link
find_package (XMLRPC REQUIRED c++2 client)
target_link_libraries(lsf-1.0-n4d ${XMLRPC_LIBRARIES})

#add include dirs
include_directories(${CMAKE_SOURCE_DIR}/include)

#add custom lib flags
add_definitions(-std=c++11 -O3)


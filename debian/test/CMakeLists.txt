

add_executable(test main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

#find_library(LSF lsf-1.0 ${CMAKE_CURRENT_SOURCE_DIR}/src)
#find_library(LSF_N4D  lsf-1.0-n4d ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package (XMLRPC REQUIRED c++2 client)

target_link_libraries(test ${XMLRPC_LIBRARIES} lsf-1.0 lsf-1.0-n4d)

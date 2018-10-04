
# ***  lsf main cmake ****
cmake_minimum_required (VERSION 2.6)
project (LSF)
add_subdirectory(src)
add_subdirectory(test)
add_custom_target(doc COMMAND doxygen) 

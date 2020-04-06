########################################################################################################################
#  编译
########################################################################################################################
unset(sources)
unset(subDiretoryList)
unset(subFileList)
string(REGEX REPLACE "/" "" module ${CMAKE_CURRENT_SOURCE_DIR})
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} sources)
foreach(var ${sources})
    target_sources(${PROJECT_NAME}_test PUBLIC ${var})
endforeach()
add_library(${module} ${sources})
########################################################################################################################
#  链接
########################################################################################################################
linkTestSubDirectories(${module} ${CMAKE_CURRENT_SOURCE_DIR})
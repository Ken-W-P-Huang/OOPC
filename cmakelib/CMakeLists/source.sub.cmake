########################################################################################################################
#  编译
########################################################################################################################
unset(temp)
unset(subDiretoryList)
unset(subFileList)
string(REGEX REPLACE "/" "" module ${CMAKE_CURRENT_SOURCE_DIR})
aux_source_directory(. temp)
add_library(${module} ${temp})
########################################################################################################################
#  链接
########################################################################################################################
linkSubDirectories(${module} ${CMAKE_CURRENT_SOURCE_DIR})

if (${IS_APP})
    unset(sources)
    string(REGEX REPLACE "/" "" module ${CMAKE_CURRENT_SOURCE_DIR})
    aux_source_directory(. sources)
    add_library(${module} ${sources})
    addSubDirectories(${module} ${CMAKE_CURRENT_SOURCE_DIR} )
else()
    FILE(GLOB asms ${CMAKE_CURRENT_SOURCE_DIR}/*.s ${CMAKE_CURRENT_SOURCE_DIR}/*.asm)
    set(sources ${sources} ${asms})
    aux_source_directory(${CMAKE_CURRENT_LIST_DIR} sources)
    addSubDirectories(module ${CMAKE_CURRENT_SOURCE_DIR} )
    set(sources ${sources} PARENT_SCOPE)
endif ()
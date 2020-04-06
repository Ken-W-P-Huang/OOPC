########################################################################################################################
#  编译
########################################################################################################################
unset(temp)
aux_source_directory(. temp)
add_executable(${PROJECT_NAME} ${temp})
########################################################################################################################
#  链接
########################################################################################################################
linkSubDirectories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
########################################################################################################################
#  安装
########################################################################################################################
install (TARGETS ${PROJECT_NAME} DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/source/include/*" DESTINATION include)


if (${IS_APP})
    unset(sources)
    unset(asms)
    FILE(GLOB asms ${CMAKE_CURRENT_SOURCE_DIR}/*.s ${CMAKE_CURRENT_SOURCE_DIR}/*.asm)
    set(sources ${sources} ${asms})
    aux_source_directory(. sources)
    add_executable(${PROJECT_NAME} ${sources})
    addSubDirectories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
    install (TARGETS ${PROJECT_NAME} DESTINATION bin)
    install (FILES "${PROJECT_BINARY_DIR}/source/include/*" DESTINATION include)
else()
    addSubDirectories(kernel ${CMAKE_CURRENT_SOURCE_DIR} sources)
    FILE(GLOB asms ${CMAKE_CURRENT_SOURCE_DIR}/*.s ${CMAKE_CURRENT_SOURCE_DIR}/*.asm)
    set(sources ${sources} ${asms})
    aux_source_directory(${CMAKE_CURRENT_LIST_DIR} sources)
    set(CMAKE_C_LINK_EXECUTABLE " ${LD} -T${LINKER_FILE} -m elf_i386 -o <TARGET> <OBJECTS> ")
    add_executable(kernel  ${sources} )
    set_target_properties(kernel PROPERTIES  SUFFIX .elf)
endif ()
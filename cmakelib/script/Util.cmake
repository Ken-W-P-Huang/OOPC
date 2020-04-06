function(getSourceFiles extensions path return)
    foreach (extension ${extensions})
        set(filter ${filter} "${path}/*.${extension}")
    endforeach ()
    FILE(GLOB_RECURSE sourceFiles ${filter})
    set(${return} ${sourceFiles} PARENT_SCOPE)
endfunction()

function(copy source destination)
    if(NOT EXISTS ${destination})
        if (EXISTS ${source})
            file(WRITE ${destination} "")
            file(GENERATE OUTPUT ${destination} INPUT ${source} )
        else()
            message(FATAL_ERROR "错误：${source}不存在!")
        endif ()
    endif ()
endfunction()

function(getSubDirectoryList directory isAbsolutePath return)
    FILE(GLOB children RELATIVE ${directory} ${directory}/*)
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${directory}/${child})
            if (isAbsolutePath)
                LIST(APPEND subDirectoryList ${directory}/${child})
            else()
                LIST(APPEND subDirectoryList ${child})
            endif ()
        ENDIF()
    ENDFOREACH()
    set(${return} ${subDirectoryList}  PARENT_SCOPE)
endfunction()

function(getSubFileList directory isAbsolutePath fileTypes return)
    foreach(fileType ${fileTypes})
        if (isAbsolutePath)
            FILE(GLOB temp "${directory}/*.${fileType}")
        else()
            FILE(GLOB temp RELATIVE ${directory} "${directory}/*.${fileType}")
        endif ()
        set(children ${children} ${temp})
    endforeach()

    FOREACH(child ${children})
        if (isAbsolutePath)
            IF(NOT IS_DIRECTORY ${child})
                LIST(APPEND subFileList ${child})
            ENDIF()
        else()
            IF(NOT IS_DIRECTORY ${directory}/${child})
                LIST(APPEND subFileList ${child})
            ENDIF()
        endif()
    ENDFOREACH()
    set(${return} ${subFileList} PARENT_SCOPE)
endfunction()

function(linkTestSubDirectories module directory)
    getSubDirectoryList(${directory} true subDiretoryList)
    foreach (subTestDiretory ${subDiretoryList})
        if (EXISTS ${subTestDiretory}/CMakeLists.txt)
            string(FIND "${subTestDiretory}" "test" prefixLength)
            string(SUBSTRING "${subTestDiretory}" 0 ${prefixLength} prefix)
            string(REGEX REPLACE "^${prefix}test" "${prefix}source" subDiretory ${subTestDiretory})
            string(REGEX REPLACE "/" "" subModule ${subDiretory})
            string(REGEX REPLACE "/" "" subTestModule ${subTestDiretory})
            add_subdirectory(${subTestDiretory})
            target_link_libraries(${module} ${subModule} ${subTestModule})
        endif ()
    endforeach ()
endfunction()


function(linkSubDirectories module directory)
    getSubDirectoryList(${directory} true subDiretoryList)
    foreach (subDiretory ${subDiretoryList})
        if (EXISTS ${subDiretory}/CMakeLists.txt)
            string(REGEX REPLACE "/" "" subModule ${subDiretory})
            add_subdirectory(${subDiretory})
            target_link_libraries(${module} ${subModule})
        endif ()
    endforeach ()
endfunction()

function(copyCMakeListsToDirectory directory suffixes content)
    set(fileList "")
    foreach(suffix ${suffixes})
        FILE(GLOB files "${directory}/*.${suffix}")
        set(fileList ${files} ${fileList})
    endforeach()

    list(LENGTH fileList length)
    if (length GREATER 0 AND NOT EXISTS ${directory}/CMakeLists.txt)
        copy(${content} ${directory}/CMakeLists.txt)
    endif ()
endfunction()

function(monitorCMakeLists directory isTest suffixes)
    if (${isTest})
        set(content ${CMAKE_LIB_PATH}/CMakeLists/test.sub.cmake)
    else()
        set(content ${CMAKE_LIB_PATH}/CMakeLists/source.sub.cmake)
    endif ()
    getSubDirectoryList(${directory} true stack)
    list(LENGTH stack length)
    while(length GREATER 0)
        math(EXPR index "${length} - 1" )
        list(GET stack ${index} element)
        list(REMOVE_AT stack ${index})
        copyCMakeListsToDirectory(${element} "${suffixes}" ${content})
        getSubDirectoryList(${element} true files)
        set(stack ${files} ${stack})
        list(LENGTH stack length)
    endwhile()
endfunction()

#监控新建源码文件，生成对应的测试文件。
function(monitorSource sourceDirectory testDirectory fileTypes)
    set(stack ${sourceDirectory})
    list(LENGTH stack length)
    while(length GREATER 0)
        math(EXPR index "${length} - 1" )
        list(GET stack ${index} element)
        list(REMOVE_AT stack ${index})
        IF(IS_DIRECTORY ${element})
            file(RELATIVE_PATH relativePart ${sourceDirectory} ${element})
            file(MAKE_DIRECTORY ${testDirectory}/${relativePart})
            FILE(GLOB temp "${element}/*")
            set(stack ${stack} ${temp})
        else()
            get_filename_component(extension ${element} EXT)
            if (NOT extension  STREQUAL "")
                string(SUBSTRING ${extension} 1 -1 shortExtension)
                if (${shortExtension} IN_LIST fileTypes)
                    string(REPLACE ${sourceDirectory} ${testDirectory} target ${element})
                    string(REGEX REPLACE "\\${extension}$" ".cpp" target ${target})
                    if(NOT EXISTS ${target})
                        get_filename_component(filename ${element} NAME_WE)
                        file(WRITE ${target} "#include \"gtest/gtest.h\"

extern \"C\"{

}

TEST(TEST, ${filename}) {

}")
                        #                    copy(${PROJECT_SOURCE_DIR}/cmakelib/template/test.cpp ${target})
                    endif()
                endif ()
            endif ()
        ENDIF()
        list(LENGTH stack length)
    endwhile()
endfunction()
########################################################################################################################
#PROJECT参数必填
#include(DownloadLibrary.cmake)
#downloadLibrary(PROJECT        googletest
#        GIT_REPOSITORY      https://github.com/google/googletest.git
#        GIT_TAG             master
#        PREFIX              ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest
#        UPDATE_DISCONNECTED 1)
########################################################################################################################
set(CURRENT_SCRIPT_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
function(downloadLibrary)
    set(options QUIET)
    set(oneValueArgs
            PROJECT
            PREFIX
            DOWNLOAD_DIR
            SOURCE_DIR
            BINARY_DIR)
    set(multiValueArgs CONFIGURE_COMMAND BUILD_COMMAND INSTALL_COMMAND TEST_COMMAND)
    cmake_parse_arguments(DL "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    #设置参数
    if (DL_QUIET)
        set(OUTPUT_QUIET "OUTPUT_QUIET")
    else()
        unset(OUTPUT_QUIET)
        message(STATUS "正在下载/更新项目 ${DL_PROJECT}")
    endif()
    if (NOT DL_PREFIX)
        set(DL_PREFIX "${CMAKE_BINARY_DIR}")
    endif()
    if (NOT DL_DOWNLOAD_DIR)
        set(DL_DOWNLOAD_DIR "${DL_PREFIX}/download")
    endif()
    if (NOT DL_SOURCE_DIR)
        set(DL_SOURCE_DIR "${DL_PREFIX}/source")
    endif()
    if (NOT DL_BINARY_DIR)
        set(DL_BINARY_DIR "${DL_PREFIX}/build")
    endif()
    #   设置下载超时时间
    #    if (NOT DL_TIMEOUT)
    #        set(DL_TIMEOUT 3600)
    #    endif ()
    set(${DL_PROJECT}_SOURCE_DIR "${DL_SOURCE_DIR}" PARENT_SCOPE)
    set(${DL_PROJECT}_BINARY_DIR "${DL_BINARY_DIR}" PARENT_SCOPE)
    #在库保存路径下生成CMakeLists.txt并开始下载库文件
    configure_file("${CURRENT_SCRIPT_DIR}/DownloadLibrary.CMakeLists.in"
            "${DL_DOWNLOAD_DIR}/CMakeLists.txt")
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            ${OUTPUT_QUIET}
            WORKING_DIRECTORY "${DL_DOWNLOAD_DIR}")
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            ${OUTPUT_QUIET}
            WORKING_DIRECTORY "${DL_DOWNLOAD_DIR}")
endfunction()

function(addGoogleTest directory)
    if (NOT EXISTS "${directory}/source/googletest/include" OR NOT EXISTS "${directory}/source/googlemock/include")
        file(REMOVE_RECURSE "${directory}")
        downloadLibrary(PROJECT     googletest
                GIT_REPOSITORY      https://github.com/google/googletest.git
                GIT_TAG             master
                PREFIX              ${directory}
                UPDATE_DISCONNECTED 1)
    endif ()

    if (EXISTS "${directory}/source" AND EXISTS "${directory}/build")
        # 防止GoogleTest和Visual Studio产生变量冲突
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        add_subdirectory(${directory}/source ${directory}/build)
        include_directories("${directory}/source/googletest/include"
                "${directory}/source/googlemock/include")
    else()
        message("${directory}/source or ${directory}/build does not exist.")
    endif ()

endfunction()
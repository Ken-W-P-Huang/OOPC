include(ExternalProject)
include(${CMAKE_CURRENT_LIST_DIR}/Util.cmake)
if(${IS_APP})
    include(${CMAKE_CURRENT_LIST_DIR}/target/APP.cmake)
else()
    include(${CMAKE_CURRENT_LIST_DIR}/target/OS.cmake)
endif()

if (CMAKE_SYSTEM MATCHES "deepin2")
    include(${CMAKE_CURRENT_LIST_DIR}/os/Deepin.cmake)
elseif (CMAKE_SYSTEM MATCHES "Windows")
    include(${CMAKE_CURRENT_LIST_DIR}/os/Windows.cmake)
elseif (CMAKE_SYSTEM MATCHES "FreeBSD")
    include(${CMAKE_CURRENT_LIST_DIR}/os/BSD.cmake)
elseif (CMAKE_SYSTEM MATCHES "Darwin")
    include(${CMAKE_CURRENT_LIST_DIR}/os/MacOS.cmake)
else ()
    include(${CMAKE_CURRENT_LIST_DIR}/os/Other.cmake)
endif ()

string(TOLOWER ${VCS_TYPE} VCS_TYPE)
if (VCS_TYPE STREQUAL "git" )
    include(${CMAKE_CURRENT_LIST_DIR}/vcs/Git.cmake)
endif()

get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
if ("ASM_NASM" IN_LIST languages)
    set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS asm nasm s S )
endif ()
set(CMAKE_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
cmake_policy(SET CMP0053 NEW)
find_package(Threads REQUIRED)
########################################################################################################################
#  配置项目
########################################################################################################################
macro(configureProject)
    if(COMMAND configureEnvironment)
        configureEnvironment()
    endif()
    configureCompilers(${PROJECT_SOURCE_DIR})
    if(COMMAND addParmsToSourceCode)
        addParmsToSourceCode()
    endif()
    if (NOT EXISTS ${PROJECT_SOURCE_DIR}/README.md)
        file(WRITE "${PROJECT_SOURCE_DIR}/README.md" "")
    endif ()
    configureVCS()
    #创建source目录和LICENSE文件
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/source)
    #删除根目录下的main.*文件
    file(GLOB fileList LIST_DIRECTORIES false ${PROJECT_SOURCE_DIR} "main.*")
    foreach (file ${fileList})
        file(COPY ${file} DESTINATION ${PROJECT_SOURCE_DIR}/source/)
        file(REMOVE ${file})
    endforeach ()
    if (DEFINED LICENSE)
        copy(${PROJECT_SOURCE_DIR}/cmakelib/license/${LICENSE} ${PROJECT_SOURCE_DIR}/LICENSE.txt)
    endif ()
    #添加第三方库
    if(COMMAND addLibraries)
        addLibraries(${PROJECT_SOURCE_DIR}/vendor)
    endif()
    include_directories("${PROJECT_SOURCE_DIR}/source/include")
    makeSoftware()
endmacro()
########################################################################################################################
#  测试
########################################################################################################################
function(testProject)
    addGoogleTest(${PROJECT_SOURCE_DIR}/vendor/googletest)
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/test/main.cpp)
        copy(${PROJECT_SOURCE_DIR}/cmakelib/template/testmain.cpp ${PROJECT_SOURCE_DIR}/test/main.cpp)
    endif()
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/test/CMakeLists.txt)
        copy(${PROJECT_SOURCE_DIR}/cmakelib/CMakeLists/test.cmake ${PROJECT_SOURCE_DIR}/test/CMakeLists.txt)
    endif()
    get_property(SOURCE_FILE_EXTENSIONS GLOBAL PROPERTY SOURCE_FILE_EXTENSIONS)
    monitorSource(${PROJECT_SOURCE_DIR}/source ${PROJECT_SOURCE_DIR}/test "${SOURCE_FILE_EXTENSIONS}")
    FILE(GLOB_RECURSE testFiles "test/*.cpp")
    get_property(sourceFiles GLOBAL PROPERTY sourceFiles)
    list(LENGTH sourceFiles length)
    if (length GREATER 0)
        list(FILTER sourceFiles EXCLUDE REGEX "main.c")
        list(FILTER sourceFiles EXCLUDE REGEX "main.cpp")
    endif ()
    add_executable(${PROJECT_NAME}_test ${testFiles} ${sourceFiles})
    target_link_libraries(${PROJECT_NAME}_test gtest gmock)
endfunction()
########################################################################################################################
#  打包
########################################################################################################################
#打包cpack --config CPackConfig.cmake
#源码包 cpack --config CPackSourceConfig.cmake
function(packProject)
    include (InstallRequiredSystemLibraries)
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    include (CPack)
    add_custom_target(pack${PROJECT_NAME}
            COMMAND echo "Packing Application ..."
            COMMAND cpack --config CPackConfig.cmake
            COMMAND cpack --config CPackSourceConfig.cmake
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    #add_dependencies(pack${PROJECT_NAME} ${PROJECT_NAME})
endfunction()
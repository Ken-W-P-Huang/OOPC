cmake_minimum_required(VERSION 3.13.2)
project(OOPC VERSION 1.10.0.1 LANGUAGES C ASM_NASM CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
set (CMAKE_VERBOSE_MAKEFILE false)
set(IS_APP true)
set(ARCH 32)
#设置版本控制类型及远程仓库，目前仅支持git
set(VCS_TYPE "Git")
set(GIT_REMOTES "origin;https://github.com/Ken-W-P-Huang/OOPC.git")
set(LICENSE "Apache-2.0")
include(${PROJECT_SOURCE_DIR}/cmakelib/script/MakeProject.cmake)
########################################################################################################################
#  配置源码参数
########################################################################################################################
function(addParmsToSourceCode)
    #        file(WRITE source/include/conf.h "// the configured options and settings for ${PROJECT_NAME}
    ##define VERSION_MAJOR @PROJECT_VERSION_MAJOR@
    ##define VERSION_MINOR @PROJECT_VERSION_MINOR@
    #")
    #    unset(USE_NATIVE_MATH CACHE)
    #    unset(HAVE_LOG CACHE)
    #    unset(HAVE_EXP CACHE)
    #    #模块自检
    #    include (CheckFunctionExists)
    #    check_function_exists (log HAVE_LOG)
    #    check_function_exists (exp HAVE_EXP)
    #    #是否使用本地方法
    #    option (USE_NATIVE_MATH "Use tutorial provided math implementation" ON)
    #    file(APPEND ${PROJECT_SOURCE_DIR}/config/config.h.in "
    ##cmakedefine USE_NATIVE_MATH
    ##cmakedefine HAVE_LOG
    ##cmakedefine HAVE_EXP")
endfunction()
########################################################################################################################
#   添加第三方库
########################################################################################################################
function(addLibraries directory)
    #调用downloadLibrary下载库
endfunction()
########################################################################################################################
#   编译项目
########################################################################################################################
configureProject()
testProject()










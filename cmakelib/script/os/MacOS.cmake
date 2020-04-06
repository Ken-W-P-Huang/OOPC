
function(findProgram return programName packageName)
    set(count 0)
    unset(programPath CACHE)
    while (true)
        if (${count} GREATER 3)
            message(FATAL_ERROR "Error：fail to install ${programName}")
        endif ()
        if (${programName} STREQUAL "i386-gnu-ld")
            find_program(programPath ${programName} PATH ${PROJECT_SOURCE_DIR}/vendor/binutils/bin/)
        else()
            find_program(programPath ${programName})
        endif ()
        if ( ${programPath} STREQUAL "programPath-NOTFOUND")
            message(STATUS "Installing ${programName}")
            if (${programName} STREQUAL "i386-gnu-ld")
                downloadLibrary(PROJECT binutils
                                URL http://ftp.gnu.org/gnu/binutils/binutils-2.27.tar.gz
                                PREFIX ${PROJECT_BINARY_DIR}/vendor/binutils-2.27
                                CONFIGURE_COMMAND ../source/configure --prefix=${PROJECT_SOURCE_DIR}/vendor/binutils
                                --target=i386-gnu
                                --enable-interwork
                                --enable-multilib
                                --disable-nls
                                --disable-werror
                                --host=x86_64-apple-darwin18.6.0
                                BUILD_COMMAND make
                                INSTALL_COMMAND make install)
            else()
                execute_process(COMMAND brew install ${packageName})
            endif ()
            math(EXPR count "${count} + 1" )
            if (${programName} STREQUAL "NASM")
                message(FATAL_ERROR "请重启电脑以便让nasm生效！")
            endif ()
        else()
            set(${return} ${programPath} PARENT_SCOPE)
            message(STATUS "Found ${programPath}")
            break()
        endif ()
    endwhile ()
endfunction()

macro(configureEnvironment )
    #todo 检查版本
    #nasm
    findProgram(program nasm nasm)
    set(NASM ${program})
    #gdb
    findProgram(program gdb gdb)
    set(GDB ${program})
    #cgdb
    findProgram(program cgdb cgdb)
    set(CGDB ${program})
    if (NOT ${IS_APP})
        #qemu-system-i386
        findProgram( program qemu-system-i386 "qemu")
        set(QEMU ${program})
        #RUBY
        findProgram(program ruby ruby)
        set(RUBY ${program})
        #mkisofs
        findProgram(program mkisofs "cdrtools")
        set(MKISOFS ${program})
        #ld
        findProgram(program i386-gnu-ld ld)
        set(LD ${program})
        #bash
        findProgram(program bash bash)
        set(BASH ${program})
    endif ()
endmacro()
#汇编文件source/kernel/boot/Entry.asm被载入的顺序会影响编译结果。它应该是第一个源文件，使用文件夹可以影响这种顺序!!!
set(KERNEL_FILE "${PROJECT_BINARY_DIR}/kernel.elf")
set(IMAGE_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.img")
set(ISO_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.iso")
set(QEMU_LINK_FILE "${PROJECT_BINARY_DIR}/qemu")
set(BASH_LINK_FILE "${PROJECT_BINARY_DIR}/bash")
set(FILE_SYSTEM_FILE "${PROJECT_BINARY_DIR}/file_system")
set(SHELL_BINARY_DIR ${PROJECT_BINARY_DIR}/shell)
set(BOOTLOADER_DIR "${PROJECT_SOURCE_DIR}/source/bootloader")
set(RESOURCE_DIR "${PROJECT_SOURCE_DIR}/source/resource")
set(LINKER_FILE "${RESOURCE_DIR}/ldscript/linker.ld")
set(BUILD_FILE_SYSTEM_SCRIPT_FILE "${RESOURCE_DIR}/bash/build_file_system")
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")

function(makeISO targetList)
    add_custom_target(RunOS DEPENDS ${targetList})
    #qemu-system-i386 -s -fda OS.iso & gdb -ex "target remote localhost:1234" -ex "symbol-file kernel_d.elf"
    add_custom_target(DebugOS DEPENDS kernel_d RunOS
                            WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    if (NOT EXISTS ${QEMU_LINK_FILE})
        execute_process(COMMAND ln -sf  ${QEMU} ${QEMU_LINK_FILE} )
    endif ()
    if (NOT EXISTS ${PROJECT_BINARY_DIR}/qemu.log)
        add_custom_command(TARGET RunOS COMMAND touch ${PROJECT_BINARY_DIR}/qemu.log)
    endif ()
    if (NOT EXISTS ${SHELL_BINARY_DIR})
        add_custom_command(TARGET RunOS COMMAND mkdir ${SHELL_BINARY_DIR})
    endif ()
    if (NOT EXISTS ${BASH_LINK_FILE})
        execute_process(COMMAND ln -sf  ${BASH} ${BASH_LINK_FILE} )
    endif ()

    if ("bootloader" IN_LIST targetList)
        #自定义启动器 -monitor stdio OS.iso -serial file:qemu.log
        add_custom_command(TARGET RunOS
                WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                COMMAND echo "Generating image..."
                COMMAND rm -rf iso && mkdir -p iso/boot
                COMMAND cat bootloader.bin kernel.bin > OS.iso
                )

    else()
        #使用grub启动 -monitor stdio -cdrom OS.iso -serial file:qemu.log
        add_custom_command(TARGET RunOS
                WORKING_DIRECTORY ${SHELL_BINARY_DIR_PATH}
                COMMAND echo generating file_system..
                COMMAND rm -rf  ${FILE_SYSTEM_PATH} tmp test
                COMMAND mkdir tmp && cp ${BUILD_FILE_SYSTEM_SCRIPT_FILE} tmp/build_file_system
                COMMAND mkdir test && echo "hello world" >> test/helloworld.txt
                COMMAND tmp/build_file_system
                )
        add_custom_command(TARGET RunOS
                WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                COMMAND echo "Generating image..."
                COMMAND pwd && rm -rf iso && mkdir -p iso/boot/grub
                COMMAND mkdir -p iso/modules
                COMMAND cp ${BOOTLOADER_DIR}/stage2_eltorito ${BOOTLOADER_DIR}/menu.lst iso/boot/grub/
                COMMAND cp ${KERNEL_FILE} iso/boot/
                COMMAND cp -R ${FILE_SYSTEM_FILE} iso/modules
                COMMAND mkisofs -R -b boot/grub/stage2_eltorito
                -no-emul-boot
                -boot-load-size 4
                -A os
                -input-charset utf8
                -quiet
                -boot-info-table
                -o ${ISO_FILE}
                iso)
    endif ()
endfunction()

macro(makeSoftware)
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/source
                        ${CMAKE_SOURCE_DIR}/source/bootloader
                        ${CMAKE_SOURCE_DIR}/source/kernel
                        ${CMAKE_SOURCE_DIR}/source/resource
                        ${CMAKE_SOURCE_DIR}/source/shell)
    get_property(SOURCE_FILE_EXTENSIONS GLOBAL PROPERTY SOURCE_FILE_EXTENSIONS)
    foreach (target "bootloader" "kernel" "shell")
        getSourceFiles("${SOURCE_FILE_EXTENSIONS}" "source/${target}" sourceFiles)
        list(LENGTH sourceFiles length)
        if (length GREATER 0)
            list(APPEND targetList "${target}")
            set(tempSourceFiles ${tempSourceFiles} ${sourceFiles} )
            if (${target} STREQUAL "bootloader")
                add_executable(${target} ${sourceFiles} )
                set_target_properties(${target} PROPERTIES  SUFFIX .bin
                        COMPILE_FLAGS "-f bin"
                        LANGUAGE ASM_NASM
                        LINK_FLAGS "--oformat binary -Ttext 0x7C00"
                        )
            elseif(${target} STREQUAL "shell")
                set(objectFiles ${PROJECT_BINARY_DIR}/CMakeFiles/kernel.dir/source/kernel/stdlib/stdio.c.o
                        ${PROJECT_BINARY_DIR}/CMakeFiles/kernel.dir/source/kernel/stdlib/string.c.o
                        ${PROJECT_BINARY_DIR}/CMakeFiles/kernel.dir/source/kernel/stdlib/syscalls.asm.o)
                add_custom_command(OUTPUT ${objectFiles}  DEPENDS kernel)
                add_executable(${target} ${sourceFiles} ${objectFiles})
                set_target_properties(shell PROPERTIES  SUFFIX .bin
                        LINK_FLAGS "-T${PROJECT_SOURCE_DIR}/source/resource/ldscript/link_user_program.ld \
                                    --ignore-unresolved-symbol _GLOBAL_OFFSET_TABLE_")
            elseif(${target} STREQUAL "kernel")
                add_executable(${target} ${sourceFiles} )
                set_target_properties(${target} PROPERTIES  SUFFIX .bin
#                                      LINK_FLAGS "-T${PROJECT_SOURCE_DIR}/source/resource/ldscript/link.ld"
                                      LINK_FLAGS "-Ttext 0x1000 --oformat binary"
                        )
                add_executable(${target}_d ${sourceFiles} )
                set_target_properties(${target}_d PROPERTIES  SUFFIX .elf
                                        LINK_FLAGS "-Ttext 0x1000"
                                        )
            endif ()
        endif ()
    endforeach ()
    makeISO("${targetList}")
endmacro()

#todo 设置编译选项，要使全局变量生效，只能使用macro而非function 能否在function下直接设置CMAKE_ASM_NASM_FLAGS？
macro(configureCompilers)
    get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
    #ASM识别.s和.S文件；ASM_NASM识别.asm和.nasm文件。它们被当作两种语言。这里将.s文件当作asm include文件
    if("ASM_NASM" IN_LIST languages)
        set(SOURCE_FILE_EXTENSIONS ${SOURCE_FILE_EXTENSIONS} ${CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS})
        #用-f elf 指定为32位，64位则为-f elf64 CMAKE_ASM_NASM_FLAGS会被Target设置的COMPILE_FLAGS覆盖
        set(CMAKE_ASM_NASM_FLAGS "-f elf")
#        set(CMAKE_ASM_NASM_OBJECT_FORMAT "bin")
        set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>")
        set(CMAKE_ASM_NASM_LINK_EXECUTABLE "mv <OBJECTS>  <TARGET>")
#        set(CMAKE_ASM_NASM_LINK_EXECUTABLE "${LD} <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES> -o <TARGET> <OBJECTS>")
#        set(CMAKE_ASM_NASM_INCLUDES "-I${PROJECT_SOURCE_DIR}/source/asm" "无效")
        include_directories(${PROJECT_SOURCE_DIR}/source/bootloader/inc)
    endif()
    if("C" IN_LIST languages)
        set(SOURCE_FILE_EXTENSIONS ${SOURCE_FILE_EXTENSIONS} ${CMAKE_C_SOURCE_FILE_EXTENSIONS})
        #查找路径以便供编译器使用 -nostartfiles
        set(CMAKE_C_FLAGS "-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector  -nodefaultlibs \
-Wall -Wextra -Werror -c -ffreestanding --target=x86_64-elf")
        set(CMAKE_C_LINK_EXECUTABLE "${LD} -m elf_i386 <LINK_FLAGS> -o <TARGET> <OBJECTS>")
endif()
    if("CXX" IN_LIST languages)
        set(SOURCE_FILE_EXTENSIONS ${SOURCE_FILE_EXTENSIONS} ${CMAKE_CXX_SOURCE_FILE_EXTENSIONS})
    endif()
    set_property(GLOBAL PROPERTY SOURCE_FILE_EXTENSIONS ${SOURCE_FILE_EXTENSIONS})
endmacro()
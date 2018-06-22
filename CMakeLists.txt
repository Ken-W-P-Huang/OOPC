cmake_minimum_required(VERSION 3.8)
project(ObjectTest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        object/foundation/Object.c
        object/foundation/Object.h
        object/Point.c
        object/Point.h
        object/Person.h
        object/Person.c
        CMakeLists.txt
        object/debug/Debug.h
        main.c object/foundation/Environment.c object/foundation/Environment.h object/Circle.c object/Circle.h object/foundation/collection/LinkedList.c object/foundation/collection/LinkedList.h object/foundation/reflect/Method.c object/foundation/reflect/Method.h object/foundation/reflect/Field.c object/foundation/reflect/Field.h object/foundation/type/Integer.c object/foundation/type/Integer.h object/foundation/type/String.c object/foundation/type/String.h object/foundation/collection/LinkedListNode.c
        object/foundation/type/type.h object/foundation/collection/HashMap.c object/foundation/collection/HashMap.h object/foundation/exception/Exception.c object/foundation/exception/Exception.h object/foundation/collection/HashMapEntry.c object/foundation/collection/HashMapEntryIterator.c object/foundation/collection/LinkedListIterator.c object/foundation/collection/ArrayList.c object/foundation/collection/ArrayList.h object/foundation/collection/ArrayListIterator.c object/foundation/type/Long.c object/foundation/type/Long.h object/foundation/type/Float.c object/foundation/type/Float.h object/foundation/type/Double.c object/foundation/type/Double.h object/foundation/type/Bool.c object/foundation/type/Bool.h object/foundation/type/Byte.c object/foundation/type/Byte.h object/foundation/type/Short.c object/foundation/type/Short.h object/foundation/type/Character.c object/foundation/type/Character.h object/foundation/type/Pointer.c object/foundation/type/Pointer.h object/foundation/thread/Thread.c object/foundation/thread/Thread.h object/foundation/interface/Runnable.c object/foundation/interface/Runnable.h object/foundation/interface/Interface.c object/foundation/interface/Interface.h object/foundation/thread/Lock.c object/foundation/thread/Lock.h object/Singleton.c object/Singleton.h object/foundation/reflect/Access.c object/foundation/reflect/Access.h)

add_executable(ObjectTest ${SOURCE_FILES})
find_package(Threads REQUIRED)
target_link_libraries(ObjectTest Threads::Threads)

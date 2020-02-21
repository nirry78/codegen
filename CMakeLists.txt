cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(CodeGenerator)

add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)

add_subdirectory(json)

if (UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Werror")
endif()

#if(UNIX)
#    add_definitions(-ggdb)
#endif()

set(SRCS 
    Container.cpp
    Document.cpp
    Formatter.cpp
    JsonReader.cpp
    Logging.cpp
    Field.cpp
    Tag.cpp
    CodeGenerator.cpp)

set_source_files_properties(DataStore.template.cpp PROPERTIES HEADER_FILE_ONLY TRUE)

set(HDRS
    Container.h
    CodeGenerator.h
    Document.h
    Formatter.h
    Logging.h
    JsonReader.h
    Field.h
    Tag.h)

if(WIN32)
    set (APP_NAME CodeGenerator)
    set (TEST_APP_NAME TestCodeGenerator)
else()
    set (APP_NAME code_generator)
    set (TEST_APP_NAME test_code_generator)
endif()

add_definitions("-D_CRT_SECURE_NO_WARNINGS")

add_executable(${APP_NAME} ${SRCS} ${HDRS})
if (UNIX)
    set_target_properties(${APP_NAME} PROPERTIES CXX_STANDARD 17)
else()
    set_target_properties(${APP_NAME} PROPERTIES CXX_STANDARD 20)
endif()

target_link_libraries(${APP_NAME} PRIVATE nlohmann_json::nlohmann_json)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/DataStore.h
                   COMMAND ${CMAKE_BINARY_DIR}/${APP_NAME} 
                           "${CMAKE_SOURCE_DIR}/test/data.json" 
                           "${CMAKE_SOURCE_DIR}/DataStore.template.h" 
                           "${CMAKE_BINARY_DIR}/DataStore.h"
                           "${CMAKE_BINARY_DIR}/DataStore.h.log"
                   DEPENDS ${APP_NAME} test/data.json DataStore.template.h)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/DataStore.cpp
                   COMMAND ${CMAKE_BINARY_DIR}/${APP_NAME} 
                           "${CMAKE_SOURCE_DIR}/test/data.json" 
                           "${CMAKE_SOURCE_DIR}/DataStore.template.cpp" 
                           "${CMAKE_BINARY_DIR}/DataStore.cpp"
                           "${CMAKE_BINARY_DIR}/DataStore.cpp.log"
                   DEPENDS ${APP_NAME} test/data.json DataStore.template.cpp)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(${TEST_APP_NAME} test/TestRunner.cxx ${CMAKE_BINARY_DIR}/DataStore.cpp ${CMAKE_BINARY_DIR}/DataStore.h)
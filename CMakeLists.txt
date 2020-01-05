cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(CodeGenerator)

add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)

add_subdirectory(json)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#if(UNIX)
#    add_definitions(-ggdb)
#endif()

set(SRCS 
    JsonReader.h
    JsonReader.cpp
    Logging.h
    Logging.cpp
    DataStore.template.cpp
    DataType.cpp
    Message.h
    Message.cpp
    MessageField.h
    MessageField.cpp
    Tag.cpp
    Tag.h
    CodeGenerator.cpp)

set_source_files_properties(DataStore.template.cpp PROPERTIES HEADER_FILE_ONLY TRUE)

set(HDRS
    #DataStore.h
    #DataStore.template.h
    DataType.h
    CodeGenerator.h)

if(WIN32)
    set (APP_NAME CodeGenerator)
else()
    set (APP_NAME code_generator)
endif()

add_definitions("-D_CRT_SECURE_NO_WARNINGS")

add_executable(${APP_NAME} ${SRCS} ${HDRS})
set_target_properties(${APP_NAME} PROPERTIES CXX_STANDARD 17)

target_link_libraries(${APP_NAME} PRIVATE nlohmann_json::nlohmann_json)
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(CodeGenerator)

set(SRCS 
    #DataStore.csv
    #DataStore.cpp
    #DataStore.template.cpp
    DataType.cpp
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


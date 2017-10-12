cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(CodeGenerator)

set(SRCS 
    CodeGenerator.cpp)

set(HDRS
    CodeGenerator.h)

if(WIN32)
    set (APP_NAME CodeGenerator)
else()
    set (APP_NAME code_generator)
endif()

add_definitions("-D_CRT_SECURE_NO_WARNINGS")

add_executable(${APP_NAME} ${SRCS} ${HDRS})


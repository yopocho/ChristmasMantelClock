cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(ChristmasMantelClock)

FILE(GLOB_RECURSE app_sources src/*.c src/ui/*.c)
FILE(GLOB INCLUDES "src/ui")

target_include_directories(app PRIVATE src/ui)

target_sources(app PRIVATE src/main.c ${app_sources})
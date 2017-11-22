cmake_minimum_required(VERSION 3.7)
project(eceCraft)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        auto.c
        auto.h
        chaine.c
        chaine.h
        chaine_b.c
        chaine_b.h
        draw.c
        draw.h
        evennement.c
        evennement.h
        load.c
        load.h
        main.c
        menu.c
        menu.h)

add_executable(eceCraft ${SOURCE_FILES})
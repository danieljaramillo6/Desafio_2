cmake_minimum_required(VERSION 3.16)

project(mundial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mundial main.cpp
    jugador.h jugador.cpp
    seleccion.h seleccion.cpp
    grupo.h grupo.cpp
    partido.h partido.cpp
    manager.cpp
    manager.h)

include(GNUInstallDirs)
install(TARGETS mundial
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

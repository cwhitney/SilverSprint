cmake_minimum_required(VERSION 3.10)
project(SilverSprint)

set(CMAKE_CXX_STANDARD 17)

# Define o caminho do Cinder (user-defined)
set(CINDER_PATH "" CACHE PATH "Path to Cinder root directory.")
if(NOT CINDER_PATH)
    message(FATAL_ERROR "CINDER_PATH not defined. Please specify the path to your Cinder installation.")
endif()


# Encontre as bibliotecas do sistema (X11, cURL, Fontconfig)
find_package(X11 REQUIRED)
find_package(CURL REQUIRED)
find_package(Fontconfig REQUIRED)

# Include directories
include_directories(
    ${CINDER_PATH}/include
    include
    blocks/OSC/src
    blocks/Serial/include
    blocks/Serial/lib/serial/include
    blocks/Sharkbox
    ../SilverSprints/blocks/Sharkbox
    ../SilverSprints/include
    blocks/Serial/lib/serial/include/serial/
    blocks/ParticleSystem
)

# Fontes principais
file(GLOB APP_SRC
    src/app/*.cpp
    src/data/*.cpp
    src/ui/*.cpp
    src/views/*.cpp
    ../SilverSprints/src/*.cpp
    ../SilverSprints/src/ui/*.cpp
)

# Fontes dos blocks
set(BLOCK_SRC
    blocks/OSC/src/cinder/osc/Osc.cpp
    blocks/ParticleSystem/ParticleSystem.cpp
    blocks/Serial/src/SerialDevice.cpp
    blocks/Serial/lib/serial/src/serial.cc
    blocks/Serial/lib/serial/src/impl/unix.cc
    blocks/Serial/lib/serial/src/impl/list_ports/list_ports_linux.cc
)

# Junta tudo
add_executable(${PROJECT_NAME}
    ${APP_SRC}
    ${BLOCK_SRC}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    # As bibliotecas do Cinder
    ${CINDER_LIBRARIES}
    /home/gabs/dev/rolofest/Cinder/lib/linux/x86_64/ogl/Release/libcinder.a

    # Bibliotecas do sistema Linux
    ${X11_LIBRARIES}
    ${X11_Xrandr_LIB}
    ${X11_Xinerama_LIB}
    ${X11_Xcursor_LIB}
    
    # Outras bibliotecas
    ${CURL_LIBRARIES}
    ${Fontconfig_LIBRARIES}
    ${Boost_LIBRARIES}
    X11
)

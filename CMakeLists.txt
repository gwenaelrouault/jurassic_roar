cmake_minimum_required(VERSION 3.28)

project(jurassic_roar LANGUAGES CXX)

# C++20 pour les modules
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Récupérer les fichiers
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/sources/*.cpp"
)

file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_SOURCE_DIR}/includes/*.h"
)

file(GLOB_RECURSE MODULE_FILES
    "${CMAKE_SOURCE_DIR}/modules/*.ixx"
)

# Créer la cible (exécutable)
add_executable(jurassic_roar)

# Sources "classiques" (cpp, h)
target_sources(jurassic_roar
    PRIVATE
        ${SRC_FILES}
        ${HEADER_FILES}
)

# Déclaration des modules C++20 dans un FILE_SET de type CXX_MODULES
target_sources(jurassic_roar
    PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES
        BASE_DIRS
            ${CMAKE_SOURCE_DIR}/modules
        FILES
            ${MODULE_FILES}
)

# Répertoires d'include classiques
target_include_directories(jurassic_roar
    PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
)

target_link_directories(jurassic_roar
    PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/SFML
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics-s
    sfml-window-s
    sfml-audio-s
    sfml-system-s
    udev
    X11
    Xrandr
    Xcursor
    Xi
    Xxf86vm
    freetype
    FLAC
    ogg
    vorbis
    vorbisenc
    vorbisfile
)
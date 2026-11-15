cmake_minimum_required(VERSION 3.28)
project(shooter-game LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(
    main 
    main.cpp 
    game/game.cpp 
    game/game.h
    item/item.cpp
    item/item.h 
    player/player.cpp
    player/player.h 
    bullet/bullet.cpp
    bullet/bullet.h
    resource_holder/template/resource_holder.hpp
    resource_holder/template/resource_holder.inl
    resource_holder/template/resource_identifiers.h
    resource_holder/texture_holders.h
)

target_compile_features(main PRIVATE cxx_std_23)
target_link_libraries(main PRIVATE SFML::Graphics)
target_link_libraries(main PRIVATE SFML::Window)
target_link_libraries(main PRIVATE SFML::Network)
target_link_libraries(main PRIVATE SFML::Audio)

include(FetchContent)
FetchContent_Declare(Candle
    GIT_REPOSITORY https://github.com/MiguelMJ/Candle.git
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(Candle)
target_link_libraries(main PRIVATE Candle)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

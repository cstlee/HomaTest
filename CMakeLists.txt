cmake_minimum_required (VERSION 3.11)

project(HomaTest VERSION 0.0.1.0 LANGUAGES CXX)

################################################################################
## Dependency Configuration ####################################################
################################################################################

# Tell CMake where to find our custom/3rd-party "Find" modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Homa 0.1.0 CONFIG REQUIRED)

################################################################################
## Target Defintions ###########################################################
################################################################################

add_executable(client
    src/client.cc
)
target_link_libraries(client
    Homa
)

add_executable(server
    src/server.cc
)
target_link_libraries(server
    Homa
)
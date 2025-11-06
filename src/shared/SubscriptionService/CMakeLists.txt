# ────────────────────────────────────────────────────────────────
#  SubscriptionService — lightweight external module
# ────────────────────────────────────────────────────────────────

cmake_minimum_required(VERSION 3.16)
project(SubscriptionService LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

set(DEP_LIST
    Quick
    Multimedia
)

# ────────────────────────────────────────────────────────────────
#  Library declaration
# ────────────────────────────────────────────────────────────────

file(GLOB_RECURSE PRIVATE_SRC
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${PRIVATE_SRC}
)

# ────────────────────────────────────────────────────────────────
#  Include directories
# ────────────────────────────────────────────────────────────────

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
        ${CMAKE_CURRENT_SOURCE_DIR}/src/base/nodes
        ${CMAKE_CURRENT_SOURCE_DIR}/src/base/managers
)

# ────────────────────────────────────────────────────────────────
#  Include dependencies
# ────────────────────────────────────────────────────────────────

foreach(dep IN LISTS DEP_LIST)
    find_package(Qt6 REQUIRED COMPONENTS ${dep})
endforeach()

foreach(dep IN LISTS DEP_LIST)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt6::${dep}
    )
endforeach()


# ────────────────────────────────────────────────────────────────
#  Compilation settings (optional)
# ────────────────────────────────────────────────────────────────

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# ────────────────────────────────────────────────────────────────
#  If used standalone
# ────────────────────────────────────────────────────────────────
message(STATUS "Building external module: ${PROJECT_NAME}")

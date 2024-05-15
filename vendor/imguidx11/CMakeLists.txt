project(imgui)

file(GLOB SRC_IMGUI
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "backends/imgui_impl_win32.cpp"
    "backends/imgui_impl_dx11.cpp"
    "misc/cpp/imgui_stdlib.cpp"
)

add_library(imgui STATIC ${SRC_IMGUI})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "imgui" FILES ${SRC_IMGUI})
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "backends"
    "misc/cpp"
)
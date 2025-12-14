cmake_minimum_required(VERSION 3.28.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(NightreignTimer)

set(FullOutputDir "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")
set(CMAKE_DEFAULT_STARTUP_PROJECT ${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
				src/main.cpp
				src/Constants.h
				src/Stopwatch.h
				src/Win32App.h
				src/Win32App.cpp
				src/D3D11App.h
				src/D3D11App.cpp
				src/NightreignLayer.h
				src/NightreignLayer.cpp
				ImGui/imgui.cpp
				ImGui/imconfig.h
				ImGui/imgui.h
				ImGui/imgui_demo.cpp
				ImGui/imgui_draw.cpp
				ImGui/imgui_internal.h
				ImGui/imgui_tables.cpp
				ImGui/imgui_widgets.cpp
				ImGui/imstb_rectpack.h
				ImGui/imstb_textedit.h
				ImGui/imstb_truetype.h
				ImGui/backends/imgui_impl_dx11.cpp
				ImGui/backends/imgui_impl_dx11.h
				ImGui/backends/imgui_impl_win32.cpp
				ImGui/backends/imgui_impl_win32.h
)

target_link_libraries(${PROJECT_NAME} d3d11)

set_target_properties(${PROJECT_NAME} PROPERTIES
	LINK_FLAGS /SUBSYSTEM:WINDOWS
)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
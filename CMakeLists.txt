cmake_minimum_required(VERSION 3.5.0)
project(SweetHome VERSION 1.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(OpenGL REQUIRED)

#Menu
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


#[[ Prueba Modelos]]
#add_executable(Main src/ModelsMain.cpp src/glad.c src/stb_image.cpp)
#target_link_libraries(Main PRIVATE OpenGL::GL glfw -lassimp)

#[[Prueba Menu]]
add_executable(main src/menuTest.cpp src/Menu.cpp)
target_link_libraries(main PRIVATE sfml-graphics )
target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()

include_directories(${CMAKE_SOURCE_DIR}/Include)
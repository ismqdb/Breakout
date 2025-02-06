cmake_minimum_required(VERSION 3.10)
project(AllegroProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Function to check and install a package (Linux/Windows)
function(check_and_install package install_cmd)
    execute_process(
        COMMAND pkg-config --exists ${package}
        RESULT_VARIABLE package_found
    )
    if(NOT package_found EQUAL 0)
        message(STATUS "${package} not found! Installing...")
        execute_process(
            COMMAND ${install_cmd}
            RESULT_VARIABLE install_result
            OUTPUT_QUIET ERROR_QUIET
        )
        if(NOT install_result EQUAL 0)
            message(FATAL_ERROR "Failed to install ${package}. Please install it manually.")
        endif()
    endif()
endfunction()

# --- Linux (Fedora) Automatic Installation ---
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Detected Fedora Linux. Checking Allegro installation...")
    check_and_install(allegro-5 "sudo dnf install -y allegro allegro-devel")
    check_and_install(allegro_main-5 "sudo dnf install -y allegro allegro-devel")
    check_and_install(allegro_image-5 "sudo dnf install -y allegro allegro-devel")
    check_and_install(allegro_font-5 "sudo dnf install -y allegro allegro-devel")
    check_and_install(allegro_ttf-5 "sudo dnf install -y allegro allegro-devel")
    check_and_install(allegro_audio-5 "sudo dnf install -y allegro allegro-devel")
    check_and_install(allegro_acodec-5 "sudo dnf install -y allegro allegro-devel")
    check_and_install(allegro_primitives-5 "sudo dnf install -y allegro allegro-devel")
endif()

# --- Windows (Automatic vcpkg Installation) ---
if(WIN32)
    message(STATUS "Detected Windows. Checking Allegro installation...")

    # Check if vcpkg is installed
    execute_process(
        COMMAND vcpkg version
        RESULT_VARIABLE VCPKG_FOUND
        OUTPUT_QUIET ERROR_QUIET
    )

    # If vcpkg is not found, install it
    if(NOT VCPKG_FOUND EQUAL 0)
        message(STATUS "vcpkg not found! Installing...")
        execute_process(
            COMMAND git clone https://github.com/microsoft/vcpkg.git ${CMAKE_SOURCE_DIR}/vcpkg
            RESULT_VARIABLE GIT_CLONE_RESULT
        )
        if(NOT GIT_CLONE_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to clone vcpkg. Please install it manually.")
        endif()

        # Bootstrap vcpkg
        execute_process(
            COMMAND ${CMAKE_SOURCE_DIR}/vcpkg/bootstrap-vcpkg.bat
            RESULT_VARIABLE BOOTSTRAP_RESULT
        )
        if(NOT BOOTSTRAP_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to bootstrap vcpkg. Please install it manually.")
        endif()
    endif()

    # Install Allegro with vcpkg
    message(STATUS "Installing Allegro using vcpkg...")
    execute_process(
        COMMAND ${CMAKE_SOURCE_DIR}/vcpkg/vcpkg.exe install allegro5:x64-windows
        RESULT_VARIABLE VCPKG_INSTALL_RESULT
    )
    if(NOT VCPKG_INSTALL_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to install Allegro via vcpkg.")
    endif()

    # Use vcpkg toolchain
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Find Allegro
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5)

# Add executable
add_executable(breakout InitFuncs.c FuncDefs.c GameLoop.c Breakout.c)

# Link Allegro libraries
target_include_directories(breakout PRIVATE ${ALLEGRO_INCLUDE_DIRS})
target_link_libraries(breakout PRIVATE ${ALLEGRO_LIBRARIES})

# Add runtime path for shared libraries (Linux/macOS)
if(UNIX AND NOT APPLE)
    set_target_properties(breakout PROPERTIES
        BUILD_RPATH ${ALLEGRO_LIBRARY_DIRS}
        INSTALL_RPATH ${ALLEGRO_LIBRARY_DIRS}
    )
endif()

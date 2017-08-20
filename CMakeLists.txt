cmake_minimum_required(VERSION 2.8.11)

project(QLife)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Os")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(HEADER_FILES
    MainWindow.h
    Life.h
    LifeWidget.h
    )

set(SOURCE_FILES
    MainWindow.cpp
    Life.cpp
    LifeWidget.cpp
    main.cpp
    )

set(SHADERS
    Shaders/FragmentShader.fsh
    Shaders/VertexShader.vsh
    )

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES})

find_package(Qt5Widgets REQUIRED)

# SHADERS only used so that they will appear in QtCreator
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${SHADERS})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${OPENGL_LIBRARY})

find_package( OpenMP )
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders" "$<TARGET_FILE_DIR:QLife>/Shaders"
    COMMENT "Copy shaders to build tree"
    VERBATIM)

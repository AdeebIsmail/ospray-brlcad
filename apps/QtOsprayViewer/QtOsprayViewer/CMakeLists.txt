cmake_minimum_required(VERSION 3.20)
project(QtOsprayViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.10.2/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Point to your OSPRay superbuild install prefix
set(OSPRAY_PREFIX "C:/ospray/build/install/ospray" CACHE PATH "OSPRay prefix")
list(APPEND CMAKE_PREFIX_PATH "${OSPRAY_PREFIX}")

find_package(ospray REQUIRED CONFIG)

# rkcommon is shipped with OSPRay; in many installs it's available as a CMake target
#find_package(rkcommon REQUIRED CONFIG)


#target_link_libraries(QtOsprayViewer PRIVATE rkcommon::rkcommon)

qt_add_executable(QtOsprayViewer
    main.cpp
    mainwindow.cpp
    mainwindow.h
    renderwidget.h
    renderwidget.cpp
    ospraybackend.cpp
    ospraybackend.h
)


# rkcommon headers live here in your install
target_include_directories(QtOsprayViewer PRIVATE "C:/ospray/build/install/rkcommon/include")

target_link_libraries(QtOsprayViewer PRIVATE Qt6::Widgets ospray::ospray)

# Copy runtime DLLs so the app runs from Qt Creator
if (WIN32)
  add_custom_command(TARGET QtOsprayViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "C:/Qt/6.10.2/msvc2022_64/bin"
      "$<TARGET_FILE_DIR:QtOsprayViewer>"
  )
  add_custom_command(TARGET QtOsprayViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "C:/ospray/build/install/ospray/bin"
      "$<TARGET_FILE_DIR:QtOsprayViewer>"
  )
endif()

if (WIN32)
  add_custom_command(TARGET QtOsprayViewer POST_BUILD
    COMMAND "C:/Qt/6.10.2/msvc2022_64/bin/windeployqt.exe"
            "$<TARGET_FILE:QtOsprayViewer>"
    COMMENT "Running windeployqt..."
  )
endif()
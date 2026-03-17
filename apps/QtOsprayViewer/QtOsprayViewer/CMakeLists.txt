cmake_minimum_required(VERSION 3.20)
project(QtOsprayViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================
# User-editable path variables
# ==============================
# Linux paths
set(QT_ROOT_LINUX "/home/adeeb/Qt/6.10.2/gcc_64" CACHE PATH "Qt install root on Linux")
set(OSPRAY_PREFIX_LINUX "/home/adeeb/workspace/csce482/build/install/ospray" CACHE PATH "OSPRay install prefix on Linux")

# Windows paths
set(QT_ROOT_WINDOWS "C:/Qt/6.10.2/msvc2022_64" CACHE PATH "Qt install root on Windows")
set(OSPRAY_PREFIX_WINDOWS "C:/ospray/build/install/ospray" CACHE PATH "OSPRay install prefix on Windows")

if(WIN32)
  set(QT_ROOT "${QT_ROOT_WINDOWS}")
  set(OSPRAY_PREFIX "${OSPRAY_PREFIX_WINDOWS}")
else()
  set(QT_ROOT "${QT_ROOT_LINUX}")
  set(OSPRAY_PREFIX "${OSPRAY_PREFIX_LINUX}")
endif()

set(QT_BIN_DIR "${QT_ROOT}/bin")
set(QT6_CMAKE_DIR "${QT_ROOT}/lib/cmake/Qt6")
set(OSPRAY_BIN_DIR "${OSPRAY_PREFIX}/bin")
set(RKCOMMON_INCLUDE_DIR "${OSPRAY_PREFIX}/../rkcommon/include")

list(APPEND CMAKE_PREFIX_PATH "${QT_ROOT}")
list(APPEND CMAKE_PREFIX_PATH "${OSPRAY_PREFIX}")
set(Qt6_DIR "${QT6_CMAKE_DIR}" CACHE PATH "Qt6 config directory")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

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


# Some OSPRay installs do not propagate rkcommon headers transitively.
if (EXISTS "${RKCOMMON_INCLUDE_DIR}")
  target_include_directories(QtOsprayViewer PRIVATE "${RKCOMMON_INCLUDE_DIR}")
endif()

target_link_libraries(QtOsprayViewer PRIVATE Qt6::Widgets ospray::ospray)

# Copy runtime DLLs so the app runs from Qt Creator
if (WIN32)
  if(EXISTS "${QT_BIN_DIR}")
  add_custom_command(TARGET QtOsprayViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${QT_BIN_DIR}"
      "$<TARGET_FILE_DIR:QtOsprayViewer>"
  )
  endif()
  if(EXISTS "${OSPRAY_BIN_DIR}")
  add_custom_command(TARGET QtOsprayViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${OSPRAY_BIN_DIR}"
      "$<TARGET_FILE_DIR:QtOsprayViewer>"
  )
  endif()
endif()

if (WIN32)
  if(EXISTS "${QT_BIN_DIR}/windeployqt.exe")
  add_custom_command(TARGET QtOsprayViewer POST_BUILD
    COMMAND "${QT_BIN_DIR}/windeployqt.exe"
            "$<TARGET_FILE:QtOsprayViewer>"
    COMMENT "Running windeployqt..."
  )
  endif()
endif()
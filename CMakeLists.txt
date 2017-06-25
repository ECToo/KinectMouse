cmake_minimum_required(VERSION 3.1)
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} )
project(KinectMouse)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)

option(KinectToolkit_INTERACTION "" ON)
find_package(KinectSDK REQUIRED)


find_package(Boost)

set(SOURCES
    "kinectmousewindow.cpp"
    "kinectmousewindow.h"
    "kinectpointer.cpp"
    "kinectpointer.h"
    "main.cpp"
    "NUIFilter.ui"
    "Pointer.cpp"
    "Pointer.h"
    "SimpleFilter.h"
)


QT5_WRAP_UI(UI_SOURCES 
    "SimpleFilter.ui"
    #"kinectmousewindow.qrc"
    "kinectmousewindow.ui"
    )

# Tell CMake to create the helloworld executable
add_executable(KinectMouse WIN32 ${SOURCES} ${UI_SOURCES})

# Use the Widgets module from Qt 5.
target_link_libraries(KinectMouse Qt5::Widgets Qt5::Charts ${KinectSDK_LIBRARIES} ${KinectToolkit_LIBRARIES})

target_include_directories(KinectMouse 
PUBLIC 
	${KinectSDK_INCLUDE_DIRS}
    ${KinectToolkit_INCLUDE_DIRS}    
	${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Charts_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
	)
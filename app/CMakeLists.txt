cmake_minimum_required(VERSION 3.16)
project(NetherTTC_GUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



if(WIN32)
   set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/app.rc")
endif()



add_executable(PS2TC WIN32

    ${APP_ICON_RESOURCE}
    main.cpp
    mainwindow.h
    mainwindow.cpp
)

# Link Qt6 Widgets library
target_link_libraries(PS2TC PRIVATE Qt6::Widgets)

cmake_minimum_required(VERSION 3.16)

project(HydraulicToolbox VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HydraulicToolbox
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(HydraulicToolbox SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(HydraulicToolbox
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(HydraulicToolbox PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.HydraulicToolbox)
endif()
set_target_properties(HydraulicToolbox PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_executable(HydraulicTests
    RectangularChannel_UnitTests.cpp
    RectangularChannel.cpp
    TriangularChannel_UnitTests.cpp
    TriangularChannel.cpp
    TrapezoidalChannel_UnitTests.cpp
    TrapezoidalChannel.cpp
    Channel.cpp
    Flow_UnitTests.cpp
    Flow.cpp
    Analyzer_UnitTests.cpp
    Analyzer.cpp
)

target_link_libraries(HydraulicTests
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(HydraulicTests)

include(GNUInstallDirs)
install(TARGETS HydraulicToolbox
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HydraulicToolbox)
endif()

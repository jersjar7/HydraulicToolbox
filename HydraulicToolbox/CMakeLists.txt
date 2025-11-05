cmake_minimum_required(VERSION 3.16)

project(HydraulicToolbox VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(VTK REQUIRED COMPONENTS
    CommonCore
    CommonDataModel
    FiltersSources
    RenderingCore
    RenderingOpenGL2
    RenderingAnnotation
    InteractionStyle
    GUISupportQt
)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# ============================================================================
# BACKEND SOURCES
# ============================================================================
set(BACKEND_SOURCES
    backend/Channel.cpp
    backend/RectangularChannel.cpp
    backend/TrapezoidalChannel.cpp
    backend/TriangularChannel.cpp
    backend/Flow.cpp
    backend/Analyzer.cpp
    backend/HydraulicCalculator.h
    backend/HydraulicCalculator.cpp
)

# ============================================================================
# SHARED DATA STRUCTURES
# ============================================================================
set(SHARED_SOURCES
    ProjectDataStructures.h
    UnitSystemConstants.h
)

# ============================================================================
# UI SOURCES - Organized by subfolder
# ============================================================================

# Main window
set(UI_MAIN_SOURCES
    ui/mainwindow.cpp
    ui/mainwindow.h
    ui/mainwindow.ui
)

# Controls subfolder (WorkflowController, WorkflowTabBar)
set(UI_CONTROLS_SOURCES
    ui/controls/WorkflowController.h
    ui/controls/WorkflowController.cpp
    ui/controls/WorkflowTabBar.h
    ui/controls/WorkflowTabBar.cpp
)

# Widgets subfolder (all 5 tab widgets + InputSummaryWidget)
set(UI_WIDGETS_SOURCES
    ui/widgets/ProjectSetupWidget.h
    ui/widgets/ProjectSetupWidget.cpp
    ui/widgets/GeometryDefinitionWidget.h
    ui/widgets/GeometryDefinitionWidget.cpp
    ui/widgets/HydraulicParametersWidget.h
    ui/widgets/HydraulicParametersWidget.cpp
    ui/widgets/AnalysisResultsWidget.h
    ui/widgets/AnalysisResultsWidget.cpp
    ui/widgets/ExportWidget.h
    ui/widgets/ExportWidget.cpp
    ui/widgets/InputSummaryWidget.h
    ui/widgets/InputSummaryWidget.cpp
)

# Panels subfolder
set(UI_PANELS_SOURCES
    ui/panels/VisualizationPanel.h
    ui/panels/VisualizationPanel.cpp
    ui/panels/ParameterPanel.h
    ui/panels/ParameterPanel.cpp
)

# Visualization subfolder (VtkWidget + renderers)
set(UI_VISUALIZATION_SOURCES
    ui/visualization/VtkWidget.h
    ui/visualization/VtkWidget.cpp
    ui/visualization/renderers/ChannelRenderer.h
    ui/visualization/renderers/ChannelRenderer.cpp
    ui/visualization/renderers/RectangularChannelRenderer.h
    ui/visualization/renderers/RectangularChannelRenderer.cpp
    ui/visualization/renderers/TrapezoidalChannelRenderer.h
    ui/visualization/renderers/TrapezoidalChannelRenderer.cpp
    ui/visualization/renderers/TriangularChannelRenderer.h
    ui/visualization/renderers/TriangularChannelRenderer.cpp
)

# Combine all UI sources
set(UI_SOURCES
    ${UI_MAIN_SOURCES}
    ${UI_CONTROLS_SOURCES}
    ${UI_WIDGETS_SOURCES}
    ${UI_PANELS_SOURCES}
    ${UI_VISUALIZATION_SOURCES}
)

# ============================================================================
# MAIN APPLICATION
# ============================================================================
set(PROJECT_SOURCES
    main.cpp
    ${BACKEND_SOURCES}
    ${SHARED_SOURCES}
    ${UI_SOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
qt_add_executable(HydraulicToolbox
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ui/visualization/renderers/TrapezoidalChannelRenderer.h
        ui/visualization/renderers/TrapezoidalChannelRenderer.cpp
        ui/visualization/renderers/TriangularChannelRenderer.h
        ui/visualization/renderers/TriangularChannelRenderer.cpp
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

# Include directories for all UI subfolders
target_include_directories(HydraulicToolbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/controls
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/panels
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/visualization
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/visualization/renderers
)

target_link_libraries(HydraulicToolbox PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${VTK_LIBRARIES}
)

# VTK module auto-initialization
vtk_module_autoinit(
    TARGETS HydraulicToolbox
    MODULES ${VTK_LIBRARIES}
)

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

# ============================================================================
# TESTS
# ============================================================================
add_executable(HydraulicTests
    tests/RectangularChannel_UnitTests.cpp
    tests/TriangularChannel_UnitTests.cpp
    tests/TrapezoidalChannel_UnitTests.cpp
    tests/Flow_UnitTests.cpp
    tests/Analyzer_UnitTests.cpp
    ${BACKEND_SOURCES}
)

target_include_directories(HydraulicTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
)

target_link_libraries(HydraulicTests
    PRIVATE
        GTest::gtest_main
        Qt${QT_VERSION_MAJOR}::Widgets
)

include(GoogleTest)
gtest_discover_tests(HydraulicTests)

# ============================================================================
# INSTALLATION
# ============================================================================
include(GNUInstallDirs)
install(TARGETS HydraulicToolbox
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HydraulicToolbox)
endif()

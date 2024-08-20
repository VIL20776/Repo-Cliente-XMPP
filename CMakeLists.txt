cmake_minimum_required(VERSION 3.16)

project(QJab VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        ui/mainwindow.ui
)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QJab
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/xmppworker.h src/xmppworker.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QJab APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QJab SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QJab
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(QXmpp_VERSION 1.8.0) # Version de la librería usada

# Busca la librería en el directorio "external" si existe en el proyecto
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
if(EXISTS ${EXTERNAL_DIR})
    # Agrega QXmpp de la carpeta "external"
    set(QXmpp_DIR ${EXTERNAL_DIR}/qxmpp-${QXmpp_VERSION}/build/lib/cmake/QXmppQt6)
    set(QXmpp_INCLUDE ${EXTERNAL_DIR}/qxmpp-${QXmpp_VERSION}/build/include/QXmppQt6)
endif()

find_package(QXmpp ${QXmpp_VERSION} REQUIRED NAMES QXmpp QXmppQt6)

target_link_libraries(QJab PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QXmpp::QXmpp)
if(DEFINED ${QXmpp_INCLUDE})
    target_include_directories(QJab PRIVATE ${QXmpp_INCLUDE})
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QJab)
endif()
set_target_properties(QJab PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QJab
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QJab)
endif()

cmake_minimum_required(VERSION 3.16)

project(company VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp


)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(company
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}



        models/auth/authmodel.cpp
        models/auth/authmodel.h
        models/auth/user.h
        views/auth/authwindow.h
        views/auth/authwindow.cpp
        views/auth/regwindow.h
        views/auth/regwindow.cpp

        controllers/auth/authcontroller.h
        controllers/auth/authcontroller.cpp

        models/databasemanager.h
        models/databasemanager.cpp
        views/auth/authwindow.ui
        views/auth/regwindow.ui

        views/main/mainwindow.ui
        views/auth/customerinfodialog.ui
        views/main/mainwindow.h
        views/main/mainwindow.cpp
        views/auth/customerinfodialog.h
        views/auth/customerinfodialog.cpp
        views/auth/changedatadialog.ui
        views/auth/changedatadialog.h
        views/auth/changedatadialog.cpp
        views/main/orderwindow.ui
        views/main/orderwindow.h
        views/main/orderwindow.cpp
        views/main/orderhistorydialog.ui
        views/main/orderhistorydialog.h
        views/main/orderhistorydialog.cpp
        controllers/main/ordercontroller.h
        controllers/main/ordercontroller.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET company APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(company SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(company
            ${PROJECT_SOURCES}
        )
    endif()
endif()
target_link_libraries(company PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.company)
endif()
set_target_properties(company PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS company
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(company)
endif()

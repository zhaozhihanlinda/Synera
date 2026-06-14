cmake_minimum_required(VERSION 3.16)

project(Synera VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/pages/mainwindow.cpp
        include/pages/mainwindow.h
        src/pages/startpage.cpp
        include/pages/startpage.h
        src/pages/profilepage.cpp
        include/pages/profilepage.h
        src/pages/initinfopage.cpp
        include/pages/initinfopage.h
        src/pages/drawpage.cpp
        include/pages/drawpage.h
        src/pages/shoppage.cpp
        include/pages/shoppage.h
        src/pages/battlecountdownpage.cpp
        include/pages/battlecountdownpage.h
        src/pages/maingamepage.cpp
        include/pages/maingamepage.h
        src/pages/roundresultpage.cpp
        include/pages/roundresultpage.h
        src/pages/victorypage.cpp
        include/pages/victorypage.h
        src/pages/defeatpage.cpp
        include/pages/defeatpage.h
        src/core/gamemanager.cpp
        include/core/gamemanager.h
        src/core/board.cpp
        include/core/board.h
        src/core/enemyencounterinfo.cpp
        include/core/enemyencounterinfo.h
        src/core/unit.cpp
        include/core/unit.h
        src/core/unittemplate.cpp
        include/core/unittemplate.h
        src/core/playertemplatelibrary.cpp
        include/core/playertemplatelibrary.h
        src/core/pagemanager.cpp
        include/core/pagemanager.h
        src/widgets/boardwidget.cpp
        include/widgets/boardwidget.h
        src/widgets/battleinfopanel.cpp
        include/widgets/battleinfopanel.h
        src/widgets/benchwidget.cpp
        include/widgets/benchwidget.h
        include/core/playerprofile.h
        include/core/playeravatar.h
        include/core/pageid.h
        include/core/roundstate.h
        src/pages/mainwindow.ui
)
list(TRANSFORM PROJECT_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Synera
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Synera APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Synera SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Synera
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(Synera PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(Synera PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Synera)
endif()
set_target_properties(Synera PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Synera
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Synera)
endif()

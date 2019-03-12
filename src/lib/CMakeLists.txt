
set(MANIPULATE_SRCS
    manipulate.cpp
)

set(MANIPULATE_HDRS
    manipulate.h
    messagehandler.h
)

set(MANIPULATE_RCC
    resources.qrc
)

qt5_add_resources(MANIPULATE_RES ${MANIPULATE_RCC})

add_library(manipulate SHARED
    ${MANIPULATE_SRCS}
    ${MANIPULATE_HDRS}
    ${MANIPULATE_RES}
)

target_link_libraries(manipulate Qt5::Core Qt5::XmlPatterns cellml)
target_include_directories(manipulate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(UTILITIES_SRCS
    utilities.cpp
)

set(UTILITIES_HDRS
    utilities.h
)

add_library(utilities SHARED
    ${UTILITIES_SRCS}
    ${UTILITIES_HDRS}
)

target_link_libraries(utilities Qt5::Core )
target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tools)

set(MAINWINDOW_SRCS
    mainwindow.cpp
    aboutdialog.cpp
    documentarea.cpp
    documentwidget.cpp
    documentwindow.cpp
)

set(MAINWINDOW_HDRS
    mainwindow.h
    aboutdialog.h
    documentarea.h
    documentwidget.h
    documentwindow.h
)

set(MAINWINDOW_UI
    mainwindow.ui
    aboutdialog.ui
    documentwidget.ui
)

qt5_wrap_cpp(MAINWINDOW_HDR_MOC ${MAINWINDOW_HDRS})
qt5_wrap_ui(MAINWINDOW_UI_MOC ${MAINWINDOW_UI})

add_library(mainwindow SHARED
    ${MAINWINDOW_SRCS}
    ${MAINWINDOW_HDRS}
    ${MAINWINDOW_HDR_MOC}
    ${MAINWINDOW_UI_MOC}
)

target_link_libraries(mainwindow Qt5::Widgets manipulate utilities tools)
target_include_directories(mainwindow PRIVATE ${CMAKE_CURRENT_BINARY_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


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

set(MAINWINDOW_SRCS
    mainwindow.cpp
    aboutdialog.cpp
    droparea.cpp
)

set(MAINWINDOW_HDRS
    mainwindow.h
    aboutdialog.h
    droparea.h
)

set(MAINWINDOW_UI
    mainwindow.ui
    aboutdialog.ui
)

qt5_wrap_cpp(MAINWINDOW_HDR_MOC ${MAINWINDOW_HDRS})
qt5_wrap_ui(MAINWINDOW_UI_MOC ${MAINWINDOW_UI})

add_library(mainwindow SHARED
    ${MAINWINDOW_SRCS}
    ${MAINWINDOW_HDRS}
    ${MAINWINDOW_HDR_MOC}
    ${MAINWINDOW_UI_MOC}
)

target_link_libraries(mainwindow Qt5::Widgets Qt5::XmlPatterns cellml manipulate)
target_include_directories(mainwindow PRIVATE ${CMAKE_CURRENT_BINARY_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


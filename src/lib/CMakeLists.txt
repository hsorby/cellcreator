
set(LIBRARY_SRCS
    mainwindow.cpp
    aboutdialog.cpp
    droparea.cpp
)

set(LIBRARY_HDRS
    mainwindow.h
    aboutdialog.h
    droparea.h
    messagehandler.h
)

set(LIBRARY_UI
    mainwindow.ui
    aboutdialog.ui
)

set(LIBRARY_RCC
    resources.qrc
)

qt5_wrap_cpp(LIBRARY_HDR_MOC ${LIBRARY_HDRS})
qt5_wrap_ui(LIBRARY_UI_MOC ${LIBRARY_UI})
qt5_add_resources(LIBRARY_RES ${LIBRARY_RCC})

add_library(mainwindow SHARED
    ${LIBRARY_SRCS}
    ${LIBRARY_HDRS}
    ${LIBRARY_HDR_MOC}
    ${LIBRARY_UI_MOC}
    ${LIBRARY_RES}
)

target_link_libraries(mainwindow Qt5::Widgets Qt5::XmlPatterns cellml)
target_include_directories(mainwindow PRIVATE ${CMAKE_CURRENT_BINARY_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

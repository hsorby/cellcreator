
set(LIBRARY_SRCS
    mainwindow.cpp
    )

set(LIBRARY_HDRS
    mainwindow.h
    )

set(LIBRARY_UI
    mainwindow.ui
    )

qt4_wrap_cpp(LIBRARY_HDR_MOC ${LIBRARY_HDRS})
qt4_wrap_ui(LIBRARY_UI_MOC ${LIBRARY_UI})

message(STATUS "LIBRARY_HDR_MOC: ${LIBRARY_HDR_MOC}")
message(STATUS "LIBRARY_UI_MOC: ${LIBRARY_UI_MOC}")

add_library(mainwindow SHARED
    ${LIBRARY_SRCS}
    ${LIBRARY_HDR_MOC}
    ${LIBRARY_UI_MOC}
    )
message(STATUS "taget link libraries: sdfef Qt4::QtGui libcellmlbdfefe")
target_link_libraries(mainwindow Qt4::QtGui cellml)
target_include_directories(mainwindow PRIVATE ${CMAKE_CURRENT_BINARY_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

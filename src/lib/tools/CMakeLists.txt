
set(TOOLS_SRCS
    conversiondialog.cpp
    droparea.cpp
)

set(TOOLS_HDRS
    conversiondialog.h
    droparea.h
)

set(TOOLS_UI
    conversiondialog.ui
)

qt5_wrap_cpp(TOOLS_HDR_MOC ${TOOLS_HDRS})
qt5_wrap_ui(TOOLS_UI_MOC ${TOOLS_UI})

add_library(tools SHARED
    ${TOOLS_SRCS}
    ${TOOLS_HDRS}
    ${TOOLS_HDR_MOC}
    ${TOOLS_UI_MOC}
)

target_link_libraries(tools Qt5::Widgets manipulate)
target_include_directories(tools PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

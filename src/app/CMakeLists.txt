

set(APPLICATION_SRCS
    main.cpp
    )

#set()

add_executable(${PROJECT_NAME} ${APPLICATION_SRCS} ${APPLICATION_HDRS})
target_link_libraries(${PROJECT_NAME} mainwindow)
#target_include_directories(${PROJECT_NAME} PRIVAT)

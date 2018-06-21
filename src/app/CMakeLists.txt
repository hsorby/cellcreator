
set(APPLICATION_SRCS
    main.cpp
)

add_executable(${PROJECT_NAME} ${APPLICATION_SRCS} ${APPLICATION_HDRS})
target_link_libraries(${PROJECT_NAME} mainwindow)

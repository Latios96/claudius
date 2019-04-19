set(Main_SRCS
    main.cpp
)

add_executable(${PROJECT_NAME}
    ${Main_SRCS}
)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

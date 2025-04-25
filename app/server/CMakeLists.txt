add_executable(${UDP_SERVER_EXECUTABLE} server_udp.cpp)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${UDP_SERVER_EXECUTABLE}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

add_executable(${TCP_SERVER_EXECUTABLE} server_tcp.cpp)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${TCP_SERVER_EXECUTABLE}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

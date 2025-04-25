
add_executable(${UDP_CLIENT_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/client_udp.cpp")
add_executable(${TCP_CLIENT_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/client_tcp.cpp")

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${UDP_CLIENT_EXECUTABLE}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${TCP_CLIENT_EXECUTABLE}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

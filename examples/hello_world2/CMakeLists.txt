#
# Copyright 2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

###############################################################################
# Define variables
###############################################################################

set(ERPC_C_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/c/shim)
set(ERPC_PY_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/py/shim)
set(ERPC_JAVA_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/java/src/main/java/org/example/)

set(APP_NAME "hello_world2")
set(IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/hello_world.erpc)

set(INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/c
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ERPC_C_OUT_DIR}
)

set(OUTPUT_FILES
        ${ERPC_C_OUT_DIR}/${APP_NAME}_interface.cpp
        ${ERPC_C_OUT_DIR}/${APP_NAME}_client.cpp
        ${ERPC_C_OUT_DIR}/${APP_NAME}_server.cpp
        ${ERPC_C_OUT_DIR}/c_${APP_NAME}_client.cpp
        ${ERPC_C_OUT_DIR}/c_${APP_NAME}_server.cpp
)

###############################################################################
# Generate shim code
###############################################################################

if (CONFIG_ERPC_HELLO_WORLD.c OR CONFIG_ERPC_HELLO_WORLD.cpp)
    erpc_generate_shim_code(
            IDL_FILE ${IDL_FILE}
            OUTPUT_DIR ${ERPC_C_OUT_DIR}
            WORKING_DIRECTORY ${ERPC_EXAMPLES}
            OUTPUT_FILES ${OUTPUT_FILES}
    )
endif ()
#
#if(CONFIG_ERPC_HELLO_WORLD.python)
#    add_custom_target(
#        python_shim ALL
#        COMMAND ${ERPCGEN_EXECUTABLE} -g py -o ${ERPC_PY_OUT_DIR} ${IDL_FILE}
#        WORKING_DIRECTORY ${ERPC_EXAMPLES}
#        DEPENDS ${IDL_FILE}
#        COMMENT "Generating eRPC files from: ${IDL_FILE} to ${ERPC_PY_OUT_DIR}"
#    )
#endif()
#
#if(CONFIG_ERPC_HELLO_WORLD.java)
#    add_custom_target(
#        java_shim ALL
#        COMMAND ${ERPCGEN_EXECUTABLE} -g java -p org.example.hello_world -o ${ERPC_JAVA_OUT_DIR} ${IDL_FILE}
#        WORKING_DIRECTORY ${ERPC_EXAMPLES}
#        DEPENDS ${IDL_FILE}
#        COMMENT "Generating eRPC files from: ${IDL_FILE} to ${ERPC_JAVA_OUT_DIR}"
#    )
#endif()

###############################################################################
# Create C/C++ targets
###############################################################################
function(erpc_link_c_library2)
    cmake_parse_arguments(_ "" "TARGET;CONFIG_DIR;TRANSPORT" "" ${ARGN})


    include(${ERPC_C}/erpc_c_sources.cmake)

    if (NOT __TARGET)
        message(FATAL_ERROR "Not target specified.")
    endif ()


    if (NOT __CONFIG_DIR)
        set(__CONFIG_DIR ${ERPC_C}/config)
    endif ()

    file(MD5 ${__CONFIG_DIR}/erpc_config.h CONFIG_HASH)
    #    set(ercp_lib_name erpc_c_lib_${CONFIG_HASH})
    set(ercp_lib_name erpc4test)

    if (NOT TARGET ${ercp_lib_name})
        add_library(${ercp_lib_name} SHARED ${ERPC_C_SOURCES})
        target_include_directories(${ercp_lib_name} PUBLIC ${ERPC_C_INCLUDES} ${__CONFIG_DIR})

        # Required for TCP transport
        if (WIN32)
            target_link_libraries(${ercp_lib_name} PUBLIC wsock32 ws2_32)
        endif ()

        target_link_libraries(${ercp_lib_name} PUBLIC Threads::Threads)
    endif ()

    find_package(Threads REQUIRED)
    target_link_libraries(${__TARGET} PRIVATE ${ercp_lib_name})
endfunction()

find_package(Threads REQUIRED)


if (CONFIG_ERPC_HELLO_WORLD.cpp)
    set(clint_name_lib ${APP_NAME}_clientlib)
    set(server_name_lib ${APP_NAME}_serverlib)
    add_library(${clint_name_lib} SHARED)
    add_library(${server_name_lib} SHARED)

    target_sources(${clint_name_lib} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/c/hello_world_client.cpp

            ${CMAKE_CURRENT_SOURCE_DIR}/c/erpc_error_handler.cpp
            ${ERPC_C_OUT_DIR}/hello_world_client.cpp
            ${ERPC_C_OUT_DIR}/hello_world_interface.cpp
    )

    target_sources(${server_name_lib} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/c/hello_world_server.cpp

            ${CMAKE_CURRENT_SOURCE_DIR}/c/erpc_error_handler.cpp
            ${ERPC_C_OUT_DIR}/hello_world_server.cpp
            ${ERPC_C_OUT_DIR}/hello_world_interface.cpp
    )

    set_target_properties(${clint_name_lib} PROPERTIES LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_LIST_DIR}/c/client.version")
    set_target_properties(${clint_name_lib} PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/c/client.version)
    set_target_properties(${server_name_lib} PROPERTIES LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_LIST_DIR}/c/server.version")
    set_target_properties(${server_name_lib} PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_LIST_DIR}//c/server.version)

    target_include_directories(${clint_name_lib} PRIVATE ${INCLUDES})
    target_include_directories(${server_name_lib} PRIVATE ${INCLUDES})

    target_link_libraries(${clint_name_lib} PRIVATE Threads::Threads)
    target_link_libraries(${server_name_lib} PRIVATE Threads::Threads)

    erpc_link_c_library2(TARGET ${clint_name_lib} CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/c)
    erpc_link_c_library2(TARGET ${server_name_lib} CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/c)
    if (ANDROID)
        target_link_libraries(${clint_name_lib} PRIVATE android log)
        target_link_libraries(${server_name_lib} PRIVATE android log)
    endif ()
endif ()

if (CONFIG_ERPC_HELLO_WORLD.c)
    set(clint_name ${APP_NAME}_client)
    set(server_name ${APP_NAME}_server)
    add_executable(${clint_name})
    add_executable(${server_name})

    target_sources(${clint_name} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/c/main_client.c
            ${CMAKE_CURRENT_SOURCE_DIR}/c/erpc_error_handler.cpp

            ${ERPC_C_OUT_DIR}/c_hello_world_client.cpp
            ${ERPC_C_OUT_DIR}/hello_world_client.cpp
            ${ERPC_C_OUT_DIR}/hello_world_interface.cpp
    )

    target_sources(${server_name} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/c/main_server.c
            ${CMAKE_CURRENT_SOURCE_DIR}/c/erpc_error_handler.cpp

            ${ERPC_C_OUT_DIR}/c_hello_world_server.cpp
            ${ERPC_C_OUT_DIR}/hello_world_server.cpp
            ${ERPC_C_OUT_DIR}/hello_world_interface.cpp
    )

    target_include_directories(${clint_name} PRIVATE ${INCLUDES})
    target_include_directories(${server_name} PRIVATE ${INCLUDES})

    target_link_libraries(${clint_name} PRIVATE Threads::Threads)
    target_link_libraries(${server_name} PRIVATE Threads::Threads)

    erpc_link_c_library2(TARGET ${clint_name} CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/c)
    erpc_link_c_library2(TARGET ${server_name} CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/c)
endif ()

if (CONFIG_ERPC_HELLO_WORLD.cpp)
    set(clint_cpp_name ${APP_NAME}_client_cpp)
    set(server_cpp_name ${APP_NAME}_server_cpp)
    add_executable(${clint_cpp_name})
    add_executable(${server_cpp_name})

    target_sources(${clint_cpp_name} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/c/main_client.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/c/erpc_error_handler.cpp

            ${ERPC_C_OUT_DIR}/hello_world_client.cpp
            ${ERPC_C_OUT_DIR}/hello_world_interface.cpp
    )

    target_sources(${server_cpp_name} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/c/main_server.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/c/erpc_error_handler.cpp

            ${ERPC_C_OUT_DIR}/hello_world_server.cpp
            ${ERPC_C_OUT_DIR}/hello_world_interface.cpp
    )

    target_include_directories(${clint_cpp_name} PRIVATE ${INCLUDES})
    target_include_directories(${server_cpp_name} PRIVATE ${INCLUDES})

    target_link_libraries(${clint_cpp_name} PRIVATE Threads::Threads)
    target_link_libraries(${server_cpp_name} PRIVATE Threads::Threads)

    erpc_link_c_library2(TARGET ${clint_cpp_name} CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/c)
    erpc_link_c_library2(TARGET ${server_cpp_name} CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/c)
endif ()
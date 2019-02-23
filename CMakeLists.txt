cmake_minimum_required(VERSION 3.02)

project(a_tale_of_two_enets LANGUAGES C)

set(ADDITIONAL_LINK_LIBRARIES winmm ws2_32)

#
# Enet-CSharp library target
# note: there was no way to override their export defines configured directly in the translation unit, so the source was modified
set(ENET_CSHARP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/deps/Enet-CSharp/Source/Native)
set(ENET_CSHARP_INC ${ENET_CSHARP_SRC})
add_library(enet_csharp STATIC ${ENET_CSHARP_SRC}/enet.c)
target_include_directories(enet_csharp PRIVATE ${ENET_CSHARP_INC})

#
# enet library target
set(ENET_SRC ${CMAKE_CURRENT_SOURCE_DIR}/deps/enet)
set(ENET_INC ${ENET_SRC}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/enet)

#
#
# executable targets
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CLIENT_SRCS
	src/client.c
)

set(SERVER_SRCS
	src/server.c
)

set(CLIENT_CSHARP_SRCS
	src/client_csharp.c
)

set(SERVER_CSHARP_SRCS
	src/server_csharp.c
)


# client enet
add_executable(client_enet ${CLIENT_SRCS})
target_link_libraries(client_enet enet ${ADDITIONAL_LINK_LIBRARIES})
target_include_directories(client_enet PRIVATE ${ENET_INC})

# server enet
add_executable(server_enet ${SERVER_SRCS})
target_link_libraries(server_enet enet ${ADDITIONAL_LINK_LIBRARIES})
target_include_directories(server_enet PRIVATE ${ENET_INC})


# client enet_csharp
add_executable(client_enet_csharp ${CLIENT_CSHARP_SRCS})
target_link_libraries(client_enet_csharp enet_csharp ${ADDITIONAL_LINK_LIBRARIES})
target_include_directories(client_enet_csharp PRIVATE ${ENET_CSHARP_INC})
target_compile_definitions(client_enet_csharp PRIVATE -DENET_IMPLEMENTATION)

# server enet_csharp
add_executable(server_enet_csharp ${SERVER_CSHARP_SRCS})
target_link_libraries(server_enet_csharp enet_csharp ${ADDITIONAL_LINK_LIBRARIES})
target_include_directories(server_enet_csharp PRIVATE ${ENET_CSHARP_INC})
target_compile_definitions(server_enet_csharp PRIVATE -DENET_IMPLEMENTATION)
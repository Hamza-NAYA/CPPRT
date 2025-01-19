cmake_minimum_required(VERSION 3.10)
project(CPPRT)

set(CMAKE_CXX_STANDARD 17)

# Chemin vers SFML
set(SFML_DIR "C:/Users/kamil/projets/CPPRT/SFML")

# Inclure SFML
include_directories(${SFML_DIR}/include)
link_directories(${SFML_DIR}/lib)

# Serveur
add_executable(server
    server/src/server.cpp
    server/src/main_server.cpp
)
target_link_libraries(server sfml-network sfml-graphics sfml-window sfml-system)
set_target_properties(server PROPERTIES LINK_FLAGS "-Wl,-subsystem,console")

# Client
add_executable(client
    client/src/client.cpp
    client/src/main_client.cpp
)
target_link_libraries(client sfml-network sfml-graphics sfml-window sfml-system sfml-audio)
set_target_properties(client PROPERTIES LINK_FLAGS "-Wl,-subsystem,console")

# Copier automatiquement wizz.png dans le dossier de construction
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/client/src/wizz.png
    $<TARGET_FILE_DIR:client>
)

cmake_minimum_required(VERSION 3.10)
project(cppzmq_client_server)

# Add the cppzmq library (ensure it's installed or in third_party)
find_package(cppzmq REQUIRED)

# Add the source files
add_executable(server src/server.cpp)
add_executable(client src/client.cpp)

# Link cppzmq to both targets
target_link_libraries(server PRIVATE cppzmq)
target_link_libraries(client PRIVATE cppzmq)

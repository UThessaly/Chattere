add_library(Net STATIC src/net.cpp src/inet_socket_address.cpp src/socket.cpp src/server_socket.cpp src/client_socket.cpp)

target_include_directories(Net PUBLIC include)

target_link_libraries(Net PRIVATE CONAN_PKG::spdlog)

include(GenerateExportHeader)
generate_export_header(Net)
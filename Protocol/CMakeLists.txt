# Find Protobuf installation

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/packet.proto proto/database.proto)

add_library(Protocol STATIC src/packet.cpp ${PROTO_SRCS} ${PROTO_HDRS})

target_include_directories(Protocol PUBLIC ${CMAKE_CURRENT_BINARY_DIR} include)

target_link_libraries(Protocol PUBLIC CONAN_PKG::protobuf)
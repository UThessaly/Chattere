add_executable(Server src/server.cpp src/main.cpp)

find_package(Threads REQUIRED)

target_include_directories(Server PUBLIC include)
target_link_libraries(Server PUBLIC Net Database CONAN_PKG::spdlog Protocol ${CMAKE_THREAD_LIBS_INIT} Emoji Actions CONAN_PKG::sqlitecpp Snowflake)
# target_link_libraries(Server INTERFACE ProtoTest)
# target_include_directories(Server PRIVATE ${CMAKE_BINARY_DIR})
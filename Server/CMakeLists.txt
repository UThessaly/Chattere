add_executable(server src/server.cpp src/main.cpp src/events/events.cpp src/user.cpp src/command_sender.cpp src/console_command_sender.cpp src/events/server_events.cpp)

find_package(Threads REQUIRED)

target_include_directories(server PUBLIC include)
target_link_libraries(server PUBLIC Net Database CONAN_PKG::spdlog Protocol ${CMAKE_THREAD_LIBS_INIT} Emoji Actions CONAN_PKG::sqlitecpp Snowflake CONAN_PKG::docopt.cpp CONAN_PKG::yaml-cpp)
# target_link_libraries(Server INTERFACE ProtoTest)
# target_include_directories(Server PRIVATE ${CMAKE_BINARY_DIR})
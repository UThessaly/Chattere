add_library(Server STATIC src/server.cpp src/user_command_sender.cpp src/events/events.cpp src/user.cpp src/command_sender.cpp src/events/server_events.cpp)
add_executable(chattere_server src/main.cpp)

find_package(Threads REQUIRED)

target_include_directories(Server PUBLIC include)
target_link_libraries(Server PUBLIC Plugins CONAN_PKG::sol2 Net Database CONAN_PKG::boost CONAN_PKG::spdlog Protocol ${CMAKE_THREAD_LIBS_INIT} Emoji Actions CONAN_PKG::sqlitecpp Snowflake CONAN_PKG::docopt.cpp CONAN_PKG::yaml-cpp)
target_link_libraries(chattere_server PUBLIC Server) # Plugins CONAN_PKG::sol2 Net Database CONAN_PKG::boost CONAN_PKG::spdlog Protocol ${CMAKE_THREAD_LIBS_INIT} Emoji Actions CONAN_PKG::sqlitecpp Snowflake CONAN_PKG::docopt.cpp CONAN_PKG::yaml-cpp)
# target_link_libraries(Server INTERFACE ProtoTest)
# target_include_directories(Server PRIVATE ${CMAKE_BINARY_DIR})
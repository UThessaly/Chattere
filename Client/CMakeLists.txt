add_executable(Client src/main.cpp)

find_package(Threads REQUIRED)

target_include_directories(Client PUBLIC include)
target_link_libraries(Client PUBLIC Net CONAN_PKG::spdlog Protocol ${CMAKE_THREAD_LIBS_INIT} Emoji)
add_executable(client src/main.cpp)

find_package(Threads REQUIRED)

target_include_directories(client PUBLIC include)
target_link_libraries(client PUBLIC Net CONAN_PKG::spdlog Protocol ${CMAKE_THREAD_LIBS_INIT} Emoji CONAN_PKG::docopt.cpp)
add_library(Events STATIC src/events.cpp)

target_include_directories(Events PUBLIC include)

target_link_libraries(Events PRIVATE CONAN_PKG::spdlog)
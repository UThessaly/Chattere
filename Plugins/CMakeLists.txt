add_library(Plugins STATIC src/plugins.cpp)

target_include_directories(Plugins PUBLIC include)

target_link_libraries(Plugins PRIVATE CONAN_PKG::spdlog Server)
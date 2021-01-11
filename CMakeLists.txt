cmake_minimum_required(VERSION 3.13.4)
project(Chattere VERSION 0.1.0)

include(CTest)
enable_testing()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES 
                    lua/5.4.1
                    nlohmann_json/3.9.1
                    spdlog/1.8.2
                    protobuf/3.13.0
                    leveldb/1.22
                    sqlitecpp/3.1.1
                    docopt.cpp/0.6.3
                    yaml-cpp/0.6.3
                    boost/1.75.0
                    sol2/3.2.3
                OPTIONS 
                  lua:shared=True
                GENERATORS    
                    cmake
                    cmake_find_package_multi
                BASIC_SETUP CMAKE_TARGETS)

# set (conan_cmd "conan")
# set (conan_arg_install "install")
# set (conan_arg_directory "..")

# execute_process(COMMAND ${conan_cmd} ${conan_arg_install} ${conan_arg_directory}
#                 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
#                 RESULT_VARIABLE conan_result
#                 OUTPUT_VARIABLE conan_ver
# )

set(CMAKE_CXX_STANDARD 20)

# if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#     include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#     conan_basic_setup(TARGETS KEEP_RPATHS)
# else() 
#     message(FATAL_ERROR "Conan was not initialized. Please initialize Conan with 'cd build && conan install ..'." "To install Conan, go to https://docs.conan.io/en/latest/installation.html")
# endif()

find_package(Protobuf REQUIRED)
include_directories(
    ${Protobuf_INCLUDE_DIRS}
)

# add_subdirectory(ProtocolBuffer)
add_subdirectory(Protocol)
add_subdirectory(Actions)
add_subdirectory(Net)
add_subdirectory(Client)
add_subdirectory(Emoji)
add_subdirectory(Events)
add_subdirectory(Database)
add_subdirectory(Snowflakes)
add_subdirectory(Server)
add_subdirectory(Plugins)
add_subdirectory(PluginManager)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.16.3)
project(Vocabulary_Backend)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++-12")
#set(CMAKE_CXX_COMPILER "/opt/intel/oneapi/compiler/2023.1.0/linux/bin/icpx")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# cmake -DCMAKE_BUILD_TYPE = DEBUG | RELEASE | RELWITHDEBINFO | MINSIZEREL ...
set(CMAKE_BUILD_TYPE RELEASE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

# restbed
set(RESTBED_INCLUDE_DIRS /usr/local/include/)
set(RESTBED_LIBRARY_DIRS /usr/local/library/)
message("restbed binary path is : " ${RESTBED_INCLUDE_DIRS})
message("restbed include path is: " ${RESTBED_LIBRARY_DIRS})

include_directories(${RESTBED_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
link_directories(${RESTBED_LIBRARY_DIRS})

# Search OpenSSL
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")
else()
    message("Error; openssl not found!")
endif()

include(FetchContent)

include_directories(/Vocabulary/include)
include_directories(/Vocabulary/src)

find_package(Threads REQUIRED)

set(CommonLibs restbed ssl crypt sqlite3 spdlog::spdlog Threads::Threads)

find_package(Boost 1.82 REQUIRED COMPONENTS system)

file(GLOB SOURCES Vocabulary/src/Core/*.cpp
                  Vocabulary/src/json_singletons/*.cpp
                  Vocabulary/src/sqlite_database/*.cpp)

add_executable(Vocabulary_Backend ${SOURCES})

target_link_libraries(Vocabulary_Backend PRIVATE ${CommonLibs} ${Boost_LIBRARIES})
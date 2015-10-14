cmake_minimum_required (VERSION 2.6)
project (BitChess C CXX)


# Check that Google Test and Google Mock source directories are set
set(GMOCK_DIR "" CACHE PATH "Path to the GMock source")
set(GTEST_DIR "" CACHE PATH "Path to the GTest source")
message(STATUS "GMOCK_DIR=${GMOCK_DIR}")
message(STATUS "GTEST_DIR=${GTEST_DIR}")

if(${GMOCK_DIR} OR NOT EXISTS "${GMOCK_DIR}/CMakeLists.txt")
  message(FATAL_ERROR "GMOCK_DIR isn't set properly!")
endif()
if(${GTEST_DIR} OR NOT EXISTS "${GTEST_DIR}/CMakeLists.txt")
  message(FATAL_ERROR "GTEST_DIR isn't set properly!")
endif()


# C++11 support

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Version
set (BitChess_VERSION_MAJOR 1)
set (BitChess_VERSION_MINOR 0)

include_directories(
  src/
  test/
 )

find_package(Threads REQUIRED)

include_directories(${GMOCK_DIR}/include)
include_directories(${GTEST_DIR}/include)

# Add GMock/GTest to build, with output in lib/gmock
#add_subdirectory(${GTEST_DIR} lib/gtest)
add_subdirectory(${GMOCK_DIR} lib/gmock)

# add source files
add_executable(
  BitChessTests

  src/position/pieces/pawn.cpp
  src/position/pieces/pieces.cpp
  src/util/bitboard.cpp
  #src/main.cpp # disabled for tests
  src/xboard.cpp

  test/all_tests.cpp
  test/position/pieces/test_king_movegen.cpp
  test/position/pieces/test_knight_movegen.cpp
  test/position/pieces/test_pawn_movegen.cpp
  test/util/test_bitboard.cpp
  test/util/test_index_to_algebraic.cpp
  test/util/test_movelookup_king.cpp
  test/util/test_movelookup_knight.cpp
  test/util/test_movelookup_pawn.cpp
)


# Create dependency of BitChessTests on googlemock
add_dependencies(BitChessTests ${GMOCK_DIR})

if(THREADS_HAVE_PTHREAD_ARG)
  set_property(TARGET BitChess PROPERTY COMPILE_OPTIONS "-pthread")
  set_property(TARGET BitChess PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()
target_link_libraries(
  BitChessTests
  ${CMAKE_THREAD_LIBS_INIT}
  gmock
  )

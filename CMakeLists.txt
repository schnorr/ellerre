cmake_minimum_required(VERSION 3.10)
project(ellerre LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

flex_target(scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
bison_target(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc VERBOSE REPORT_FILE parser.output)
add_flex_bison_dependency(scanner parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${ellerre_SOURCE_DIR}/src/)

add_executable(firstfollow
  src/main.cc
  src/symbol.cc
  src/rule.cc
  src/item.cc
  src/grammar.cc
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
)

add_executable(lr0
  src/main_lr0.cc
  src/symbol.cc
  src/rule.cc
  src/item.cc
  src/grammar.cc
  src/state.cc
  src/parser.cc
  src/LR0.cc
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
)

add_executable(lr1
  src/main_lr1.cc
  src/symbol.cc
  src/rule.cc
  src/item.cc
  src/grammar.cc
  src/state.cc
  src/parser.cc
  src/LR1.cc
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
)

add_executable(lalr1
  src/main_lalr1.cc
  src/symbol.cc
  src/rule.cc
  src/item.cc
  src/grammar.cc
  src/state.cc
  src/parser.cc
  src/LALR1.cc
  src/LR1.cc
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
)

target_link_libraries(firstfollow ${FLEX_LIBRARIES})
target_link_libraries(lr0 ${FLEX_LIBRARIES})
target_link_libraries(lr1 ${FLEX_LIBRARIES})
target_link_libraries(lalr1 ${FLEX_LIBRARIES})

option(SHOW_STEPS "Print automata generation steps" OFF)

if(SHOW_STEPS)
  add_definitions(-DSTEPS)
endif()
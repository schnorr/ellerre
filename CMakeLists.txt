CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT(ellerre)
FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(BISON REQUIRED)
FLEX_TARGET(scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc VERBOSE REPORT_FILE parser.output)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${ellerre_SOURCE_DIR}/src/)
ADD_EXECUTABLE(firstfollow
  src/main.cc
  src/symbol.cc
  src/rule.cc
  src/item.cc
  src/grammar.cc
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
)
ADD_EXECUTABLE(lr0
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
ADD_EXECUTABLE(lr1
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
ADD_EXECUTABLE(lalr1
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
TARGET_LINK_LIBRARIES(firstfollow ${FLEX_LIBRARIES})
TARGET_LINK_LIBRARIES(lr0 ${FLEX_LIBRARIES})
TARGET_LINK_LIBRARIES(lr1 ${FLEX_LIBRARIES})
TARGET_LINK_LIBRARIES(lalr1 ${FLEX_LIBRARIES})
SET(CMAKE_CXX_FLAGS "-std=gnu++11")

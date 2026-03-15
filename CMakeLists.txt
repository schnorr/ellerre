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
# ---- Unit tests: one per (binary, example) pair ----
enable_testing()

add_test(NAME firstfollow_9.23_book
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/9.23_book.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/9.23_book.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_9.30_book
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/9.30_book.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/9.30_book.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_AB
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/AB.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/AB.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_Aa-s
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/Aa-s.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/Aa-s.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_SA
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/SA.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/SA.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_XYZ
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/XYZ.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/XYZ.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_a6
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/a6.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/a6.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_aa
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/aa.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/aa.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_abba
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/abba.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/abba.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_apr01
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apr01.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/apr01.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_apr02
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apr02.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/apr02.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_apri03
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apri03.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/apri03.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_ex0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/ex0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_ex1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/ex1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_ex1b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex1b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/ex1b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_g0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/g0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_g1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/g1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_g2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/g2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_g2a
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g2a.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/g2a.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_g3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/g3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_g4
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g4.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/g4.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_g5
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g5.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/g5.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_gra01
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra01.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/gra01.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_gra02
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra02.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/gra02.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_gra03
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra03.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/gra03.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_gra03b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra03b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/gra03b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_if-else
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/if-else.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/if-else.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_jan
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/jan.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/jan.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_kka
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/kka.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/kka.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_lalr1_bug_check
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lalr1_bug_check.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/lalr1_bug_check.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_ll0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/ll0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_ll1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/ll1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_ll2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/ll2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_ll3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/ll3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_lr1_nao_lalr1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_lalr1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/lr1_nao_lalr1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_lr1_nao_slr1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_slr1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/lr1_nao_slr1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_lr1_nao_slr1-v2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_slr1-v2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/lr1_nao_slr1-v2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_muito_simples
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/muito_simples.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/muito_simples.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_n0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/n0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/n0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_n00
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/n00.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/n00.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q2y22
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2y22.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q2y22.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q2y22b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2y22b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q2y22b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q3a
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q3a.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q3a.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q4
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q4.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q4.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q5
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q5.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q5.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q6
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q6.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q6.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q7
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q7.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q7.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q8
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q8.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q8.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q9
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q9.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q9.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_q9_ll1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q9_ll1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/q9_ll1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_set
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/set.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/set.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_slr1-problema
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/slr1-problema.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/slr1-problema.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_www
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/www.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/www.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME firstfollow_y17
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:firstfollow>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/y17.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/firstfollow/y17.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_9.23_book
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/9.23_book.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/9.23_book.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_9.30_book
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/9.30_book.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/9.30_book.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_AB
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/AB.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/AB.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_Aa-s
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/Aa-s.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/Aa-s.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_SA
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/SA.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/SA.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_XYZ
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/XYZ.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/XYZ.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_a6
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/a6.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/a6.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_aa
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/aa.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/aa.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_abba
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/abba.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/abba.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_apr01
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apr01.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/apr01.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_apr02
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apr02.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/apr02.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_apri03
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apri03.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/apri03.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_ex0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/ex0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_ex1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/ex1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_ex1b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex1b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/ex1b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_g0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/g0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_g1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/g1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_g2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/g2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_g2a
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g2a.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/g2a.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_g3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/g3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_g4
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g4.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/g4.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_g5
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g5.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/g5.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_gra01
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra01.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/gra01.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_gra02
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra02.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/gra02.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_gra03
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra03.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/gra03.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_gra03b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra03b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/gra03b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_if-else
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/if-else.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/if-else.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_jan
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/jan.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/jan.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_kka
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/kka.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/kka.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_lalr1_bug_check
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lalr1_bug_check.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/lalr1_bug_check.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_ll0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/ll0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_ll1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/ll1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_ll2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/ll2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_ll3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/ll3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_lr1_nao_lalr1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_lalr1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/lr1_nao_lalr1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_lr1_nao_slr1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_slr1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/lr1_nao_slr1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_lr1_nao_slr1-v2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_slr1-v2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/lr1_nao_slr1-v2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_muito_simples
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/muito_simples.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/muito_simples.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_n0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/n0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/n0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_n00
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/n00.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/n00.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q2y22
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2y22.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q2y22.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q2y22b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2y22b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q2y22b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q3a
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q3a.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q3a.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q4
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q4.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q4.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q5
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q5.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q5.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q6
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q6.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q6.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q7
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q7.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q7.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q8
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q8.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q8.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q9
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q9.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q9.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_q9_ll1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q9_ll1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/q9_ll1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_set
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/set.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/set.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_slr1-problema
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/slr1-problema.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/slr1-problema.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_www
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/www.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/www.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr0_y17
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr0>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/y17.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr0/y17.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_9.23_book
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/9.23_book.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/9.23_book.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_9.30_book
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/9.30_book.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/9.30_book.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_AB
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/AB.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/AB.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_Aa-s
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/Aa-s.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/Aa-s.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_SA
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/SA.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/SA.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_XYZ
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/XYZ.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/XYZ.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_a6
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/a6.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/a6.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_aa
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/aa.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/aa.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_abba
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/abba.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/abba.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_apr01
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apr01.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/apr01.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_apr02
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apr02.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/apr02.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_apri03
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apri03.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/apri03.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_ex0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/ex0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_ex1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/ex1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_ex1b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex1b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/ex1b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_g0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/g0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_g1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/g1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_g2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/g2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_g2a
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g2a.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/g2a.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_g3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/g3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_g4
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g4.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/g4.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_g5
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g5.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/g5.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_gra01
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra01.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/gra01.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_gra02
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra02.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/gra02.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_gra03
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra03.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/gra03.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_gra03b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra03b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/gra03b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_if-else
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/if-else.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/if-else.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_jan
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/jan.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/jan.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_kka
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/kka.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/kka.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_lalr1_bug_check
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lalr1_bug_check.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/lalr1_bug_check.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_ll0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/ll0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_ll1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/ll1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_ll2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/ll2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_ll3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/ll3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_lr1_nao_lalr1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_lalr1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/lr1_nao_lalr1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_lr1_nao_slr1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_slr1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/lr1_nao_slr1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_lr1_nao_slr1-v2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_slr1-v2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/lr1_nao_slr1-v2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_muito_simples
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/muito_simples.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/muito_simples.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_n0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/n0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/n0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_n00
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/n00.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/n00.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q2y22
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2y22.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q2y22.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q2y22b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2y22b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q2y22b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q3a
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q3a.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q3a.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q4
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q4.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q4.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q5
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q5.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q5.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q6
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q6.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q6.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q7
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q7.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q7.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q8
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q8.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q8.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q9
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q9.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q9.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_q9_ll1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q9_ll1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/q9_ll1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_set
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/set.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/set.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_slr1-problema
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/slr1-problema.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/slr1-problema.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_www
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/www.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/www.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lr1_y17
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/y17.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lr1/y17.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_9.23_book
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/9.23_book.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/9.23_book.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_9.30_book
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/9.30_book.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/9.30_book.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_AB
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/AB.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/AB.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_Aa-s
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/Aa-s.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/Aa-s.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_SA
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/SA.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/SA.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_XYZ
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/XYZ.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/XYZ.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_a6
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/a6.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/a6.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_aa
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/aa.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/aa.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_abba
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/abba.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/abba.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_apr01
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apr01.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/apr01.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_apr02
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apr02.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/apr02.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_apri03
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/apri03.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/apri03.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_ex0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/ex0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_ex1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/ex1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_ex1b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ex1b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/ex1b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_g0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/g0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_g1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/g1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_g2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/g2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_g2a
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g2a.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/g2a.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_g3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/g3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_g4
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g4.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/g4.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_g5
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/g5.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/g5.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_gra01
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra01.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/gra01.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_gra02
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra02.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/gra02.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_gra03
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra03.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/gra03.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_gra03b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/gra03b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/gra03b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_if-else
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/if-else.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/if-else.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_jan
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/jan.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/jan.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_kka
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/kka.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/kka.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_lalr1_bug_check
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lalr1_bug_check.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/lalr1_bug_check.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_ll0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/ll0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_ll1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/ll1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_ll2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/ll2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_ll3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/ll3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/ll3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_lr1_nao_lalr1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_lalr1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/lr1_nao_lalr1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_lr1_nao_slr1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_slr1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/lr1_nao_slr1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_lr1_nao_slr1-v2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/lr1_nao_slr1-v2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/lr1_nao_slr1-v2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_muito_simples
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/muito_simples.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/muito_simples.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_n0
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/n0.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/n0.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_n00
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/n00.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/n00.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q2
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q2.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q2y22
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2y22.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q2y22.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q2y22b
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q2y22b.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q2y22b.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q3
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q3.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q3.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q3a
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q3a.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q3a.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q4
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q4.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q4.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q5
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q5.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q5.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q6
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q6.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q6.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q7
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q7.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q7.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q8
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q8.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q8.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q9
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q9.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q9.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_q9_ll1
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/q9_ll1.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/q9_ll1.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_set
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/set.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/set.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_slr1-problema
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/slr1-problema.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/slr1-problema.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_www
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/www.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/www.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

add_test(NAME lalr1_y17
  COMMAND ${CMAKE_COMMAND}
    -DBINARY=$<TARGET_FILE:lalr1>
    -DINPUT=${CMAKE_SOURCE_DIR}/examples/y17.ee
    -DEXPECTED=${CMAKE_SOURCE_DIR}/tests/expected/lalr1/y17.txt
    -P ${CMAKE_SOURCE_DIR}/tests/run_test.cmake
)

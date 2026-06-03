/**
 * @file tests/main.cpp
 * @author BorisKlimov
 *
 * Главный файл запуска модульных тестов проекта.
 */

#include <iostream>
#include "test.hpp"

int main() {
  std::cout << "=== STARTING ALL TESTS ===" << std::endl;

  TestImplicitHeatConductionSolver();

  std::cout << "=== ALL TESTS SUCCESSFULLY PASSED ===" << std::endl;
  return 0;
}

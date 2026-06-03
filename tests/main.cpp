/**
 * @file tests/main.cpp
 * @author Ваше Имя
 *
 * Главный файл запуска модульных тестов проекта.
 */

#include <iostream>
#include "test.hpp"

int main() {
  std::cout << "=== STARTING ALL TESTS ===" << std::endl;

  // 1. Вызов эталонного теста
  TestHeatConductionReferenceExampleSolver();

  // 2. Вызов нашего нового теста (по алфавиту)
  TestImplicitHeatConductionSolver();

  std::cout << "=== ALL TESTS SUCCESSFULLY PASSED ===" << std::endl;
  return 0;
}

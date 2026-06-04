/**
 * @file tests/test.hpp
 * @author BorisKlimov
 * @brief Заголовочный файл со списками прототипов тестов.
 */

#ifndef TESTS_TEST_HPP_
#define TESTS_TEST_HPP_

#include <httplib.h>

/* Сюда нужно добавить объявления тестовых функций. */

/**
 * @brief Набор тестов для класса mm::ImplicitHeatConductionSolver.
 */
void TestImplicitHeatConductionSolver(httplib::Client* cli);

/* Конец вставки. */

#endif  // TESTS_TEST_HPP_
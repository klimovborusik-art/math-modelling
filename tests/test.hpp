/**
 * @file tests/test.hpp
 * @author BorisKlimov
 *
 * Заголовочный файл со списками прототипов тестов.
 */

#ifndef TESTS_TEST_HPP_
#define TESTS_TEST_HPP_

/**
 *  Тест эталонного примера явной схемы теплопроводности.
 */
void TestHeatConductionReferenceExampleSolver();

/**
 *  Тест нашего нового алгоритма неявной схемы теплопроводности.
 */
void TestImplicitHeatConductionSolver();

#endif  // TESTS_TEST_HPP_
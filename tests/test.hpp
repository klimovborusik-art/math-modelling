/**
 * @file tests/test.hpp
 * @author BorisKlimov
 *
 * Заголовочный файл со списками прототипов тестов.
 */

#ifndef TEST_HPP_
#define TEST_HPP_

/**
 *  Тест эталонного примера явной схемы теплопроводности.
 */
void TestHeatConductionReferenceExampleSolver();

/**
 *  Тест нашего нового алгоритма неявной схемы теплопроводности.
 */
void TestImplicitHeatConductionSolver();

#endif // TEST_HPP_

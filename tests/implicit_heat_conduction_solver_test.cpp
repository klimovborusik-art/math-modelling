/**
 * @file tests/implicit_heat_conduction_solver_test.cpp
 * @author BorisKlimov
 *
 * Набор модульных тестов для неявного метода теплопроводности.
 */

#include "test.hpp"
#include <implicit_heat_conduction_solver.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

void TestImplicitHeatConductionSolver() {
  std::cout << "TestImplicitHeatConductionSolver..." << std::endl;

  // ---------------------------------------------------------------------------
  // Тест 1: Проверка базовой размерности сетки (Статический тест)
  // ---------------------------------------------------------------------------
  {
    double tau = 0.01;
    double finishTime = 0.02; // Выполнится ровно 2 шага (0.01, 0.02)
    double exportPeriod = 0.01;
    int M = 4; // Размерность сетки (M+1)x(M+1) = 5x5 = 25 точек

    mm::ImplicitHeatConductionSolver<double> solver(
      tau, finishTime, exportPeriod, M);

    // Запуск пустой заглушки/алгоритма для компиляции шаблона
    bool status = solver.MakeStep();
    assert(status == true);

    nlohmann::json output;
    solver.ExportData(&output);

    assert(output.contains("M"));
    assert(output.contains("fn"));
    assert(output["M"].get<int>() == M);
    
    // Общее количество точек на слое должно быть (M + 1) * (M + 1) = 25
    assert(output["fn"].size() == static_cast<size_t>((M + 1) * (M + 1)));
  }

  // ---------------------------------------------------------------------------
  // Тест 2: Проверка граничного условия Дирихле справа (Статический тест)
  // ---------------------------------------------------------------------------
  {
    double tau = 0.005;
    double finishTime = 0.005;
    double exportPeriod = 0.005;
    int M = 10;

    mm::ImplicitHeatConductionSolver<double> solver(
      tau, finishTime, exportPeriod, M);

    solver.MakeStep();

    nlohmann::json output;
    solver.ExportData(&output);
    auto fn = output["fn"];

    // Справа (при j = M) условие Дирихле u = -1
    // Индекс в одномерном векторе: i * (M + 1) + M
    for (int i = 0; i <= M; ++i) {
      double uRight = fn[i * (M + 1) + M].get<double>();
      assert(std::abs(uRight - (-1.0)) < 1e-6);
    }
  }

  // ---------------------------------------------------------------------------
  // Тест 3: Тест со случайными параметрами разбиения (Случайный тест)
  // ---------------------------------------------------------------------------
  {
    std::mt19937 gen(1337); 
    std::uniform_int_distribution<int> distM(5, 15);

    int randM = distM(gen);
    double tau = 0.001;
    double finishTime = 0.001;
    double exportPeriod = 0.001;

    mm::ImplicitHeatConductionSolver<double> solver(
      tau, finishTime, exportPeriod, randM);

    assert(solver.MakeStep() == true);

    nlohmann::json output;
    solver.ExportData(&output);
    
    assert(output["M"].get<int>() == randM);
    assert(output["fn"].size() == 
      static_cast<size_t>((randM + 1) * (randM + 1)));
  }

  std::cout << "TestImplicitHeatConductionSolver passed" 
            << std::endl;
}

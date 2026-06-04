/**
 * @file methods/implicit_heat_conduction_solver_method.cpp
 * @author BorisKlimov
 * @brief Парсер входных данных и инициализатор для класса mm::ImplicitHeatConductionSolver.
 */

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <implicit_heat_conduction_solver.hpp>
#include "tasks_queue.hpp"

using mm::ImplicitHeatConductionSolver;

namespace mm {

/**
 * @brief Вспомогательная шаблонная функция для парсера входных данных и
 * инициализатора класса mm::ImplicitHeatConductionSolver.
 * @tparam T Тип данных (float или double).
 * @tparam Wrapper Тип обертки для задачи.
 * @param input Входной JSON с параметрами.
 * @param output Выходной JSON с ID задачи.
 * @param tasksQueue Очередь задач сервера.
 * @return int 0 в случае успеха, -1 при ошибке.
 */
template<typename T, typename Wrapper>
int ImplicitHeatConductionSolverMethodHelper(
  const nlohmann::json& input,
  nlohmann::json* output,
  TasksQueue* tasksQueue);

/**
 * @brief Основной обработчик метода неявной теплопроводности.
 * @param input Входной JSON от клиента.
 * @param output Ответ сервера для клиента.
 * @param tasksQueue Очередь асинхронных задач.
 * @return int 0 в случае успеха, -1 при ошибке.
 */
int ImplicitHeatConductionSolverMethod(
  const nlohmann::json& input,
  nlohmann::json* output,
  TasksQueue* tasksQueue) {
  std::string valueType = input.at("value_type");

  if (valueType == "float") {
    return ImplicitHeatConductionSolverMethodHelper
      <float, FloatAbstractSolverWrapper>(input, output, tasksQueue);
  } else if (valueType == "double") {
    return ImplicitHeatConductionSolverMethodHelper
      <double, DoubleAbstractSolverWrapper>(input, output, tasksQueue);
  }

  return -1;
}

template<typename T, typename Wrapper>
int ImplicitHeatConductionSolverMethodHelper(
  const nlohmann::json& input,
  nlohmann::json* output,
  TasksQueue* tasksQueue) {
  ImplicitHeatConductionSolver<T>* solver;

  T tau = input.at("tau");
  T finishTime = input.at("finish_time");
  T exportPeriod = input.at("export_period");
  int M = input.at("M");

  if (M < 0 || tau < 0 || finishTime < 0) {
    return -1;
  }

  solver = new ImplicitHeatConductionSolver<T>(
    tau, finishTime, exportPeriod, M);

  Wrapper* wrapper = new Wrapper(solver);
  int taskId = tasksQueue->AddTask(wrapper);

  (*output)["id"] = taskId;
  (*output)["status"] = "ok";

  return 0;
}

}  // namespace mm

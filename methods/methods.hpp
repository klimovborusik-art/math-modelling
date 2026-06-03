/**
 * @file methods/methods.hpp
 * @author BorisKlimov
 *
 * Объявления серверных методов (парсеров) для алгоритмов.
 */

#ifndef METHODS_METHODS_HPP_
#define METHODS_METHODS_HPP_

#include <nlohmann/json.hpp>
#include "tasks_queue.hpp"

namespace mm {

/**
 * @brief Парсер входных данных и инициализатор для класса
 * mm::HeatConductionReferenceExampleSolver.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @param tasksQueue Очередь задач.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 */
int HeatConductionReferenceExampleSolverMethod(
  const nlohmann::json& input,
  nlohmann::json* output,
  TasksQueue* tasksQueue);

/**
 * @brief Парсер входных данных и инициализатор для класса
 * mm::ImplicitHeatConductionSolver.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @param tasksQueue Очередь задач.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 */
int ImplicitHeatConductionSolverMethod(
  const nlohmann::json& input,
  nlohmann::json* output,
  TasksQueue* tasksQueue);

}  // namespace mm

#endif  // METHODS_METHODS_HPP_
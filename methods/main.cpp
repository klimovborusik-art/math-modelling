/**
 * @file methods/main.cpp
 * @author BorisKlimov
 *
 * Точка входа для веб-сервера математического моделирования.
 */

#include <iostream>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "methods.hpp"
#include "tasks_queue.hpp"

int main(int argc, char* argv[]) {
  // По умолчанию сервер слушает порт 8080 на локальном хосте
  std::string host = "localhost";
  int port = 8080;

  if (argc > 1) {
    host = argv[1];
  }
  if (argc > 2) {
    port = std::atoi(argv[2]);
  }

  httplib::Server svr;
  mm::TasksQueue tasksQueue;

  std::cout << "[SERVER] Starting math server on " 
            << host << ":" << port << "..." << std::endl;

  svr.Post("/HeatConductionReferenceExampleSolver",
    [&](const httplib::Request& req, httplib::Response& res) {
      nlohmann::json input = nlohmann::json::parse(req.body);
      nlohmann::json output;
      
      if (mm::HeatConductionReferenceExampleSolverMethod(
            input, &output, &tasksQueue) < 0) {
        res.status = 400;
      }
      
      res.set_content(output.dump(), "application/json");
    });

  // 2. Обработчик для неявной схемы 
  svr.Post("/ImplicitHeatConductionSolver",
    [&](const httplib::Request& req, httplib::Response& res) {
      nlohmann::json input = nlohmann::json::parse(req.body);
      nlohmann::json output;
      
      if (mm::ImplicitHeatConductionSolverMethod(
            input, &output, &tasksQueue) < 0) {
        res.status = 400;
      }
      
      res.set_content(output.dump(), "application/json");
    });

  // Запуск прослушивания сетевых запросов
  if (!svr.listen(host.c_str(), port)) {
    std::cerr << " Failed to start httplib server" << std::endl;
    return 1;
  }

  return 0;
}

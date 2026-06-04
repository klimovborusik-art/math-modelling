/**
 * @file tests/implicit_heat_conduction_solver_test.cpp
 * @author BorisKlimov
 * @brief Набор модульных тестов для 2D неявного метода теплопроводности.
 */

#include <httplib.h>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
#include "test.hpp"
#include "test_core.hpp"

void TestImplicitHeatConductionSolver(httplib::Client* cli)
{
  // -------------------------------------------------------------------------
  // 1. Отправка задачи на сервер (2D параметры)
  // -------------------------------------------------------------------------
  nlohmann::json input = R"(
{
  "value_type": "double",
  "Nx": 50,
  "Ny": 50,
  "tau": 0.0001,
  "finish_time": 0.01,
  "export_period": 0.002
}
)"_json;

  auto res = cli->Post("/ImplicitHeatConductionSolver", input.dump(),
                       "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);
  REQUIRE(output.find("id") != output.end());

  int taskId = output["id"];

  // -------------------------------------------------------------------------
  // 2. Ожидание завершения задачи
  // -------------------------------------------------------------------------
  const int numTries = 200;
  bool success = false;

  for (int k = 0; k < numTries; ++k) {
    char buffer[1024];
    const char* format = R"({"id": %d})";
    snprintf(buffer, sizeof(buffer), format, taskId);

    auto statusRes = cli->Post("/CheckTaskStatus", buffer,
                               "application/json");

    if (!statusRes) {
      REQUIRE(false);
    }

    nlohmann::json statusOutput = nlohmann::json::parse(statusRes->body);
    std::string status = statusOutput.at("status");

    if (status == "finished") {
      success = true;
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  REQUIRE(success);

  // -------------------------------------------------------------------------
  // 3. Загрузка и проверка результатов
  // -------------------------------------------------------------------------
  {
    char buffer[1024];
    const char* format = R"({"id": %d})";
    snprintf(buffer, sizeof(buffer), format, taskId);

    auto dataRes = cli->Post("/DownloadTaskData", buffer,
                             "application/json");

    if (!dataRes) {
      REQUIRE(false);
    }

    nlohmann::json dataOutput = nlohmann::json::parse(dataRes->body);
    REQUIRE(dataOutput.at("status") == "ok");

    int numFrames = dataOutput.at("data").size();
    REQUIRE(numFrames > 0);

    auto& lastFrame = dataOutput.at("data").back();
    int Nx = lastFrame.at("data").at("Nx");
    int Ny = lastFrame.at("data").at("Ny");
    auto& data = lastFrame.at("data").at("fn");

    REQUIRE(Nx > 0);
    REQUIRE(Ny > 0);
    REQUIRE(data.size() == static_cast<size_t>(Nx * Ny));

    // -----------------------------------------------------------------------
    // Проверка граничных условий для 2D задачи
    // -----------------------------------------------------------------------
    const double precision = 1e-6;

    // Левая граница (x = 0): u = y - 1
    for (int i = 0; i < Ny; ++i) {
      double y = static_cast<double>(i) / (Ny - 1);
      double expected = y - 1.0;
      double value = data[i * Nx + 0];
      REQUIRE_CLOSE(value, expected, precision);
    }

    // Правая граница (x = 1): u = -1
    for (int i = 0; i < Ny; ++i) {
      double value = data[i * Nx + (Nx - 1)];
      REQUIRE_CLOSE(value, -1.0, precision);
    }

    // Нижняя граница (y = 0): du/dy = 0
    // Проверяем близость значений на y=0 и y=dy
    for (int j = 0; j < Nx; ++j) {
      double valueBottom = data[0 * Nx + j];
      double valueNext = data[1 * Nx + j];
      REQUIRE(std::abs(valueBottom - valueNext) < 0.01);
    }

    // Верхняя граница (y = 1): u = -x
    for (int j = 0; j < Nx; ++j) {
      double x = static_cast<double>(j) / (Nx - 1);
      double expected = -x;
      double value = data[(Ny - 1) * Nx + j];
      REQUIRE_CLOSE(value, expected, precision);
    }

    // Принцип максимума: решение между min и max граничных условий
    // min = -1 (правый край), max = 0 (левая граница при y=1: u = 1-1=0)
    for (double value : data) {
      REQUIRE(value <= 0.001 && value >= -1.001);
    }
  }
}


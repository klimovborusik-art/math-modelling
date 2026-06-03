cmake_minimum_required(VERSION 3.5)

project(math-modelling CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

set(MATH_MODELLING_INCLUDE_DIRS "include")

# ====================================================================
# ЭТАЛОННЫЙ АВТОМАТИЧЕСКИЙ БЛОК ДЛЯ ЗАГРУЗКИ БИБЛИОТЕКИ HTTPLIB
# ====================================================================
find_package(Httplib)

if (NOT HTTPLIB_INCLUDE_DIR)
  file(DOWNLOAD https://github.com
      "${CMAKE_BINARY_DIR}/deps/cpp-httplib-v0.8.8.tar.gz"
      STATUS HTTPLIB_DOWNLOAD_STATUS_LIST LOG HTTPLIB_DOWNLOAD_LOG
      SHOW_PROGRESS)

  list(GET HTTPLIB_DOWNLOAD_STATUS_LIST 0 HTTPLIB_DOWNLOAD_STATUS)

  if (HTTPLIB_DOWNLOAD_STATUS EQUAL 0)
    execute_process(COMMAND ${CMAKE_COMMAND} -E
        tar xzf "${CMAKE_BINARY_DIR}/deps/cpp-httplib-v0.8.8.tar.gz"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/deps/")

    # Исправленный эталонный поиск: ищем строго заголовочный файл внутри распакованной папки
    file(GLOB_RECURSE HTTPLIB_FILES "${CMAKE_BINARY_DIR}/deps/cpp-httplib-*/httplib.h")
    
    if (HTTPLIB_FILES)
      list(GET HTTPLIB_FILES 0 FIRST_HTTPLIB_FILE)
      get_filename_component(HTTPLIB_INCLUDE_DIR "${FIRST_HTTPLIB_FILE}" DIRECTORY)

      set(MATH_MODELLING_INCLUDE_DIRS ${MATH_MODELLING_INCLUDE_DIRS} "${HTTPLIB_INCLUDE_DIR}")
      message(STATUS "Successfully downloaded httplib into ${HTTPLIB_INCLUDE_DIR}/")
    else ()
      message(FATAL_ERROR "Problem unpacking httplib! Cannot find httplib.h inside deps.")
    endif ()
  else ()
    list(GET HTTPLIB_DOWNLOAD_STATUS_LIST 1 HTTPLIB_DOWNLOAD_ERROR)
    message(FATAL_ERROR
        "Could not download httplib! Error code ${HTTPLIB_DOWNLOAD_STATUS}: ${HTTPLIB_DOWNLOAD_ERROR}!")
  endif ()
else ()
  set(MATH_MODELLING_INCLUDE_DIRS ${MATH_MODELLING_INCLUDE_DIRS} "${HTTPLIB_INCLUDE_DIR}")
endif ()

# ====================================================================
# ЭТАЛОННЫЙ АВТОМАТИЧЕСКИЙ БЛОК ДЛЯ ЗАГРУЗКИ БИБЛИОТЕКИ NLOHMANN JSON
# ====================================================================
find_package(NlohmannJson)

if (NOT NLOHMANN_JSON_INCLUDE_DIR)
  set(NLOHMANN_JSON_DIR "nlohmann-json-3.9.1")

  file(DOWNLOAD https://github.com
      "${CMAKE_BINARY_DIR}/deps/${NLOHMANN_JSON_DIR}/nlohmann/json.hpp"
      STATUS NLOHMANN_JSON_DOWNLOAD_STATUS_LIST LOG NLOHMANN_JSON_DOWNLOAD_LOG
      SHOW_PROGRESS)

  list(GET NLOHMANN_JSON_DOWNLOAD_STATUS_LIST 0 NLOHMANN_JSON_DOWNLOAD_STATUS)

  if (NLOHMANN_JSON_DOWNLOAD_STATUS EQUAL 0)
    file(SHA256 "${CMAKE_BINARY_DIR}/deps/${NLOHMANN_JSON_DIR}/nlohmann/json.hpp" NLOHMANN_JSON_LOCAL_HASH)
    set(NLOHMANN_JSON_EXPECTED_HASH "7804b38146921d03374549c9e2a5e3acda097814c43caf2b96a0278e58df26e0")

    if (NLOHMANN_JSON_LOCAL_HASH STREQUAL NLOHMANN_JSON_EXPECTED_HASH)
      set(NLOHMANN_JSON_INCLUDE_DIR "${CMAKE_BINARY_DIR}/deps/${NLOHMANN_JSON_DIR}")
      set(MATH_MODELLING_INCLUDE_DIRS ${MATH_MODELLING_INCLUDE_DIRS} "${NLOHMANN_JSON_INCLUDE_DIR}")
      message(STATUS "Successfully downloaded nlohmann json into ${CMAKE_BINARY_DIR}/deps/${NLOHMANN_JSON_DIR}/")
    else ()
      message(FATAL_ERROR "Error checking hash for nlohmann json!")
    endif ()
  else ()
    list(GET NLOHMANN_JSON_DOWNLOAD_STATUS_LIST 1 NLOHMANN_JSON_DOWNLOAD_ERROR)
    message(FATAL_ERROR "Could not download json! Error: ${NLOHMANN_JSON_DOWNLOAD_ERROR}")
  endif ()
else ()
  set(MATH_MODELLING_INCLUDE_DIRS ${MATH_MODELLING_INCLUDE_DIRS} "${NLOHMANN_JSON_INCLUDE_DIR}")
endif ()

if (NOT MSVC)
  set(MATH_MODELLING_LIBS "pthread")
endif ()

include_directories(${MATH_MODELLING_INCLUDE_DIRS})

# ====================================================================
# ИСХОДНЫЕ ФАЙЛЫ СЕРВЕРНОЙ ЧАСТИ (MATH_MODELLING_SERVER)
# ====================================================================
add_executable(
  math_modelling_server
  include/abstract_solver.hpp
  include/abstract_solver_impl.hpp
  include/abstract_solver_wrapper.hpp
  include/implicit_heat_conduction_solver.hpp
  methods/implicit_heat_conduction_solver_method.cpp
  methods/main.cpp
  methods/methods.hpp
  methods/tasks_queue.cpp
  methods/tasks_queue.hpp
)

target_link_libraries(math_modelling_server ${MATH_MODELLING_LIBS})

# ====================================================================
# ИСХОДНЫЕ ФАЙЛЫ КЛИЕНТСКОЙ ЧАСТИ (НАБОР МОДУЛЬНЫХ ТЕСТОВ)
# ====================================================================
add_executable(
  math_modelling_test
  include/implicit_heat_conduction_solver.hpp
  tests/implicit_heat_conduction_solver_test.cpp
  tests/io.hpp
  tests/main.cpp
  tests/test.hpp
  tests/test_core.cpp
  tests/test_core.hpp
)

target_link_libraries(math_modelling_test ${MATH_MODELLING_LIBS})

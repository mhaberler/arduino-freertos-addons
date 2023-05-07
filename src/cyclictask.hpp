
#pragma once
#include "freertos-config.h"
#include "task.hpp"

namespace cpp_freertos {
/**
 * @brief A Thread which has the Run method implemented as loop which is
 * executing the function that is passed in the constructor.
 *
 */
class CyclicTask : public Thread {
public:
  CyclicTask(const char *Name, uint16_t StackDepth, UBaseType_t Priority,
             void (*fn)(), uint32_t cycleTimeMs = 1)
      : Thread(Name, StackDepth, Priority) {
    task = fn;
    running = true;
    xFrequency = pdMS_TO_TICKS(cycleTimeMs);
  }
 /**
   * @brief set the task cycle time, in mS
   *
   */
  virtual void setRate(uint32_t cycleTimeMs) {
    xFrequency = pdMS_TO_TICKS(cycleTimeMs);
  }
//   virtual void suspend(void) {}
//   virtual void resume(void) {}

  /**
   * @brief Custom Implementation which is executing the provided method in a
   * loop
   *
   */
  virtual void Run() {
    xLastWakeTime = xTaskGetTickCount();
    while (true) {
      if (running) {
        task();
      }
      xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
    }

    // end task
    vTaskDelete(NULL);
    setThreadStarted(false);
  }
  /**
   * @brief number of times the task missed its
   * rescheduling time
   *
   */
  virtual int numDelays(void) { return xWasDelayed; }

protected:
  void (*task)() = nullptr;
  bool running;
  BaseType_t xWasDelayed;
  TickType_t xLastWakeTime;
  TickType_t xFrequency;
  uint32_t cycleTimeMs;
};

} // namespace cpp_freertos
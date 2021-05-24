#ifndef PERF_DATA_H
#define PERF_DATA_H

#include <chrono>
#include <iostream>
#include <unordered_map>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;

class PerfData {
 private:
  PerfData() {}

 public:
  static PerfData& getInstance() {
    static PerfData instance;
    return instance;
  }

  std::unordered_map<std::string, high_resolution_clock::duration> data;
  std::unordered_map<std::string, high_resolution_clock::time_point>
      running_timers;

  void StartTime(const std::string& timer_name) {
    if (running_timers.count(timer_name) > 0) {
      throw std::logic_error(
          "there is already a running timer with the name \"" + timer_name +
          "\"");
    }

    running_timers[timer_name] = high_resolution_clock::now();
  }

  high_resolution_clock::duration EndTime(const std::string& timer_name) {
    if (running_timers.count(timer_name) == 0) {
      throw std::logic_error("there is no running timer with the name \"" +
                             timer_name + "\"");
    }

    high_resolution_clock::duration duration =
        high_resolution_clock::now() - running_timers[timer_name];
    running_timers.erase(timer_name);
    data[timer_name] += duration;
    return duration;
  }

  template <typename T>
  void PrintDuration(const std::string& timer_name,
                     high_resolution_clock::duration duration,
                     const std::string& suffix) {
    typename T::rep time_num = duration_cast<T>(duration).count();
    std::cout << timer_name << ": " << time_num << suffix << std::endl;
  }

  template <typename T>
  void PrintData(const std::string& suffix) {
    if (running_timers.size() != 0) {
      throw std::logic_error("there are still timers running!");
    }

    if (data.size() == 0) {
      std::cout << "there is no data" << std::endl;
      return;
    }

    for (const auto& time : data) {
      PrintDuration<T>(time.first, time.second, suffix);
    }
  }

  void PrintUs() { PrintData<microseconds>("us"); }
  void PrintMs() { PrintData<milliseconds>("ms"); }
  void PrintS() { PrintData<seconds>("sec"); }

  PerfData(PerfData const&) = delete;
  void operator=(PerfData const&) = delete;
};

#endif
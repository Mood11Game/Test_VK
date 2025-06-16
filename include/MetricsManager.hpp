#pragma once
#include "Metric.hpp"
#include <vector>
#include <memory>
#include <thread>
#include <fstream>
#include <mutex>
#include <atomic>
#include <condition_variable>

class MetricsManager {
public:
    MetricsManager(const std::string& filename, int interval_ms = 1000);
    ~MetricsManager();

    void addMetric(std::shared_ptr<Metric> metric);
    void start();
    void stop();

private:
    std::vector<std::shared_ptr<Metric>> metrics;
    std::thread writer_thread;
    std::mutex mtx;
    std::atomic<bool> running;
    std::ofstream outfile;
    int interval;

    void run();
    std::string timestamp();
};
#include "MetricsManager.hpp"
#include <chrono>
#include <ctime>
#include <sstream>

MetricsManager::MetricsManager(const std::string& filename, int interval_ms)
    : interval(interval_ms), running(false), outfile(filename) {}

MetricsManager::~MetricsManager() {
    stop();
}

void MetricsManager::addMetric(std::shared_ptr<Metric> metric) {
    std::lock_guard<std::mutex> lock(mtx);
    metrics.push_back(metric);
}

void MetricsManager::start() {
    running = true;
    writer_thread = std::thread(&MetricsManager::run, this);
}

void MetricsManager::stop() {
    running = false;
    if (writer_thread.joinable()) writer_thread.join();
    outfile.close();
}

void MetricsManager::run() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        std::lock_guard<std::mutex> lock(mtx);

        outfile << timestamp();
        for (const auto& metric : metrics) {
            outfile << " " << metric->collect();
            metric->reset();
        }
        outfile << "\n";
        outfile.flush();
    }
}

std::string MetricsManager::timestamp() {
    using namespace std::chrono;
    auto now = steady_clock::now().time_since_epoch();
    double seconds = duration_cast<duration<double>>(now).count();
    std::ostringstream ss;
    ss << seconds;
    return ss.str();
}
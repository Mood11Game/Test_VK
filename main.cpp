#include "CPUMetric.hpp"
#include "HttpRequestsMetric.hpp"
#include "MetricsManager.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    auto cpu = std::make_shared<CPUMetric>();
    auto http = std::make_shared<HttpRequestsMetric>();

    MetricsManager manager("metrics_output.txt");
    manager.addMetric(cpu);
    manager.addMetric(http);
    manager.start();

    for (int i = 0; i < 100; ++i) {
        http->increment();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    manager.stop();

    std::cout << "complet\n";
    return 0;
}

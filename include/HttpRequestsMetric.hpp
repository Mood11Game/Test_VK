#pragma once
#include "Metric.hpp"
#include <atomic>

class HttpRequestsMetric : public Metric {
public:
    HttpRequestsMetric();

    std::string name() const override;
    std::string collect() override;
    void reset() override;

    void increment();

private:
    std::atomic<int> count;
};
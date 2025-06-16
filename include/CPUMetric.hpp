#pragma once
#include "Metric.hpp"
#include <mutex>

class CPUMetric : public Metric {
public:
    CPUMetric();

    std::string name() const override;
    std::string collect() override;
    void reset() override;

private:
    double last_value;
    mutable std::mutex mtx;

    double read_cpu_usage();
};
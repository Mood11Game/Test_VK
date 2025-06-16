#pragma once
#include <string>
#include <chrono>

class Metric {
public:
    virtual ~Metric() = default;
    virtual std::string name() const = 0;
    virtual std::string collect() = 0;
    virtual void reset() = 0;
};
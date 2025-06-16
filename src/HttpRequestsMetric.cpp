#include "HttpRequestsMetric.hpp"
#include <sstream>

HttpRequestsMetric::HttpRequestsMetric() : count(0) {}

std::string HttpRequestsMetric::name() const {
    return "HTTP_requests_RPS";
}

std::string HttpRequestsMetric::collect() {
    std::ostringstream ss;
    ss << name() << " " << count.load();
    return ss.str();
}

void HttpRequestsMetric::reset() {
    count.store(0);
}

void HttpRequestsMetric::increment() {
    count++;
}
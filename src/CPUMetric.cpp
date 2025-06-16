#include "CPUMetric.hpp"
#include <windows.h>
#include <sstream>

CPUMetric::CPUMetric() : last_value(0.0) {}

std::string CPUMetric::name() const {
    return "CPU";
}

std::string CPUMetric::collect() {
    std::lock_guard<std::mutex> lock(mtx);
    last_value = read_cpu_usage();
    std::ostringstream ss;
    ss << name() << " " << last_value;
    return ss.str();
}

void CPUMetric::reset() {
    std::lock_guard<std::mutex> lock(mtx);
    last_value = 0.0;
}

double CPUMetric::read_cpu_usage() {
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
        return 0.0;

    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;
    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;
    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;

    static ULARGE_INTEGER prevIdle = {0}, prevKernel = {0}, prevUser = {0};

    ULONGLONG sysIdle = idle.QuadPart - prevIdle.QuadPart;
    ULONGLONG sysKernel = kernel.QuadPart - prevKernel.QuadPart;
    ULONGLONG sysUser = user.QuadPart - prevUser.QuadPart;

    prevIdle = idle;
    prevKernel = kernel;
    prevUser = user;

    ULONGLONG total = sysKernel + sysUser;
    if (total == 0) return 0.0;

    return 1.0 * (1.0 - (double)sysIdle / total);
}
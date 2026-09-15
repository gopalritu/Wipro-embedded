#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

// Get CPU usage
double getCPUUsage() {
    ifstream file("/proc/stat");

    string cpu;
    long long user, nice, system, idle, iowait, irq, softirq, steal;

    file >> cpu >> user >> nice >> system >> idle
         >> iowait >> irq >> softirq >> steal;

    long long idleTime = idle + iowait;
    long long totalTime = user + nice + system + idle +
                          iowait + irq + softirq + steal;

    static long long previousIdle = 0;
    static long long previousTotal = 0;

    long long idleDiff = idleTime - previousIdle;
    long long totalDiff = totalTime - previousTotal;

    previousIdle = idleTime;
    previousTotal = totalTime;

    if (totalDiff == 0)
        return 0.0;

    return 100.0 * (1.0 - (double)idleDiff / totalDiff);
}

// Get memory usage
void getMemoryUsage(double &usedPercent, long long &usedMB, long long &totalMB) {
    ifstream file("/proc/meminfo");

    string key;
    long long value;
    string unit;

    long long total = 0;
    long long available = 0;

    while (file >> key >> value >> unit) {
        if (key == "MemTotal:")
            total = value;

        else if (key == "MemAvailable:")
            available = value;
    }

    long long used = total - available;

    totalMB = total / 1024;
    usedMB = used / 1024;

    usedPercent = (double)used / total * 100.0;
}

// Get system uptime
double getUptime() {
    ifstream file("/proc/uptime");

    double uptime;
    file >> uptime;

    return uptime;
}

// Display monitor
void displayMonitor() {

    // First CPU reading
    getCPUUsage();

    this_thread::sleep_for(chrono::seconds(1));

    double cpuUsage = getCPUUsage();

    double memoryPercent;
    long long usedMB, totalMB;

    getMemoryUsage(memoryPercent, usedMB, totalMB);

    double uptime = getUptime();

    int hours = uptime / 3600;
    int minutes = ((int)uptime % 3600) / 60;

    cout << "\033[2J\033[H";

    cout << "========================================\n";
    cout << "       LINUX SYSTEM RESOURCE MONITOR\n";
    cout << "========================================\n\n";

    cout << fixed << setprecision(2);

    cout << "CPU Usage       : " << cpuUsage << "%\n";

    cout << "Memory Usage    : " << memoryPercent
         << "% (" << usedMB << " MB / "
         << totalMB << " MB)\n";

    cout << "System Uptime   : "
         << hours << " hours "
         << minutes << " minutes\n";

    cout << "\n========================================\n";
    cout << "Press Ctrl+C to exit\n";
}

int main() {

    while (true) {
        displayMonitor();

        this_thread::sleep_for(chrono::seconds(2));
    }

    return 0;
}
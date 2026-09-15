#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <sys/utsname.h>

using namespace std;

// Class for System Information
class SystemInformation {
public:
    void displaySystemInfo() {
        struct utsname systemInfo;

        cout << "\n========================================\n";
        cout << "          SYSTEM INFORMATION\n";
        cout << "========================================\n";

        if (uname(&systemInfo) == 0) {
            cout << left << setw(25) << "Operating System:"
                 << systemInfo.sysname << endl;

            cout << left << setw(25) << "Kernel Release:"
                 << systemInfo.release << endl;

            cout << left << setw(25) << "Kernel Version:"
                 << systemInfo.version << endl;

            cout << left << setw(25) << "Architecture:"
                 << systemInfo.machine << endl;
        }

        char hostname[256];

        if (gethostname(hostname, sizeof(hostname)) == 0) {
            cout << left << setw(25) << "Hostname:"
                 << hostname << endl;
        }

        ifstream uptimeFile("/proc/uptime");

        if (uptimeFile.is_open()) {
            double uptimeSeconds;
            uptimeFile >> uptimeSeconds;

            int days = uptimeSeconds / 86400;
            int hours = ((int)uptimeSeconds % 86400) / 3600;
            int minutes = ((int)uptimeSeconds % 3600) / 60;

            cout << left << setw(25) << "System Uptime:"
                 << days << " days, "
                 << hours << " hours, "
                 << minutes << " minutes" << endl;

            uptimeFile.close();
        }

        cout << left << setw(25) << "CPU Cores:"
             << sysconf(_SC_NPROCESSORS_ONLN) << endl;
    }
};


// Class for CPU Information
class CPUInformation {
public:
    void displayCPUInfo() {
        ifstream cpuFile("/proc/cpuinfo");

        if (!cpuFile.is_open()) {
            cout << "Unable to open /proc/cpuinfo\n";
            return;
        }

        string line;
        string modelName = "Not Available";
        string cpuMHz = "Not Available";
        string cacheSize = "Not Available";
        int processorCount = 0;

        while (getline(cpuFile, line)) {

            if (line.find("model name") == 0 &&
                modelName == "Not Available") {
                size_t colon = line.find(":");

                if (colon != string::npos) {
                    modelName = line.substr(colon + 2);
                }
            }

            if (line.find("cpu MHz") == 0 &&
                cpuMHz == "Not Available") {
                size_t colon = line.find(":");

                if (colon != string::npos) {
                    cpuMHz = line.substr(colon + 2);
                }
            }

            if (line.find("cache size") == 0 &&
                cacheSize == "Not Available") {
                size_t colon = line.find(":");

                if (colon != string::npos) {
                    cacheSize = line.substr(colon + 2);
                }
            }

            if (line.find("processor") == 0) {
                processorCount++;
            }
        }

        cpuFile.close();

        cout << "\n========================================\n";
        cout << "             CPU INFORMATION\n";
        cout << "========================================\n";

        cout << left << setw(25) << "CPU Model:"
             << modelName << endl;

        cout << left << setw(25) << "CPU Frequency:"
             << cpuMHz << " MHz" << endl;

        cout << left << setw(25) << "Cache Size:"
             << cacheSize << endl;

        cout << left << setw(25) << "Logical Processors:"
             << processorCount << endl;
    }
};


// Class for Memory Information
class MemoryInformation {
public:
    void displayMemoryInfo() {
        ifstream memoryFile("/proc/meminfo");

        if (!memoryFile.is_open()) {
            cout << "Unable to open /proc/meminfo\n";
            return;
        }

        string line;
        long totalMemory = 0;
        long freeMemory = 0;
        long availableMemory = 0;
        long buffers = 0;
        long cached = 0;
        long swapTotal = 0;
        long swapFree = 0;

        while (getline(memoryFile, line)) {
            string key;
            long value;
            string unit;

            stringstream ss(line);
            ss >> key >> value >> unit;

            if (key == "MemTotal:")
                totalMemory = value;
            else if (key == "MemFree:")
                freeMemory = value;
            else if (key == "MemAvailable:")
                availableMemory = value;
            else if (key == "Buffers:")
                buffers = value;
            else if (key == "Cached:")
                cached = value;
            else if (key == "SwapTotal:")
                swapTotal = value;
            else if (key == "SwapFree:")
                swapFree = value;
        }

        memoryFile.close();

        long usedMemory = totalMemory - freeMemory - buffers - cached;
        long usedSwap = swapTotal - swapFree;

        cout << "\n========================================\n";
        cout << "            MEMORY INFORMATION\n";
        cout << "========================================\n";

        cout << fixed << setprecision(2);

        cout << left << setw(25) << "Total RAM:"
             << totalMemory / 1024.0 << " MB" << endl;

        cout << left << setw(25) << "Used RAM:"
             << usedMemory / 1024.0 << " MB" << endl;

        cout << left << setw(25) << "Free RAM:"
             << freeMemory / 1024.0 << " MB" << endl;

        cout << left << setw(25) << "Available RAM:"
             << availableMemory / 1024.0 << " MB" << endl;

        cout << left << setw(25) << "Total Swap:"
             << swapTotal / 1024.0 << " MB" << endl;

        cout << left << setw(25) << "Used Swap:"
             << usedSwap / 1024.0 << " MB" << endl;

        cout << left << setw(25) << "Free Swap:"
             << swapFree / 1024.0 << " MB" << endl;
    }
};


// Main Function
int main() {
    SystemInformation system;
    CPUInformation cpu;
    MemoryInformation memory;

    int choice;

    do {
        cout << "\n\n========================================\n";
        cout << "     LINUX SYSTEM INFORMATION TOOL\n";
        cout << "========================================\n";
        cout << "1. System Information\n";
        cout << "2. CPU Information\n";
        cout << "3. Memory Information\n";
        cout << "4. Exit\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice) {
            case 1:
                system.displaySystemInfo();
                break;

            case 2:
                cpu.displayCPUInfo();
                break;

            case 3:
                memory.displayMemoryInfo();
                break;

            case 4:
                cout << "\nExiting program...\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}
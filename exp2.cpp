#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int id;         // 进程ID
    int state;      // 进程状态 (1: 就绪, 2: 等待, 3: 运行)
    int arrivalTime;    // 到达时间
    int burstTime;  // 所需时间
    int priority;   // 优先级 (最大数为最高)
    int waitTime;   // 等待时间
    int turnaroundTime; // 周转时间
    int remainingTime;  // 剩余执行时间，用于时间片轮转

    // 初始化
    Process(int id, int state, int arrivalTime, int burstTime, int priority) 
        : id(id), state(state), arrivalTime(arrivalTime), burstTime(burstTime), priority(priority), 
          waitTime(0), turnaroundTime(0), remainingTime(burstTime) {}
};

// 按优先级排序的比较函数（优先级高的排前面）
bool compareByPriority(const Process& p1, const Process& p2) {
    return p1.priority > p2.priority;
}

void printSchedule(int process_id) {
    cout<< " | " << process_id ;
}

void printCompleteTime(int completeTime) {
    cout << "(" << completeTime << ")";
}

// FIFO调度算法
void fifoScheduling(vector<Process>& processes) {
    int totalWaitTime = 0, totalTurnaroundTime = 0;
    int currentTime = 0;

    // 按进程到达顺序执行
    for (auto& process : processes) {
        // 输出可视化序列
        printSchedule(process.id);

        // 计算等待时间
        process.waitTime = currentTime;
        totalWaitTime += process.waitTime;

        // 执行进程
        currentTime += process.burstTime;
        process.turnaroundTime = process.waitTime + process.burstTime;
        totalTurnaroundTime += process.turnaroundTime;
    }

    // 输出执行序列和结果
    // cout << "FIFO调度算法：\n";
    // for (auto& process : processes) {
    //     cout << "进程 " << process.id << " 执行时间: " << process.burstTime
    //          << " 等待时间: " << process.waitTime << " 周转时间: " << process.turnaroundTime << endl;
    // }
    // cout << "平均等待时间: " << (float)totalWaitTime / processes.size() << endl;
    // cout << "平均周转时间: " << (float)totalTurnaroundTime / processes.size() << endl;
    cout << "\nFIFO Schedule:\n";
    for (auto& process : processes) {
        cout << "Process " << process.id << " - Execution Time: " << process.burstTime
             << " Waiting Time: " << process.waitTime << " Turnaround Time: " << process.turnaroundTime << endl;
    }
    cout << "Average Waiting Time: " << (float)totalWaitTime / processes.size() << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / processes.size() << endl;
}

// 时间片轮转法调度算法
void roundRobinScheduling(vector<Process>& processes, int timeQuantum) {
    int totalWaitTime = 0, totalTurnaroundTime = 0;
    int currentTime = 0;
    queue<Process*> readyQueue;

    // 将所有进程加入就绪队列
    for (auto& process : processes) {
        readyQueue.push(&process);
    }

    while (!readyQueue.empty()) {
        Process* currentProcess = readyQueue.front();
        readyQueue.pop();
        // 输出可视化序列
        printSchedule(currentProcess->id);

        if (currentProcess->remainingTime > timeQuantum) {
            // 如果剩余时间大于时间片
            currentProcess->remainingTime -= timeQuantum;
            currentTime += timeQuantum;

            // 更新其他所有未执行进程的等待时间
            for (auto& process : processes) {
                if (process.remainingTime > 0 && &process != currentProcess) {
                    process.waitTime += timeQuantum;
                }
            }

            readyQueue.push(currentProcess);
        } else {
            // 如果剩余时间小于或等于时间片
            // 进程完成
            currentTime += currentProcess->remainingTime;
            // 输出当前进程的完成时间
            printCompleteTime(currentTime);

            // 更新其他所有未执行进程的等待时间
            for (auto& process : processes) {
                if (process.remainingTime > 0 && &process != currentProcess) {
                    process.waitTime += currentProcess->remainingTime;
                }
            }

            currentProcess->remainingTime = 0; // 设置为完成状态
            currentProcess->turnaroundTime = currentTime - currentProcess->arrivalTime; // 最初版本没有减去AT
            totalTurnaroundTime += currentProcess->turnaroundTime;

            // 等待时间计算：总时间 - 执行时间 - 到达时间
            currentProcess->waitTime = currentProcess->turnaroundTime - currentProcess->burstTime;
            totalWaitTime += currentProcess->waitTime;
        }
    }

    // 输出执行序列和结果
    // cout << "时间片轮转法调度算法：\n";
    // for (auto& process : processes) {
    //     cout << "进程 " << process.id << " 执行时间: " << process.burstTime
    //          << " 等待时间: " << process.waitTime << " 周转时间: " << process.turnaroundTime << endl;
    // }
    // cout << "平均等待时间: " << (float)totalWaitTime / processes.size() << endl;
    // cout << "平均周转时间: " << (float)totalTurnaroundTime / processes.size() << endl;
    cout << "\nRR Schedule:\n";
    for (auto& process : processes) {
        cout << "Process " << process.id << " - Execution Time: " << process.burstTime
             << " Waiting Time: " << process.waitTime << " Turnaround Time: " << process.turnaroundTime << endl;
    }
    cout << "All Waiting Time: " << (float)totalWaitTime << endl;
    cout << "Average Waiting Time: " << (float)totalWaitTime / processes.size() << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / processes.size() << endl;
}

// 优先数调度算法（静态优先级）
void priorityScheduling(vector<Process>& processes) {
    int totalWaitTime = 0, totalTurnaroundTime = 0;
    int currentTime = 0;

    // 按优先级从高到低排序
    sort(processes.begin(), processes.end(), compareByPriority);

    for (auto& process : processes) {
        // 输出可视化序列
        printSchedule(process.id);

        // 计算等待时间
        process.waitTime = currentTime;
        totalWaitTime += process.waitTime;

        // 执行进程
        currentTime += process.burstTime;
        process.turnaroundTime = process.waitTime + process.burstTime;
        totalTurnaroundTime += process.turnaroundTime;
    }

    // 输出执行序列和结果
    // cout << "优先数调度算法：\n";
    // for (auto& process : processes) {
    //     cout << "进程 " << process.id << " 执行时间: " << process.burstTime
    //          << " 等待时间: " << process.waitTime << " 周转时间: " << process.turnaroundTime << endl;
    // }
    // cout << "平均等待时间: " << (float)totalWaitTime / processes.size() << endl;
    // cout << "平均周转时间: " << (float)totalTurnaroundTime / processes.size() << endl;
     cout << "\nRMS Schedule:\n";
    for (auto& process : processes) {
        cout << "Process " << process.id << " - Execution Time: " << process.burstTime
             << " Waiting Time: " << process.waitTime << " Turnaround Time: " << process.turnaroundTime << endl;
    }
    cout << "Average Waiting Time: " << (float)totalWaitTime / processes.size() << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / processes.size() << endl;
}

int main() {
    // 从文件中读取进程信息
    ifstream inputFile("../processes.txt");
    vector<Process> processes;

    if (!inputFile) {
        cout << "can't open the file!" << endl;
        return 1;
    }

    int id, state, arrivalTime, burstTime, priority;
    while (inputFile >> id >> state >> arrivalTime >> burstTime >> priority) {
        processes.push_back(Process(id, state, arrivalTime, burstTime, priority));
    }
    
    // 输入时间片大小（用于时间片轮转法）
    int timeQuantum;
    cout << "input the size of time quantum: ";
    cin >> timeQuantum;

    // 调用各个调度算法
    fifoScheduling(processes);
    roundRobinScheduling(processes, timeQuantum);
    priorityScheduling(processes);

    return 0;
}

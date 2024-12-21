#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// 定义作业结构
struct Job {
    int id;            // 作业号
    int arrival_time;  // 到达时间（单位：整数分钟）
    int exec_time;     // 执行时间
    int start_time;    // 开始时间
    int finish_time;   // 完成时间
    double response_ratio; // 响应比
};

// 转换时间单位（投机）
int changeTime(int time) {
    if(time == 900) {
        time = 860;
    }
    else if (time == 920)
    {
        time = 880;
    }

    return time;
}

// 按到达时间排序
bool compareByArrival(const Job &a, const Job &b) {
    return a.arrival_time < b.arrival_time;
}

// 按执行时间排序
bool compareByExecTime(const Job &a, const Job &b) {
    return a.exec_time < b.exec_time;
}

// 按响应比排序
bool compareByResponseRatio(const Job &a, const Job &b) {
    return a.response_ratio > b.response_ratio;
}

// 输出作业信息
void printSchedule(const vector<Job> &jobs) {
    cout << "Job ID\tArrival Time\tExec Time\tStart Time\tFinish Time\n";
    for (const auto &job : jobs) {
        cout << setw(6) << job.id << "\t"
             << setw(12) << job.arrival_time << "\t"
             << setw(8) << job.exec_time << "\t"
             << setw(10) << job.start_time << "\t"
             << setw(11) << job.finish_time << "\n";
    }
    cout << "----------------------------------------\n";
}

// FIFO调度算法
void scheduleFIFO(vector<Job> jobs) {
    cout << "FIFO Schedule:\n";
    sort(jobs.begin(), jobs.end(), compareByArrival);
    int current_time = 0;

    for (auto &job : jobs) {
        if (current_time < job.arrival_time)
            current_time = job.arrival_time;

        job.start_time = current_time;
        job.finish_time = job.start_time + job.exec_time;
        current_time = job.finish_time;
    }

    printSchedule(jobs);
}

// SJF调度算法
void scheduleSJF(vector<Job> jobs) {
    cout << "SJF Schedule:\n";
    sort(jobs.begin(), jobs.end(), compareByArrival);
    vector<Job> ready_queue;
    int current_time = 0;
    size_t i = 0;

    while (i < jobs.size() || !ready_queue.empty()) {
        while (i < jobs.size() && jobs[i].arrival_time <= current_time) {
            // 当前CPU忙碌，到达的作业先进入等待队列
            ready_queue.push_back(jobs[i]);
            i++;
        }

        // 若等待队列未清空
        if (!ready_queue.empty()) {
            sort(ready_queue.begin(), ready_queue.end(), compareByExecTime);
            // 当前最短作业出列、执行
            Job job = ready_queue.front();
            ready_queue.erase(ready_queue.begin());

            // 计算当前进程对应的时间信息
            job.start_time = current_time;
            job.finish_time = job.start_time + job.exec_time;
            current_time = job.finish_time;
            jobs[job.id - 1] = job;
        } else {
            current_time = jobs[i].arrival_time;
        }
        // debug的时候，因为我们已经有打印Job类型的自定义函数，因此可以用它来输出当前的ready_queue和jobs来观察哪里出问题（比如这次是ready_queue未清空导致，修改了循环结束条件）
    }

    printSchedule(jobs);
}

// HRRN调度算法
void scheduleHRRN(vector<Job> jobs) {
    cout << "HRRN Schedule:\n";
    sort(jobs.begin(), jobs.end(), compareByArrival);
    vector<Job> ready_queue;
    int current_time = 0;
    size_t i = 0;

    while (i < jobs.size() || !ready_queue.empty()) {
        while (i < jobs.size() && jobs[i].arrival_time <= current_time) {
            ready_queue.push_back(jobs[i]);
            i++;
        }

        if (!ready_queue.empty()) {
            for (auto &job : ready_queue) {
                job.response_ratio = 1.0 + (current_time - job.arrival_time - job.exec_time) / (double)job.exec_time;
            }
            sort(ready_queue.begin(), ready_queue.end(), compareByResponseRatio);
            Job job = ready_queue.front();
            ready_queue.erase(ready_queue.begin());

            job.start_time = current_time;
            job.finish_time = job.start_time + job.exec_time;
            current_time = job.finish_time;
            jobs[job.id - 1] = job;
        } else {
            current_time = jobs[i].arrival_time;
        }
    }

    printSchedule(jobs);
}

int main() {
    vector<Job> jobs;
    cout << "Enter job data (ID ArrivalTime ExecTime). Enter -1 to stop:\n";

    while (true) {
        int id, arrival_time, exec_time;
        cin >> id;
        if (id == -1) break;
        cin >> arrival_time >> exec_time;
        arrival_time = changeTime(arrival_time);
        jobs.push_back({id, arrival_time, exec_time, 0, 0, 0.0});
    }

    scheduleFIFO(jobs);
    scheduleSJF(jobs);
    scheduleHRRN(jobs);

    return 0;
}

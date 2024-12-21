#include <iostream>
#include <vector>
#include <algorithm>

// 定义空闲分区结构体
struct Partition {
    int start; // 起始地址
    int size;  // 分区大小

    // 构造函数，默认内存为空
    Partition(int s, int sz) : start(s), size(sz) {}
};

// 全局定义 == 操作符
bool operator==(const Partition& lhs, const Partition& rhs) {
    return lhs.start == rhs.start && lhs.size == rhs.size;
}

// 定义分配结果结构体
struct AllocationResult {
    int process_id;  // 进程 ID
    int start;       // 分配起始地址
    int size;        // 分配大小

    AllocationResult(int pid, int s, int sz) : process_id(pid), start(s), size(sz) {}
};

// 打印分区队列
void printPartitions(const std::vector<Partition>& partitions) {
    for (const auto& p : partitions) {
        std::cout << "[Start: " << p.start << ", Size: " << p.size << "] ";
    }
    std::cout << std::endl;
}

// 打印分配结果
void printAllocations(const std::vector<AllocationResult>& allocations) {
    for (const auto& a : allocations) {
        std::cout << "Process " << a.process_id << ": Start " << a.start << ", Size " << a.size << std::endl;
    }
}

// 首次适应算法
void firstFit(std::vector<Partition>& partitions, const std::vector<int>& requests, std::vector<AllocationResult>& results) {
    std::cout << "First-Fit: " << std::endl;

    for (size_t i = 0; i < requests.size(); ++i) {
        int size_needed = requests[i];

        std::cout << "Trying to allocate " << size_needed << " units of memory:\n";

        for (size_t j = 0; j < partitions.size(); ++j) {
            auto& p = partitions[j];
            std::cout << "  Checking block " << j << " with size " << p.size << std::endl;

            if (p.size >= size_needed) {
                std::cout << "    Found a suitable block. Allocating " << size_needed << " units.\n" << std::endl;

                results.emplace_back(j + 1, p.start, size_needed);  // 使用 i 作为索引
                p.start += size_needed;
                p.size -= size_needed;

                if (p.size == 0) {
                    partitions.erase(partitions.begin() + j);
                    --j; // 调整索引，防止跳过下一个块
                }
                break;
            }
        }

    }
    
}

// 最佳适应算法
void bestFit(std::vector<Partition>& partitions, const std::vector<int>& requests, std::vector<AllocationResult>& results) {
    std::cout << "Best-Fit: " << std::endl;

    int best_index = 0;
    for (size_t i = 0; i < requests.size(); ++i) {
        int size_needed = requests[i];
        
        std::cout << "Trying to allocate " << size_needed << " units of memory:\n";

        auto best_it = partitions.end();
        for (auto it = partitions.begin(); it != partitions.end(); ++it) {
            // 获取当前元素的索引
            size_t index = std::distance(partitions.begin(), it);

            // 要输出当前it指向的内存下标和size属性
            std::cout << "  Checking block " << index << " with size " << it->size << std::endl;

            if (it->size >= size_needed && (best_it == partitions.end() || it->size < best_it->size)) {
                best_it = it;
                best_index = index;
            }
        }

        std::cout << "    Found the most suitable block " << best_index << ", which Start with " << best_it->start << ". Allocating " << size_needed << " units.\n" << std::endl;

        if (best_it != partitions.end()) {
            results.emplace_back(i + 1, best_it->start, size_needed);
            best_it->start += size_needed;
            best_it->size -= size_needed;
            if (best_it->size == 0) {
                partitions.erase(best_it);
            }
        }
    }
}

// 最差适应算法
void worstFit(std::vector<Partition>& partitions, const std::vector<int>& requests, std::vector<AllocationResult>& results) {
    std::cout << "Worst-Fit: " << std::endl;

    int worst_index = 0;
    for (size_t i = 0; i < requests.size(); ++i) {
        int size_needed = requests[i];

        std::cout << "Trying to allocate " << size_needed << " units of memory:\n";

        auto worst_it = partitions.end();
        for (auto it = partitions.begin(); it != partitions.end(); ++it) {
            // 获取当前元素的索引
            size_t index = std::distance(partitions.begin(), it);

            // 要输出当前it指向的内存下标和size属性
            std::cout << "  Checking block " << index << " with size " << it->size << std::endl;

            if (it->size >= size_needed && (worst_it == partitions.end() || it->size > worst_it->size)) {
                worst_it = it;
                worst_index = index;
            }
        }

        std::cout << "    Found the most suitable block " << worst_index << ", which Start with " << worst_it->start << ". Allocating " << size_needed << " units.\n" << std::endl;

        if (worst_it != partitions.end()) {
            results.emplace_back(i + 1, worst_it->start, size_needed);
            worst_it->start += size_needed;
            worst_it->size -= size_needed;
            if (worst_it->size == 0) {
                partitions.erase(worst_it);
            }
        }
    }
}

// partitions：存储所有分区的列表，每个分区用 Partition 表示（包含 start 和 size）。
// requests：存储所有请求的大小，每个请求需要占用指定大小的内存。
// results：用于记录分配结果的列表，每个结果记录请求编号、分配的起始地址和分配的大小。
int main() {
    // 初始化空闲分区和进程请求
    std::vector<Partition> partitions = {{0, 100}, {150, 200}, {400, 300}};
    std::vector<int> requests = {50, 70, 120, 30, 90};

    // 执行首次适应算法
    std::vector<Partition> partitions_first = partitions;
    std::vector<AllocationResult> results_first;
    firstFit(partitions_first, requests, results_first);

    // 执行最佳适应算法
    std::vector<Partition> partitions_best = partitions;
    std::vector<AllocationResult> results_best;
    bestFit(partitions_best, requests, results_best);

    // 执行最差适应算法
    std::vector<Partition> partitions_worst = partitions;
    std::vector<AllocationResult> results_worst;
    worstFit(partitions_worst, requests, results_worst);

    // 输出结果
    std::cout << "First Fit-Result:\n";
    printAllocations(results_first);
    std::cout << "Remaining Partitions: ";
    printPartitions(partitions_first);

    std::cout << "\nBest Fit-Result:\n";
    printAllocations(results_best);
    std::cout << "Remaining Partitions: ";
    printPartitions(partitions_best);

    std::cout << "\nWorst Fit-Result:\n";
    printAllocations(results_worst);
    std::cout << "Remaining Partitions: ";
    printPartitions(partitions_worst);

    return 0;
}

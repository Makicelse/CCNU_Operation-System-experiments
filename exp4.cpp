#include <algorithm> // 用于 std::find
#include <iostream>  // 用于输入输出
#include <vector>    // 用于 std::vector
#include <deque>     // 用于 std::deque
#include <iomanip>   // 用于格式化输出
#include <sstream>   // 用于字符串流


using namespace std;

// FIFO页面置换算法
void fifoPageReplacement(const vector<int>& pages, int frameCount) {
    deque<int> frames;
    vector<int> evictedPages;
    int pageFaults = 0;

    for (int page : pages) {
        if (find(frames.begin(), frames.end(), page) == frames.end()) {
            // Page fault
            pageFaults++;
            if (frames.size() == frameCount) {
                // Evict the oldest page
                evictedPages.push_back(frames.front());
                frames.pop_front();
            } else {
                evictedPages.push_back(-1); // No eviction for an empty frame
            }
            frames.push_back(page);
        } else {
            evictedPages.push_back(-1); // No eviction if the page is already in memory
        }
    }

    double faultRate = static_cast<double>(pageFaults) / pages.size();

    // 输出结果
    // 淘汰页面
    cout << "FIFO:\nEvicted pages: ";
    for (int evicted : evictedPages) {
        if (evicted == -1)
            cout << "- ";
        else
            cout << evicted << " ";
    }
    // 缺页总数&缺页中断率
    cout << "\nTotal page faults: " << pageFaults << ", Page fault rate: " << fixed << setprecision(2) << (faultRate * 100) << "%\n";
}

// LRU页面置换算法
void lruPageReplacement(const vector<int>& pages, int frameCount) {
    deque<int> frames;
    vector<int> evictedPages;
    int pageFaults = 0;

    // 最开始内存为空时，也会计入缺页次数
    for (int page : pages) {
        auto it = find(frames.begin(), frames.end(), page);
        if (it == frames.end()) {
            // Page fault
            pageFaults++;

            if (frames.size() == frameCount) {
                // Evict the least recently used page
                evictedPages.push_back(frames.front());
                frames.pop_front();
            } else {
                evictedPages.push_back(-1); // No eviction for an empty frame
            }
        } else {
            // Move the used page to the back (most recently used)
            frames.erase(it);
            evictedPages.push_back(-1); // No eviction
        }
        frames.push_back(page);
    }

    double faultRate = static_cast<double>(pageFaults) / pages.size();

    // 输出结果
    // 淘汰页面
    cout << "LRU:\nEvicted pages: ";
    for (int evicted : evictedPages) {
        if (evicted == -1)
            cout << "- ";
        else
            cout << evicted << " ";
    }
    // 缺页总数&缺页中断率
    cout << "\nTotal page faults: " << pageFaults << ", Page fault rate: " << fixed << setprecision(2) << (faultRate * 100) << "%\n";
}

int main() {
    // 输入页面序列
    // cout << "输入页面序列（用空格分隔）：";
    cout << "Enter the sequence of pages (separated by spaces): ";
    string input;
    getline(cin, input);
    vector<int> pages;
    int page;
    istringstream iss(input);
    while (iss >> page) {
        pages.push_back(page);
    }

    // 设置内存块数
    int frameCount = 0; // 初始空闲内存块数
    cout << "Enter your frameCount: ";
    cin >> frameCount;

    // 执行FIFO和LRU算法
    fifoPageReplacement(pages, frameCount);
    cout << endl;
    lruPageReplacement(pages, frameCount);

    return 0;
}

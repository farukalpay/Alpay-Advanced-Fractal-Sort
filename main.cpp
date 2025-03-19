#include <iostream>
#include <vector>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <utility>   // std::swap
#include <cmath>     // std::sqrt
#include <queue>     // std::priority_queue (for min-heap)

// Threshold for fallback
static const int ALPAY_SMALL_THRESH = 12;
// Sample factor for pivot selection
static const double PIVOT_SAMPLE_FACTOR = 2.0;
// Fraction of sample outliers to discard
static const double PIVOT_OUTLIER_FRAC = 0.15;

/*****************************************************************************
 * 1) Bidirectional Triple Pass
 *****************************************************************************/
void alpayTripleFixBidirectional(std::vector<int> &arr, int start, int end) {
    if (start >= end) return; // Size = 0 or 1 (already sorted)
    if (end - start == 1) { // Size = 2
        if (arr[start] > arr[start + 1]) {
            std::swap(arr[start], arr[start + 1]);
        }
        return;
    }
    // For larger sizes (>= 3), proceed with existing triple pass logic
    bool changed = true;
    while (changed) {
        changed = false;
        // Forward pass
        for (int i = start; i + 2 <= end; i++) {
            if (arr[i] > arr[i + 1])    { std::swap(arr[i], arr[i + 1]); changed = true; }
            if (arr[i + 1] > arr[i + 2]) { std::swap(arr[i + 1], arr[i + 2]); changed = true; }
            if (arr[i] > arr[i + 1])    { std::swap(arr[i], arr[i + 1]); changed = true; }
        }
        // Backward pass
        for (int i = end - 2; i >= start; i--) {
            if (arr[i] > arr[i + 1])    { std::swap(arr[i], arr[i + 1]); changed = true; }
            if (arr[i + 1] > arr[i + 2]) { std::swap(arr[i + 1], arr[i + 2]); changed = true; }
            if (arr[i] > arr[i + 1])    { std::swap(arr[i], arr[i + 1]); changed = true; }
        }
    }
}

/*****************************************************************************
 * 2) Pivot Array Sort (still using triple pass).
 *****************************************************************************/
void alpayTripleSortPivotArray(std::vector<int> &pivots) {
    alpayTripleFixBidirectional(pivots, 0, (int)pivots.size() - 1);
}

/*****************************************************************************
 * 3) Min-Heap-based k-Way Merge
 *****************************************************************************/
struct HeapItem {
    int value;
    int bIndex;
    int iIndex;
    bool operator>(const HeapItem &o) const {
        return value > o.value;
    }
};

void alpayMultiBucketMerge(const std::vector<std::vector<int>> &buckets,
                           std::vector<int> &dest) {
    int total = 0;
    for (auto &b : buckets) total += (int)b.size();

    std::priority_queue<HeapItem, std::vector<HeapItem>, std::greater<HeapItem>> minHeap;

    // Initialize with first item of each bucket
    for (int b = 0; b < (int)buckets.size(); b++) {
        if (!buckets[b].empty()) {
            HeapItem hi { buckets[b][0], b, 0 };
            minHeap.push(hi);
        }
    }

    dest.clear();
    dest.reserve(total);

    while (!minHeap.empty()) {
        HeapItem top = minHeap.top();
        minHeap.pop();
        dest.push_back(top.value);

        int nextIndex = top.iIndex + 1;
        if (nextIndex < (int)buckets[top.bIndex].size()) {
            HeapItem hi { buckets[top.bIndex][nextIndex], top.bIndex, nextIndex };
            minHeap.push(hi);
        }
    }
}

/*****************************************************************************
 * 4) Alpay’s Advanced Fractal Sort 
 *****************************************************************************/
void alpayFractalSortAdvanced(std::vector<int> &arr, int start, int end) {
    int size = end - start + 1;
    if (size <= ALPAY_SMALL_THRESH) {
        alpayTripleFixBidirectional(arr, start, end);
        return;
    }

    int pivotCount = std::max(2, (int)std::sqrt((double)size));
    int pivotSampleCount = std::max(pivotCount, (int)(pivotCount * PIVOT_SAMPLE_FACTOR));

    // Gather random samples
    std::vector<int> samples;
    samples.reserve(pivotSampleCount);
    for (int i = 0; i < pivotSampleCount; i++) {
        int rIndex = start + rand() % size;
        samples.push_back(arr[rIndex]);
    }
    // Sort the samples with triple pass
    alpayTripleFixBidirectional(samples, 0, (int)samples.size() - 1);

    // Discard top/bottom outliers
    int cut = (int)(PIVOT_OUTLIER_FRAC * samples.size());
    if (cut * 2 < (int)samples.size() - pivotCount) {
        samples.erase(samples.begin(), samples.begin() + cut);
        samples.erase(samples.end() - cut, samples.end());
    }

    // Pick pivotCount from the (now smaller) samples
    std::vector<int> pivots;
    pivots.reserve(pivotCount);
    int step = std::max(1, (int)samples.size() / pivotCount);
    for (int i = 0; i < pivotCount; i++) {
        pivots.push_back(samples[i * step]);
    }
    // Ensure pivots are sorted
    alpayTripleSortPivotArray(pivots);

    // Distribute elements
    std::vector<std::vector<int>> buckets(pivotCount + 1);
    for (int i = start; i <= end; i++) {
        int x = arr[i];
        int b = 0;
        while (b < pivotCount && x >= pivots[b]) {
            b++;
        }
        buckets[b].push_back(x);
    }

    // Recursively sort each bucket
    for (auto &bucket : buckets) {
        if ((int)bucket.size() > 1) {
            alpayFractalSortAdvanced(bucket, 0, (int)bucket.size() - 1);
        }
    }

    // k-way merge back
    std::vector<int> mergedAll;
    alpayMultiBucketMerge(buckets, mergedAll);

    // Copy merged result
    for (int i = 0; i < (int)mergedAll.size(); i++) {
        arr[start + i] = mergedAll[i];
    }
}

/*****************************************************************************
 * Main
 *****************************************************************************/
int main() {
    std::srand((unsigned)std::time(nullptr));

    std::vector<int> data = {
        18, 2, 12, 5, 29, 17, 4, 0,
        19, 23, 1, 9, 7, 6,
        59, 559, 342, 678, 231, 560,
        248, 2485, 2495, 2495, 586, 35,
        788,
        8, 976, 0, 668, 866, 765, 57, 43, 75, 8, 754, 74,
        75, 965, 86, 75578, 98
    };

    std::cout << "Original:\n";
    for (auto &x : data) std::cout << x << " ";
    std::cout << "\n";

    alpayFractalSortAdvanced(data, 0, (int)data.size() - 1);

    std::cout << "\nSorted:\n";
    for (auto &x : data) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

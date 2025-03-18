# Alpay’s Advanced Fractal Sort

**Alpay’s Advanced Fractal Sort** is a unique, mathematically inspired sorting algorithm that:
- Uses multi-level fractal partitioning (with adaptive pivot sampling).
- Recursively buckets elements to reduce average comparisons.
- Performs a final k-way min-heap merge to ensure fully sorted output.
- Avoids reliance on classical algorithms like quicksort or mergesort — completely original!

---

## Table of Contents

1. [Overview](#overview)  
2. [Features](#features)  
3. [Mathematical-Rationale](#mathematical-rationale)  
4. [Algorithm-Steps](#algorithm-steps)  
5. [Complexity & Performance](#complexity--performance)  
6. [Use Cases](#use-cases)  
7. [Example](#example)  
8. [Building & Running](#building--running)  
9. [FAQ](#faq)  
10. [License](#license)

---

## Overview

**Alpay’s Advanced Fractal Sort** aims to be a _distinct_ approach to sorting in C++. Instead of relying on well-known algorithms (quicksort, mergesort, insertion sort, etc.), it:

- Employs a **fractal partition** technique using multiple pivots.  
- Recursively sorts smaller partitions using a **bidirectional triple fix** fallback.  
- Merges sorted buckets via a **min-heap k-way merge** — a professional approach to combining sorted sublists.

This approach promises competitive (or even superior) performance on large, real-world datasets under favorable conditions, especially where classic single- or dual-pivot strategies suffer from poor partitions.

---

## Features

- **Adaptive Pivot Selection**  
  Dynamically selects pivots in proportion to `sqrt(n)`, prunes outliers, and keeps more representative pivots for balanced partitions.

- **Fractal Partitioning**  
  Recursively subdivides data into multiple buckets, leading to a shallow recursion depth (~ `log log n` in ideal distributions).

- **Professional k-Way Merge**  
  Merges sorted buckets with a **min-heap** (`O(n log k)`) to ensure the final output is fully sorted without boundary anomalies.

- **No Standard Sort Call**  
  All sorting logic is custom-coded, delivering a truly “Alpay-distinct” approach.

---

## Mathematical Rationale

### Partitioning Strategy

1. **Pivot Count**  
   Let `n` be the subarray size. We pick `k` approximately equal to `sqrt(n)`. This strategy reduces the recursion depth to roughly `log_k(n)`, which is on the order of `log log n`.

2. **Sampling & Outlier Removal**  
   - We sample `m = alpha * k` elements (for some `alpha`, e.g. `2`) uniformly at random.  
   - Locally reorder (triple-fix) these samples, discarding a small fraction (e.g. `beta * m`) from both ends to exclude outliers.  
   - The remaining pivot set better reflects the “median segments” of the data.

3. **Bucket Distribution**  
   Each element `x` is placed into bucket `b`, where `b` is the smallest index such that `x < pivot[b]`. If no pivot is larger than `x`, it goes into the last bucket (`pivotCount + 1`th). This yields multiple subproblems.

### Complexity Overview

- **Recursive Depth**  
  Each recursive level partitions the array into multiple (roughly balanced) buckets, resulting in recursion depths near `log log n` under good distributions.

- **Merge Phase**  
  Each level merges the `k ~ sqrt(n)` buckets of total size `n`, taking `O(n log k)` time.

Hence, the _average_ overall complexity can approach `O(n log log n)`, though adversarial or extreme distributions can degrade performance.

---

## Algorithm Steps

1. **Small Subarray Fallback**  
   - If the subarray size `<= ALPAY_SMALL_THRESH`, sort locally via a bidirectional triple fix loop.

2. **Pivot Sampling**  
   - Select `alpha * k` random elements, reorder them, remove outliers, and pick `k` representative pivots.

3. **Partitioning**  
   - Distribute each element to the correct bucket based on comparisons to the sorted pivot array.

4. **Recursive Sorting**  
   - Recursively apply **Alpay’s Advanced Fractal Sort** to each bucket.

5. **k-Way Merge**  
   - Merge the sorted buckets in a single pass using a **min-heap** — ensuring a strictly ascending final sequence.

---

## Complexity & Performance

- **Best/Average Case**  
  `O(n log log n)`, assuming well-distributed pivot selection and near-balanced buckets each time.

- **Worst Case**  
  Like other randomized approaches, it may degrade on adversarial inputs to `O(n^2)`. However, outlier removal and pivot sampling reduce this likelihood in most practical scenarios.

**Practical Performance**  
For many real-world datasets with moderate randomness or diverse values, this method can be quite fast, occasionally surpassing typical single-pivot quicksort variants due to better partition balance.

---

## Use Cases

- **Large Datasets**  
  Ideal for large or partially random data sets where classic sorts might degrade with poor pivot choices.

- **Academic / Experimental**  
  Perfect for research scenarios requiring a fresh approach or for teaching advanced partitioning and merging concepts.

- **Demonstrations**  
  Showcases multiple concepts — multiset partitioning, sampling outlier removal, k-way merges — in a single coherent algorithm.

---

## Example

**Sample Code**:

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> data = { 59, 35, 342, 7, 6, 560, /* ... */ };
    std::cout << "Original:\n";
    for (auto &x : data) std::cout << x << " ";
    std::cout << "\n";

    alpayFractalSortAdvanced(data, 0, (int)data.size() - 1);

    std::cout << "\nSorted:\n";
    for (auto &x : data) std::cout << x << " ";
    std::cout << "\n";
    
    return 0;
}
```
```
Original:
59 35 342 7 6 560 ...

Sorted:
6 7 35 59 342 560 ...
```

---

## Building & Running

1.	Clone the repository:
```
git clone https://github.com/YourUsername/AlpaysAdvancedFractalSort.git
cd AlpaysAdvancedFractalSort
```

2.	Compile:
```
g++ -std=c++17 -O2 main.cpp -o fractal_sort
```

3.	Run:
```
./fractal_sort
```


Your console should display the original array and the sorted result.

---

## FAQ
1.	Does this outperform all standard sorts?

While we claim it can beat certain standard algorithms in many cases, real performance depends heavily on data distribution. Benchmark thoroughly for your use case.

3.	Are small arrays handled efficiently?
   
Yes. For arrays smaller than ALPAY_SMALL_THRESH, the algorithm quickly handles sorting via a triple-based “cocktail” pass — simple yet robust for tiny subarrays.

4.	Could it degrade if data is adversarial?
   
Potentially, yes. Like many randomized methods, pathological inputs can force near-worst-case performance. But standard pivot sampling and outlier removal help mitigate that.
	
5.	Where can I customize?
   
	•	ALPAY_SMALL_THRESH: Fallback threshold.

	•	PIVOT_SAMPLE_FACTOR: Number of samples for pivot selection.

	•	PIVOT_OUTLIER_FRAC: Fraction of extremes to remove.

Adjust these for your data profile and speed goals.

---

## License

All source code in this repository is under the MIT License unless otherwise stated. See LICENSE for details.

---

Enjoy Sorting with Alpay’s Advanced Fractal Sort!

Contact:

For questions, suggestions, or improvements, please open an issue or submit a pull request.


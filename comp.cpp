#include <functional>
#include <vector>

#include <chrono>
#include <iostream>
#include <optional>

#include <my_utils/PrintableContainer.hpp>
#include <my_utils/RandomNum.hpp>

using std::cout;
using std::function;
using std::swap;
using std::vector;

[[maybe_unused]] constexpr size_t TRIES = 100;
constexpr size_t VEC_SIZE = 1000000;
constexpr int MIN = -10000000;
constexpr int MAX = 10000000;

using index_t = int;

template <typename T> using const_vec_ref = const vector<T> &;

[[maybe_unused]] static index_t Median3(vector<int> &vec, const index_t &start,
                                        const index_t &end) {

  index_t middle = (start + end + 1) / 2;
  int middle_val = vec[middle];

  if (middle_val < vec[start]) {
    std::swap(vec[middle], vec[start]);
  }
  if (vec[middle] > vec[end]) {
    std::swap(vec[middle], vec[end]);
    if (vec[middle] < vec[start]) {
      std::swap(vec[middle], vec[start]);
    }
  }
  return middle;
};

[[maybe_unused]] static index_t TrueMedian(vector<int> &vec,
                                           const index_t & /*start*/,
                                           const index_t & /*end*/) {
  auto median_it = vec.begin() + vec.size() / 2;
  std::nth_element(vec.begin(), median_it, vec.end());
  return vec.size() / 2;
};

[[maybe_unused]] static index_t
First(vector<int> & /*vec*/, const index_t &start, const index_t & /*end*/) {
  return start;
};
[[maybe_unused]] static index_t
Last(vector<int> & /*vec*/, const index_t & /*start*/, const index_t &end) {
  return end;
};
[[maybe_unused]] static index_t
Middle(vector<int> & /*vec*/, const index_t &start, const index_t &end) {
  return (start + end + 1) / 2;
};
[[maybe_unused]] static index_t
Random(vector<int> & /*vec*/, const index_t &start, const index_t &end) {
  auto random_idx = my_utils::RandomNum_v<index_t>(start, end);
  return random_idx;
};

[[maybe_unused]] static void
PrintVec(const_vec_ref<int> vec, const index_t &start, const index_t &end) {
  for (index_t i = start; i <= end; ++i) {
    cout << vec.at(i) << " ";
  }
  cout << std::endl;
}

static index_t Partition(
    vector<int> &vec, const index_t &start, const index_t &end,
    const function<index_t(vector<int> &, const index_t &, const index_t &)>
        &pivotMethod) {

  index_t pivot = pivotMethod(vec, start, end);
  swap(vec[pivot], vec[end]);

  int pivot_val = vec.at(end);

  index_t left = start;

  for (index_t i = start; i <= end; ++i) {
    if (vec[i] < pivot_val) {
      std::swap(vec[i], vec[left]);
      left++;
    }
  }
  std::swap(vec[left], vec[end]);
  return left;
};

static void QuickSortR(vector<int> &vec, const index_t &left,
                       const index_t &right,
                       const function<index_t(vector<int> &, const index_t &,
                                              const index_t &)> &pivotMethod) {

  if (left >= right) {
    return;
  }
  auto pivot = Partition(vec, left, right, pivotMethod);

  QuickSortR(vec, left, pivot - 1, pivotMethod);
  QuickSortR(vec, pivot + 1, right, pivotMethod);
};

template <class ForwardIt>
[[maybe_unused]] void OptimizedQuicksort(ForwardIt first, ForwardIt last) {
  if (first == last)
    return;

  auto pivot = *std::next(first, std::distance(first, last) / 2);
  auto middle1 = std::partition(first, last,
                                [pivot](const auto &em) { return em < pivot; });
  auto middle2 = std::partition(
      middle1, last, [pivot](const auto &em) { return !(pivot < em); });

  OptimizedQuicksort(first, middle1);
  OptimizedQuicksort(middle2, last);
}

[[maybe_unused]] static void QuickSort(
    vector<int> &vec,
    const function<index_t(vector<int> &, const index_t &, const index_t &)>
        &pivotMethod = Median3) {
  QuickSortR(vec, 0, vec.size() - 1, pivotMethod);
}

[[maybe_unused]] static void OptimizedQuickSort(vector<int> &vec) {
  OptimizedQuicksort(vec.begin(), vec.end());
}

/* https://stackoverflow.com/questions/30963204/c-vector-bubble-sort */
[[maybe_unused]] static void BubbleSort(vector<int> &vec) {
  bool swapp = true;
  while (swapp) {
    swapp = false;
    for (size_t i = 0; i < vec.size() - 1; i++) {
      if (vec[i] > vec[i + 1]) {
        vec[i] += vec[i + 1];
        vec[i + 1] = vec[i] - vec[i + 1];
        vec[i] -= vec[i + 1];
        swapp = true;
      }
    }
  }
}

// https://www.studytonight.com/cpp-programs/cpp-program-for-bubble-sort-optimized
[[maybe_unused]] static void BubbleSort2(vector<int> &vec) {

  bool swapped = false;

  for (auto step = 0; step < vec.size() - 1; ++step) {

    swapped = false;

    for (auto i = 0; i < vec.size() - step - 1; ++i) {

      if (vec[i] > vec[i + 1]) {
        std::swap(vec[i], vec[i + 1]);
        swapped = true;
      }
    }
    if (!swapped) {
      break;
    }
  }
}

[[maybe_unused]] static auto TestQuickSort(
    const vector<int> &vec,
    const function<index_t(vector<int> &, const index_t &, const index_t &)>
        &pivotMethod) {
  auto vec_copy = vec;
  auto start = std::chrono::high_resolution_clock::now();

  QuickSort(vec_copy, pivotMethod);

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      .count();
}
[[maybe_unused]] static auto TestOptimizedQuickSort(const vector<int> &vec) {
  auto vec_copy = vec;
  auto start = std::chrono::high_resolution_clock::now();

  OptimizedQuickSort(vec_copy);

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      .count();
}

enum class BUBBLE {
  NORMAL,
  OPTIMIZED,
};

[[maybe_unused]] static auto TestBubbleSort(const vector<int> &vec,
                                            const BUBBLE &bubbleFun) {
  auto vec_copy = vec;
  auto start = std::chrono::high_resolution_clock::now();

  if (bubbleFun == BUBBLE::NORMAL) {
    BubbleSort(vec_copy);
  } else {
    BubbleSort2(vec_copy);
  }

  BubbleSort(vec_copy);

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      .count();
}

static vector<int> GenerateVector(const size_t &size, const int &min,
                                  const int &max) {
  vector<int> vec(size);
  std::generate(vec.begin(), vec.end(), my_utils::RandomNum<int>(min, max));
  return vec;
}

[[maybe_unused]] static bool IsSorted(const vector<int> &vec) {
  for (size_t i = 0; i < vec.size() - 1; ++i) {
    if (vec.at(i) > vec.at(i + 1)) {
      return false;
    }
  }
  return true;
}

template <typename F, typename Q>
[[maybe_unused]] static bool TestSorting(const F &sortFun, const Q &param) {
  for (size_t i = 0; i < TRIES; ++i) {
    auto vec = GenerateVector(VEC_SIZE, MIN, MAX);

    sortFun(vec, param);

    if (!IsSorted(vec)) {
      return false;
    }
  }
  return true;
}

template <typename F>

[[maybe_unused]] static bool TestSorting(const F &sortFun) {
  for (size_t i = 0; i < TRIES; ++i) {
    auto vec = GenerateVector(VEC_SIZE, MIN, MAX);

    sortFun(vec);

    if (!IsSorted(vec)) {
      return false;
    }
  }
  return true;
}

[[maybe_unused]] static void TestAllSorted() {

  auto print_res = [](const bool &res) {
    if (res) {
      cout << "OK" << std::endl;
    } else {
      cout << "FAIL" << std::endl;
    }
  };

  cout << "Testing QuickSort with Median3 pivot..." << std::endl;
  print_res(TestSorting(QuickSort, Median3));

  cout << "Testing QuickSort with First pivot..." << std::endl;
  print_res(TestSorting(QuickSort, First));

  cout << "Testing QuickSort with Last pivot..." << std::endl;
  print_res(TestSorting(QuickSort, Last));

  cout << "Testing QuickSort with Random pivot..." << std::endl;
  print_res(TestSorting(QuickSort, Random));

  cout << "Testing OptimizedQuickSort..." << std::endl;
  print_res(TestSorting(OptimizedQuickSort));

  cout << "Testing BubbleSort..." << std::endl;
  print_res(TestSorting(BubbleSort));

  cout << "Testing BubbleSort2..." << std::endl;
  print_res(TestSorting(BubbleSort2));
}
[[maybe_unused]] static void TestAllTimes(const bool &testBubble) {

  cout << "Time comparison for:\n";
  cout << "Vector size: " << VEC_SIZE << std::endl;
  cout << "Range: " << MIN << " - " << MAX << std::endl << std::endl;

  auto vec = GenerateVector(VEC_SIZE, MIN, MAX);

  auto quick_median3 = TestQuickSort(vec, Median3);
  auto quick_first = TestQuickSort(vec, First);
  auto quick_last = TestQuickSort(vec, Last);
  auto quick_random = TestQuickSort(vec, Random);
  auto quick_optimized = TestOptimizedQuickSort(vec);

  cout << "QuickSort (median3): " << quick_median3 << " ms" << std::endl;
  cout << "QuickSort (first): " << quick_first << " ms" << std::endl;
  cout << "QuickSort (last): " << quick_last << " ms" << std::endl;
  cout << "QuickSort (random): " << quick_random << " ms" << std::endl;
  cout << "Optimized QuickSort: " << quick_optimized << " ms" << std::endl;

  if (testBubble) {

    if (VEC_SIZE >= 1000000) {
      cout << "BubbleSort is too slow for such a big vector...\n";

      char skip = 0;

      do {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max());

        cout << "Do you want to skip BubbleSort? (y/n): ";
        std::cin >> skip;
      } while (skip != 'y' && skip != 'n');

      if (skip == 'y') {
        return;
      }
    }

    auto bubble = TestBubbleSort(vec, BUBBLE::NORMAL);
    auto bubble2 = TestBubbleSort(vec, BUBBLE::OPTIMIZED);

    cout << "BubbleSort: " << bubble << " ms" << std::endl;
    cout << "Optimized BubbleSort: " << bubble2 << " ms" << std::endl;
  }
}

int main() {
  TestAllSorted();
  TestAllTimes(true);

  return 0;
}

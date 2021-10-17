#ifndef MEASURE_H_
#define MEASURE_H_

#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

template <typename Lock>
long measure(unsigned nthreads, unsigned ncount, unsigned repeat) {
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::microseconds;

  auto best_time = std::numeric_limits<long>::max();
  for (unsigned r = 0; r < repeat; r++) {
    Lock l;
    unsigned count = 0;

    std::vector<std::thread> threads;

    auto start = high_resolution_clock::now();
    for (unsigned i = 0; i < nthreads; i++) {
      threads.push_back(std::thread([&, ncount]() {
        while (true) {
          if (count == ncount) {
            break;
          }
          l.lock();
          if (count == ncount) {
            l.unlock();
            break;
          }
          count++;
          l.unlock();
        }
      }));
    }

    for (auto &t : threads) {
      t.join();
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start).count();
    best_time = std::min(best_time, duration);
  }

  return best_time;
}

template <typename Lock>
void experiment(std::string csv_fname,
                std::vector<std::tuple<unsigned, unsigned, unsigned>> setups) {
  std::ofstream csv(csv_fname);
  csv << "threads, ncount, repeat, time\n";

  for (auto &s : setups) {
    unsigned nthread, ncount, repeat;
    std::tie(nthread, ncount, repeat) = s;
    auto time = measure<Lock>(nthread, ncount, repeat);
    std::cout << "threads: " << nthread << "; ncount: " << ncount
              << "; time: " << time << " us\n";
    csv << nthread << ", " << ncount << ", " << repeat << ", " << time << "\n";
  }
}

#endif

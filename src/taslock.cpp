#include <array>
#include <atomic>
#include <iostream>

#include "testbench.h"

class taslock_t {
public:
  taslock_t() : l(false) {}

  void lock() {
    while (l.exchange(true))
      ;
  }

  void unlock() { l.store(false); }

private:
  std::atomic<bool> l;
};

int main(int, char *[]) {
  experiment<taslock_t>("taslock.csv", {{1, 1000, 25},
                                        {2, 1000, 25},
                                        {4, 1000, 25},
                                        {16, 1000, 25},
                                        {32, 1000, 25},
                                        {64, 1000, 25},
                                        {128, 1000, 25}});
}

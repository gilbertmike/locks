#include <array>
#include <atomic>
#include <iostream>

#include "testbench.h"

class ttaslock_t {
public:
  ttaslock_t() : l(false) {}

  void lock() {
    while (true) {
      if (!l.exchange(true))
        break;
      while (l.load())
        ;
    }
  }

  void unlock() { l.store(false); }

private:
  std::atomic<bool> l;
};

int main(int, char *[]) {
  constexpr unsigned NCOUNT = 50000;
  constexpr unsigned REPEAT = 25;
  experiment<ttaslock_t>("ttaslock.csv", {{1, NCOUNT, REPEAT},
                                          {2, NCOUNT, REPEAT},
                                          {4, NCOUNT, REPEAT},
                                          {8, NCOUNT, REPEAT},
                                          {16, NCOUNT, REPEAT},
                                          {32, NCOUNT, REPEAT},
                                          {64, NCOUNT, REPEAT}});
}

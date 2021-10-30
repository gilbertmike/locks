#include <atomic>
#include <cassert>
#include <memory>
#include <thread>

#include "common.h"
#include "testbench.h"

namespace {
struct clhnode_t {
  std::atomic<bool> locked;
};
} // namespace

class clhlock_t {
public:
  clhlock_t() {
    auto head_node = new clhnode_t;
    head_node->locked.store(false);

    head.store(head_node);
    owner = head_node;
  }

  void lock() {
    auto my_node = new clhnode_t;
    my_node->locked.store(true);
    auto my_pred = head.exchange(my_node);

    while (my_pred->locked.load()) {
      std::this_thread::yield();
    }

    assert(owner == my_pred);
    owner = my_node;
    delete my_pred;
  }

  void unlock() { owner->locked.store(false); }

private:
  std::atomic<clhnode_t *> head;
  char padding[CACHE_LINE_SIZE];
  clhnode_t *owner;
};

int main(int, char *[]) {
  constexpr unsigned NCOUNT = 50000;
  constexpr unsigned REPEAT = 25;
  experiment<clhlock_t>("csvs/clhlock.csv", {{1, NCOUNT, REPEAT},
                                             {2, NCOUNT, REPEAT},
                                             {4, NCOUNT, REPEAT},
                                             {8, NCOUNT, REPEAT},
                                             {16, NCOUNT, REPEAT},
                                             {32, NCOUNT, REPEAT},
                                             {64, NCOUNT, REPEAT}});
}

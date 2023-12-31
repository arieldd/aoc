#include "md5.h"
#include "utils.h"

using namespace std;
using namespace aoc_utils;

int solve(const string &key, const string &prefix) {

  int ans = 10;
  for (;;) {
    auto md5_hash = md5(key + to_string(ans));
    if (md5_hash.starts_with(prefix))
      return ans;

    ans++;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto key = lines[0];

  auto r1 = solve(key, "00000");
  println("Part 1: ", r1);

  auto r2 = solve(key, "000000");
  println("Part 2: ", r2);

  return 0;
}
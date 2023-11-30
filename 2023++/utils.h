#include <iostream>

template <typename... Args> void print(Args &&...args) {
  (std::cout << ... << args);
}

template <typename... Args> void println(Args &&...args) {
  print(std::forward<Args>(args)...);

  std::cout << std::endl;
}
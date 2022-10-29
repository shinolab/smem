// File: simple.cpp
// Project: examples
// Created Date: 27/10/2022
// Author: Shun Suzuki
// -----
// Last Modified: 29/10/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#include <exception>
#include <iostream>

#include "smem/smem.hpp"

int main() try {
  std::cout << "enter q to finish..." << std::endl;

  auto smem = smem::SMem();
  smem.create("smem_ipc_test", 65536);
  auto* ptr = static_cast<char*>(smem.map());

  while (true) {
    while (ptr[0] != 0x00) {
    }
    std::cout << "enter something...: ";

    std::string in;
    getline(std::cin, in);
    std::char_traits<char>::copy(ptr + 1, in.c_str(), in.size() + 1);
    ptr[0] = 1;

    if (in == "q") break;
  }

  smem.unmap();

  return 0;

} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return -1;
}

// File: ipc-server.cpp
// Project: examples
// Created Date: 29/10/2022
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
  auto smem = smem::SMem();
  smem.create("smem_ipc_test", 65536);
  volatile auto* ptr = static_cast<char*>(smem.map());
  ptr[0] = 0;
  while (true) {
    if (ptr[0] == 0x00) continue;
    std::cout << "Receive: " << const_cast<char*>(ptr + 1) << std::endl;
    if (std::strcmp(const_cast<char*>(ptr + 1), "q") == 0) break;
    ptr[0] = 0;
  }

  smem.unmap();
  smem.close();

  return 0;

} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return -1;
}

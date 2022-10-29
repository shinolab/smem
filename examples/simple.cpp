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
#include <thread>

#include "smem/smem.hpp"

int main() try {
  std::cout << "enter q to finish..." << std::endl;

  bool created = false;
  auto server_th = std::thread([&created] {
    auto smem = smem::SMem();
    smem.create("test_smem", 65536);
    volatile auto* ptr = static_cast<char*>(smem.map());
    ptr[0] = 0;
    created = true;
    while (true) {
      if (ptr[0] != 0x00) {
        std::cout << "Receive: " << const_cast<char*>(ptr) << std::endl;
        break;
      }
    }
    smem.unmap();
    smem.close();
  });

  auto client_th = std::thread([&created] {
    while (!created) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto smem = smem::SMem();
    smem.create("test_smem", 65536);
    auto* ptr = static_cast<char*>(smem.map());
    std::string in;
    getline(std::cin, in);
    std::char_traits<char>::copy(ptr, in.c_str(), in.size() + 1);
    smem.unmap();
  });

  if (server_th.joinable()) server_th.join();
  if (client_th.joinable()) client_th.join();

  return 0;

} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return -1;
}

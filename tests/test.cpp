// File: test.cpp
// Project: tests
// Created Date: 26/10/2022
// Author: Shun Suzuki
// -----
// Last Modified: 28/10/2022
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2022 Shun Suzuki. All rights reserved.
//

#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26439 26495 26812)
#endif
#include <gtest/gtest.h>
#if _MSC_VER
#pragma warning(pop)
#endif

#include <random>
#include <thread>
#include <cstring>
#include <smem/smem.hpp>

TEST(FunctionTest, RandomRW) {
  constexpr size_t size = 65536;
  constexpr size_t iter = 1000;

  std::vector<std::vector<uint8_t>> sets;
  sets.reserve(iter);

  for (size_t i = 0; i < iter; i++) {
    std::random_device rnd_device;
    std::mt19937 engine{rnd_device()};
    std::uniform_int_distribution dist{0x00, 0xFF};
    std::vector<uint8_t> data(size);
    std::generate(data.begin(), data.end(), [&engine, &dist] { return dist(engine); });
    sets.emplace_back(std::move(data));
  }

  auto server_th = std::thread([&sets, size, iter] {
    auto smem = smem::SMem();
    smem.create("test_smem", size + 1);
    auto* ptr = static_cast<uint8_t*>(smem.map());
    ptr[0] = 0;
    std::vector<uint8_t> buf;
    buf.resize(size);
    size_t c = 0;
    while (true) {
      if (ptr[0] == 0) std::this_thread::sleep_for(std::chrono::milliseconds(10));
      if (ptr[0] != 1) continue;
      std::memcpy(buf.data(), ptr + 1, size);
      ptr[0] = 0;
      for (size_t i = 0; i < size; i++) ASSERT_EQ(sets[c][i], buf[i]);
      if (++c == iter) break;
    }
    smem.unmap();
    smem.close();
  });

  auto client_th = std::thread([&sets, size, iter] {
    auto smem = smem::SMem();
    smem.create("test_smem", size + 1);
    auto* ptr = static_cast<uint8_t*>(smem.map());
    ptr[0] = 0;
    for (size_t i = 0; i < iter; i++) {
      while (ptr[0] == 1) std::this_thread::sleep_for(std::chrono::milliseconds(10));
      std::memcpy(ptr + 1, sets[i].data(), size);
      ptr[0] = 1;
    }
    smem.unmap();
    smem.close();
  });

  if (server_th.joinable()) server_th.join();
  if (client_th.joinable()) client_th.join();
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

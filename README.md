# SMEM

A C++ cross-platform thin wrapper library for shared memory.

# Usage

```cpp
#include <smem/smem.hpp>

...

const auto size = 65536;
auto smem = smem::SMem();
smem.create("your memory name", size);
void* ptr = smem.map();

// Do something with ptr...

smem.unmap();
smem.close();
``` 

# Author

Shun Suzuki, 2022

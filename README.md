# Hashlib

### Acceptance under license:
The `xxhash.h` library is licensed under the BSD 2-Clause License.

### Description:
A collection of implementations of non-cryptographic hash functions for C/C++

### List of algorithms:
* DJB2/DJB2_XOR
* SDBM
* FNV1_32bit
* FNV1_64bit
* FNV1A_32bit
* FNV1A_64bit
* MurMur3_x86_32bit
* MurMur3_x86_128bit
* MurMur3_x64_128bit
* xxHash_32bit
* xxHash_64bit
* xxHash3_64bit
* xxHash3_128bit
* CRC_32bit

### Use:
Include the `hashlib.h` and `xxhash.h` header files and add `hashlib.c` to the build process.

For xxHash, the ```XXH_STATIC_LINKING_ONLY``` and ```XXH_IMPLEMENTATION``` directives are included in ```hashlib.c```

### Examples:

### C:
```c
#include <stdio.h>
#include "hashlib.h"

int main(void)
{
    printf("%x\n", hash_crc32("string"));
}
```

```c
#include <stdio.h>
#include "hashlib.h"

int main(void)
{
    printf("%x\n", hash_xxh32_buf("string", 7, 0));
}
```

```c
#include <stdio.h>
#include "hashlib.h"

int main(void)
{
    uint32_t out = 0;

    hash_murmur3_x86_32("string", 7, 0, &out);

    printf("%x\n", out);
}
```

### C++:

```cpp
#include <iostream>
#include "hashlib.h"

int main(void)
{
    std::cout << std::hex << hash_crc32(reinterpret_cast<const unsigned char*>("string")) << std::endl;
    return 0;
}
```

```cpp
#include <iostream>
#include "hashlib.h"

int main(void)
{
    std::cout << std::hex << hash_xxh32_buf("string", 7, 0) << std::endl;
    return 0;
}
```

```cpp
#include <iostream>
#include "hashlib.h"

int main(void)
{
    uint32_t out = 0;
    
    hash_murmur3_x86_32("string", 7, 0, &out);
    
    std::cout << std::hex << out << std::endl;
    return 0;
}
```
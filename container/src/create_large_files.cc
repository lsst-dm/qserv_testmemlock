#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    const char * files[] =
    {
        "/tmp/qserv/big-file-1.txt",
        "/tmp/qserv/big-file-2.txt"
    };

    int32_t buf[256]; // Block size.
    for (int i = 0; i < 256; ++i)
    {
        buf[i] = rand(); // random to be non-compressible.
    }
    int blocksToWrite = 1024 * 1024 *10; // 10 GB

    for (int j = 0; j < 2; ++j)
    {
        FILE* file = fopen(files[j], "wb");
        for (int i = 0; i < blocksToWrite; ++i)
        {
            fwrite(buf, sizeof(int32_t), 256, file);
        }
        fclose(file);
    }
}

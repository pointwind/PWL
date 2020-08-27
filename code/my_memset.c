#include <stdint.h>
#include <stddef.h>
void* my_memset(void* str, int c, size_t len)
{
    uint8_t* bp = str;
    uint8_t* ep = bp + len;
    if (len > 16) {
        //align to 8-byte
        size_t r = -(uintptr_t)bp % 8;
        while (r != 0)
        {
            *bp = c;
            ++bp;
            --r;
        }

        //wordwise 
        r = (size_t)(ep - bp) / 8;
        uint64_t word = c & 0xff;
        word |= word << 8;
        word |= word << 16;
        word |= word << 32;
        //duff device
        switch (r%8)
        {
	        do
	        {
				default:
                    *(uint64_t*)bp = word,bp += 8,--r;
				case 1:
                    *(uint64_t*)bp = word, bp += 8, --r;
				case 2:
                    *(uint64_t*)bp = word, bp += 8, --r;
				case 3:
                    *(uint64_t*)bp = word, bp += 8, --r;
				case 4:
                    *(uint64_t*)bp = word, bp += 8, --r;
				case 5:
                    *(uint64_t*)bp = word, bp += 8, --r;
				case 6:
                    *(uint64_t*)bp = word, bp += 8, --r;
				case 7:
                    *(uint64_t*)bp = word, bp += 8, --r;
	        }
	        while (r!= 0);


        }
        /*while (r != 0)
        {
            *(uint64_t*)bp = word;
            bp += 8;
            --r;
        }*/
    }
    while(bp != ep)
    {
        *bp = c;
        bp++;
    }
    return str;
}

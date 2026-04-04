#include <stdio.h>
#include <stdint.h>

int main(){
    int t;
    scanf("%d", &t);

    while(t--){
        uint32_t n;
        scanf("%x", &n);

        //Rule 1
        if(n & (n << 1)){
            printf("0xFFFFFFFF\n");
            continue;
        }

        //Rule 2
        if(n != 0){
            // find LSB position: lowest set bit
            int lsb = 0;
            while(!((n >> lsb) & 1)) lsb++;

            // find MSB position: highest set bit
            int msb = 31;
            while(!((n >> msb) & 1)) msb--;

            if(msb != lsb){
                uint32_t mask = ((uint32_t)1 << msb) - 1;  // bits 0 ~ msb-1
                mask &= ~(((uint32_t)1 << (lsb+1)) - 1);   // bits 0 ~ lsb
                n ^= mask; //XOR
            }
        }

        // Rule 3: XOR 4 bytes, replace lowest byte
        uint8_t b3 = (n >> 24) & 0xFF;
        uint8_t b2 = (n >> 16) & 0xFF;
        uint8_t b1 = (n >>  8) & 0xFF;
        uint8_t b0 =  n        & 0xFF;

        uint8_t checksum = b3 ^ b2 ^ b1 ^ b0;

        n = (n & 0xFFFFFF00) | checksum;  // keep top 24 bits, replace bottom 8

        printf("0x%08X\n", n);
    }
    return 0;
}
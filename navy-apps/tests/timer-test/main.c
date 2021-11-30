#include <stdio.h>
#include <NDL.h>

int main() {
    NDL_Init(0);
    uint32_t t = NDL_GetTicks();
    uint32_t new;
    uint32_t n = 0;
    while (1) {
        new = NDL_GetTicks();
        int update = (new - t) / 500;
        for (int i = n; i < update; i++) {
            n++;
            printf("%d.%ds passed\n", n / 2, (n % 2) ? 5 : 0);
        }
    }
    NDL_Quit(0);
    return 0;
}
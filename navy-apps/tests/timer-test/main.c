#include <sys/time.h>
#include <stdio.h>
#include <assert.h>

int main() {
    struct timeval t;
    struct timeval new;
    int n = 0;
    assert(gettimeofday(&t, NULL) == 0);
    while (1) {
        assert(gettimeofday(&new, NULL) == 0);
        int update = ((new.tv_sec - t.tv_sec) * 1000000 + (new.tv_usec - t.tv_usec)) / 500000;
        for (int i = 0; i < update; i++) {
            n++;
            printf("%d.%ds passed\n", n / 2, (n % 2) ? 5 : 0);
        }
        t = new;
    }
    return 0;
}
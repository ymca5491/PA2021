#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
// this should be enough
static char buf[65536] = {'\0'};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static void gen(char* s);
static void gen_num();
static void gen_rand_op();
static char* op[] = {"+", "-", "*", "/"};

uint32_t choose(uint32_t n) {
  return rand() % n;
}

static void gen_rand_expr(bool initialize) {
  if (initialize) buf[0] = '\0';
  switch (choose(7)) {
    case 0: case 1: case 2: gen_num(); break;
    case 3: case 4: gen("("); gen_rand_expr(false); gen(")"); break;
    default: gen_rand_expr(false); gen_rand_op(); gen_rand_expr(false); break;
  }
}

static void gen(char* s) {
  strcat(buf, s);
}

static void gen_num() {
  int32_t rnd;
  rnd= choose(1000);
  char temp[32] = {};
  sprintf(temp, "%u", rnd);
  strcat(buf, temp);
}

static void gen_rand_op() {
  char* rand_op = op[choose(4)];
  strcat(buf, rand_op);
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr(true);
    if(strlen(buf) >= 31) {
      i--;
      continue;
    }
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc -Werror /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    int t = fscanf(fp, "%d", &result);
    t = t;          // avoid unused error
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}

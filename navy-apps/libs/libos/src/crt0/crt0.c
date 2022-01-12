#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = *(int *)args;
  args = (uintptr_t *)((void *)args + sizeof(int));
  char **argv = (char **)args;
  args += argc + 1; // for all argv and a 'NULL'
  char **envp = (char **)args;
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}

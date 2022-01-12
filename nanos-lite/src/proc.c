#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

//void naive_uload(PCB *pcb, const char *filename);
void context_kload(PCB *pcb, void (*entry)(), void *arg);
void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    //Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here

  char *empty[] = {NULL};
  char *argv_exec[] = {"/bin/exec-test", 0, NULL};
  //char *argv_pal[] = {"--skip", NULL};
  //context_kload(&pcb[0], hello_fun, NULL);
  context_uload(&pcb[0], "/bin/exec-test", argv_exec, empty);
  //context_uload(&pcb[1], "/bin/pal", argv_pal, empty);
  switch_boot_pcb();

  //naive_uload(NULL, "/bin/nterm");

}

Context* schedule(Context *prev) {
  // save the context pointer
  current->cp = prev;

  // always select pcb[0] as the new process
  current = &pcb[0];
  //current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);

  // then return the new context
  return current->cp;
}

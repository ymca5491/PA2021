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
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here

  char *empty[] = {NULL};
  char *argv_pal[] = {"/bin/pal","--skip", NULL};
  context_kload(&pcb[0], hello_fun, (void *)-1);
  //context_uload(&pcb[0], "/bin/hello", NULL, NULL);
  context_uload(&pcb[3], argv_pal[0], argv_pal, empty);
  context_uload(&pcb[2], "/bin/bird", NULL, NULL);
  context_uload(&pcb[1], "/bin/nterm", NULL, NULL);
  //context_uload(&pcb[1], "/bin/menu", empty, empty);
  switch_boot_pcb();


  //naive_uload(NULL, "/bin/nterm");

}

Context* schedule(Context *prev) {
  static int counter = 0;
  static PCB *fg_pcb = &pcb[1];
  if (counter == 0) {
    AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
    if (ev.keydown) {
      switch (ev.keycode) {
        case 2: fg_pcb = &pcb[1]; break;
        case 3: fg_pcb = &pcb[2]; break;
        case 4: fg_pcb = &pcb[3]; break;
        default:                  break;
      }
    }
  }
  // save the context pointer
  current->cp = prev;
  // always select pcb[0] as the new process
  //current = &pcb[0];
  current = (counter == 0 ? &pcb[0] : fg_pcb);
  counter++; counter %= 100;

  // then return the new context
  return current->cp;
}

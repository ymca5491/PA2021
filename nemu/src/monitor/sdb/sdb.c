#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <utils.h>
#include <ctype.h>
#include <memory/paddr.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char* args);

static int cmd_p(char* args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */

  {"si", "Execute the next N instrcutins", cmd_si },
  {"info", "Print the status of program", cmd_info},
  {"x", "Print the next N 4-bytes of the input address", cmd_x},
  {"p", "Print the value of the expression", cmd_p},

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i = 0;

  if(arg == NULL){
    /* no argument given */
    cpu_exec(1);
  }
  else{
    for(int j = 0; j < strlen(arg); j++){
      if(isdigit(arg[j])){
        i = 10 * i + arg[j] - '0';
      }
      else{
        printf("Not a valid argument\n");
        return 0;
      }
    }
    cpu_exec(i);
  }
  return 0;
}

static int cmd_info(char* args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");

  if(arg == NULL){
    /* no argument given */
    printf("Unvalid command for no argument is given");
  }
  else{
    if(strcmp(arg, "r") == 0){
      /* print the status of registers */
      isa_reg_display();
    }
    else if(strcmp(arg, "w") == 0){
      /* print the information of watch points */
      printf("Not implemented yet\n");
    }
    else{
      /* invalid argument */
      printf("Invalid argument '%s'\n", arg);
    }
  }
  return 0;
}

static int cmd_x(char* args) {
  /* extract the first argument */
  char *arg1 = strtok(NULL, " ");
  char *arg2 = strtok(NULL, " ");
    
  if(arg1 == NULL || arg2 == NULL){
    /* no argument given */
    printf("Unvalid command for no argument is given");
  }
  else{
    int n;
    paddr_t addr;
    sscanf(arg1, "%d", &n);
    sscanf(arg2, "0x%x", &addr);
    int i;
    word_t value;
    /* print each byte in storage order */
    for(i = 0; i < n; i++){
      value = paddr_read(addr, 1);
      printf("0x%8x:    %02x", addr, value);
      value = paddr_read(addr + 1, 1);
      printf(" %02x", value);
      value = paddr_read(addr + 2, 1);
      printf(" %02x", value);
      value = paddr_read(addr + 3, 1);
      printf(" %02x\n", value);
      addr += 4;
    }
  }
  return 0;
}

static int cmd_p(char* args) {
  bool success;
  word_t value = expr(args, &success);
  if (success){
    printf("%d\n", value);
    return 0;
  }
  else {
    printf("Not a valid expression\n");
    return 0;
  }
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) {
          if (nemu_state.state == NEMU_STOP) nemu_state.state = NEMU_QUIT;
          return;
        }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}

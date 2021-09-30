#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

  char expr[64];  // the expression watched

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char* expr) {
  assert(free_);
  WP* new = free_;
  free_ = free_->next;
  strcpy(new->expr, expr);
  return new;
}

void add_wp(char* expr) {
  WP* new = new_wp(expr);
  new->next = head;
  head = new;
}

void free_wp(WP *wp) {
  wp->next = free_;
  free_= wp;
}

WP* find_wp(int NO) {
  WP* wp_NO = head;
  while(wp_NO && head->NO != NO) {
    wp_NO = wp_NO->next;
  }
  return wp_NO;
}

void delete_wp(int NO) {
  free_wp(find_wp(NO));
}

void wp_display() {
  printf("%-6s%-20s\n", "Num", "What");
  WP* temp = head;
  while(temp){
    printf("%-6d%-20s\n", temp->NO, temp->expr);
    temp = temp->next;
  }
}
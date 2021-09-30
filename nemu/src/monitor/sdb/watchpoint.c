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

bool delete_wp(int NO) {
  WP* wp_NO = head;
  if (wp_NO->NO == NO) {
    head = wp_NO->next;
    free_wp(wp_NO);
    return true;
  }
  else {
    while(wp_NO->next && wp_NO->next->NO != NO) {
      wp_NO = wp_NO->next;
    }
    if (!wp_NO->next) return false;
    else {
      WP* temp = wp_NO->next;
      wp_NO->next = temp->next;
      free_wp(temp);
      return true;
    }
  }
}

void wp_display() {
  printf("%-20s%-20s\n", "Num", "What");
  WP* temp = head;
  while(temp){
    printf("%-20d%-20s\n", temp->NO, temp->expr);
    temp = temp->next;
  }
}
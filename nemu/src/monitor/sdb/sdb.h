#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

word_t expr(char *e, bool *success);

/* watcpoint */

void add_wp(char* expr);
void delete_wp(int NO);

#endif

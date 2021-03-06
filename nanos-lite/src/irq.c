#include <common.h>
void do_syscall(Context *c);
Context* schedule(Context *prev);

static Context* do_event(Event e, Context* c) {
  //printf("EVENT_IRQ_TIMER = %d\n", EVENT_IRQ_TIMER);
  switch (e.event) {
    case EVENT_YIELD: c->mepc += 4; return schedule(c); break;
    case EVENT_SYSCALL: do_syscall(c); c->mepc += 4; break;
    case EVENT_IRQ_TIMER: 
      //Log("Reiceive EVENT_IRQ_TIMER");
      return schedule(c); break;// irq_time comes after updatepc
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}

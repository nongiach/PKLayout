#include "functions.h"

const char *get_ev_name(int ev) {
  if (ev == EV_SYN) return "EV_SYN";
  if (ev == EV_KEY) return "EV_KEY";
  if (ev == EV_REL) return "EV_REL";
  if (ev == EV_ABS) return "EV_ABS";
  if (ev == EV_MSC) return "EV_MSC";
  if (ev == EV_SW) return "EV_SW";
  if (ev == EV_LED) return "EV_LED";
  if (ev == EV_SND) return "EV_SND";
  if (ev == EV_REP) return "EV_REP";
  if (ev == EV_FF) return "EV_FF";
  if (ev == EV_PWR) return "EV_PWR";
  if (ev == EV_FF_STATUS) return "EV_FF_STATUS";
  if (ev == EV_MAX) return "EV_MAX";
  //if ((EV_MAX) return1), "EV_CNT";
  return "Unknow";
}

int get_next_event(int fd, t_event *ie, int size) {
  int i = 0;
  for (i = 0; i < size; ++i) {
    if (read(fd, &ie[i],
          sizeof(t_event)) != sizeof(t_event))
      return i;
    if (ie[i].type == EV_SYN)
      return i + 1; 
  }
  return i;
}


void print_event(t_event *ie, int size) {
  printf("\t\t\t");
  for (int i = 0; i < size; ++i) {
    printf("%s|c%d[%d  ", get_ev_name(ie[i].type), ie[i].code, ie[i].value); 
  }
}

void send_keyevent(int fd, int value,
    struct timeval atime, int type) {
  //if (atime == 0)
  //  atime = time(NULL); 
  t_event ie_p[] = {
    {.time = atime, .code = 4, .value = value, .type = EV_MSC},
    {.time = atime, .code = value, .value = type, .type = EV_KEY},
    {.time = atime, .code = 0, .value = SYN_REPORT, .type = EV_SYN},
  };
  if (value == 184)
    ie_p[1].code = 100;
  write(fd, ie_p, sizeof(t_event) * 3);
}

/* void send_keyevent(int fd, int value, struct timeval atime) { */
/*   //if (atime == 0) */
/*   //  atime = time(NULL);  */
/*   send_keyevent_press(fd, value, atime); */
/*   send_keyevent_release(fd, value, atime); */
/* } */


int keycode(char *name) {
  //int k_val = XKeysymToString(XKeycodeToKeysym(ctrl_disp, event[0].value + 8, 0));
  //if (!name[0])
   // return 0;
  int k_val = XKeysymToKeycode(ctrl_disp, XStringToKeysym(name));
  printf("%d | %d = %s\n", k_val, k_val - 8, name);
  return k_val - 8;
}

/* void send_combo(int fd,  t_combo *combo, struct timeval atime) { */
/*   if (combo->mod == 0) { */
/*     printf("\t\n\t\tsend combo push\n"); */
/*     send_keyevent_push(fd, combo->key, atime); */
/*   } */
/*   else { */
/*     printf("\t\n\t\tsend combo push\n"); */
/*     send_keyevent_push(fd, combo->mod, atime); */
/*     send_keyevent(fd, combo->key, atime); */
/*     send_keyevent_release(fd, combo->mod, atime); */
/*   } */
/* } */

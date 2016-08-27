#include "functions.h"

void send_combo(int fd, t_combo *combo,
    struct timeval aatime, int type) {
  //if (atime == 0)
  // int atime = time(NULL); 
  printf("\t\t\tsend combo %d %d|%d\n", type, combo->mod, combo->key);
  if (combo->mod == 0) {
    send_keyevent(fd, combo->key, aatime, type);
  }
  else {
    switch (type) {
      case KEY_PUSHED:
        send_keyevent(fd, combo->mod, aatime, type);
        send_keyevent(fd, combo->key, aatime, type);
        break;
      case KEY_PRESSED:
        send_keyevent(fd, combo->key, aatime, type);
        break;
      case KEY_RELEASED:
        send_keyevent(fd, combo->key, aatime, type);
        send_keyevent(fd, combo->mod, aatime, type);
        break;
    }
  }
}

t_combo *is_combo(int mod, t_event *event) {
  static t_combo combo;
  for (int i = 0; i < remap_size; ++i) {
    if (mod == remap[i].from.mod &&
        K_VALUE == remap[i].from.key) {
      if (remap[i].to.mod == 0 &&
          remap[i].to.key == 0) {
        printf("##### Disabled ##### \n");
        return 0;
      }
      printf("#### is combo %d ####\n", i);
      combo = remap[i].to;
      printf("\t\t\tsend combo %d|%d\n", remap[i].to.mod,  remap[i].to.key);
      return &combo; 
    }
  }
  for (int i = 0; i < remap_mod_size; ++i) {
    if (mod == remap_mod[i].from) {
      combo.key = K_VALUE;
      combo.mod = remap_mod[i].to;
      printf("\t||| ### send mod = %d\n", combo.mod);
      return &combo;
    }
  }
  return NULL;
}

// s_wait_mod => s_wait_mod | s_wait_combo
// s_wait_combo => s_wait_mod | s_in_combo
// s_in_combo => s_in_combo | s_clean_combo | s_wait_combo
// s_clean_combo => s_wait_combo | s_wait_mod

void new_combo(int uinput, t_keyboard *kb, t_combo *combo, t_event *event) {
  printf("\tnew_combo\n");
  kb->combo = combo;
  kb->key = K_VALUE;
  send_combo(uinput, kb->combo, event[0].time, KEY_PUSHED);
  kb->state = s_in_combo;
}

void s_wait_mod(int uinput, t_keyboard *kb, t_event *event, int nbr_event) {
  printf("\twait_mod\n");
  if (is_mod[K_VALUE] && K_STATE == KEY_PUSHED) {
    kb->mod = K_VALUE;
    kb->state = s_wait_combo;
  } else
    write(uinput, event, nbr_event * sizeof(t_event));
}

void s_wait_combo(int uinput, t_keyboard *kb, t_event *event, int nbr_event) {
  printf("\twait_combo\n");
  t_combo *combo = NULL;
  if (K_STATE == KEY_PUSHED && (combo = is_combo(kb->mod, event)))
    new_combo(uinput, kb, combo, event);
  else {
    //if (kb->mod == K_VALUE) {
      send_keyevent(uinput, kb->mod, event[0].time, KEY_PUSHED);
    /* } */
    kb->state = s_wait_mod;
    write(uinput, event, nbr_event * sizeof(t_event));
  }
}

void s_in_combo(int uinput, t_keyboard *kb, t_event *event, int nbr_event) {
  printf("\tin_combo\n");
  if (kb->mod == K_VALUE || kb->key == K_VALUE) {
    send_combo(uinput, kb->combo, event[0].time, K_STATE);
    if (K_STATE == KEY_RELEASED) {
      if (kb->mod == K_VALUE)
        kb->state = s_clean_combo;
      else
        kb->state = s_wait_recombo;
    }
  } else 
    write(uinput, event, nbr_event * sizeof(t_event));
}

// wait the pushed key to be released
void s_clean_combo(int uinput, t_keyboard *kb, t_event *event, int nbr_event) {
  printf("\tclean_combo\n");
  if (kb->key == K_VALUE) {
    if (K_STATE == KEY_RELEASED)
      kb->state = s_wait_mod;
  } else
    write(uinput, event, nbr_event * sizeof(t_event));
}

void s_wait_recombo(int uinput, t_keyboard *kb, t_event *event, int nbr_event) {
  printf("\twait_recombo\n");
  t_combo *combo = NULL;
  if (kb->mod == K_VALUE) {
    if (K_STATE == KEY_RELEASED)
      kb->state = s_wait_mod;
  } else if (K_STATE == KEY_PUSHED && (combo = is_combo(kb->mod, event)))
    new_combo(uinput, kb, combo, event);
  else
    write(uinput, event, nbr_event * sizeof(t_event));
}

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_ value

#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <assert.h>
#include "pklayout.h"

// init.c

int
constructKeyboard (char *name, struct input_id *id, unsigned long *keymask);

int find_keyboard(char *root, struct input_id *id);

// key_utils.c

int get_next_event(int fd, t_event *ie, int size);
void send_keyevent(int fd, int value, struct timeval, int type);
void print_event(t_event *ie, int size);
int keycode(char *name);

// handle_key.c

void s_wait_recombo(int uinput, t_keyboard *kb, t_event *event, int nbr_event);
void s_clean_combo(int uinput, t_keyboard *kb, t_event *event, int nbr_event);
void s_in_combo(int uinput, t_keyboard *kb, t_event *event, int nbr_event);
void s_wait_combo(int uinput, t_keyboard *kb, t_event *event, int nbr_event);
void s_wait_mod(int uinput, t_keyboard *kb, t_event *event, int nbr_event);

// init.c
int init(int *keyboard_p, int *uinput_p);

#endif /* ifndef FUNCTIONS_H_ */

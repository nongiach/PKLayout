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

Display *ctrl_disp = NULL;

  static int
has_event(int fd, unsigned short type, unsigned short code)
{
#define MAX(a,b) a>b?a:b
  const unsigned int len = sizeof(unsigned long)*8;
  const unsigned int max = MAX(EV_MAX,
      MAX(KEY_MAX,
        MAX(REL_MAX,
          MAX(ABS_MAX,
            MAX(LED_MAX,
              MAX(SND_MAX, FF_MAX))))));
  unsigned long bits[(max+len-1)/len];
  if (ioctl(fd, EVIOCGBIT(type, max), bits)) {
    return (bits[code/len] >> (code%len)) & 1;
  }
  return 0;
#undef MAX
}

// http://lxr.free-electrons.com/source/include/linux/input.h?v=2.6.35#L90
// https://www.kernel.org/doc/Documentation/input/event-codes.txt

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
  static int
constructKeyboard (char *name, struct input_id *id, unsigned long *keymask)
{
  int fd;
  if ((fd = open("/dev/uinput", O_WRONLY)) > 0) {
    struct uinput_user_dev description;

    memset(&description, 0, sizeof(description));
    strncpy(description.name, name, UINPUT_MAX_NAME_SIZE);
    memcpy(&description.id, id, sizeof(description.id));
    if (write(fd, &description, sizeof(description)) == sizeof(description)) {
      int key;

      ioctl(fd, UI_SET_EVBIT, EV_KEY);
      ioctl(fd, UI_SET_EVBIT, EV_REP);
      //ioctl(fd, UI_SET_EVBIT, EV_SYN);

      for (key=KEY_RESERVED; key<=KEY_MAX; key++) {
        if ((keymask[key/(sizeof(unsigned long)*8)] >> (key%(sizeof(unsigned long)*8)))&1) {
          ioctl(fd, UI_SET_KEYBIT, key);
        }
      }
      if (ioctl(fd, UI_DEV_CREATE) != -1) {
        return fd;
      } else {
        perror("ioctl UI_DEV_CREATE");
      }
    } else {
      perror("write (struct uinput_user_dev)");
    }
  } else {
    perror("open /dev/input/uinput");
  }
  return -1;
}

int find_keyboard(char *root, struct input_id *id) {
  int rootLength = strlen(root);
  int foundKeyboard = 0;
  DIR *directory;

  if ((directory = opendir(root))) {
    struct dirent *entry;

    while ((entry = readdir(directory))) {
      size_t nameLength = strlen(entry->d_name);
      struct stat status;
      char path[nameLength + 1 + rootLength + 1];

      snprintf(path, sizeof(path), "%s/%s", root, entry->d_name);
      if (stat(path, &status) == -1) continue;
      if (S_ISCHR(status.st_mode)) {
        int fd;
        if ((fd = open(path, O_RDONLY)) > 0) {
          printf("Opening %s\n", path);
          if (ioctl(fd, EVIOCGID, id) != -1) {
            printf("Keyboard found %s\n", path);
            /* if (ioctl(fd, EVIOCGBIT(EV_KEY, KEY_MAX), keymask)) */
            if (!strcmp(entry->d_name, "event0")) {
              //if (1 || id.bustype == 3)
              foundKeyboard = fd;            
              break;
            }
          }
          close(fd);
        }
      }
    }
    closedir(directory);

    if (!foundKeyboard) {
      printf("No supported USB keyboard found.\n");
    }
  } else {
    fprintf(stderr, "Unable to open %s\n", root);
  }
  return foundKeyboard;
}

typedef struct input_event t_event;

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

void send_keyevent(int fd, int value, int atime);

void print_event(t_event *ie, int size) {
  for (int i = 0; i < size; ++i) {
    printf("%s|c%d[%d  ", get_ev_name(ie[i].type), ie[i].code, ie[i].value); 
  }
  printf("\n");
}

  /* {{KEY_E, KEY_J}, {0, KEY_U}}, */
  /* {{KEY_E, KEY_K}, {0, KEY_I}}, */
  /* {{KEY_E, KEY_L}, {0, KEY_O}}, */
/* }; */

char is_mod[255] = {0};
#define KEY_RELEASED 0
#define KEY_PUSHED 1
#define KEY_PRESSED 2

void send_keyevent_press(int fd, int value, int atime) {
  if (atime == 0)
    atime = time(NULL); 
  t_event ie_p[3] = {
    {.time = atime, .code = 4, .value = value, .type = EV_MSC},
    {.time = atime, .code = value, .value = KEY_PUSHED, .type = EV_KEY},
    {.time = atime, .code = 0, .value = SYN_REPORT, .type = EV_SYN},
  };
  if (value == 184)
    ie_p[1].code = 100;
  write(fd, ie_p, sizeof(t_event) * 3);
}

void send_keyevent_release(int fd, int value, int atime) {
  if (atime == 0)
    atime = time(NULL); 

  t_event ie_r[3] = {
    {.time = atime, .code = 4, .value = value, .type = EV_MSC},
    {.time = atime, .code = value, .value = KEY_RELEASED, .type = EV_KEY},
    {.time = atime, .code = 0, .value = SYN_REPORT, .type = EV_SYN},
  };
  if (value == 184)
    ie_r[1].code = 100;
  write(fd, ie_r, sizeof(t_event) * 3);
}


void send_keyevent(int fd, int value, int atime) {
  if (atime == 0)
    atime = time(NULL); 
  send_keyevent_press(fd, value, atime);
  send_keyevent_release(fd, value, atime);
}

void send_keyevent_combo(int fd, t_combo *combo, int atime) {
  if (atime == 0)
    atime = time(NULL); 
  if (combo->mod == 0) {
    printf("\t\n\t\tsend combo\n");
    send_keyevent(fd, combo->key, atime);
  }
  else {
    printf("\t\n\t\tsend combo\n");
    send_keyevent_press(fd, combo->mod, atime);
    send_keyevent(fd, combo->key, atime);
    send_keyevent_release(fd, combo->mod, atime);
  }
}
int keycode(char *name) {
  //int k_val = XKeysymToString(XKeycodeToKeysym(ctrl_disp, event[0].value + 8, 0));
  int k_val = XKeysymToKeycode(ctrl_disp, XStringToKeysym(name));
  printf("%d | %d = %s\n", k_val, k_val - 8, name);
  return k_val - 8;
}

int main(int argc, char *argv[])
{
  ctrl_disp = XOpenDisplay (NULL);
  if (!ctrl_disp) {
    fprintf(stderr, "failed to open display\n");
    return 0; 
  }
  XSynchronize(ctrl_disp,True);
  struct input_id id;
  int keyboard = find_keyboard("/dev/input", &id);
  if (keyboard == 0) 
    return -1;
  assert(remap_size < 512);

  for (int i = 0; i < remap_size; ++i) {
    remap[i].from.mod = keycode(remap_string[i].from.mod);
    remap[i].from.key = keycode(remap_string[i].from.key);
    remap[i].to.mod = keycode(remap_string[i].to.mod);
    remap[i].to.key = keycode(remap_string[i].to.key);
    is_mod[remap[i].from.mod] = 1;
  }
  keycode("Up");
  unsigned long keymask[(KEY_MAX+(sizeof(unsigned long)*8)-1)/(sizeof(unsigned long)*8)];
#define NO_MOD -1
  int virtual_mod = NO_MOD;
  int virtual_mod_consumed = 0;
  int cached_key = -1;
  t_combo *cached_combo = NULL;
  int nbr_pressed_key = 0;
  int max_size = 8;
  t_event event[max_size];
#define K_VALUE event[0].value
#define K_STATE event[1].value
  int k_escape = keycode("Escape");
  int k_caps = keycode("Caps_Lock");
  if (ioctl(keyboard, EVIOCGBIT(EV_KEY, KEY_MAX), keymask)) {
    int uinput = constructKeyboard("test", &id, keymask);

    if (uinput != -1) {
      ioctl(keyboard, EVIOCGRAB, 1);
      int nbr_event = 0;
      while (nbr_event = get_next_event(keyboard, event, max_size)) {
        print_event(event, nbr_event);
        // Global remap of Escape => Nop and CAPS_LOCK => Escape
        printf("KeyPress: \t%s\n",
            XKeysymToString(XKeycodeToKeysym(ctrl_disp, event[0].value + 8, 0)));
        if (K_VALUE == k_caps) {
          event[0].value = k_escape;
          event[1].code = k_escape;
        } else if (K_VALUE == k_escape)
          continue;

        if (is_mod[event[0].value] &&
            ((virtual_mod == NO_MOD && nbr_pressed_key == 0) ||
             (virtual_mod == event[0].value))) {
          printf(" [MOD] ");
          if (event[1].value == KEY_PUSHED) {
            virtual_mod = K_VALUE;
            printf("Activate MOD nbr_pressed = %d\n", nbr_pressed_key);
          }
          else if (event[1].value == KEY_RELEASED) {
            if (!virtual_mod_consumed && cached_key == -1)
              send_keyevent(uinput, K_VALUE, 0);
            virtual_mod = NO_MOD;
            virtual_mod_consumed = 0; 
            if (cached_key != -1) {
              //send_keyevent_press(uinput, cached_key, 0);

              send_keyevent_combo(uinput, cached_combo, 0);
              cached_key = -1;
            }
          }
          else {// KEY_PRESSED
            if (!virtual_mod_consumed)
              send_keyevent_press(uinput, K_VALUE, 0);
            virtual_mod = NO_MOD;
            virtual_mod_consumed = 0; 
          }
        }
        else if (virtual_mod != NO_MOD) {
          int good_combo = 0;
          for (int i = 0; i < remap_size; ++i) {
            if (virtual_mod == remap[i].from.mod &&
                event[0].value == remap[i].from.key) {
              if (event[1].value == KEY_RELEASED) {
                send_keyevent_combo(uinput, &remap[i].to, 0);
                virtual_mod_consumed = 1; 
              } else if (event[1].value == KEY_PUSHED) {
                cached_key = K_VALUE;
                cached_combo = &remap[i].to;
                printf("Caching Key\n");
              }
              good_combo = 1;
              break;
            } }
            if (!good_combo) // replay mod
            {
              send_keyevent_press(uinput, virtual_mod, 0);              
              virtual_mod = NO_MOD;
              virtual_mod_consumed = 0;
              write(uinput, event, nbr_event * sizeof(t_event));
            }
          }
          else {
            printf("normal");
            write(uinput, event, nbr_event * sizeof(t_event));
          }
          if (event[1].value == KEY_PUSHED)
            nbr_pressed_key++;
          if (event[1].value == KEY_RELEASED) {
            cached_key = -1;
            nbr_pressed_key--;
          }
          if (nbr_pressed_key < 0)
            nbr_pressed_key = 0;
          printf("nbr_pressed_key %d\n", nbr_pressed_key);
        }
        ioctl(keyboard, EVIOCGRAB, 0);
        close(uinput);
      }
    }
    return 0;
  }

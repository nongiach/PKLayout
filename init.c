#include "functions.h"

int
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

Display *ctrl_disp = NULL;
t_remap_mod remap_mod[255];
char is_mod[255] = {0};

int init(int *keyboard_p, int *uinput_p) {
  ctrl_disp = XOpenDisplay (NULL);
  if (!ctrl_disp) {
    fprintf(stderr, "failed to open display\n");
    return -1;
  }
  XSynchronize(ctrl_disp,True);
  struct input_id id;
  int keyboard = find_keyboard("/dev/input", &id);
  if (keyboard == 0) 
    return -1;
  assert(remap_size < 512);
  assert(remap_mod_size < 256);
  for (int i = 0; i < remap_size; ++i) {
    remap[i].from.mod = keycode(remap_string[i].from.mod);
    remap[i].from.key = keycode(remap_string[i].from.key);
    remap[i].to.mod = keycode(remap_string[i].to.mod);
    remap[i].to.key = keycode(remap_string[i].to.key);
    is_mod[remap[i].from.mod] = 1;
    printf("\t\t\tsend combo %d|%d\n", remap[i].to.mod,  remap[i].to.key);
  }
  
  for (int i = 0; i < remap_mod_size; ++i) {
    remap_mod[i].from = keycode(remap_mod_string[i].from);
    remap_mod[i].to = keycode(remap_mod_string[i].to);
    is_mod[remap_mod[i].from] = 2;
  }

  unsigned long keymask[(KEY_MAX+(sizeof(unsigned long)*8)-1)/
    (sizeof(unsigned long)*8)];
  if (!ioctl(keyboard, EVIOCGBIT(EV_KEY, KEY_MAX), keymask))
    return -1;
  int uinput = constructKeyboard("test", &id, keymask);
  if (uinput == -1)
    return -1;
  ioctl(keyboard, EVIOCGRAB, 1);
  *keyboard_p = keyboard;
  *uinput_p = uinput;
  printf("\t\t\tsend combo %d|%d\n", remap[0].to.mod,  remap[0].to.key);
  return 0;
}

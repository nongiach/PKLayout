#include "functions.h"

int main(int argc, char *argv[])
{
  (void)argv;
  (void)argc;

  int keyboard, uinput;
  if (init(&keyboard, &uinput))
    return -1;
  printf("[+] init\n");
  int k_escape = keycode("Escape");
  int k_caps = keycode("Caps_Lock");
  keycode("XK_UP");
  int max_size = 8;
  t_event event[max_size];
  t_keyboard kb;
  kb.state = s_wait_mod;
  int nbr_event = 0;
  while ((nbr_event = get_next_event(keyboard, event, max_size))) {
    print_event(event, nbr_event);
    // Global remap of Escape => Nop and CAPS_LOCK => Escape
    printf("KeyPress: %s",
        XKeysymToString(XKeycodeToKeysym(ctrl_disp, event[0].value + 8, 1)));
    if (K_VALUE == k_caps) {
      event[0].value = k_escape;
      event[1].code = k_escape;
    } else if (K_VALUE == k_escape) {
      event[0].value = k_caps;
      event[1].code = k_caps;
    }
      
    if (nbr_event == 3)
      kb.state(uinput, &kb, event, nbr_event);
    else
      write(uinput, event, nbr_event * sizeof(t_event));
  }
  ioctl(keyboard, EVIOCGRAB, 0);
  close(uinput);
  return 0;
}

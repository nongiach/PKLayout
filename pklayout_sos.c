#include "functions.h"
/* int is_big_mod(int uinput, t_event *event, int nbr_event) { */
/*   for (int i = 0; i < remap_mod_size; ++i) { */
/*     remap_mod[i].from = keycode(remap_mod_string[i].from); */
/*     remap_mod[i].to = keycode(remap_mod_string[i].to); */
/*     is_mod[remap_mod[i].from] = 1; */
/*   } */
/* } */

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

  int nbr_event = 0;
  while ((nbr_event = get_next_event(keyboard, event, max_size))) {
    print_event(event, nbr_event);
    // Global remap of Escape => Nop and CAPS_LOCK => Escape
    printf("KeyPress: \t%s\n",
        XKeysymToString(XKeycodeToKeysym(ctrl_disp, event[0].value + 8, 1)));
    if (K_VALUE == k_caps) {
      event[0].value = k_escape;
      event[1].code = k_escape;
    } else if (K_VALUE == k_escape)
      continue;
    if (nbr_event == 3) {
      switch (K_STATE) {
        case KEY_PUSHED:
          on_key_pushed(uinput, event, nbr_event);
          break;
        case KEY_PRESSED:
          on_key_pressed(uinput, event, nbr_event);
          break;
        case KEY_RELEASED:
          on_key_realease(uinput, event, nbr_event);
          break;
      }
    }
    //else
     // write(uinput, event, nbr_event * sizeof(t_event));
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
      if (!good_combo) // replay mod
      {
        if (1 /* !is_big_mod(uinput, event, nbr_event) */) {
          send_keyevent_press(uinput, virtual_mod, 0);              
          virtual_mod = NO_MOD;
          virtual_mod_consumed = 0;
          write(uinput, event, nbr_event * sizeof(t_event));
        }
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
  return 0;
}

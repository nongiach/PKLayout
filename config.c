#include "functions.h"

t_remap_mod_string remap_mod_string[] = {
  {"a", "Shift_L"},
  {"p", "Shift_R"},
  /* {"z", "Control_L"}, */
  /* {"o", "Control_R"}, */
  /* {"e", "Meta_L"}, */
  /* {"i", "Meta_R"}, */
  /* {"r", "XF86Launch5"}, */
  /* {"u", "XF86Launch5"}, */
};

int remap_mod_size = sizeof(remap_mod_string) / sizeof(t_remap_mod_string);

t_remap_mod_string hard_remap_string[] = {
  {"Escape", "Caps_Lock"},
  {"Caps_Lock", "Escape"}
};

int hard_remap_size = sizeof(hard_remap_string) / sizeof(t_remap_mod_string);
// f + uiop -> +-=_
// f + ,;:! -> @|&`
t_remap_string remap_string[] = {
  // disable the super mod for some key
  //  {{"r", "e"}, {"", ""}},

  {{"f", "j"}, {"", "parenleft"}}, // (
  {{"j", "f"}, {"", "parenright"}}, // )
  {{"f", "k"}, {"XF86Launch5", "parenleft"}}, // [
  {{"k", "f"}, {"XF86Launch5", "parenright"}}, // ]
  {{"f", "l"}, {"XF86Launch5", "apostrophe"}}, // {
  {{"l", "f"}, {"XF86Launch5", "equal"}}, // } 
  {{"f", "m"}, {"", "less"}}, // <
  {{"m", "f"}, {"Shift_L", "less"}}, // >

  {{"d", "m"}, {"", "equal"}}, // =
  {{"m", "d"}, {"", "underscore"}}, // _
  {{"d", "l"}, {"Shift_L", "equal"}}, // +
  {{"l", "d"}, {"", "minus"}}, // -

  {{"s", "j"}, {"", "Left"}}, // 
  {{"s", "k"}, {"", "Down"}}, // 
  {{"s", "l"}, {"", "Up"}}, // 
  {{"s", "m"}, {"", "Right"}}, // 

  {{"z", "j"}, {"Control_L", "Left"}}, // 
  {{"z", "k"}, {"Control_L", "Down"}}, // 
  {{"z", "l"}, {"Control_L", "Up"}}, // 
  {{"z", "m"}, {"Control_L", "Right"}}, // 

  {{"space", "q"}, {"Shift_L", "1"}}, // 1
  {{"space", "s"}, {"Shift_L", "2"}}, // 2
  {{"space", "d"}, {"Shift_L", "3"}}, // 3
  {{"space", "f"}, {"Shift_L", "4"}}, // 4
  {{"space", "g"}, {"Shift_L", "5"}}, // 5
  {{"space", "h"}, {"Shift_L", "6"}}, // 6
  {{"space", "j"}, {"Shift_L", "7"}}, // 7
  {{"space", "k"}, {"Shift_L", "8"}}, // 8
  {{"space", "l"}, {"Shift_L", "9"}}, // 9
  {{"space", "m"}, {"Shift_L", "0"}}, // 0

  //  {{"space", "q"}, {"Shift_L", "ampersand"}}, // 1
  //  {{"space", "s"}, {"Shift_L", "eacute"}}, // 2
  //  {{"space", "d"}, {"Shift_L", "quotedbl"}}, // 3
  //  {{"space", "f"}, {"Shift_L", "apostrophe"}}, // 4
  //  {{"space", "g"}, {"Shift_L", "parenleft"}}, // 5
  //  {{"space", "h"}, {"Shift_L", "minus"}}, // 6
  //  {{"space", "j"}, {"Shift_L", "egrave"}}, // 7
  //  {{"space", "k"}, {"Shift_L", "underscore"}}, // 8
  //  {{"space", "l"}, {"Shift_L", "ccedilla"}}, // 9
  //  {{"space", "m"}, {"Shift_L", "agrave"}}, // 0

//  {{"t", "j"}, {"Shift_L", "1"}}, // test 
};

#define REMAP_SIZE sizeof(remap_string) / sizeof(t_remap_string)
int remap_size = REMAP_SIZE;

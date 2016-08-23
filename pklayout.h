#ifndef PKLAYOUT_H
#define PKLAYOUT_H value

typedef struct s_combo_string {
  char mod[16];
  char key[16];
} t_combo_string;

typedef struct s_remap_string {
  t_combo_string from;
  t_combo_string to;
} t_remap_string;

t_remap_string remap_string[] = {
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

  {{"space", "q"}, {"Shift_L", "ampersand"}}, // 1
  {{"space", "s"}, {"Shift_L", "eacute"}}, // 2
  {{"space", "d"}, {"Shift_L", "quotedbl"}}, // 3
  {{"space", "f"}, {"Shift_L", "apostrophe"}}, // 4
  {{"space", "g"}, {"Shift_L", "parenleft"}}, // 5
  {{"space", "h"}, {"Shift_L", "minus"}}, // 6
  {{"space", "j"}, {"Shift_L", "egrave"}}, // 7
  {{"space", "k"}, {"Shift_L", "underscore"}}, // 8
  {{"space", "l"}, {"Shift_L", "ccedilla"}}, // 9
  {{"space", "m"}, {"Shift_L", "agrave"}}, // 0

  /* {{"space", "a"}, {"Shift_L", "ampersand"}}, // 1 */
  /* {{"space", "z"}, {"Shift_L", "eacute"}}, // 2 */
  /* {{"space", "e"}, {"Shift_L", "quotedbl"}}, // 3 */
  /* {{"space", "r"}, {"Shift_L", "apostrophe"}}, // 4 */
  /* {{"space", "t"}, {"Shift_L", "parenleft"}}, // 5 */
  /* {{"space", "y"}, {"Shift_L", "minus"}}, // 6 */
  /* {{"space", "u"}, {"Shift_L", "egrave"}}, // 7 */
  /* {{"space", "i"}, {"Shift_L", "underscore"}}, // 8 */
  /* {{"space", "o"}, {"Shift_L", "ccedilla"}}, // 9 */
  /* {{"space", "p"}, {"Shift_L", "agrave"}}, // 0 */
};

typedef struct s_combo {
  unsigned short mod;
  unsigned short key;
} t_combo;

typedef struct s_remap {
  t_combo from;
  t_combo to;
} t_remap;

#define REMAP_SIZE sizeof(remap_string) / sizeof(t_remap_string)
int remap_size = REMAP_SIZE;
t_remap remap[512];

#endif /* ifndef PKLAYOUT_H */

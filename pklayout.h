#ifndef PKLAYOUT_H
#define PKLAYOUT_H value

typedef struct s_remap_mod_string {
  char from[16];
  char to[16];
} t_remap_mod_string;

extern t_remap_mod_string remap_mod_string[];

typedef struct s_remap_mod {
  int from;
  int to;
} t_remap_mod;

extern t_remap_mod remap_mod[255];

extern int remap_mod_size; 

typedef struct s_combo_string {
  char mod[16];
  char key[16];
} t_combo_string;

typedef struct s_remap_string {
  t_combo_string from;
  t_combo_string to;
} t_remap_string;


typedef struct s_combo {
  unsigned short mod;
  unsigned short key;
} t_combo;

typedef struct s_remap {
  t_combo from;
  t_combo to;
} t_remap;


extern t_remap_string remap_string[];
extern int remap_size;
t_remap remap[512];

extern t_remap_mod_string hard_remap[];

char is_mod[255];
#define KEY_RELEASED 0
#define KEY_PUSHED 1
#define KEY_PRESSED 2

extern Display *ctrl_disp;

typedef struct input_event t_event;

#define K_VALUE event[0].value
#define K_STATE event[1].value

typedef struct s_keyboard {
  void (*state)(int , struct s_keyboard *, t_event *, int ); 
  t_combo *combo;
  int mod;
  int key;
} t_keyboard;

void s_wait_mod(int uinput, t_keyboard *kb, t_event *event, int nbr_event); 
#endif /* ifndef PKLAYOUT_H */

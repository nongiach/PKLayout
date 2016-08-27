prefix := /usr/local
sbindir := $(prefix)/bin

VER="0.01"
	
CC       = gcc
CFLAGS   += -O2 -DVERSION=\"$(VER)\" -W -Wall -Wextra -Wmissing-braces

ifdef deb
    CFLAGS += -g -Ddeb=
endif

LDFLAGS  += -lX11

NAME	= PKLayout

SRC	= init.c\
			pklayout.c\
			key_utils.c\
			config.c\
			handle_key.c

OBJ	= $(SRC:%.c=obj/%.o)

all: $(NAME)

obj/%.o : %.c
	mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@ $(LDFLAGS)

$(NAME):$(OBJ)
	$(CC) -o $(NAME) $(LDFLAGS) $(OBJ) $(CFLAGS)

install: all
	install -D -m755 PKLayout $(sbindir)/PKLayout

clean:
	$(RM) $(OBJ)
	$(RM) -r obj

fclean: clean
	$(RM) $(NAME)

re: fclean all

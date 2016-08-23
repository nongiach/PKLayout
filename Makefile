prefix := /usr/local
sbindir := $(prefix)/bin

DEBUG :=
CFLAGS := -lX11 -O2 -Wall $(DEBUG)

VER="0.01"

all: PKLayout

PKLayout: pklayout.c
	$(CC) $(CFLAGS) -DVERSION=\"$(VER)\" $< -o $@

install: all
	install -D -m755 PKLayout $(sbindir)/PKLayout

re: clean all

clean:
	rm -f *.o PKLayout

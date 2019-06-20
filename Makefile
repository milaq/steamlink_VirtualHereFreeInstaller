# Source setenv.sh before running make

CFLAGS := -I$(MARVELL_ROOTFS)/usr/include/SDL2
LDFLAGS := -lSDL2 -lSDL2_ttf -lpthread

vhf_installer: vhf_installer.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	$(RM) vhf_installer.o

distclean: clean
	$(RM) vhf_installer
	$(RM) -r steamlink

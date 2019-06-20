# Source setenv.sh before running make

CFLAGS := -I$(MARVELL_ROOTFS)/usr/include/SDL2
LDFLAGS := -lSDL2 -lSDL2_ttf

vhf_installer_screen: vhf_installer_screen.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	$(RM) vhf_installer_screen.o

distclean: clean
	$(RM) vhf_installer_screen
	$(RM) -r steamlink

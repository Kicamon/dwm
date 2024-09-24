# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

all: check options dwm dwm_statusbar

check:
	@ [ -f "config.h" ] || echo -e "\033[31mconfig.h not found, please run 'cp DEF/config.h .'\033[0m"
	@ [ -d "scripts" ] || echo -e "\033[31mautostart.sh not found, please run 'cp DEF/autostart.sh .'\033[0m"
	@ ([ -f "config.h" ] && [ -d "scripts" ]) || exit 1

options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

dwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

dwm_statusbar: ./statusbar/statusbar.c
	${CC} -o dwm_statusbar ./statusbar/statusbar.c ${LDFLAGS}

clean:
	rm -f dwm dwm_statusbar ${OBJ} dwm-${VERSION}.tar.gz

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwm ${DESTDIR}${PREFIX}/bin
	cp -f dwm_statusbar ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm_statusbar
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1
	rm -f dwm dwm_statusbar ${OBJ} dwm-${VERSION}.tar.gz

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${PREFIX}/bin/dwm_statusbar\
		${DESTDIR}${MANPREFIX}/man1/dwm.1

.PHONY: all check options clean install uninstall

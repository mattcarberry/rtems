include $(top_srcdir)/automake/compile.am

SUBDIRS = @libbsp_cpu_subdir@
DIST_SUBDIRS = @libbsp_cpu_subdir@

EXTRA_DIST = MERGE.PROCEDURE bsp.am

# shared
EXTRA_DIST += shared/bootcard.c shared/bspclean.c \
    shared/bsplibc.c shared/bsppost.c shared/console-polled.c \
    shared/console.c shared/gnatinstallhandler.c shared/sbrk.c \
    shared/tod.c
EXTRA_DIST += shared/vmeUniverse/vmeUniverse.c \
    shared/vmeUniverse/vmeUniverse.h \
    shared/vmeUniverse/vmeUniverseDMA.h \
    shared/vmeUniverse/vmeTsi148.c	\
    shared/vmeUniverse/vmeTsi148.h	\
    shared/vmeUniverse/vmeTsi148DMA.h	\
    shared/vmeUniverse/vme_am_defs.h \
    shared/vmeUniverse/VME.h \
    shared/vmeUniverse/VMEDMA.h \
    shared/vmeUniverse/bspVmeDmaList.c \
    shared/vmeUniverse/bspVmeDmaList.h \
    shared/vmeUniverse/bspVmeDmaListP.h \
    shared/vmeUniverse/README.porting \
    shared/vmeUniverse/README.universe

EXTRA_DIST += shared/include/coverhd.h
EXTRA_DIST += shared/gdbstub/rtems-stub-glue.c
EXTRA_DIST += shared/include/bootcard.h

include_bspdir = $(includedir)/bsp

include_bsp_HEADERS =
include_bsp_HEADERS += shared/include/default-initial-extension.h
include_bsp_HEADERS += shared/include/fatal.h
include_bsp_HEADERS += shared/include/console-termios.h
include_bsp_HEADERS += shared/include/gpio.h

include $(srcdir)/preinstall.am
include $(top_srcdir)/automake/subdirs.am
include $(top_srcdir)/automake/local.am

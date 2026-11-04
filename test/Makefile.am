## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = no-dependencies

EXTRA_DIST = notest.c

noinst_PROGRAMS = wtest wm_fsm_test

check_PROGRAMS = icon_order_test
TESTS = icon_order_test

icon_order_test_SOURCES = icon_order_test.c $(top_srcdir)/src/iconorder.c

icon_order_test_CPPFLAGS = -I$(top_srcdir)/src

wtest_SOURCES = wtest.c

wtest_LDADD = $(top_builddir)/wmlib/libWMaker.la @XLFLAGS@ @XLIBS@

AM_CPPFLAGS = -g -D_BSD_SOURCE @XCFLAGS@ -I$(top_srcdir)/wmlib

wm_fsm_test_SOURCES = wm_fsm_test.c

wm_fsm_test_CPPFLAGS = $(shell pkg-config --cflags gtk+-3.0)

wm_fsm_test_LDFLAGS = $(shell pkg-config --libs gtk+-3.0) -lX11

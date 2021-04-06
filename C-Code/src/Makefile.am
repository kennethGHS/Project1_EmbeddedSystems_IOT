AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = gpiomanager
gpiomanager_SOURCES = main.c
gpiomanager_LDADD = $(top_builddir)/lib/librpigpio.la -lrt -lpthread -lcurl -ljson-c

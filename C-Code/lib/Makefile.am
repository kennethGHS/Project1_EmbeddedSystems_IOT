lib_LTLIBRARIES = librpigpio.la
include_HEADERS = $(top_srcdir)/include/HTTPRequests.h $(top_srcdir)/include/ManualGPIO.h $(top_srcdir)/include/ImageGet.h $(top_srcdir)/include/JsonAdmin.h $(top_srcdir)/include/Monitoring.h
AM_CPPFLAGS = -I$(top_srcdir)/include
librpigpio_la_SOURCES = HTTPRequests.c ManualGPIO.c ImageGet.c JsonAdmin.c Monitoring.c
librpigpio_la_LIBADD = -lrt -lpthread -lcurl -ljson-c

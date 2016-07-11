AM_CPPFLAGS = -I$(top_srcdir)/libevent -I$(top_srcdir)/libevent/compat/sys -I$(top_srcdir)/src

bin_PROGRAMS=trader_mduser_svr

trader_mduser_svr_SOURCES=trader_mduser_svr.c
trader_mduser_svr_LDADD=-L$(top_srcdir)/libevent -L$(top_srcdir)/src/log -L$(top_srcdir)/src/util
trader_mduser_svr_LDADD+=-L$(top_srcdir)/src/api -L$(top_srcdir)/src/wrap -L$(top_srcdir)/lts -L$(top_srcdir)/src/cfg
trader_mduser_svr_LDADD+=-lutil -llog -levent_core -lmduser -lltsmduserapi -lstdc++ -lpthread -lsecuritymduserapi -lcfg



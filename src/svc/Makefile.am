AM_CPPFLAGS = -I$(top_srcdir)/libevent -I$(top_srcdir)/libevent/compat/sys -I$(top_srcdir)/src -I$(top_srcdir)/hiredis -DCTP
AM_CPPFLAGS += -I$(top_srcdir)/src/cfg -I$(top_srcdir)/src/log -I$(top_srcdir)/src/api -I$(top_srcdir)/src/util

bin_PROGRAMS=trader_mduser_svr

trader_mduser_svr_SOURCES=trader_mduser_svr.c
trader_mduser_svr_SOURCES+=trader_mduser_boardcast.c
trader_mduser_svr_SOURCES+=trader_mduser_cnn.c
trader_mduser_svr_LDADD=-L$(top_srcdir)/libevent -L$(top_srcdir)/hiredis -L$(top_srcdir)/src/log -L$(top_srcdir)/src/util 
trader_mduser_svr_LDADD+=-L$(top_srcdir)/src/api -L$(top_srcdir)/src/cfg
trader_mduser_svr_LDADD+=-L$(top_srcdir)/lts
trader_mduser_svr_LDADD+=-lltsmduser -lsecuritymduserapi
trader_mduser_svr_LDADD+=-L$(top_srcdir)/ctp
trader_mduser_svr_LDADD+=-lctpmduser -lthostmduserapi
trader_mduser_svr_LDADD+=-L$(top_srcdir)/femas
trader_mduser_svr_LDADD+=-lfemasmduser -lUSTPmduserapi
trader_mduser_svr_LDADD+=-lutil -llog -levent_core -lmduserapi -lstdc++ -lpthread -lcfg -lhiredis



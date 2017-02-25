AM_CPPFLAGS = -I$(top_srcdir)/libevent -I$(top_srcdir)/libevent/compat/sys -I$(top_srcdir)/src -I$(top_srcdir)/hiredis -DCTP
AM_CPPFLAGS += -I$(top_srcdir)/src/cfg -I$(top_srcdir)/src/log -I$(top_srcdir)/src/api -I$(top_srcdir)/src/util

bin_PROGRAMS=traderMduserSvr

traderMduserSvr_SOURCES=trader_mduser_svr.c
traderMduserSvr_SOURCES+=trader_mduser_boardcast.c
traderMduserSvr_SOURCES+=trader_mduser_cnn.c
traderMduserSvr_LDADD=-L$(top_srcdir)/libevent -L$(top_srcdir)/hiredis -L$(top_srcdir)/src/log -L$(top_srcdir)/src/util 
traderMduserSvr_LDADD+=-L$(top_srcdir)/src/api -L$(top_srcdir)/src/cfg
traderMduserSvr_LDADD+=-L$(top_srcdir)/lts
traderMduserSvr_LDADD+=-lltsmduser -lsecuritymduserapi
traderMduserSvr_LDADD+=-L$(top_srcdir)/ctp
traderMduserSvr_LDADD+=-lctpmduser -lthostmduserapi
traderMduserSvr_LDADD+=-L$(top_srcdir)/femas
traderMduserSvr_LDADD+=-lfemasmduser -lUSTPmduserapi
traderMduserSvr_LDADD+=-lutil -llog -levent_core -lmduserapi -lstdc++ -lpthread -lcfg -lhiredis



AM_CPPFLAGS = -I$(top_srcdir)/libevent -I$(top_srcdir)/libevent/compat/sys -I$(top_srcdir)/src -I$(top_srcdir)/src/api

bin_PROGRAMS= trader_mduser_test trader_trader_test

trader_mduser_test_SOURCES=trader_mduser_test.c
trader_mduser_test_LDADD=-L$(top_srcdir)/libevent -L$(top_srcdir)/src/log -L$(top_srcdir)/src/util
trader_mduser_test_LDADD+=-L$(top_srcdir)/src/api -L$(top_srcdir)/src/wrap -L$(top_srcdir)/lts -L$(top_srcdir)/src/cfg
trader_mduser_test_LDADD+=-lutil -llog -levent_core -lmduser -lltsmduserapi -lstdc++ -lpthread -lsecuritymduserapi -lcfg

trader_trader_test_SOURCES=trader_trader_test.c
trader_trader_test_LDADD=
trader_trader_test_LDADD=-L$(top_srcdir)/libevent -L$(top_srcdir)/src/log -L$(top_srcdir)/src/util
trader_trader_test_LDADD+=-L$(top_srcdir)/src/api -L$(top_srcdir)/src/wrap -L$(top_srcdir)/lts -L$(top_srcdir)/src/cfg
trader_trader_test_LDADD+=-lutil -llog -levent_core -ltrader -lltstraderapi -lstdc++ -lpthread -lsecuritytraderapi 
trader_trader_test_LDADD+=-lsecurityqueryapi -lcfg 

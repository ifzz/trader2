AM_CPPFLAGS = -I$(top_srcdir)/libevent -I$(top_srcdir)/libevent/compat/sys -I$(top_srcdir)/src -I$(top_srcdir)/src/api

bin_PROGRAMS= trader_mduser_test trader_trader_test

trader_mduser_test_SOURCES=trader_mduser_test.c
trader_mduser_test_LDADD=-L$(top_srcdir)/libevent -L$(top_srcdir)/src/log -L$(top_srcdir)/src/util
trader_mduser_test_LDADD+=-L$(top_srcdir)/src/api -L$(top_srcdir)/src/cfg
trader_mduser_test_LDADD+=-L$(top_srcdir)/lts
trader_mduser_test_LDADD+=-lltsmduser -lsecuritymduserapi
trader_mduser_test_LDADD+=-L$(top_srcdir)/ctp
trader_mduser_test_LDADD+=-lctpmduser -lthostmduserapi
trader_mduser_test_LDADD+=-L$(top_srcdir)/femas 
trader_mduser_test_LDADD+=-lfemasmduser -lUSTPmduserapi
trader_mduser_test_LDADD+=-lutil -llog -levent_core -lmduserapi -lstdc++ -lpthread -lcfg

trader_trader_test_SOURCES=trader_trader_test.c
trader_trader_test_LDADD=
trader_trader_test_LDADD=-L$(top_srcdir)/libevent -L$(top_srcdir)/src/log -L$(top_srcdir)/src/util
trader_trader_test_LDADD+=-L$(top_srcdir)/src/api -L$(top_srcdir)/lts -L$(top_srcdir)/src/cfg
trader_trader_test_LDADD+=-L$(top_srcdir)/lts
trader_trader_test_LDADD+=-lltstrader -lsecuritytraderapi -lsecurityqueryapi
trader_trader_test_LDADD+=-L$(top_srcdir)/ctp
trader_trader_test_LDADD+=-lctptrader -lthosttraderapi
trader_trader_test_LDADD+=-L$(top_srcdir)/femas 
trader_trader_test_LDADD+=-lfemastrader -lUSTPtraderapi
trader_trader_test_LDADD+=-lutil -llog -levent_core -ltraderapi -lstdc++ -lpthread -lcfg

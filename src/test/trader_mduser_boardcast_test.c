
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <sys/time.h>

#include <event2/util.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "trader_mduser_boardcast.h"


typedef struct trader_mduser_boardcast_test_def trader_mduser_boardcast_test;
struct trader_mduser_boardcast_test_def {

  struct event_base *pBase;
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;
  
  struct event* pTimer;

  trader_mduser_boardcast* pApi;

};


static void boardcast_test_signal_cb(evutil_socket_t fd, short event, void *arg);

static void boardcast_test_timer_cb(evutil_socket_t fd, short event, void *arg);

void boardcast_test_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_boardcast_test* test = (trader_mduser_boardcast_test*)arg;
  struct timeval tv = {
    2, 0
  };
  event_base_loopexit(test->pBase, &tv);
}

void boardcast_test_timer_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_boardcast_test* test = (trader_mduser_boardcast_test*)arg;

  test->pApi->method->xBoardcase(test->pApi, "0000", 4);
  struct timeval tv;
  tv.tv_usec = 0;
  tv.tv_sec = 5;
  evtimer_add(test->pTimer, &tv);
}



int main(int argc, char* argv[])
{
  trader_mduser_boardcast_test trader_mduser_boardcast_test_st;
  trader_mduser_boardcast_test* test = &trader_mduser_boardcast_test_st;

  test->pBase = event_base_new();

  test->pSigIntEvt = evsignal_new(test->pBase, SIGINT, boardcast_test_signal_cb, (void *)test);
  test->pSigTermEvt = evsignal_new(test->pBase, SIGTERM, boardcast_test_signal_cb, (void *)test);

  test->pTimer = evtimer_new(test->pBase, boardcast_test_timer_cb, (void *)test);
  
  test->pApi = trader_mduser_boardcast_new();

  test->pApi->method->xInit(test->pApi, test->pBase, "0.0.0.0", 22345);

	struct timeval tv;
	tv.tv_usec = 0;
	tv.tv_sec = 5;
	evtimer_add(test->pTimer, &tv);

  event_base_dispatch(test->pBase);

  test->pApi->method->xExit(test->pApi);

  event_free(test->pTimer);
  event_free(test->pSigTermEvt);
  event_free(test->pSigIntEvt);

  trader_mduser_boardcast_free(test->pApi);

  return 0;
}



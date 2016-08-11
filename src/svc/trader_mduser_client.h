
#ifndef _TRADER_MDUSER_CLIENT_H_
#define _TRADER_MDUSER_CLIENT_H_

typedef struct trader_mduser_client_def trader_mduser_client;
typedef struct trader_mduser_client_method_def trader_mduser_client_method;

typedef void trader_mduser_client_connect_callback(trader_mduser_client* self);
typedef void trader_mduser_client_disconnect_callback(trader_mduser_client* self);
typedef void trader_mduser_client_recv_callback(trader_mduser_client* self, void* data, int len);

struct trader_mduser_client_def {
  

};

struct trader_mduser_client_method_def {

};

extern trader_mduser_client* trader_mduser_client_new();
extern void trader_mduser_client_free(trader_mduser_client* self);


#endif //_TRADER_MDUSER_CLIENT_H_


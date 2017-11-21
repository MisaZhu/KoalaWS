#include "JSWebSocket.h"
#include <cstdlib>
#include <uWS/uWS.h>

using namespace std;

uWS::Hub* _getWSHub(BCVar* var) {
	BCVar* thisV = var->getParameter(THIS);
	if(thisV == NULL)
		return NULL;
	BCNode* n = thisV->getChild("wsHub");
	if(n == NULL)
		return NULL;
	return (uWS::Hub*)n->var->getPoint();
}

#define GET_WSHUB uWS::Hub* wsHub = _getWSHub(c); \
		if(wsHub == NULL) \
			return;

static void _destroyWSHub(void* p) {
	if(p != NULL) {
		uWS::Hub* wsHub = (uWS::Hub*)p;
		delete wsHub;
	}
}

void JSWebSocketHub::constructor(KoalaJS* js, BCVar *c, void *) {
	BCVar* thisV = c->getParameter(THIS);
	if(thisV == NULL)
		return;
	uWS::Hub* wsHub = new uWS::Hub();

	BCVar* v = new BCVar();
	v->setPoint(wsHub, NO_BYTES, _destroyWSHub, true);
	thisV->addChild("wsHub", v);

	wsHub->onError([](void* p) {
		KoalaJS* js = (KoalaJS*)p;
		Interrupter* inter = js->getInterrupter();
		inter->interrupt("_onWSHubError", 0);
	});

	wsHub->onConnection([](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
		KoalaJS* js = (KoalaJS*)ws->getUserData();
		Interrupter* inter = js->getInterrupter();
		BCVar* w = js->newObject("RWebSocket");
		w->addChild("wsocket", new BCVar(ws, NO_BYTES, NULL, false));
		inter->interrupt("_onWSHubConnection", 1, w);
	});

	wsHub->onDisconnection([](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
		KoalaJS* js = (KoalaJS*)ws->getUserData();
		Interrupter* inter = js->getInterrupter();
		BCVar* w = js->newObject("RWebSocket");
		w->addChild("wsocket", new BCVar(ws, NO_BYTES, NULL, false));

		BCVar* cd = new BCVar(code);
		BCVar* bytes = new BCVar(message, (int)length, NULL, false);
		inter->interrupt("_onWSHubDisconnection", 3, w, cd, bytes);
	});

	wsHub->onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
		KoalaJS* js = (KoalaJS*)ws->getUserData();
		Interrupter* inter = js->getInterrupter();
		BCVar* w = js->newObject("RWebSocket");
		w->addChild("wsocket", new BCVar(ws, NO_BYTES, NULL, false));

		BCVar* cd = new BCVar((int)opCode);
		BCVar* bytes = new BCVar(message, (int)length, NULL, false);
		inter->interrupt("_onWSHubMessage", 3, w, cd, bytes);
	
	});

	c->setReturnVar(thisV);
}


void JSWebSocketHub::connect(KoalaJS* js, BCVar *c, void *) {
	string uri = c->getParameter("uri")->getString();
	GET_WSHUB
	wsHub->connect(uri.c_str(), js);
}

void JSWebSocketHub::run(KoalaJS* js, BCVar *c, void *) {
	GET_WSHUB
	wsHub->run();
}

typedef uWS::WebSocket<uWS::CLIENT> WSClient;

WSClient * _getWS(BCVar* var) {
	BCVar* thisV = var->getParameter(THIS);
	if(thisV == NULL)
		return NULL;
	BCNode* n = thisV->getChild("wsocket");
	if(n == NULL)
		return NULL;
	return (WSClient*)n->var->getPoint();
}

#define GET_WS WSClient* ws = _getWS(c); \
		if(ws == NULL) \
			return;

void JSWebSocket::close(KoalaJS* js, BCVar *c, void *) {
	GET_WS
	ws->close();
}


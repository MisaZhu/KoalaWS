#include "JSWebSocket.h"
#include <cstdlib>
#include <pthread.h>
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

#define PRE_HANDLE	KoalaJS* js = (KoalaJS*)ws->getUserData(); \
		Interrupter* inter = js->getInterrupter();

void JSWebSocketHub::constructor(KoalaJS* js, BCVar *c, void *) {
	BCVar* thisV = c->getParameter(THIS);
	uWS::Hub* wsHub = new uWS::Hub();

	BCVar* v = new BCVar();
	v->setPoint(wsHub, NO_BYTES, _destroyWSHub, true);
	thisV->addChild("wsHub", v);

	wsHub->onError([thisV](void* p) {
		KoalaJS* js = (KoalaJS*)p;
		Interrupter* inter = js->getInterrupter();
		inter->interrupt(thisV, "onError", 0);
		//TRACE("WSHub error!\n");
	});

	wsHub->onConnection([thisV](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
		PRE_HANDLE
		BCVar* w = js->newObject("RWebSocketClient");
		w->addChild("wsocket", new BCVar(ws, NO_BYTES, NULL, false));
		inter->interrupt(thisV, "onConnection", 1, w);
		//TRACE("WSHub connection.\n");
	});

	wsHub->onDisconnection([thisV](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
		PRE_HANDLE
		BCVar* w = js->newObject("RWebSocketClient");
		w->addChild("wsocket", new BCVar(ws, NO_BYTES, NULL, false));

		BCVar* cd = new BCVar(code);
		BCVar* bytes = js->newObject("Bytes");
		bytes->setPoint(message, (int)length, NULL, false);
		inter->interrupt(thisV, "onDisconnection", 3, w, cd, bytes);
		//TRACE("WSHub disconnection.\n");
	});

	wsHub->onMessage([thisV](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
		PRE_HANDLE
		BCVar* w = js->newObject("RWebSocketClient");
		w->addChild("wsocket", new BCVar(ws, NO_BYTES, NULL, false));

		BCVar* cd = new BCVar((int)opCode);
		BCVar* bytes = js->newObject("Bytes");
		bytes->setPoint(message, (int)length, NULL, false);
		inter->interrupt(thisV, "onMessage", 3, w, cd, bytes);
		//TRACE("WSHub message.\n");
	});

	wsHub->onPing([thisV](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length) {
		PRE_HANDLE
		BCVar* w = js->newObject("RWebSocketClient");
		w->addChild("wsocket", new BCVar(ws, NO_BYTES, NULL, false));

		BCVar* bytes = js->newObject("Bytes");
		bytes->setPoint(message, (int)length, NULL, false);
		inter->interrupt(thisV, "onPing", 2, w, bytes);
		//TRACE("WSHub ping.\n");
	});

	wsHub->onPong([thisV](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length) {
		PRE_HANDLE
		BCVar* w = js->newObject("RWebSocketClient");
		w->addChild("wsocket", new BCVar(ws, NO_BYTES, NULL, false));

		BCVar* bytes = js->newObject("Bytes");
		bytes->setPoint(message, (int)length, NULL, false);
		inter->interrupt(thisV, "onPong", 2, w, bytes);
		//TRACE("WSHub pong.\n");
	});

	c->setReturnVar(thisV);
}


void JSWebSocketHub::connect(KoalaJS* js, BCVar *c, void *) {
	string uri = c->getParameter("uri")->getString();
	GET_WSHUB
	wsHub->connect(uri.c_str(), js);
}

void* _WSHubThread(void* p) {
	uWS::Hub* wsHub = (uWS::Hub*)p;
	pthread_detach(pthread_self());
	wsHub->run();
	return NULL;
}

void JSWebSocketHub::run(KoalaJS* js, BCVar *c, void *) {
	GET_WSHUB
	pthread_t t;
	pthread_create(&t, NULL, _WSHubThread, wsHub);
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

void JSWebSocketClient::close(KoalaJS* js, BCVar *c, void *) {
	GET_WS
	ws->close();
}

void JSWebSocketClient::send(KoalaJS* js, BCVar *c, void *) {
	GET_WS
	BCVar* v = c->getParameter("buf");
	BCVar* sv = c->getParameter("size");
	
	int size = 0;	
	if(sv != NULL)
		size = sv->getInt();

	const char* p = NULL;
	std::string s;
	if(v->isBytes()) {
		p = (const char*)v->getPoint();
		if(size == 0 || size > v->getInt())
			size = v->getInt();
	}
	else {
		s = v->getString();
		p = (const char*)s.c_str();
		if(size == 0 || size > (int)s.length())
			size = (int)s.length();
	}

	if(p != NULL) {
		ws->send(p, size, uWS::OpCode::BINARY);
	}
}

void JSWebSocketClient::ping(KoalaJS* js, BCVar *c, void *) {
	GET_WS
	BCVar* v = c->getParameter("buf");
	
	int size = 0;	
	const char* p = NULL;
	std::string s;
	if(v->isBytes()) {
		p = (const char*)v->getPoint();
		if(size == 0 || size > v->getInt())
			size = v->getInt();
	}
	else {
		s = v->getString();
		p = (const char*)s.c_str();
		if(size == 0 || size > (int)s.length())
			size = (int)s.length();
	}

	if(p != NULL) {
		ws->send(p, size, uWS::OpCode::PING);
	}
}


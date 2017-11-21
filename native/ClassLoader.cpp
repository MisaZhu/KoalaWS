#include "native/JSWebSocket.h"

extern "C" void _moduleLoader(KoalaJS* js) {
	JSWebSocketHub::instance().load(js, "RWebSocketHub");
	JSWebSocket::instance().load(js, "RWebSocket");
}



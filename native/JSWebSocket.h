#ifndef JSM_JSWebSocket
#define JSM_JSWebSocket

#include "ClassLoader.h"

class JSWebSocketHub : public JSClass {
	static void connect(KoalaJS* js, BCVar *c, void *);
	static void constructor(KoalaJS* js, BCVar *c, void *);
	static void run(KoalaJS* js, BCVar *c, void *);

	protected:
	inline void registerFunctions(KoalaJS* js, const std::string& className) {
		addFunction(js, className, "constructor()", constructor, NULL);
		addFunction(js, className, "run()", run, NULL);
		addFunction(js, className, "connect(uri)", connect, NULL);
	}

	public:
	DECL_INSTANCE(JSWebSocketHub)
};

class JSWebSocket : public JSClass {
	static void close(KoalaJS* js, BCVar *c, void *);
	static void send(KoalaJS* js, BCVar *c, void *);

	protected:
	inline void registerFunctions(KoalaJS* js, const std::string& className) {
		addFunction(js, className, "close()", close, NULL);
		addFunction(js, className, "send(buf, size)", send, NULL);
		addFunction(js, className, "send(buf)", send, NULL);
	}

	public:
	DECL_INSTANCE(JSWebSocket)
};

#endif

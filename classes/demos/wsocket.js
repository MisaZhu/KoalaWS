include "../RWebSocket.js";

var _end = false;

var wshub = new RWebSocketHub();

wshub.onConnection = function(ws) {
	ws.send("hello");
};

wshub.onDisconnection = function(ws, code, bytes) {
	_end = true;
};

wshub.onMessage = function(ws, code, bytes) {
	println(bytes.toString());
	ws.close();
};

wshub.onError = function() {
	ws.close();
};

wshub.connect("ws://demos.kaazing.com/echo");
wshub.run();

while(!_end) {
	RThread.usleep(10000);
}

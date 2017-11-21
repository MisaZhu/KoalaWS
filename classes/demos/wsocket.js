include "../RWebSocket.js";

var wshub = new RWebSocketHub();
wshub.connect("ws://echo.websocket.org");

while(true) {
	RThread.usleep(10000);
}

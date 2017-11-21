include "../RWebSocket.js";

var _end = false;

var ws = new WebSocket();
ws.onConnection = function() {
	send("hello");
};

ws.onDisconnection = function(bytes) {
	_end = true;
};

ws.onMessage = function(bytes) {
	println(bytes.toString());
};

ws.onError = function() {
	close();
};

ws.onPing = function(bytes) {
	ws.ping("got");
};

ws.onPong = function(bytes) {
	close();
};

ws.connect("wss://demos.kaazing.com/echo");

while(!_end) {
	RThread.usleep(10000);
}

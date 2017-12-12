import "../RWebSocket.js";

var ws = new WebSocketClient();
ws.onConnection = function() {
	send("hello");
};

ws.onDisconnection = function(bytes) {
	RVM.terminate();
};

ws.onMessage = function(bytes) {
	console.log(bytes.toString());
};

ws.onError = function() {
	close();
};

ws.onPing = function(bytes) {
	ping("got");
};

ws.onPong = function(bytes) {
	close();
};

ws.connect("wss://demos.kaazing.com/echo");

while(!RVM.terminated()) {
	RThread.usleep(10000);
}

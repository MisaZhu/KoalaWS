RVM.loadExt("KoalaWS");

function _onWSHubConnection(ws) {
	println("connection: " + ws);
}

function _onWSHubError() {
	println("Error");
}

function _onWSHubDisconnection(ws, code, bytes) {
	println("disconnection: " + ws);
}

function _onWSHubMessage(ws, code, bytes) {
	println("Message: " + ws);
}

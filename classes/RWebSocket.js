RVM.loadExt("KoalaWS");

function _onWSHubConnection(wsHub, ws) {
	if(wsHub.onConnection != undefined) {
		wsHub.onConnection(ws);
	}
}

function _onWSHubError(wsHub) {
	if(wsHub.onError != undefined)
		wsHub.onError();
}

function _onWSHubDisconnection(wsHub, ws, code, bytes) {
	if(wsHub.onDisconnection != undefined)
		wsHub.onDisconnection(ws, code, bytes);
}

function _onWSHubMessage(wsHub, ws, code, bytes) {
	if(wsHub.onMessage != undefined)
		wsHub.onMessage(ws, code, bytes);
}


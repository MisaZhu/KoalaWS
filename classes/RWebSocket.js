RVM.loadExt("KoalaWS");

function _onWSHubConnection(wsHub, ws) {
	if(wsHub.onHubConnection != undefined) {
		wsHub.onHubConnection(ws);
	}
}

function _onWSHubError(wsHub) {
	if(wsHub.onHubError != undefined)
		wsHub.onHubError();
}

function _onWSHubDisconnection(wsHub, ws, code, bytes) {
	if(wsHub.onHubDisconnection != undefined)
		wsHub.onHubDisconnection(ws, code, bytes);
}

function _onWSHubMessage(wsHub, ws, code, bytes) {
	if(wsHub.onHubMessage != undefined)
		wsHub.onHubMessage(ws, code, bytes);
}

function _onWSHubPing(wsHub, ws, bytes) {
	if(wsHub.onHubPing != undefined)
		wsHub.onHubPing(ws, bytes);
}

function _onWSHubPong(wsHub, ws, bytes) {
	if(wsHub.onHubPong != undefined)
		wsHub.onHubPong(ws, bytes);
}

class WebSocketClient {
	constructor() {
		this.wsHub = new RWebSocketHub();
		wsHub._this = this;

		this.wsClient = undefined;
		this.onConnection = undefined;
		this.onDisconnection = undefined;
		this.onError = undefined;
		this.onMessage = undefined;
		this.onPing = undefined;
		this.onPong = undefined;

		wsHub.onHubConnection = function(ws) {
			_this.wsClient = ws;
			if(_this.onConnection != undefined)
				_this.onConnection();
		};
		
		wsHub.onHubDisconnection = function(ws, code, bytes) {
			_this.wsClient = ws;
			if(_this.onDisconnection != undefined)
				_this.onDisconnection(bytes);
		};	

		wsHub.onHubError = function() {
			if(_this.onError != undefined)
				_this.onError();
		};

		wsHub.onHubMessage = function(ws, code, bytes) {
			_this.wsClient = ws;
			if(_this.onMessage != undefined)
				_this.onMessage(bytes);
		};

		wsHub.onHubPing = function(ws, bytes) {
			_this.wsClient = ws;
			if(_this.onPing != undefined)
				_this.onPing(bytes);
		};

		wsHub.onHubPong = function(ws, bytes) {
			_this.wsClient = ws;
			if(_this.onPong != undefined)
				_this.onPong(bytes);
		};
	}

	connect(uri) {
		wsHub.connect(uri);
		wsHub.run();
	}

	close() {
		if(wsClient != undefined)
			wsClient.close();
		wsClient = undefined;
	}

	send(buf) {
		if(wsClient != undefined)
			wsClient.send(buf);
	}

	send(buf, size) {
		if(wsClient != undefined)
			wsClient.send(buf, size);
	}

	ping(buf) {
		if(wsClient != undefined)
			wsClient.ping(buf);
	}
}

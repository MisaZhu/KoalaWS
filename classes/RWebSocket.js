RVM.loadModule("KoalaWS");

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

		wsHub.onConnection = function(ws) {
			_this.wsClient = ws;
			if(_this.onConnection != undefined)
				_this.onConnection();
		};
		
		wsHub.onDisconnection = function(ws, code, bytes) {
			_this.wsClient = ws;
			if(_this.onDisconnection != undefined)
				_this.onDisconnection(bytes);
		};	

		wsHub.onError = function() {
			if(_this.onError != undefined)
				_this.onError();
		};

		wsHub.onMessage = function(ws, code, bytes) {
			_this.wsClient = ws;
			if(_this.onMessage != undefined)
				_this.onMessage(bytes);
		};

		wsHub.onPing = function(ws, bytes) {
			_this.wsClient = ws;
			if(_this.onPing != undefined)
				_this.onPing(bytes);
		};

		wsHub.onPong = function(ws, bytes) {
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


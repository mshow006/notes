openfire 属于 XMPP 服务器。

XMPP（Extensible Messaging and Presence Protocol）是一种网络即时通讯协议，它基于XML。

XMPP服务器和客户端之间，是通过XML节（XML Stanza）来进行通讯。其中有三种非常重要的XML Stanza类型：`<message>`、`<presence>`、`<iq>`。

- message

聊天消息的发送和接收就是通过message节来实现。例如xxg1@host发送一条信息”你好”给xxg2@host，xxg1@host客户端会将下面的这段XML发送到XMPP服务器，服务器再推送给xxg2@host客户端。其中`<message>`的from属性是发送者，to属性是接收者，`<body>`子元素的内容就是聊天信息。

```xml
<message from="xxg1@host" to="xxg2@host" type="chat">
    <body>你好</body>
</message>
```

- presence

可用于表明用户的状态，例如用户状态改变成“Do not disturb”（“请勿打扰”），会向服务器发送：

```xml
<presence from="xxg@host">
    <status>Do not disturb</status>
    <priority>0</priority>
    <show>dnd</show>
</presence>
```

- iq

iq即Info/Query，采用“请求-响应”机制，类似于HTTP的机制。下面的例子是客户端通过`<iq>`请求获取联系人，XMPP服务器将结果返回。

客户端请求获取联系人：
```xml
<iq from='xxg@host' id='bv1bs71f' type='get'>
    <query xmlns='jabber:iq:roster'/>
</iq>
```

服务器结果返回：
```xml
<iq to='xxg@host' id='bv1bs71f' type='result'>
    <query xmlns='jabber:iq:roster'>
        <item jid='xxg2@host'/>
        <item jid='xxg3@host'/>
    </query>
</iq>
```


实现web私聊

聊天信息是通过上面介绍的`<message>`来进行传递交换。例如接收到一条别人发来的聊天信息，即接收一个`<message>`元素，发送给别人一条聊天信息，即发送一个`<message>`元素。

```html
<!DOCTYPE html>
<html>
<head>
	<script src='http://cdn.bootcss.com/jquery/1.9.1/jquery.min.js'></script>
	<script src='http://cdn.bootcss.com/strophe.js/1.1.3/strophe.min.js'></script>
	<script src='test.js'></script>
</head>
<body>
	JID：<input type="text" id="input-jid">
	<br>
	密码：<input type="password" id="input-pwd">
	<br>
	<button id="btn-login">登录</button>
	<div id="msg" style="height: 400px; width: 400px; overflow: scroll;"></div>
	联系人JID：
	<input type="text" id="input-contacts">
	<br>
	消息：
	<br>
	<textarea id="input-msg" cols="30" rows="4"></textarea>
	<br>
	<button id="btn-send">发送</button>
</body>
</html>
```

```javascript
// XMPP服务器BOSH地址
var BOSH_SERVICE = 'http://host:5280';

// XMPP连接
var connection = null;

// 当前状态是否连接
var connected = false;

// 当前登录的JID
var jid = "";

// 连接状态改变的事件
function onConnect(status) {
	console.log(status)
    if (status == Strophe.Status.CONNFAIL) {
		alert("连接失败！");
    } else if (status == Strophe.Status.AUTHFAIL) {
		alert("登录失败！");
    } else if (status == Strophe.Status.DISCONNECTED) {
		alert("连接断开！");
		connected = false;
    } else if (status == Strophe.Status.CONNECTED) {
		alert("连接成功，可以开始聊天了！");
		connected = true;
		
		// 当接收到<message>节，调用onMessage回调函数
		connection.addHandler(onMessage, null, 'message', null, null, null);
		
		// 首先要发送一个<presence>给服务器（initial presence）
		connection.send($pres().tree());
    }
}

// 接收到<message>
function onMessage(msg) {
	
	// 解析出<message>的from、type属性，以及body子元素
    var from = msg.getAttribute('from');
    var type = msg.getAttribute('type');
    var elems = msg.getElementsByTagName('body');

    if (type == "chat" && elems.length > 0) {
		var body = elems[0];
		$("#msg").append(from + ":<br>" + Strophe.getText(body) + "<br>")
    }
    return true;
}

$(document).ready(function() {

	// 通过BOSH连接XMPP服务器
	$('#btn-login').click(function() {
		if(!connected) {
			connection = new Strophe.Connection(BOSH_SERVICE);
			connection.connect($("#input-jid").val(), $("#input-pwd").val(), onConnect);
			jid = $("#input-jid").val();
		}
	});
	
	// 发送消息
	$("#btn-send").click(function() {
		if(connected) {
			if($("#input-contacts").val() == '') {
				alert("请输入联系人！");
				return;
			}

			// 创建一个<message>元素并发送
			var msg = $msg({
				to: $("#input-contacts").val(), 
				from: jid, 
				type: 'chat'
			}).c("body", null, $("#input-msg").val());
			connection.send(msg.tree());

			$("#msg").append(jid + ":<br>" + $("#input-msg").val() + "<br>");
			$("#input-msg").val('');
		} else {
			alert("请先登录！");
		}
	});
});
```
//通过HTTP协议登录EPORTAL邮件系统
参数名：sUserId 用户ID。
参数名：sPassword 用户密码。
function logonEportal(sCertUrl,sUserId,sPassword)
{
	var strURL;
	var xhreq;
	xhreq = new ActiveXObject("Microsoft.XMLHTTP");
	strUrl =sCertUrl+"?userid="+sUserId;
	strUrl += "&pass="+sPassword;
	xhreq.open("POST", strUrl, false);
	xhreq.send();
	// If we got "OK" back, then consider the post successful.
	if("OK" == xhreq.statusText){
		//do something here.
	}
	else
		alert( xhreq.statusText );
}
//通过HTTP协议发送手机短信
参数名：MSGID
OA系统填写值：短消息编号（8位数字，每天不能重复）
参数名：SPID
OA系统填写值：JSCNC
参数名：APPID
OA系统填写值：OA
参数名：RECEIVER
OA系统填写值：接收方手机号码
参数名：SENDDATE
OA系统填写值：发送日期，格式（yyyymmdd）
参数名：SENDTIME
OA系统填写值：发送时间，格式（hhmmss）
参数名：CONTENTS
OA系统填写值：短消息内容（70个汉字或140个英文）
OA系统可以使用HTTP 协议应答的头域来检查执行结果。RET_CODE:   0   成功  -1  失败	  ERR_MSG 失败原因
function sendMMS(sSendUrl,sMsgId,sSpid,sAppId,sReceiver,sSendDate,sSendTime,sContents){
	var strURL;
	var xhreq;
	if(sSendUrl==null || sSendUrl==""){
		alert("短信息发送网关地址为空！")
		return false;
	}
	if(sMsgId==null || sMsgId==""){
		alert("短信息编号为空！");
		return false;
	}
	if(sReceiver==null || sReceiver==""){
		alert("请输入接收短信息的手机号码！");
		return false;
	}
	if(sContents==null || sContents==""){
		alert("短信息的内容部门为空！！");
		return false;
	}
	if(sAppId==null)
		sAppId="OA";
	if(sSpid==null)
		sSpid="JSCNC";
	xhreq = new ActiveXObject("Microsoft.XMLHTTP");
	strUrl =sSendUrl+"?MSGID="+sMsgId;
	strUrl += "&SPID="+sSpid;
	strUrl += "&APPID="+sAppId;
	strUrl += "&RECEIVER="+sReceiver;
	strUrl += "&SENDDATE="+sSendDate;
	strUrl += "&SENDTIME="+sSendTime;
	strUrl += "&CONTENTS="+sContents;
	xhreq.open("POST", strUrl, false);
	xhreq.send();
	// If we got "OK" back, then consider the post successful.
	if("OK" == xhreq.statusText){
		//do something here.
	}
	else
		alert( xhreq.statusText );
}
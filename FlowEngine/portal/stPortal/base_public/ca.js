function printStatus(status){
	var strStatus=status+"";
    switch(status){
    	case -2113667072:
		strStatus="装载动态库错误";
		break;
		case -2113667071:
		strStatus="内存分配错误";
		break;
		case -2113667070:
		strStatus="读私钥设备错误";
		break;
		case -2113667069:
		strStatus="私钥密码错误";
		break;
		case -2113667068:
		strStatus="读证书链设备错误";
		break;
		case -2113667067:
		strStatus="证书链密码错误";
		break;
		case -2113667066:
		strStatus="读证书设备错误";
		break;
		case -2113667065:
		strStatus="证书密码错误";
		break;
		case -2113667064:
		strStatus="私钥超时";
		break;
		case -2113667063:
		strStatus="缓冲区太小";
		break;
		case -2113667062:
		strStatus="初始化环境错误";
		break;
		case -2113667061:
		strStatus="清除环境错误";
		break;
		case -2113667060:
		strStatus="数字签名错误";
		break;
		case -2113667059:
		strStatus="验证签名错误";
		break;
		case -2113667058:
		strStatus="摘要错误";
		break;
		case -2113667057:
		strStatus="证书格式错误";
		break;
		case -2113667056:
		strStatus="数字信封错误";
		break;
		case -2113667055:
		strStatus="从LDAP获取证书错误";
		break;
		case -2113667054:
		strStatus="证书已过期";
		break;
		case -2113667053:
		strStatus="获取证书链错误";
		break;
		case -2113667052:
		strStatus="证书链格式错误";
		break;
		case -2113667051:
		strStatus="验证证书链错误";
		break;
		case -2113667050:
		strStatus="证书已废除";
		break;
		case -2113667049:
		strStatus="CRL格式错误";
		break;
		case -2113667048:
		strStatus="连接OCSP服务器错误";
		break;
		case -2113667047:
		strStatus="OCSP请求编码错误";
		break;
		case -2113667046:
		strStatus="OCSP回包错误";
		break;
		case -2113667045:
		strStatus="OCSP回包格式错误";
		break;
		case -2113667044:
		strStatus="OCSP回包过期";
		break;
		case -2113667043:
		strStatus="OCSP回包验证签名错误";
		break;
		case -2113667042:
		strStatus="证书状态未知";
		break;
		case -2113667041:
		strStatus="对称加解密错误";
		break;
		case -2113667040:
		strStatus="获取证书信息错误";
		break;
		case -2113667039:
		strStatus="获取证书细目错误";
		break;
		case -2113667038:
		strStatus="获取证书唯一标识错误";
		break;
		case -2113667037:
		strStatus="获取证书扩展项错误";
		break;
		case -2113667036:
		strStatus="PEM编码错误";
		break;
		case -2113667035:
		strStatus="PEM解码错误";
		break;
		case -2113667034:
		strStatus="产生随机数错误";
		break;
		case -2113667033:
		strStatus="PKCS12参数错误";
		break;
		case -2113667032:
		strStatus="私钥格式错误";
		break;
		case -2113667031:
		strStatus="公私钥不匹配";
		break;
		case -2113667030:
		strStatus="PKCS12编码错误";
		break;
		case -2113667029:
		strStatus="PKCS12格式错误";
		break;
		case -2113667028:
		strStatus="PKCS12解码错误";
		break;
		case -2113667027:
		strStatus="非对称加解密错误";
		break;
		case -2113667026:
		strStatus="OID格式错误";
		break;
		case -2113667025:
		strStatus="LDAP地址格式错误";
		break;
		case -2113667024:
		strStatus="LDAP地址错误";
		break;
		case -2113667023:
		strStatus="连接LDAP服务器错误";
		break;
		case -2113667022:
		strStatus="LDAP绑定错误";
		break;
		case -2113667021:
		strStatus="没有OID对应的扩展项";
		break;
		case -2113667020:
		strStatus="获取证书级别错误";
		break;
		case -2113667019:
		strStatus="读取配置文件错误";
		break;
		case -2113667018:
		strStatus="私钥未载入";
		break;
		case -2113666824:
		strStatus="无效的登录凭证";
		break;
		case -2113666823:
		strStatus="参数错误";
		break;
		case -2113666822:
		strStatus="不是服务器证书";
		break;
		case -2113666821:
		strStatus="登录错误";
		break;
		case -2113666820:
		strStatus="证书验证方式错误";
		break;
		case -2113666819:
		strStatus="随机数验证错误";
		break;
		case -2113666818:
		strStatus="与单点登录客户端代理通信";
		break;
      }
	return strStatus;
}
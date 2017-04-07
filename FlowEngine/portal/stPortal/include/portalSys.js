<script type="text/javascript">
var mapTable =new Array();
var c2b=null;
var pad=61;//¼´'='
var charsample="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
for(i=0;i<64;i++){
	mapTable[i]=charsample.charCodeAt(i);
}
function encode(toEncode){
        var abyte0=new Array();abyte0[0]=13;abyte0[1]=10;
        var i = 0;
        var j = 0;
        var abyte1;
        var currIndex=0;
        var sRet="";
        while((abyte1=readData(toEncode,currIndex)) != null){
            alert("abyte1="+abyte1);
            i=abyte1.length;
            for(j = 0; j < i; j += 3)
                if(j + 3 <= i)
                    sRet=sRet+encodeAtom(abyte1, j, 3);
                else
                    sRet=sRet+encodeAtom(abyte1, j, i - j);
             currIndex=currIndex+i;
        }
        return sRet;
    }
function readData(toRead,startIndex){
    var iEndIndex=startIndex
	if(toRead.length-1>startIndex){
		if(toRead.length>=startIndex+56)
			iEndIndex=startIndex+56;
		else
		    iEndIndex=toRead.length-1;
		var arrRet=new Array();
		var i=0;
		for(i=startIndex;i<=iEndIndex;i++)
			arrRet[i-startIndex]=toRead.charCodeAt(i);
		return arrRet;
	}
	return null;
}
function encodeAtom(abyte0,i,j){
        var sRet="";
        if(j == 1) {
            var byte0 = abyte0[i];
            var k = 0;
            var flag = false;
            sRet=sRet+String.fromCharCode(mapTable[byte0 >>> 2 & 0x3f]);
            sRet=sRet+String.fromCharCode(mapTable[(byte0 << 4 & 0x30) + (k >>> 4 & 0xf)]);
            sRet=sRet+String.fromCharCode(61);
            sRet=sRet+String.fromCharCode(61);
        } else
        if(j == 2) {
            var byte1 = abyte0[i];
            var byte3 = abyte0[i + 1];
            var l = 0;
            sRet=sRet+String.fromCharCode(mapTable[byte1 >>> 2 & 0x3f]);
            sRet=sRet+String.fromCharCode(mapTable[(byte1 << 4 & 0x30) + (byte3 >>> 4 & 0xf)]);
            sRet=sRet+String.fromCharCode(mapTable[(byte3 << 2 & 0x3c) + (l >>> 6 & 3)]);
            sRet=sRet+String.fromCharCode(61);
        } else {
            var byte2 = abyte0[i];
            var byte4 = abyte0[i + 1];
            var byte5 = abyte0[i + 2];
            sRet=sRet+String.fromCharCode(mapTable[byte2 >>> 2 & 0x3f]);
            sRet=sRet+String.fromCharCode(mapTable[(byte2 << 4 & 0x30) + (byte4 >>> 4 & 0xf)]);
            sRet=sRet+String.fromCharCode(mapTable[(byte4 << 2 & 0x3c) + (byte5 >>> 6 & 3)]);
            sRet=sRet+String.fromCharCode(mapTable[byte5 & 0x3f]);
        }
        return sRet;
    }
function decode(es){
      if (c2b==null) {
        c2b = new Array();
        for (b=0;b<64;b++)
          c2b[mapTable[b]]=b;
      }

      var nibble = new Array();
      var decode = new Array();
      var d=0;
      var n=0;
      var b=0;
      for (i=0;i<es.length;i++) {
        var c = es.charCodeAt(i);
        nibble[n] = c2b[c];
        if (c==pad) break;
        switch(n) {
        case 0:
          n++;
          break;

        case 1:
          b=(nibble[0]*4 + nibble[1]/16);
          decode[d++]=Math.floor(b);
          n++;
          break;

        case 2:
          b=((nibble[1]&0xf)*16 + nibble[2]/4);
          decode[d++]=Math.floor(b);
          n++;
          break;

        default:
          b=((nibble[2]&0x3)*64 + nibble[3]);
          decode[d++]=Math.floor(b);
          n=0;
          break;
        }
      }
      var index=0;
      var sRet="";
      for(index=0;index<decode.length;index++)
      	sRet+=String.fromCharCode(decode[index]);
      return sRet;
    }
    function loginWithBasic(authUrl,user,pass){
		xhreq = new ActiveXObject("Microsoft.XMLHTTP");
		var strUrl =authUrl;
		var authInfo="Basic ";
		authInfo=authInfo+encode(user+":"+pass);
		xhreq.setRequestHeader("Authorization",authInfo);
		xhreq.open("GET", strUrl, false);
		xhreq.send();
		var returnValue= xhreq.statusText;
		if(returnValue!="OK")
        	alert("µÇÂ¼["+strUrl+"]Ê§°ÜÁË£¡£¡");
	}
</script>

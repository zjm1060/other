
function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}

function MM_reloadPage(init) {  //reloads the window if Nav4 resized
  if (init==true) with (navigator) {if ((appName=="Netscape")&&(parseInt(appVersion)==4)) {
    document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; onresize=MM_reloadPage; }}
  else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH) location.reload();
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_goToURL() { //v3.0
  var i, args=MM_goToURL.arguments; document.MM_returnValue = false;
  for (i=0; i<(args.length-1); i+=2) eval(args[i]+".location='"+args[i+1]+"'");
}

function MM_callJS(jsStr) { //v2.0
  return eval(jsStr);
}

function MM_openBrWindow(theURL,winName,features) { //v2.0
  window.open(theURL,winName,features).focus();
}

function SYNX_showModalDialog(theURL,args,features) { // added by Clark Lu, 03/30/2004, for modal window
  //if(ie) {
    return window.showModalDialog(theURL,args,features);
  //}
  //else {
  //  return MM_openBrWindow(theURL,'modal dialog',features);
  //}
}

function tmd_findObj(n) {
    var x,t; if((n.indexOf("?"))>0&&parent.frames.length){t=n.split("?");
    x=eval("parent.frames['"+t[1]+"'].document.getElementById('"+t[0]+"')");
    }else{x=document.getElementById(n);}return x;
}

function tmd_DivMove(theDiv, l, t) {
    var x = (document.layers) ? ".left" : ".style.left";
    var y = (document.layers) ? ".top" : ".style.top";
    var fun = (document.getElementById) ? "tmd_findObj" : "MM_findObj";
    var obj = eval(fun+"(theDiv)");
    if(obj) {
      if(l) eval(fun+"('"+theDiv+"')"+x+"="+l);
      if(t) eval(fun+"('"+theDiv+"')"+y+"="+t);
    }
}

function tmd_SwitchDiv(theDiv, l1, l2, t1, t2) { // written by Clark Lu, switch div between two positions
    var x = (document.layers) ? ".left" : ".style.left";
    var y = (document.layers) ? ".top" : ".style.top";
    var fun = (document.getElementById) ? "tmd_findObj" : "MM_findObj";
    var obj = eval(fun+"(theDiv)");
    if(obj){
      if(l1) {
        var oldL = eval(fun+"('"+theDiv+"')"+x);
        oldL = (oldL.substring(0, oldL.indexOf('px'))==(""+l1)) ? l2 : l1;
        eval(fun+"('"+theDiv+"')"+x+"="+oldL);
      }
      if(t1) {
        var oldT = eval(fun+"('"+theDiv+"')"+y);
        oldT = (oldT.substring(0, oldT.indexOf('px'))==(""+t1)) ? t2 : t1;
        eval(fun+"('"+theDiv+"')"+y+"="+oldT);
      }
    }
}

function MM_showHideLayers() { //v3.0 revised by Clark Lu for switch show and hide
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3)
    if ((obj=MM_findObj(args[i]))!=null) {
      v=args[i+2];
      if (obj.style) {
        obj=obj.style;
        if(!v)
          v=(obj.visibility=="hidden")?"visible":"hidden";
        else
          v=(v=='show')?'visible':(v='hide')?'hidden':v;
      }
      else if(!v)
        v=(obj.visibility=="hide")?"show":"hide";

      obj.visibility=v;
  }
}

function expendCollapseLayers() { // written by Clark Lu
  var i,p,v,obj,args=expendCollapseLayers.arguments;
  for (i=0; i<(args.length-2); i+=3)
    if ((obj=MM_findObj(args[i]))!=null) {
        v=args[i+2];
        if (obj.style) {
            obj=obj.style;
            if(!v)
                v=(obj.display=="none")?"block":"none";
            else
                v=(v=='show')?'block':(v='hide')?'none':v;
        }
        obj.display=v;
    }
}

function isLayerVisible(name) { // written by Clark Lu
  if ((obj=MM_findObj(name))!=null) {
    if (obj.style)
      obj=obj.style;
    return (obj.visibility!="hidden");
  }
  return false;
}

function isLayerExpended(name) { // written by Clark Lu
  if ((obj=MM_findObj(name))!=null) {
    if (obj.style)
      obj=obj.style;
    return (obj.display!="none");
  }
  return false;
}

function switchImage() { // written by Clark Lu
  var i,x,a=switchImage.arguments; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){
   if(srcEquals(x.src, a[i+1])) x.src=a[i+2]; else x.src=a[i+1];}
}

function expendCollapseSection(name) {
  expendCollapseLayers(name,'','');
  switchImage('img_' + name,url_expand_img,url_collapse_img);
}

function srcEquals(str1, str2) { // written by Clark Lu
  var endStr1 = str1.substring(str1.lastIndexOf('/'), str1.length);
  var endStr2 = str2.substring(str2.lastIndexOf('/'), str2.length);

  return (endStr1==endStr2);
}

function setEnable(objName, bool) {
  var obj = MM_findObj(objName);
  if(obj==null)
    return;

  if (bool) {
    if(!obj.length || (obj.type && (obj.type=="select-one" || obj.type == "select-multiple")))
      obj.disabled = 0;
    else {
        var i;
        for(i=0;i<obj.length;++i)
          obj[i].disabled=0;
    }
  }
  else {
    if(!obj.length || (obj.type && (obj.type=="select-one" || obj.type == "select-multiple")))
      obj.disabled = true;
    else {
        var i;
        for(i=0;i<obj.length;++i)
          obj[i].disabled=true;
    }
  }
}

function disable(obj) {
  if(obj==null)
    return;
  if(obj.length==null)
    obj.disabled=true;
  else {
    var i;
    for(i=0;i<obj.length;++i)
      obj[i].disabled=true;
  }
}

// added by Clark Lu, 05/29/2003
// checkboxName is the name of checkbox with a form name, such as:
//   document.formName.checkboxName
// the return value is boolean, which is true if at least one checkbox is checked
function isChecked(checkboxName) {
    if( checkboxName == null )
      return false;
    if( checkboxName.length == null ) {  // one item
        return checkboxName.checked;
    }
    else {  // muti items
      var i;
      for (i=0; i<checkboxName.length; ++i) {
        if (checkboxName[i].checked)
            return true;
      }
    }

    return false;
}

// added by Clark Lu, 05/29/2003
// 'checkboxName' is the name of checkbox with a form name, such as:
//   document.formName.checkboxName
// 'delimiter' is the delimiter for return String
// the return value is String, include all values which is checked
// such as '232;5345;43534;6456;5333', here ';' is the delimiter
function getAllCheckedValues(checkboxName, delimiter) {
    if( checkboxName == null )
        return null;

    if( checkboxName.length == null ) {  // one item
        if( checkboxName.checked )
            return checkboxName.value;
        else
            return null;
    }
    else {  // muti items
        var i, count = 0;
        var skus = '';
        for (i=0; i<checkboxName.length; i++) {
            if (checkboxName[i].checked) {
                if (count > 0)
                    skus = skus + delimiter;

                skus = skus + checkboxName[i].value;
                count++;
            }
        }

        return skus;
    }
}

function validEMail(objName) {
  var obj = MM_findObj(objName);
  if (!obj)
    return false;

  var val = obj.value;
  if (val.indexOf("@",0) < 0 || val.indexOf(".")<0)
    return false;
  else
     return true;
}

// added by Clark Lu, 06/09/2004
// valid the email address according to the rules in bad_email_check.sp
function checkEmail(name) {
    var obj = MM_findObj(name);
    if (!obj)
        return false;
    else
        return checkEmailObj(obj);
}

function checkDomain(name) {
/*
	var rx = new Array();
	rx[0]=/\.com$/i;
	rx[1]=/\.org$/i;
	rx[2]=/\.net$/i;
	rx[3]=/\.gov$/i;
	rx[4]=/\.us$/i;
	rx[5]=/\.biz$/i;
	rx[6]=/\.edu$/i;
	rx[7]=/\.ca$/i;
	rx[8]=/\.tv$/i;
	rx[9]=/\.ws$/i;
	rx[10]=/\.info$/i;
	rx[11]=/\.to$/i;
	rx[12]=/\.uk$/i;
	rx[13]=/\.cz$/i;
	rx[14]=/\.it$/i;
	rx[15]=/\.br$/i;
	rx[16]=/\.cc$/i;
	rx[17]=/\.cn$/i;
	rx[18]=/\.mx$/i;
    rx[19]=/\.bm$/i;
    rx[20]=/\.com\.uy$/i;
    rx[21]=/\.ve$/i;
    rx[22]=/\.com.py$/i;
    rx[23]=/\infopoint.ie$/i;
	rx[24]=/\.bz$/i;
	rx[25]=/\.so.kr$/i;
	rx[26]=/\.es$/i;
	rx[27]=/\.gs$/i;
	rx[28]=/\.mil$/i;
*/

    for (i=0; i<g_email_domains.length; i++)
        if (g_email_domains[i].test(name))
            return true;

    return false;
}

//
function checkEmailObj(obj) {
    if (obj.value)
        return checkEmailValue(obj.value);
    else 
        return false;
}

function checkEmailValue(val) {
	var regExp=/@[\w\W]*@/;
	
	if (val == null) {
        document.errorMsg = "The email address is NULL!";
        return false;
	}

    if (val.indexOf(" ",0) >= 0) {
        document.errorMsg = "The email address '" + val + "' is invalid! \n\n--Reason: Space Found";
        return false;
    }
    else if (val.length < 6) {
        document.errorMsg = "The email address '" + val + "' is invalid! \n\n--Reason: E-Mail too Short (less than 6)";
        return false;
    }
    else if (val.indexOf("@",0) < 0) {
        document.errorMsg = "The email address '" + val + "' is invalid! \n\n--Reason: No @ character";
        return false;
    }
    else if (val.indexOf("@",0) == 0) {
        document.errorMsg = "The email address '" + val + "' is invalid! \n\n--Reason: Begins with @ character";
        return false;
    }
    else if (val.indexOf("@",0) == val.length-1) {
        document.errorMsg = "The email address '" + val + "' is invalid! \n\n--Reason: Ends with @ character";
        return false;
    }
    else if (regExp.test(val)) {
        document.errorMsg = "The email address '" + val + "' is invalid! \n\n--Reason: Multiple @ character";
        return false;
    }
    else if (val.indexOf(".@",0) >= 0 || val.indexOf("@.",0) >= 0) {
        document.errorMsg = "The email address '" + val + "' is invalid! \n\n--Reason: '.@' or '@.' Found";
        return false;
    }
    else if (!checkDomain(val)) {
        document.errorMsg = "The email address '" + val + "' is invalid! \n\n--Reason: Bad Domain";
        return false;
    }
    else
        return true;
}


function setFocus(objName, bool) {
  var obj = MM_findObj(objName);

  if (bool) {
    obj.focus();
    obj.select();
  }
  else {
    obj.blur();
  }
}

function clearForm(frm){
  var elements=frm.elements;
  var element;
  var i;
  for(i=0;i<elements.length;++i){
    element=elements[i];
    if(element.type=="text"||element.type=="textarea")
      element.value="";
    else if(element.type=="select-one"||element.type=="select-multiple"){
      var options=element.options,j,item;
      for(j=0;j<options.length;++j){
        item=options[j];
        item.selected=item.defaultSelected;
      }
    }else if(element.type=="checkbox"||element.type=="radio")
      element.checked=element.defaultChecked;
  }
}

function gotoPage(url)
{
    window.location.href = url;
}

function maximizeWindow() {
    top.window.moveTo(0,0);
    if (document.all) {
        top.window.resizeTo(screen.availWidth,screen.availHeight);
    }
    else if (document.layers||document.getElementById) {
        if (top.window.outerHeight<screen.availHeight||top.window.outerWidth<screen.availWidth) {
            top.window.outerHeight = screen.availHeight;
            top.window.outerWidth = screen.availWidth;
        }
    }
}

function setHeightOfFooter() {
    //var footer = MM_findObj("footer");
    var footer = document.getElementById("footer");
    if (!footer) return;

    //var content = MM_findObj("content");
    var content = document.getElementById("content");
    //var sidebar = MM_findObj("sidebar");
    var sidebar = document.getElementById("sidebar");
    var gap = 10;
    var top = document.body.clientHeight - footer.clientHeight;
    var tmp;

    if (content) {
        tmp = Math.floor(content.offsetTop) + Math.floor(content.clientHeight) + gap;
        if(tmp > top) {
            top = tmp;
        }
    }

    if (sidebar) {
        tmp = Math.floor(sidebar.offsetTop) + Math.floor(sidebar.clientHeight) + gap;
        if(tmp > top) {
            top = tmp;
        }
    }

    if (typeof(oCMenu) != "undefined" && oCMenu != null)
    {
        var height = 21;
        var offset0 = 18;
        if(oCMenu.l[0] && oCMenu.l[0].m.length>0)
        {
            for (i=0;i<oCMenu.l[0].m.length;i++)
            {
                var m = oCMenu.m[oCMenu.l[0].m[i]];
                if (m) {
                    tmp = (m.subs.length) * height + offset0;
                    if(tmp > top) {
                        top = tmp;
                    }
                }
            }
        }
    }

    footer.style.top = top;

    var ie5menu = MM_findObj("ie5menu");
    if (ie5menu) {
        hidemenuie5();
    }
}

function setHeightOfSideBar() {
    //var sidebar = MM_findObj("sidebar");
    var sidebar = document.getElementById("sidebar");
    var minHeight=350;

    if (sidebar && sidebar.clientHeight < minHeight) {
        sidebar.style.height = minHeight;
    }
}

function setHeightOfContent() {
    //var header_bg = MM_findObj("header_bg");
    var header_bg = document.getElementById("header_bg");
    var header_info = MM_findObj("header_info");
    //var content = MM_findObj("content");
    var content = document.getElementById("content");
    var sideimg = MM_findObj("sideimg");
    //var sidebar = MM_findObj("sidebar");
    var sidebar = document.getElementById("sidebar");
    var top = 8;

    if (!header_bg && !header_info) {
        if (content)
            content.style.top = top;
        if (sideimg)
            sideimg.style.top = top;
        if (sidebar)
            sidebar.style.top = top;
    }
}

function trim(str)
{
	if(str==null || str=="")
		return "";
	str = str.toString();
	var prePos,lastPos;
	var i,result;

	prePos = 0;
	lastPos = str.length-1;
	for(;str.charAt(prePos)==" ";prePos++);
	for(;str.charCodeAt(lastPos)=="\n" || str.charAt(lastPos)==" ";lastPos--);
	if (prePos > lastPos) return "";
	else return (str.substring(prePos,lastPos+1));
}

function checkNumber(num)
{
	var pattern=/[^0-9]/;
	var testString = trim(num);
	if(pattern.test(testString)) return false;
	return true;
}

function isBlank(obj) {
	var input=trim(obj.value);
	if(input=="\t"||input==" "|| input=="") return true;
	return false;
}

// added by Clark Lu, 06/07/2004
// to show tooltip
// use showtip on onmouseover and hidetip on onmouseout
// example:
// <INPUT onmouseover="showtip(this,event,'the tip text')" onmouseout=hidetip() type=button>
//
if (!document.layers&&!document.all)
	event="test";

function showtip(current, e, text) {
    if (document.all) {
        thetitle=text.split('<br>');
        if (thetitle.length>1) {
            thetitles='';
            for (i=0;i<thetitle.length;i++)
                thetitles+=thetitle[i];
            current.title=thetitles;
        }
        else
            current.title=text;
    }

    else if (document.layers) {
        document.tooltip.document.write('<layer bgColor="white" style="border:1px solid black;font-size:12px;">'+text+'</layer>');
        document.tooltip.document.close();
        document.tooltip.left=e.pageX+5;
        document.tooltip.top=e.pageY+5;
        document.tooltip.visibility="show";
    }
}

function hidetip() {
    if (document.layers)
        document.tooltip.visibility="hidden";
}

function initForms() {
    var fs = document.forms;

    for (j = 0; j< fs.length; j++) {
        var frm = fs[j];
        if (frm.className && frm.className.toUpperCase() == "NOTOUCH") {
          continue;
        }

        len = frm.elements.length;
        for (i=0; i<len; i++) {
            var obj = frm.elements[i];
            if (obj.tagName && obj.tagName.toUpperCase() != "INPUT") {
                //alert("objName = " + obj.name + ", tagName = " + obj.tagName);
              
                if (obj.type == "select-one" || obj.type == "select-multiple") {
                    if (!obj.className )
                        obj.className = "select";
                    if (obj.className == "select" || obj.className == "select_disabled") {
                        if (obj.disabled)
                            obj.className = "select_disabled";
                        else
                            obj.className = "select";
                    }
                }
                else if (obj.type == "textarea") {
                    if (!obj.className )
                        obj.className = "textarea";
                    if (obj.className == "textarea" || obj.className == "textarea_readonly") {
                        if (obj.readOnly)
                            obj.className = "textarea_readonly";
                        else
                            obj.className = "textarea";
                    }
                }
                else
                    continue;
            }
            if (!obj.type)
              obj.type = "text";
            if (obj.type.toUpperCase() == "HIDDEN") {
              //alert("objName = " + obj.name + ", type = " + obj.type);
              continue;
            }

            if (obj.type == "button" || obj.type == "submit" || obj.type == "reset") {
                if (!obj.className )
                  obj.className = "button";
            }
            else if (obj.type == "text" || obj.type == "password") {
                if (!obj.className || obj.className == "text" || obj.className == "text_readonly") {
                    if (obj.readOnly)
                        obj.className = "text_readonly";
                    else
                        obj.className = "text";
                }

                if (obj.className.toLowerCase() == "integer") {
                  initIntegerField(obj);
                }
              else if (obj.className.toLowerCase() == "float") {
                  initFloatField(obj);
                }
              else if (obj.className.toLowerCase() == "date") {
                  initDateField(obj);
                }
              else if (obj.className.toLowerCase() == "phone") {
                  initPhoneField(obj);
                }
              else if (obj.className.toLowerCase() == "currency") {
                  initCurrencyField(obj);
                }
              else if (obj.className.toLowerCase() == "alphanumeric") {
                  initAlphanumericField(obj);
                }
            }
            else if (obj.type == "checkbox") {
                if (!obj.className )
                  obj.className = "checkbox";
                if (obj.className == "checkbox" || obj.className == "checkbox_disabled") {
                    if (obj.disabled)
                        obj.className = "checkbox_disabled";
                    else
                        obj.className = "checkbox";
                }
            }
            else if (obj.type == "radio") {
                if (!obj.className )
                  obj.className = "radio";
                if (obj.className == "radio" || obj.className == "radio_disabled") {
                    if (obj.disabled)
                        obj.className = "radio_disabled";
                    else
                        obj.className = "radio";
                }
            }
        }
    }
    document.attachEvent ('onkeydown', filterKeys);
}

function filterKeys() {
    var obj = window.event.srcElement;
    if (!obj)
        return;

    var kc = window.event.keyCode;
    //alert('key: ' + kc);
    
    if (kc == 27) { // 27: ESC key
		window.event.cancelBubble = true;
		window.event.returnValue = false;
    }
}

var default_number_allow_negative = "true";

function initIntegerField(obj) {
  if (!obj.negative)
    obj.negative = default_number_allow_negative;

  obj.attachEvent ('onkeydown', limitInputAsInteger);
}

function initFloatField(obj) {
  if (!obj.negative)
    obj.negative = default_number_allow_negative;

  obj.attachEvent ('onkeydown', limitInputAsFloat);
}

function initCurrencyField(obj) {
  if (!obj.negative)
    obj.negative = default_number_allow_negative;

  obj.attachEvent ('onkeydown', limitInputAsFloat);
}

function initDateField(obj) {
  obj.attachEvent ('onkeydown', limitInputAsDate);
}

function initPhoneField(obj) {
  obj.attachEvent ('onkeydown', limitInputAsPhone);
}

function initAlphanumericField(obj) {
  obj.attachEvent ('onkeydown', limitInputAsAlphanumeric);
}

function limitInputAsInteger() {
  var obj = window.event.srcElement;
  if (!obj)
    return;

  var canBeNegative = (obj.negative == "true" && obj.value.indexOf("-") < 0);
	var kc = window.event.keyCode;
  if (!(isNumberOrControlKey(kc) || (canBeNegative && isNegativeSign(kc))) ) {
		window.event.cancelBubble = true;
		window.event.returnValue = false;
  }
}

function limitInputAsFloat() {
  var obj = window.event.srcElement;
  if (!obj)
    return;
  var canBeNegative = (obj.negative == "true" && obj.value.indexOf("-") < 0);
  var hasDot = (obj.value.indexOf(".") >= 0);
	var kc = window.event.keyCode;
  if (!(isNumberOrControlKey(kc) || (canBeNegative && isNegativeSign(kc)) || (!hasDot && isDot(kc))) ) {
		window.event.cancelBubble = true;
		window.event.returnValue = false;
  }
}

function limitInputAsDate() {
  var obj = window.event.srcElement;
  if (!obj)
    return;
	var kc = window.event.keyCode;
  if (!(isNumberOrControlKey(kc) || (isNegativeSign(kc)) || (isBacklash(kc))) ) {
		window.event.cancelBubble = true;
		window.event.returnValue = false;
  }
}

function limitInputAsPhone() {
  var obj = window.event.srcElement;
  if (!obj)
    return;
	var kc = window.event.keyCode;
  if (!(isPhoneOrControlKey(kc)) ) {
		window.event.cancelBubble = true;
		window.event.returnValue = false;
  }
}

function limitInputAsAlphanumeric() {
  var obj = window.event.srcElement;
  if (!obj)
    return;
	var kc = window.event.keyCode;
  if (!(isAlphanumericOrControlKey(kc, window.event.shiftKey)) ) {
		window.event.cancelBubble = true;
		window.event.returnValue = false;
  }
}

function isNumberOrControlKey(kc) {
	return (  (kc >= 48 && kc <= 57)   // number 0-9
  			 || (kc >= 96 && kc <= 105)  // number 0-9 in the right of keyboard
  		   || (kc == 8)                // backspace
  		   || (kc == 35)               // end
  		   || (kc == 36)               // home
  		   || (kc == 37)               // left arrow
  		   || (kc == 39)               // right arrow
  		   || (kc == 46)               // delete
  		   || (kc == 9)                // tab
  		   || (kc == 13)               // enter
  		   || (kc == 188) );           // ","
}

function isPhoneOrControlKey(kc) {
	return (  (kc >= 48 && kc <= 57)   // number 0-9
  			 || (kc >= 96 && kc <= 105)  // number 0-9 in the right of keyboard
  		   || (kc == 8)                // backspace
  		   || (kc == 35)               // end
  		   || (kc == 36)               // home
  		   || (kc == 37)               // left arrow
  		   || (kc == 39)               // right arrow
  		   || (kc == 46)               // delete
  		   || (kc == 9)                // tab
  		   || (kc == 13)               // enter
  		   || (kc == 32)               // space
  		   || (kc == 189)               // "-"
  		   || (kc == 109)               // "-" in right
  		   || (kc == 188) );           // ","
}

function isAlphanumericOrControlKey(kc, shiftKey) {
	return (  (kc >= 48 && kc <= 57 && !shiftKey)   // number 0-9
  			 || (kc >= 96 && kc <= 105 && !shiftKey)  // number 0-9 in the right of keyboard
  		   || (kc == 8)                // backspace
  		   || (kc == 35)               // end
  		   || (kc == 36)               // home
  		   || (kc == 37)               // left arrow
  		   || (kc == 39)               // right arrow
  		   || (kc == 46)               // delete
  		   || (kc == 9)                // tab
  		   || (kc == 13)               // enter
  		   || (kc >= 65 && kc <= 90)   // a-z, A-Z
  		   );                          //
}


function isDot(kc) {
  return (kc == 190 || kc ==110);
}

function isNegativeSign(kc) {
  return (kc == 189 || kc ==109);
}


function isBacklash(kc) {
  return (kc == 191 || kc ==111);
}


// add tab functions by Clark Lu, 12/27/2004
var oldTabId = null;
var tabFrameId = null;
var tabContentId = null;

function setTab(id, url) {
	var newTab = MM_findObj(id+"Tab");
	var newBar = MM_findObj(id+"Bar");
	var newTop = MM_findObj(id+"Top");
	var newTopShadow = MM_findObj(id+"TopShadow");
		
	if (newTab) {
	  newTab.bgColor = "#608c8c";
	  newTab.color = "#ffffff";
	  newTab.style.fontWeight = "bold";
  }

  if (newBar)
	  newBar.bgColor = "#608c8c";

  if (newTop) {
	  newTop.bgColor = "#608c8c";
	  newTopShadow.bgColor = "#000000";
  }

  if (oldTabId && oldTabId != id) {
  	var oldTab = MM_findObj(oldTabId+"Tab");
  	var oldBar = MM_findObj(oldTabId+"Bar");
  	var oldTop = MM_findObj(oldTabId+"Top");
  	var oldTopShadow = MM_findObj(oldTabId+"TopShadow");
  		
  	if (oldTab) {
  	  oldTab.bgColor = "#b8cece";
  	  oldTab.color = "#000000";
	    oldTab.style.fontWeight = "normal";
    }
    
  	if (oldBar)
  	  oldBar.bgColor = "#b8cece";
  	if (oldTop)
  	  oldTop.bgColor = "#ffffff";
  	if (oldTopShadow)
  	  oldTopShadow.bgColor = "#ffffff";
  }
  
  expendCollapseLayers(oldTabId + "Div","", "hide" );
  expendCollapseLayers(id + "Div","", "show" );
  
  var fr = MM_findObj(id+"Frame");
  var fv = MM_findObj(id+"Div");
  if (fr && url) {
    if ((trim(fr.cached) == "true" && trim(fr.src).length == 0) || trim(fr.cached) != "true")
      loadIFrame(id+"Frame", url);
      
      var con = MM_findObj(tabContentId);
      if (con && con.show && trim(con.show).toLowerCase() == "visible") {
        //alert("set height as visible");
        tabFrameId = id+"Frame";
        document.getElementById(id+"Frame").attachEvent ('onload', adjustTabHeightAsVisible);
      }
  }

  oldTabId = id;
}

function adjustTabHeightAsVisible() {
  //alert("adjustTabHeightAsVisible: " + tabFrameId);
  var con = MM_findObj(tabContentId);
  //alert(tabFrameId + ", " + tabContentId + ", con:" + con + ", " + document.getElementById(tabFrameId).document.body.scrollHeight);
  
  if (con) {
    con.style.height = document.getElementById(tabFrameId).document.body.scrollHeight + 20;
    setHeightOfFooter();
  }
}

function initTab(id) {
  tabContentId = id+"Content";
  var con = MM_findObj(id+"Content");
  
  if(con.show && trim(con.show).toLowerCase() == "fit-screen-height") {  
    var tmpRect = con.getBoundingClientRect();  
    var h = document.body.clientHeight - tmpRect.top ;  
    
    var footer = document.getElementById("footer");
    if (footer)
      h -= 25;
    con.style.height = h ;  
  }

  //if (firstTab)
  //  setTab(firstTab, url);
}


function openWindow(url, id, width,height) {
   var xMax = 1024, yMax=768;
   if (document.all){
      xMax = screen.width, yMax = screen.height;
   }
   else if (document.layers){
       xMax = window.outerWidth, yMax = window.outerHeight;
   }
       
   var xOffset = (xMax - width)/2, yOffset = (yMax - height)/2;

   var win = window.open (url, id, 'height='+height+', width='+width+', toolbar=no, menubar=no,scrollbars=yes ,resizable=yes, location=no, status=no,screenX='+xOffset+',screenY='+yOffset+',top='+yOffset+',left='+xOffset+'');
   win.focus();
   return win;
}

function parseNumber(f){
	var s=""+f;
	var ss="";
	var c='';
	if(s==""){
		return 0;
	}else{
		for(var i=0;i<s.length;i++){
			c=s.charAt(i);
			if(!isNaN(c)||c=='.'||c=='-')
				ss+=c;
		}
		//alert(ss);
		if(isNaN(ss)||ss=="")
			return 0;
		else
			return parseFloat(ss);
	}
	return 0;
}



/**
  * Show progress bar in a HTML element
  * Example 1 start a progress bar in indeterminate mode
  * ProgressBar bar = new ProgressBar('parentElement'); //parentElement is the element id the process bar will display in
  * bar.start(); //Start the progress bar
  * bar.destory(); //Stop and destory the progress bar
  *
  * Example 2 start a progress bar in maually mode
  * ProgressBar bar = new ProgressBar('parentElement'); //parentElement is the element id the process bar will display in
  * bar.showProgress(10);
  * bar.showProgress(20);
  * bar.destory(); //Destory the progress bar
**/

function ProgressBar(parentId, width, docRef){
	if(!docRef){
	    docRef = document;
	}
	this.parentId = parentId;
	if(docRef.getElementById(this.parentId+'_ProgressBar_')){
		return;
	}
	this.layer = docRef.createElement("div");
	this.layer.style.visibility="hidden";
	this.layer.id = this.parentId+'_ProgressBar_';
	this.parentElement = docRef.getElementById(parentId);
	this.parentElement.appendChild( this.layer );
	
	this.width = this.parentElement.offsetWidth * 0.6;
	if(width != null){
	    this.width = width;
	}
	
	this.height = 70;
	this.size = 100;
	this.groupCellWidth = 10;
	this.indeterminatedGroupCellWidth = 20;
	this.circulate = false;
	
	this.start = 0;
	this.end = 0;
	
	var tableText = "";
	var cellWidth = Math.round(this.width/this.size);
	if(cellWidth == 0){
	    cellWidth = 1;
	}
	
	
	this.groupCellSize = Math.round(this.groupCellWidth/cellWidth);
	if(this.groupCellSize == 0){
	    this.groupCellSize = 1;
	}
	this.groupCellCount = Math.round(this.size/this.groupCellSize);
	
	this.size = this.groupCellSize*this.groupCellCount;
	this.width = this.groupCellCount * this.groupCellWidth;
	
	for (x = 0; x < this.groupCellCount; x++) {
	    tableText += "<td><table border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr>";
	    for (y = 0; y < this.groupCellSize; y++) {
	        var cellId = (x*this.groupCellSize + y);
	        tableText += "<td id='"+parentId+"_progress_" + cellId + "' height='15px' bgcolor=\"white\"><img src='"+contextPath+"/resources/images/blank.png' width='"+cellWidth+"px' height='1px'></td>";
	    }
	    tableText += "</tr></table></td>";
	}
	
	this.layer.style.visibility="hidden";
	this.layer.align="center";
	this.layer.innerHTML= "<table width='"+this.width+"px' height=100%><tr><td  valign=center align=center><table width='100%' bgcolor=\"#FFFFFF\" cellspacing=\"2\" cellpadding=\"0\" style=\"border:1px solid #C0C0C0; \"><tr>" + tableText + "</tr></table></td></tr></table>";
	
	this.showProgress = function(percentage) {
	  	var tableText = "";
	  	var tmp = this.size*percentage/100;
	  	for (x = 0; x < this.size; x++) {
		    var cell = docRef.getElementById(this.parentId+"_progress_" + x);
		    
		    if(cell != null){
		        if (x > tmp) {
		        	if(cell.style.backgroundColor != "white"){
		            	cell.style.backgroundColor = "white";
		            }
		        } else {
		            if(this.circulate && (x + this.indeterminatedGroupCellWidth) < tmp){
		            	if(cell.style.backgroundColor != "white"){
		                	cell.style.backgroundColor = "white";
		                }
		            }else{
		            	if(cell.style.backgroundColor != "#7CE47E"){
		                	cell.style.backgroundColor = "#7CE47E";
		                }
		            }
		        }    
		    }
	  	}
	};

	this.start = function(){
		if(this.layer == null){
			return;
		}
	    this.circulate = true;
	    this.layer.style.visibility="visible";
	    var bar = this;
	    this.start = 0;
	    this.end = this.size + this.indeterminatedGroupCellWidth;
	    
	    this.timeId = setInterval( function(){
	    try{
	        if(bar != null){
	            if(bar.start < bar.end){
	                bar.showProgress(bar.start);
	                bar.start += bar.groupCellSize;
	            }else{
	                bar.start = bar.size/4;
	                bar.showProgress(bar.start);
	                bar.start += bar.groupCellSize;
	            }
	        }
	    }catch(e){bar.destroy();}
	    }, 80);
	};
	
	this.stop = function(){
		if(this.layer == null){
			return;
		}
	    try{
	        this.circulate = false;
	        this.layer.style.visibility="hidden";
	        this.pos = 1;
	        for (x = 0; x < this.size; x++) {
	            var cell = docRef.getElementById(this.parentId+"_progress_" + x);
	            if ((cell)) {
	                cell.style.backgroundColor = "white";
	            }
	        }      
	        clearInterval(this.timeId);
	    }catch(e){}
	};
	
	this.destroy = function(){
		if(this.layer == null){
			return;
		}
	    try{
	        this.stop();
	        this.parentElement.removeChild(this.layer);
	    }catch(e){}
	};
}



function formatNumber(num,decimal){   
//alert(num + ', ' + decimal);
     var tmpNumber1=num.toString();
     var numlen=tmpNumber1.length;
     var decimalIdx=tmpNumber1.indexOf('.'); 
     var Intlen;   
     var decimallen;   
     if(decimalIdx!=-1)   
         decimallen=numlen-decimalIdx-1;  
     else  
         decimalIdx=0;   
     var tmpNumber2;    
   
     if(decimal!=0)   
     {   
         tmpNumber2 = num*(Math.pow(10,decimal));   
         tmpNumber2=Math.round(tmpNumber2)/(Math.pow(10,decimal));   
     }   
     else  
         tmpNumber2 = tmpNumber1 + '.0';   
        
     var decimalNum=''; 
     if(tmpNumber1.indexOf('.')!=-1)   
         decimalNum = tmpNumber2.toString().substring(tmpNumber1.toString().indexOf('.')+1,tmpNumber1.toString().indexOf('.')+decimal+1);  
     var rtndecimal=decimalNum;   
    
     for(var j=0;j<(decimal-decimalNum.length);j++)   
         rtndecimal = rtndecimal + '0';   
     var IntNum;  
     IntNum=tmpNumber2.toString().substring(tmpNumber2.toString().indexOf('.'),0);   
     if(tmpNumber2.toString().indexOf('.')==-1)   
         IntNum = tmpNumber2.toString();   
     if(tmpNumber2.toString().indexOf('.')==0)   
         IntNum = '0';     
         
     var sign = '';
     if(IntNum.substring(0,1)=='-') {
     	sign = '-';
     	IntNum = IntNum.substring(1);
     }
     var lpcnt = Math.floor(IntNum.length/3);   
  
     var tmpNumber3='';  
    
     for(var i=0;i<lpcnt;i++)   
     {   
         tmpNumber3=',' + IntNum.substring(IntNum.length,IntNum.length-3).toString() + tmpNumber3;   
         IntNum=IntNum.substring(IntNum.length-3,0);   
     }   
   
     tmpNumber3 = IntNum + tmpNumber3;   
     if(tmpNumber3.substring(0,1)==',')   
         tmpNumber3 = tmpNumber3.substring(1,tmpNumber3.length);   
         
      if(decimal==0){
     	return tmpNumber3;
     }
     return sign + tmpNumber3 + '.' + rtndecimal;      
 }
 
 function setCookie(name,value,duration){
 	duration= (duration==null?30:duration);
	var cookiestring=name+"="+escape(value)+";EXPIRES="+getExpiryDate(duration);
	document.cookie=cookiestring;
}

function getCookie(cookiename) {
 	var cookiestring=""+document.cookie;
 	var index1=cookiestring.indexOf(cookiename);
 	if (index1==-1 || cookiename=="") return null; 
 	var index2=cookiestring.indexOf(';',index1);
 	if (index2==-1) index2=cookiestring.length; 
 	return unescape(cookiestring.substring(index1+cookiename.length+1,index2));
}

function getUserDataStore(){
	var oMycisPersist = document.getElementById('oMycisPersist');
	if(oMycisPersist == null){
		oMycisPersist = document.createElement('DIV');
		oMycisPersist.id = 'oMycisPersist';
		oMycisPersist.style.behavior = "url('#default#userData')";
		oMycisPersist.style.visibility = 'hidden';
		document.body.appendChild(oMycisPersist);
	}
	oMycisPersist.load('MycisStore');
	return oMycisPersist;
}

function saveUserData(attributeName, attributeValue){
	try{
	var oMycisPersist = getUserDataStore();
	if(attributeValue == null){
		oMycisPersist.removeAttribute(attributeName);
	}else{
		oMycisPersist.setAttribute(attributeName, attributeValue);
	}
	
	oMycisPersist.expires = getExpiryDate(30);
	
	oMycisPersist.save('MycisStore');
	}catch(e){}
}

function loadUserData(attributeName){
	try{
	var oMycisPersist = getUserDataStore();
	return oMycisPersist.getAttribute(attributeName);
	}catch(e){return null;}
}

function getExpiryDate( nodays){
	var UTCstring;
	Today = new Date();
	nomilli=Date.parse(Today);
	Today.setTime(nomilli+nodays*24*60*60*1000);
	UTCstring = Today.toUTCString();
	return UTCstring;
}

//open WebQuote by query a Sales Order
function openWebQuoteWindow(orderNo){
	var url = "/webQuote/viewMainPage.do?openWebquote=true&orderNo="+orderNo;
 	var feature = "toolbar=no,scrollbars=no,statusbar=no,resizable=no,width=1024,height=768";
    var subwin = window.open(url,"webquote",feature);
    subwin.focus();
}

//open WebQuote by query a Sales Order
function openWebQuoteWindowEx(orderNo){
	//Check whether the current window is child window of webQuote. 
	//E.g. myCIS OOS could be nested in webQuote
	var myParent = window.top;
	if(myParent != null && myParent.document != null
		&& myParent.document.quickSearchForm != null
		&& myParent.document.quickSearchForm.searchKey != null
		&& myParent.document.quickSearchForm.searchValue != null){
		myParent.document.quickSearchForm.searchValue.value = orderNo;
		myParent.document.quickSearchForm.searchKey[2].checked = true;
		myParent.document.quickSearchForm.action="order/searchSO.do";
		myParent.document.quickSearchForm.method.value="quickSOBO";			
		myParent.document.quickSearchForm.submit();
	}else{
		openWebQuoteWindow(orderNo);
	}
}

//open WebQuote by query a CPO
function openWebquoteByCPO(poid){
    url = "/webQuote/viewMainPage.do?openWebquote=true&poid="+poid;
    var feature = "toolbar=no,scrollbars=no,statusbar=no,resizable=no,width=1024,height=768";
    window.open(url,"webquote",feature).focus();
}
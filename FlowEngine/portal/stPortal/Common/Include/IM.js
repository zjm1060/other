//对查询框结果的控制
var intIndex=0;arrList = new Array();

function dearray(aa)//定义array
{
    //document.write(aa);
    arrList = aa.split(',');
    intIndex = arrList.length;

}

//function init() {
//if (arrList.constructor!=Array){alert('smanPromptList初始化失败:第一个参数非数组!');return ;}
//    arrList.sort( function(a, b) {
//    if(a.length>b.length)return 1;
//    else if(a.length==b.length)return a.localeCompare(b);
//    else return -1;
//        }
//    ); 
//}


function smanPromptList(objInputId){
	
        var objouter=document.getElementById("__smanDisp") //显示的DIV对象
        var objInput = document.getElementById(objInputId); //文本框对象
        var selectedIndex=-1;
        var intTmp; //循环用的:)

        if (objInput==null) {alert('smanPromptList初始化失败:没有找到"'+objInputId+'"文本框');return ;}
            //文本框失去焦点
            objInput.onblur=function(){
                objouter.style.display='none';
            }
            objInput.onkeyup=checkKeyCode;
            //文本框得到焦点
            objInput.onfocus=checkAndShow;
            function checkKeyCode(evt){
			                //文本框按键抬起
			if(window.event.keyCode!=13)
			{
			    return;
			}
			    evt = evt || window.event;
                var ie = (document.all)? true:false
                if (ie){
                    var keyCode=evt.keyCode;
                     if (keyCode==13){//回车
                        checkAndShow(evt);
                    }else if(keyCode==8 || keyCode==46){
					alert();
						objouter.style.display='none';
					}
                }
                divPosition(evt)
            }

            function checkAndShow(evt){
                        var strInput = objInput.value
                        if (strInput!=""){
                            divPosition(evt);
                            selectedIndex=-1;
                            objouter.innerHTML ="";
                           message = HttpSubmitString('service=onlineusers&func=search&searchWhere='+strInput,"./portalSvl");
						    arrList=dearray(message);
							for (intTmp=0;intTmp<arrList.length;intTmp++){
                                    addOption(arrList[intTmp]);
                            }
                            objouter.style.display='';
                        }else{
                            objouter.style.display='none';
                    }
                    function addOption(value){
                        objouter.innerHTML +="<div onmouseover=\"this.className='sman_selectedStyle'\" onmouseout=\"this.className=''\" onmousedown=\"document.getElementById('"+objInputId+"').value='" + value + "'\">" + value + "</div>"    
                    }
            }
            
            function divPosition(evt){
                var left = 0;
	            var top  = 0;				
				var e = objInput;
	            while (e.offsetParent){
		            left += e.offsetLeft + (e.currentStyle?(parseInt(e.currentStyle.borderLeftWidth)).NaN0():0);
		           top  += e.offsetTop  + (e.currentStyle?(parseInt(e.currentStyle.borderTopWidth)).NaN0():0);
		          e     = e.offsetParent;
	           }

	          left += e.offsetLeft + (e.currentStyle?(parseInt(e.currentStyle.borderLeftWidth)).NaN0():0);
	          top  += e.offsetTop  + (e.currentStyle?(parseInt(e.currentStyle.borderTopWidth)).NaN0():0);
	
                objouter.style.top    = (top  + objInput.clientHeight) + 'px' ;
                objouter.style.left    = left + 'px' ; 
                objouter.style.width= objInput.clientWidth;
            }
    }
    document.write("<div id='__smanDisp' style='position:absolute;display:none;background:#E8F7EB;border: 1px solid #CCCCCC;font-size:14px;cursor: default;' onbulr> </div>");
    document.write("<style>.sman_selectedStyle{background-Color:#102681;color:#FFFFFF}</style>");
    function getAbsoluteHeight(ob){
        return ob.offsetHeight
    }
    function getAbsoluteWidth(ob){
        return ob.offsetWidth
    }
    function getAbsoluteLeft(ob){
        var mendingLeft = ob .offsetLeft;
        while( ob != null && ob.offsetParent != null && ob.offsetParent.tagName != "BODY" ){
            mendingLeft += ob .offsetParent.offsetLeft;
            mendingOb = ob.offsetParent;
        }
        return mendingLeft ;
    }
    function getAbsoluteTop(ob){
        var mendingTop = ob.offsetTop;
        while( ob != null && ob.offsetParent != null && ob.offsetParent.tagName != "BODY" ){
            mendingTop += ob .offsetParent.offsetTop;
            ob = ob .offsetParent;
        }
        return mendingTop ;
    }
Number.prototype.NaN0 = function()
{
    return isNaN(this)?0:this;
}

//对整个IM的统计
function hideAll() {
	try {
		for (i = 0; i < IM_DIV.length; i++) {
			IM_DIV[i].style.display = "none";
		}
	}
	catch (e) {
		IM_DIV.style.display = "none";
	}
}
function showAll() {
	try {
		for (i = 0; i < IM_DIV.length; i++) {
			IM_DIV[i].style.display = "";
		}
	}
	catch (e) {
		IM_DIV.style.display = "";
	}
}

function hiddleIM(divHeight) {
	var IM_DIV = document.getElementById("IM_DIV");
	if (IM_DIV == null || typeof (IM_DIV) == "undefined") {
		return;
	}
	document.getElementById("IM_DIV").style.height = divHeight;
}
function showObj(num) {
	try {
	 //IM_DIV[num]==null||typeof(IM_DIV[num]=="undefined")
		if (IM_DIV[num].style.display == "none") {
			hideAll();
			IM_DIV[num].style.display = "inline";
		} else {
			IM_DIV[num].style.display = "none";
		}
	}
	catch (e) {
		if (IM_DIV.style.display == "none") { 
			//hideAll();     
			IM_DIV.style.display = "inline";
		} else {
			IM_DIV.style.display = "none";
		}
	}
}
function showIM(divHeight) {
	var IM_DIV = document.getElementById("IM_DIV");
	if (IM_DIV == null || typeof (IM_DIV) == "undefined") {
		return;
	}
	if (event.srcElement == IM_DIV) {
		document.getElementById("IM_DIV").style.height = divHeight;
	}
}

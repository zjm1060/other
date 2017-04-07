var text = "";

function submitonce(theform)
{
	if (document.all||document.getElementById)
	{
		for (i=0;i<theform.length;i++)
		{
			var tempobj=theform.elements[i];
			if(tempobj.type.toLowerCase()=="submit"||tempobj.type.toLowerCase()=="reset")
				tempobj.disabled=true;
		}
	}
}

function AddText(NewCode) 
{
	if (document.FORM.content.createTextRange && document.FORM.content.caretPos) 
	{
		var caretPos = document.FORM.content.caretPos;
		caretPos.text = caretPos.text.charAt(caretPos.text.length - 1) == ' ' ? NewCode + ' ' : NewCode;
	} 
	else 
	{
		document.FORM.content.value+=NewCode
	}
	setfocus();
}

function setfocus()
{
  document.FORM.content.focus();
}


        helpmode = false;
        divmode = false;
        nomode = true;

function checkmode(swtch){
        if (swtch == 1){
                nomode = false;
                divmode = false;
                helpmode = true;
                alert("99mill 代码 - 帮助信息\n\n点击相应的代码按钮即可获得相应的说明和提示");
        } else if (swtch == 0) {
                helpmode = false;
                divmode = false;
                nomode = true;
                alert("99mill 代码 - 直接插入\n\n点击代码按钮后不出现提示即直接插入相应代码");
        } else if (swtch == 2) {
                helpmode = false;
                nomode = false;
                divmode = true;
                alert("99mill 代码 - 提示插入\n\n点击代码按钮后出现向导窗口帮助您完成代码插入");
        }
}

function showsize(size) {
	if (helpmode) {
		alert("文字大小标记\n设置文字大小.\n可变范围 1 - 6.\n 1 为最小 6 为最大.\n用法: [size="+size+"]这是 "+size+" 文字[/size]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[size="+size+"]"+text+"[/size]";
		AddText(AddTxt);
	} else {
		txt=prompt("大小 "+size,"文字");
		if (txt!=null) {
			AddTxt="[size="+size+"]"+txt;
			AddText(AddTxt);
			AddTxt="[/size]";
			AddText(AddTxt);
		}
	}
}

function showcolor(color) {
	if (helpmode) {
		alert("颜色标记\n设置文本颜色.  任何颜色名都可以被使用.\n用法: [span class="+color+"]颜色要改变为"+color+"的文字[/span]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[span class="+color+"]"+text+"[/span]";
		AddText(AddTxt);
	} else {  
     	txt=prompt("选择的颜色是: "+color,"文字");
		if(txt!=null) {
			AddTxt="[span class="+color+"]"+txt;
			AddText(AddTxt);
			AddTxt="[/span]";
			AddText(AddTxt);
		}
	}
}

function bold() {
	if (helpmode) {
		alert("加粗标记\n使文本加粗.\n用法: [b]这是加粗的文字[/b]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[b]"+text+"[/b]";
		AddText(AddTxt);
	} else {
		txt=prompt("文字将被变粗.","文字");
		if (txt!=null) {
			AddTxt="[b]"+txt;
			AddText(AddTxt);
			AddTxt="[/b]";
			AddText(AddTxt);
		}
	}
}

function italicize() {
	if (helpmode) {
		alert("斜体标记\n使文本字体变为斜体.\n用法: [i]这是斜体字[/i]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[i]"+text+"[/i]";
		AddText(AddTxt);
	} else {
		txt=prompt("文字将变斜体","文字");
		if (txt!=null) {
			AddTxt="[i]"+txt;
			AddText(AddTxt);
			AddTxt="[/i]";
			AddText(AddTxt);
		}
	}
}

function underline() {
  	if (helpmode) {
		alert("下划线标记\n给文字加下划线.\n用法: [u]要加下划线的文字[/u]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[u]"+text+"[/u]";
		AddText(AddTxt);
	} else {
		txt=prompt("下划线文字.","文字");
		if (txt!=null) {
			AddTxt="[u]"+txt;
			AddText(AddTxt);
			AddTxt="[/u]";
			AddText(AddTxt);
		}
	}
}

function center() {
 	if (helpmode) {
		alert("对齐标记\n使用这个标记, 可以使文本左对齐、居中、右对齐.\n用法: [align=center|left|right]要对齐的文本[/align]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[align=center]"+text+"[/align]";
		AddText(AddTxt);
	} else {
		txt2=prompt("对齐样式\n输入 'center' 表示居中, 'left' 表示左对齐, 'right' 表示右对齐.","center");
		while ((txt2!="") && (txt2!="center") && (txt2!="left") && (txt2!="right") && (txt2!=null)) {
			txt2=prompt("错误!\n类型只能输入 'center' 、 'left' 或者 'right'.","");
		}
		txt=prompt("要对齐的文本","文本");
		if (txt!=null) {
			AddTxt="\r[align="+txt2+"]"+txt;
			AddText(AddTxt);
			AddTxt="[/align]";
			AddText(AddTxt);
		}
	}
}

function quote() {
	if (helpmode){
		alert("引用标记\n引用一些文字.\n用法: [quote]引用内容[/quote]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[quote]"+text+"[/quote]";
		AddText(AddTxt);
	} else {
		txt=prompt("被引用的文字","文字");
		if(txt!=null) {
			AddTxt="[quote]"+txt;
			AddText(AddTxt);
			AddTxt="[/quote]";
			AddText(AddTxt);
		}
	}
}

function image() {
	if (helpmode){
		alert("图片标记\n插入图片\n用法: [img]http:\/\/www.99mill.com\/images\/php.gif[/img]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[img]"+text+"[/img]";
		AddText(AddTxt);
	} else {
		txt=prompt("图片的 URL","http://");
		if(txt!=null) {
			AddTxt="\r[img]"+txt;
			AddText(AddTxt);
			AddTxt="[/img]";
			AddText(AddTxt);
		}
	}
}


function code() {
	if (helpmode) {
		alert("代码标记\n使用代码标记,可以使你的程序代码里面的 html 等标志不会被破坏.\n使用方法:\n [code]这里是代码文字[/code]");
	} else if (nomode || document.selection && document.selection.type == "Text") {
		AddTxt="[code]"+text+"[/code]";
		AddText(AddTxt);
	} else {
		txt=prompt("输入代码","");
		if (txt!=null) { 
			AddTxt="[code]"+txt;
			AddText(AddTxt);
			AddTxt="[/code]";
			AddText(AddTxt);
		}
	}
}

//////////////////////////////////
function copytext(theField) 
{
	var tempval=eval("document."+theField);
	tempval.focus();
	tempval.select();
	therange=tempval.createTextRange();
	therange.execCommand("Copy");
}

function replac()
{
	if (helpmode)
	{
		alert("替换关键字");
	}
	else
	{
		txt2=prompt("请输入搜寻目标关键字","");
		if (txt2 != null)
		{
			if (txt2 != "") 
			{
				txt=prompt("关键字替换为:",txt2);
			}
			else
			{
				replac();
			}
			var Rtext = txt2; var Itext = txt; document.FORM.content.value = eval('FORM.content.value.replace("'+Rtext+'","'+Itext+'")');
		}
	}
}

function checklength(theform)
{
	alert("你的信息已经有 "+theform.content.value.length+" 字节.");
}
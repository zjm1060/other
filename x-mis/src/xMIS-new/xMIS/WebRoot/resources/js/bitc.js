function doKey(type) {
	// alert(event.keyCode);
	/*
	 * if(event.keyCode == 45){ alert("The Value should be POSITIVE number!");
	 * event.keyCode = 0; event.returnvalue = false; return false; }
	 */

	if (type == 'idCard') {
		if (event.keyCode < 48 || event.keyCode > 57) {
			alert("身份证号码必须是数字!");
			event.keyCode = 0;
			event.returnvalue = false;
			return false;
		}
	} else if (type == 'nubmer') {
		if (event.keyCode != 45 && (event.keyCode < 48 || event.keyCode > 57)) {
			// alert("必须是数字!");
			event.keyCode = 0;
			event.returnvalue = false;
			return false;
		}
	} else if (type == 'float') {

		if (event.keyCode != 46 && event.keyCode != 13) {
			if (event.keyCode != 13) {
				if (event.keyCode < 48 || event.keyCode > 57) {
					// alert("必须是数字!");
					event.keyCode = 0;
					event.returnvalue = false;
					return false;
				}
			}
		}
	}

}

function checkDateTime(obj) {

	if (obj.value == "")
		return true;
	var strTime = obj.value;
	var dateReg = /^(\d{1,4})(-|\/)(\d{1,2})\2(\d{1,2})$/;
	// return dateReg.test(strTime.trim());
	if (dateReg.test(strTime.trim()))
		return true;
	else {
		alert("日期设置无效，请按 “年-月-日” 格式设置！");
		obj.value = "";
		obj.focus();
		// document.all.birthday.focus();
		return false;
	}
}

function limitCalculate(sourceFieldName, spanFieldName, limitFieldName,
		unitFieldName, hiddenFileName) {

	var spanObj, sourceObj, limitObj, unitObj, hiddenObj, result, type;
	spanObj = MM_findObj(spanFieldName);
	sourceObj = MM_findObj(sourceFieldName);
	limitObj = MM_findObj(limitFieldName);
	unitObj = MM_findObj(unitFieldName);
	hiddenObj = MM_findObj(hiddenFileName);

	initCurrencyField(limitObj);

	var unitName = unitObj.options[unitObj.selectedIndex].name;
	type = 0;

	if (unitName == "日") {
		type = 4;
	} else if (unitName == "月") {
		type = 5;
	} else if (unitName == "年") {
		type = 6;
	}

	result = (sourceObj.value.trim() == "" || limitObj.value.trim() == "")
			? ""
			: addDate(
				type, limitObj.value, sourceObj.value);

	hiddenObj.value = result;
	spanObj.innerHTML = result;
}

function addDate(type, NumDay, vdate) {

	var date = new Date();

	date.setFullYear(
		parseInt(
			vdate.substring(
				0, 4), 10), parseInt(
			vdate.substring(
				5, 7), 10) - 1, parseInt(
			vdate.substring(
				8, 10), 10));

	type = parseInt(type) // 类型
	var lIntval = parseInt(NumDay)// 间隔
	switch (type) {
		case 6 :// 年
			date.setYear(date.getYear() + lIntval)
			break;
		case 7 :// 季度
			date.setMonth(date.getMonth() + (lIntval * 3))
			break;
		case 5 :// 月
			date.setMonth(date.getMonth() + lIntval)
			break;
		case 4 :// 天
			date.setDate(date.getDate() + lIntval);
			break;
		case 3 :// 时
			date.setHours(date.getHours() + lIntval)
			break
		case 2 :// 分
			date.setMinutes(date.getMinutes() + lIntval)
			break
		case 1 :// 秒
			date.setSeconds(date.getSeconds() + lIntval)
			break;
		default :

	}

	return date.getYear()
			+ '-'
			+ ((date.getMonth() + 1) < 10 ? '0' + (date.getMonth() + 1) : (date
					.getMonth() + 1)) + '-'
			+ (date.getDate() < 10 ? '0' + date.getDate() : date.getDate());
	// return date.getYear() +'-' + (date.getMonth()+1) + '-' +date.getDate()+ '
	// '+ date.getHours()+':'+date.getMinutes()+':'+date.getSeconds()
}

function convertDateObjToFormatStr(dateValue) {

	return dateValue == "" ? "" : dateValue.format('Y-m-d');

}

function dateCompare(begDate, endDate) {

	// alert(new Date(Date.parse(endDate)).getTime()-new
	// Date(Date.parse(begDate)).getTime());

	if (new Date(Date.parse(endDate)).getTime() < new Date(Date.parse(begDate))
			.getTime())
		return false;
	else
		return true;

}

function dispatchRequest(req) {
	alert(req.responseText || '0');
	parent.location.reload();
	location.reload(true);
}

function tipMessage(req) {
	alert(req.responseText || '');
}

function f_check_float(obj) {
	if (/^(\+|-)?\d+($|\.\d+$)/.test(obj.value)) {
		return true;
	} else {
		return false;
	}
}

function customer_formatDate(val) {
	return val ? val.trim().substring(
		0, 10) : '';
}

function nameLinker(val) {
	if (!val) {
		return '';
	}
	var text = val.substring(
		0, val.indexOf("^"));
	var url = val.substring(
		val.indexOf("^") + 1, val.length);
	if (text != '0') {
		return '<a href="' + url + '">' + text + '</a>';
	} else
		return text;
}

function biuldLinker(val) {
	if (!val) {
		return;
	}
	val = val + '';
	if (val.indexOf("^") > 0) {
		var text = val.substring(
			0, val.indexOf("^"));
		var url = val.substring(
			val.indexOf("^") + 1, val.length);
		if (text != "0") {
			return "<a href='javascript:;' onclick=\"javascript:parent.xMis.turn('0000','业务查询','"
					+ url + "');\">" + text + "</a>";
		} else {
			return text;
		}
	} else {
		return val;
	}
}

function convertNameWithoutLinker(val) {
	if (!val) {
		return '';
	}
	val = val + '';
	if (val.indexOf("^") > 0) {
		return val.substring(
			0, val.indexOf("^"));
	} else {
		return val;
	}
}

function biuldTip(title, val) {
	val = val + '';
	var extTip = "ext:qtitle='" + title + "'" + " ext:qtip='";

	// var extTip = "ext:qtip='";

	return (val.indexOf("^") > 0) ? extTip + val.substring(
		0, val.indexOf("^")) + "'" : extTip + val + "'";
}

function formatCurrency(format, val, color) {
	if (!val) {
		return '';
	}
	
	format = (format == null || format == "") ? "" : format;
	
	format = (format == '￥') ? "" : format;

	format = (format == '欧元') ? "€" : format;

	var v, urlFlag, fontColor, onclickStr;
	if ((val + '').indexOf("^") > 0) {
		var text = val.substring(
			0, val.indexOf("^"));
		var url = val.substring(
			val.indexOf("^") + 1, val.length);

		// return "<a href=\"" + url + "\">" + text + "</a>";
		v = text;

		onclickStr = url.indexOf("onClick") == 0 ? url : "";
		url = url.indexOf("onClick") == 0 ? "javascript:;" : url;
		urlFlag = true;
	} else {
		v = val;
		urlFlag = false;
	}

	if (format == "null")
		return v;

	v = (Math.round((v - 0) * 100)) / 100;
	v = (v == Math.floor(v)) ? v + ".00" : ((v * 10 == Math.floor(v * 10)) ? v
			+ "0" : v);
	v = String(v);
	var ps = v.split("."), whole = ps[0], sub = ps[1] ? "." + ps[1] : ".00", r = /(\d+)(\d{3})/;
	while (r.test(whole)) {
		whole = whole.replace(
			r, "$1,$2");
	}
	v = whole + sub;
	v = format + v;
	v = (val == 0 || text == 0) ? "--" : v;
	fontColor = "<font color=" + color + ">";
	if (color == ""){
		return urlFlag
				? "<a href=" + url + " " + onclickStr + ">" + v + "</a>"
				: v;
	}else if(color == "red"){
			return urlFlag ? "<a href=" + url + " " + onclickStr + ">" 
				+ "<img src='"+__cxtPath+"/resources/images/exclamation.gif' style='width:16px; height:16px;'/>"
				+ fontColor
				+ v + "</font></a>" : fontColor + v + "</font>";
		
	}
	else if(color == "green"){
			return urlFlag ? "<a href=" + url + " " + onclickStr + ">" 
				+ "<img src='"+__cxtPath+"/resources/images/dollar.png' style='width:16px; height:16px;'/>"
				+ fontColor
				+ v + "</font></a>" : fontColor + v + "</font>";
	}			
	else
		return urlFlag ? "<a href=" + url + " " + onclickStr + ">" 
				+ fontColor
				+ v + "</font></a>" : fontColor + v + "</font>";

}

function convertUserName(val) {
	return val == "未知" ? "" : val;
}

function gridColOnClick(val) {
	var v, urlFlag, fontColor, onclickStr;
	if (val.indexOf("^") > 0) {
		var text = val.substring(
			0, val.indexOf("^"));
		var url = val.substring(
			val.indexOf("^") + 1, val.length);

		// return "<a href=\"" + url + "\">" + text + "</a>";
		v = text;

		onclickStr = url.indexOf("onClick") == 0 ? url : "";
		url = url.indexOf("onClick") == 0 ? "#" : url;
		urlFlag = true;
	} else {
		v = val;
		urlFlag = false;
	}
	return urlFlag ? "<a href=" + url + " " + onclickStr + ">" + v + "</a>" : v;

}

function showContractInfoSimple(val, contractID) {

	// alert(val);
	if (val == null || val == '')
		return '';

	val = convertNameWithoutLinker(val) + '^onClick=showContractWin('
			+ contractID + ')';

	var v, urlFlag, fontColor, onclickStr;
	if (val.indexOf("^") > 0) {
		var text = val.substring(
			0, val.indexOf("^"));
		var url = val.substring(
			val.indexOf("^") + 1, val.length);

		// return "<a href=\"" + url + "\">" + text + "</a>";
		v = text;

		onclickStr = url.indexOf("onClick") == 0 ? url : "";
		url = url.indexOf("onClick") == 0 ? "#" : url;
		urlFlag = true;
	} else {
		v = val;
		urlFlag = false;
	}
	return urlFlag ? "<a href=" + url + " " + onclickStr + ">" + v + "</a>" : v;

}

function setWarningBackgroudColor(warningDate, warningAmt, warningDays,
		currenyGraph,val,baseNum) {

	var colorStr = "", tipTitle = "";
	var tipMessage = "预计收款日期：" + customer_formatDate(warningDate)
			+ "<br/>预计收款金额：" + formatCurrency(
				currenyGraph, warningAmt, "");
	switch (warningDays) {
		case Type_Warning_Earlier_30_Days :
			colorStr = Color_BG_Warning_Earlier_30_Days;
			tipTitle = Tip_Title_Warning_Earlier_30_Days;
			break;
		case Type_Warning_Earlier_15_Days :
			colorStr = Color_BG_Warning_Earlier_15_Days;
			tipTitle = Tip_Title_Warning_Earlier_15_Days;
			break;
		case Type_Warning_Earlier_7_Days :
			colorStr = Color_BG_Warning_Earlier_7_Days;
			tipTitle = Tip_Title_Warning_Earlier_7_Days;
			break;
		case Type_Warning_Current_Date :
			colorStr = Color_BG_Warning_Current_Date;
			tipTitle = Tip_Title_Warning_Current_Date;
			break;
		case Type_Warning_Overtime_Current_Date :
			colorStr = Color_BG_Warning_Overtime_Current_Date;
			tipTitle = Tip_Title_Warning_Overtime_Current_Date;
			break;
		case Type_Warning_Overtime_More_7_Days :
			colorStr = Color_BG_Warning_Overtime_More_7_Days;
			tipTitle = Tip_Title_Warning_Overtime_More_7_Days;
			tipMessage = delayReason_format(warningDate,warningAmt,currenyGraph,val,baseNum);
			break;
		case Type_Warning_Overtime_More_15_Days :
			colorStr = Color_BG_Warning_Overtime_More_15_Days;
			tipTitle = Tip_Title_Warning_Overtime_More_15_Days;
			tipMessage = delayReason_format(warningDate,warningAmt,currenyGraph,val,baseNum);
			break;
		case Type_Warning_Overtime_More_30_Days :
			colorStr = Color_BG_Warning_Overtime_More_30_Days;
			tipTitle = Tip_Title_Warning_Overtime_More_30_Days;
			tipMessage = delayReason_format(warningDate,warningAmt,currenyGraph,val,baseNum);
			break;
		case Type_Warning_Info :
			tipTitle = Tip_Title_Warning_Info;
			break;
		case Type_Warning_Without_Definition :
			colorStr = Color_BG_Warning_Without_Definition;
			tipTitle = Tip_Title_Warning_Without_Definition;
			break;

	}

	// var extTip = colorStr==""?"":biuldTip(tipTitle, tipMessage);

	return colorStr == "" ? biuldTip(
		tipTitle, tipMessage) : "style=background-color:" + colorStr + " "
			+ biuldTip(
				tipTitle, tipMessage);
}

function delayReason_format(warningDate,amt,currenyGraph,val,baseNum){
	//默认每行13个字符
	if(undefined == baseNum){
		baseNum = 13;
	}
	if(undefined == val||val==''){
		val = "暂无信息";
	}
	var tipMessage = "预计收款日期：" + customer_formatDate(warningDate) + "<br/>";
	var tipWarningAmt = "预计收款金额：" + formatCurrency(currenyGraph, amt, "") + "<br/>";
	var tipReason = "付款推迟原因："+ val;
	var tipReason_format = "";
	for(var i = 0 ; i< tipReason.length/baseNum ; i++){
		tipReason_format += tipReason.substr(i*baseNum,baseNum) + "<br/>";
	}
	return tipMessage + tipWarningAmt + tipReason_format;
}





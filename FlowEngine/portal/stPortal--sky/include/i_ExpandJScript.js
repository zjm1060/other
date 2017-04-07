function escapeURL(a_sURL,a_sFind)
{
	var sURL,sFind,ay,i;
	sFind = (a_sFind == null) ? 'hidParent=' : a_sFind;
	ay = a_sURL.split(sFind);
	if (ay.length < 2)
		return a_sURL;
	sURL = '';
	for (i=1;i<ay.length;i++)
	{
		if (sURL != '')
			 sURL += sFind;
		sURL += ay[i];
	}
	sURL = escape(sURL);
	sURL = ay[0] + sFind + sURL;
	return sURL;
}

function Jump2List(strFormName,strNextPage,strPara)
{
	var aryPara,i,strTemp,form;
	aryPara = strPara.toString().split(",");

	if (document.all){
		form = document.all.item(strFormName);
	}else{
		form = document.forms[strFormName];
	}
	if (form == null){
		alert('找不到表单对象：' + strFormName);
		return;
	}

	strTemp = "";
	for (i=0;i<aryPara.length;i+=2)
	{
		if (aryPara[i] == null || aryPara[i] == '') continue;

		if (document.all){
			form.item(aryPara[i]).value = aryPara[i+1];
		}else{
			form.elements[aryPara[i]].value = aryPara[i+1];
		}
		strTemp += aryPara[i] + "=" + aryPara[i+1] + ",";
	}
	strTemp = strTemp.substr(0,strTemp.length - 1);

	form.action = strNextPage;
	form.submit();
}

function Jump2New(frmActive,sFlag,sParam)
{
	document.forms[frmActive].hidFlag.value = sFlag;
	document.forms[frmActive].hidParam.value = sParam;
	javascript:Jump2List(frmActive,'Routine_Addnew.jsp','');
}

function Jump2Day(frmActive,sParam)
{
	document.forms[frmActive].hidDay.value = sParam;
	javascript:Jump2List(frmActive,'RoutineMain_ByDay.jsp','');
}

function goback(nI)
{
	if ( history.length >0  )
	{
		for(nJ=0;nJ<nI;nJ++)
		{
			history.back();
		}
	}
}

function Trim(a_strVal)
{
	return(a_strVal.replace(/^\s*|\s*$/g,''));
}

function CheckDate(a_date)  // 判断输入的日期是否有效
{
	a_date = a_date.replace(/\-/g,"/");
	var ayDate = a_date.split("/");
	if (ayDate.length != 3)
		return false;
	var sY_M_D = checkday(ayDate[0],ayDate[1],ayDate[2]);
	if ( sY_M_D == "" )
		return true;
	alert(sY_M_D);
	var sY_D_M = checkday(ayDate[0],ayDate[2],ayDate[1]);
	if ( sY_D_M == "")
		return true;
	return false;
}

function checkday(year,month,day)
{

	if (year != Trim(year) || isNaN(year) || (year < 1900)) {
		return "年不合法";
	}
	if (month != Trim(month) || isNaN(month) || (month < 1) || (month > 12)){
		return "月不合法";
	}
	if (day != Trim(day) || isNaN(day) || (day < 1) || (day > 31)){
		return "日不合法";
	}
	switch (parseInt(month)){
		case 2:
			high =28;
			if ((year % 4 == 0) && (year % 100 != 0))
				{high =29;}
			else if (year % 400 == 0) {high=29;}
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			high =31;
			break;
		default:
			high =30;
	}
	if ((day < 1) || (day > high)){
		return "日期不合法";
	}
	return "";
}

function Parse2String(sStr)
{
	var aTmp,aCasual;
	var aItemCode = new Array();
	var aItemDesc = new Array();
	var nLen;

	aTmp = sStr.split(";");

	nLen = aTmp.length;

	for(i = 0;i<nLen;i++)
	{
		aCasual = aTmp[i].split("=");
		aItemDesc[i] = aCasual[0];
		aItemCode[i] = aCasual[1];
	}

	return aItemCode;
}

function parse2Location(sItem)
{
	var aItem = new Array;
	var nCount;
	var sOutput;

	aItem = sItem.split(";");

	if (aItem.length == 1)
	{
		return sItem;
	}
	else
	{
		nCount = aItem.length;
		sOutput = aItem[nCount - 1];

		return sOutput;
	}
}

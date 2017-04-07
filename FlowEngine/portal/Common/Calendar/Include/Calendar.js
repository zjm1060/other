var sMon = new Array(12);
	sMon[0] = "January"
	sMon[1] = "February"
	sMon[2] = "March"
	sMon[3] = "April"
	sMon[4] = "May"
	sMon[5] = "June"
	sMon[6] = "July"
	sMon[7] = "August"
	sMon[8] = "September"
	sMon[9] = "October"
	sMon[10] = "November"
	sMon[11] = "December"

function calendar(t,LinkPage) {
		var sPath = LinkPage;
		strFeatures = "dialogWidth=206px;dialogHeight=180px;center:yes;edge:raised;scroll:no;status:no;unadorned:yes;help:no";
		st = t.value;
		sDate = showModalDialog(sPath,st,strFeatures);
		t.value = formatDate(sDate, 0);

}
function checkDate(t) {
	dDate = new Date(t.value);
	if (dDate == "NaN") {t.value = ""; return;}

	iYear = dDate.getFullYear()

	if ((iYear > 1899)&&(iYear < 1950)) {

		sYear = "" + iYear + ""
		if (t.value.indexOf(sYear,1) == -1) {
			iYear += 100
			sDate = (dDate.getMonth() + 1) + "/" + dDate.getDate() + "/" + iYear
			dDate = new Date(sDate)
		}
	}



	t.value = formatDate(dDate);
}

function formatDate(sDate) {
	var sScrap = "";
	var dScrap = new Date(sDate);
	if (dScrap == "NaN") return sScrap;

	iDay = dScrap.getDate();
	iMon = dScrap.getMonth();
	iYea = dScrap.getFullYear();

	sScrap = iYea + "-" + (iMon + 1) + "-" + iDay ;
	return sScrap;
}

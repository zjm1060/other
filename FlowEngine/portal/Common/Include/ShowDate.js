<SCRIPT language=JavaScript>

function showdate(syear,smon,sdate,sday){

var enabled = 0;

today = new Date();

var day;

var date;

date = '';



if(today.getDay()==0) day = "星期日"

if(today.getDay()==1) day = "星期一"

if(today.getDay()==2) day = "星期二"

if(today.getDay()==3) day = "星期三"

if(today.getDay()==4) day = "星期四"

if(today.getDay()==5) day = "星期五"

if(today.getDay()==6) day = "星期六"

if(syear){



if(today.getYear()>90){

date = today.getYear();

}else{

date = '20' + today.getYear();

}



date = date + "年";

}

if(smon){

date = date + (today.getMonth() + 1 ) + "月";

}

if(sdate){

date = date + today.getDate() + "日, ";

}

if(sday){

date = date + day +"";

}

document.write( date );

}

</SCRIPT>
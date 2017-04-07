<!--
  function showHideLayers()
		{
			 var i,visStr,args,theObj;
			 args=showHideLayers.arguments;
			 for(i=0;i<(args.length-2);i+=3)
				 {
				    visStr=args[i+2];
				    if(navigator.appName=='Netscape'&&document.layers!=null)
					    {
						    theObj=eval(args[i]);
						    if(theObj)theObj.visibility=visStr;
					     }
				    else if(document.all!=null)
					    {
						    if(visStr=='show')visStr='visible';
						    if(visStr=='hide')visStr='hidden';
						    theObj=eval(args[i+1]);
						    if(theObj)theObj.style.visibility=visStr;
					     }
				   }
			}


function check()
{ 
   if (document.search.words.value.length==0){
      alert("输入不能为空！");
	  document.search.words.focus();
   }else
       document.search.submit();
}


nereidFadeObjects = new Object();
nereidFadeTimers = new Object();

function nereidFade(object, destOp, rate, delta){
if (!document.all)
return
    if (object != "[object]"){
        setTimeout("nereidFade("+object+","+destOp+","+rate+","+delta+")",0);
        return;
    }
        
    clearTimeout(nereidFadeTimers[object.sourceIndex]);
    
    diff = destOp-object.filters.alpha.opacity;
    direction = 1;
    if (object.filters.alpha.opacity > destOp){
        direction = -1;
    }
    delta=Math.min(direction*diff,delta);
    object.filters.alpha.opacity+=direction*delta;

    if (object.filters.alpha.opacity != destOp){
        nereidFadeObjects[object.sourceIndex]=object;
        nereidFadeTimers[object.sourceIndex]=setTimeout("nereidFade(nereidFadeObjects["+object.sourceIndex+"],"+destOp+","+rate+","+delta+")",rate);
    }
}
//-->
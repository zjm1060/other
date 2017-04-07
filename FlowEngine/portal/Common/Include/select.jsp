<html>
<script>
	
 function canSend(){     
	if  (!CheckAction()){
		message = "没有填写发送必填字段";
		return false;
	}
		return true;
	}
  
  function trimStr(a_strVal){
   		return(a_strVal.replace(/^\s*|\s*$/g,""));
	}
	
	function canSave(){   
  	if  (!CheckAction()){
    	message = "没有填写发送必填字段";

	 		return false;
    	} 
		return true;
	}

	function getMessage(){
  		return message;
	}

	function send(){
		return true;
	}

	function save(){
		return true;
	}
    
    
function selectButton(){     
	var sFeatures;
  var sReturn = window.showModalDialog("Calendar3.htm",null,null);
  if (sReturn != null)
  {
    if (returnVal!=null && returnVal!="")
      returnVal.value = sReturn[0];
      window.parent.document.all.item("Main1_Text55").value=returnVal.value;
  }					  
}

</script>
<body   scroll="no">
<form action="select.jsp" target="_self" method="post" name="manager" id="manager" >
<table>
    <tr>                                                    
     <td>
     	<input type="button" name=""  value="选择" onclick="selectButton();">
     </td>    
    </tr>
</table>
<input type="hidden" name="Main1_Text55" value="">
</form>
</body>
</html>

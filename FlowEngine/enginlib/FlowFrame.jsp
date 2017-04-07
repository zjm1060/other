<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="flowengineweb.*" %>
<%@ page import="flowengineforjava.structure.WFConstants" %>
<%@ page import="flowengineforjava.util.RecordSet" %>
<%@ page import="flowengineweb.*" %>
<%@ page import="flowengineforjava.util.*" %>
<%@ page import="flowengineforjava.structure.*" %>
<%@ page import="flowengineweb.helper.*" %>
<%@ page import="java.util.*" %>
<%
long flowInid = -1;
long stepInid = -1;

WorkFlowHelper wfHelper=new WorkFlowHelper();
wfHelper.initialize(request,response,pageContext);

try{
  //取得路径变量
  String sCommonPath="",sImagesPath="",sPublicPath="";
  sCommonPath = SysPara.getCommonPath(request);
  sImagesPath = SysPara.getImagesPath(request);
  sPublicPath = SysPara.getPublicPath(request);
%>
<%
  int traceFlag=1;
  int otherTraceFlag=0;
  String script = "";
  String stepName = "";
  String attachFlag = "";
  boolean openState = false;

  String formTrace = wfHelper.getSysParaValue(WFConstants.WF_FORMTRACE);
  if(formTrace == null || !formTrace.trim().equals("1"))
    traceFlag = 0;
  String formOtherTrace=wfHelper.getSysParaValue(WFConstants.WF_FORMFLOWTRACE);
  if(formOtherTrace==null) formOtherTrace="0";
  if(formOtherTrace.trim().length()==0) formOtherTrace="0";
  if(formOtherTrace.equals("1"))
     otherTraceFlag=1;
  String sReadOnly=wfHelper.readParameter("readOnly","");
  int action = wfHelper.readParameterInt("action",0);
  long dealIndx=wfHelper.readParameterLong("dealIndx",-1);
  flowInid = wfHelper.readParameterLong("flowInid",-1);
  stepInid = wfHelper.readParameterLong("stepInid",-1);
  if(stepInid < 0){
    traceFlag=0;
    otherTraceFlag=0;
    String flows = wfHelper.getWorkflowHelper().getHaveRightFlows("start_acc");
    if((flows).indexOf("'" + flowInid + "'") == -1){
      out.println("你没有权限启动该流程！");
      return;
    }
  }
  try{
      if(action == 0){//新建表单或实例化表单
        while(stepInid<0){
          String tag = wfHelper.composeName(flowInid,stepInid,"tag");
          if(session.getAttribute(tag) == null){
            session.setAttribute(tag,"already exists");
            break;
          }//end if
          stepInid--;
        }//end while!!!!!!!!!!!!!!!!!!!!!
      }//end if
      //if(session.getAttribute(myFlowEngine.composeName(flowInid,stepInid,"openState")) != null)
      //   openState = true;
      if(!openState){
        wfHelper.getWorkflowHelper().loadData(flowInid,stepInid,dealIndx,wfHelper.readParameterMap());
        if(stepInid >= 0){
          String attachList = wfHelper.getWorkflowHelper().getAttachFlow("'" + String.valueOf(flowInid) + "'");
          String tag = wfHelper.composeName(flowInid,stepInid,"attachList");
          session.setAttribute(tag,attachList);
          if(!attachList.equals(";")){
            attachFlag = "（有附件）";
          }//end if
        }//end if
        //stepName = myFlowEngine.getStepName(flowInid,stepInid);
        RecordSet someInfoRs = (RecordSet)session.getAttribute(wfHelper.composeName(flowInid,stepInid,WFConstants.TAG_SOMEINFO_RS));
        stepName = (String)someInfoRs.getObject("step_name");
      }
  }catch(Exception e){
      System.out.println("打开表单失败，失败原因：" + e.getMessage());
      script = e.getMessage();
  }
  String urlPara = "action=0&flowInid=" + flowInid + "&stepInid=" + stepInid+"&dealIndx="+dealIndx+"&readOnly="+sReadOnly;
  String formTitle = stepName + attachFlag;
  String ifReload="0";
  ifReload = request.getParameter("ifReload");
  if (ifReload == null || ifReload.equals(""))
    ifReload = "0";
%>
<html>
<head>
<title><%=formTitle%></title>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<script type="text/javascript" src="<%=sCommonPath%>Include/Common.js"></script>
<script type="text/javascript">
var ifReload = <%=ifReload%>;
var stepName = '<%=stepName%>';
var gFlowInid = <%=flowInid%>;
var gStepInid = <%=stepInid%>;
var gDealIndex= <%=dealIndx%>
document.onkeydown = keyDownFun;

function window_unload()
{
  <%if(!openState){%>
    var url= "Recycle.jsp?flowInid=" + gFlowInid + "&stepInid=" + gStepInid;
    document.forms.item("myForm").method = "post";
    document.forms.item("myForm").target = "SubmitFrame";
    document.forms.item("myForm").action = url;
    document.forms.item("myForm").submit();
  <%}%>
}
function initWindow(){
  <%if(openState){%>
    AlertWindow("/FlowEngineWeb/workflow/Public/","您选择的公文已经打开或者还没有完全关闭，请稍后再试！");
    window.close();
    return;
  <%}%>
  var screenW = window.screen.width;
  var screenH	= window.screen.height;
  var objToolbar = document.all.item("Toolbar");
  var objFlowForm = document.all.item("FlowForm");
  try{
    var objTrace = document.all.item("TraceFrame");
    var objTitle = parent.frames("titleFrame").document.all.item("titleSpan");
    if(objTitle!=null)
      objTitle.innerText="<%=formTitle%>";
  }
  catch(e){}
  if((<%=traceFlag%>==1) && (objTrace!=null)){
    if(screenW == 800){
      objToolbar.style.height=100;
      objFlowForm.style.height=screenH-300;
      objTrace.style.width="100%";
      objTrace.style.height=150;
    }
    if(screenW >= 1024){
      objToolbar.style.height=100;
      objFlowForm.style.height=screenH-400;
      objTrace.style.width="100%";
      objTrace.style.height=200;
    }
  }else{
    if(screenW == 800){
      objToolbar.style.height=100;
      objFlowForm.style.height=screenH-150;
    }
    if(screenW >= 1024){
      objToolbar.style.height=100;
      objFlowForm.style.height=screenH-150;
    }
  }
  var url= "InitForm.jsp?flowInid=" + gFlowInid + "&stepInid=" + gStepInid;
  document.forms.item("myForm").method = "post";
  document.forms.item("myForm").target = "SubmitFrame";
  document.forms.item("myForm").action = url;
  document.forms.item("myForm").submit();
}
function keyDownFun(){
  if(event.keyCode==78 && event.ctrlKey)
    return false;
}
</script>
</head>
<%
  if(script != null && !script.equals("")){
    out.print(script);
  }else{
%>
<body  style="margin:0 0 0 0;overflow:hidden;" onload="javascript:initWindow();" onunload="javascript:window_unload();">
<%if(!openState){%>
<FORM name="myForm" action="">
  <iframe name="Toolbar" id="Toolbar" width="100%" marginWidth="0" marginHeight="0" frameBorder="0" src="Toolbar.jsp?<%=urlPara%>"></iframe>
  <iframe name="FlowForm" id="FlowForm" width="100%" marginWidth="0" marginHeight="0" frameBorder="0" src="FlowForm.jsp?<%=urlPara%>&traceFlag=<%=traceFlag%>&otherTraceFlag=<%=otherTraceFlag%>"></iframe>
  <%if(traceFlag == 1){%>
    <iframe id="TraceFrame" name="TraceFrame" marginWidth="0" marginHeight="0" frameBorder="0" width="0" height="0" src="FlowTrace2.jsp?flowInid=<%=flowInid%>&tableFlag=0"></iframe>
  <%}%>
  <iframe id="SubmitFrame" name="SubmitFrame" marginWidth="0" marginHeight="0" frameBorder="0" style="position:absolute;top:0px;left:0px;width:0px;height:0px;"></iframe>
</FORM>
<%}%>
</body>
<%}
}catch(Exception e){
  try{
    wfHelper.getWorkflowHelper().unloadData(flowInid,stepInid);
  }
  catch(Exception exx){
    exx.printStackTrace();
  }
}
%>
</html>

<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="jbportal.runBean"%>
<%@ page import="jbportal.DBUtilBean"%>
<%@ page import="com.minstone.portal.util.XMLWriter"%>
<%@ page import="java.io.StringReader"%>
<%@ page import="java.util.Vector" %>

<jsp:useBean id="dbutil" scope="page" class="jbportal.DBUtilBean" />
<jsp:useBean id="run" scope="page" class="jbportal.runBean"/>
<%!
  String strWebPartCC = "urn:schemas-microsoft-com:webpart";
  String l_sBankFilePath ;
  StringBuffer sDashboard;
  String URL;
  String SERVER_NAME;
  final static int PAGE_DBVIEW=0;
  final static int PAGE_DBCONTENT=1;
  final static int PAGE_DBEDIT=2;
  final static int PAGE_WPEDIT=3;
  final static int PAGE_NEWWP=4;
  final static int PAGE_DBLAYOUT=5;
  final static int PAGE_ADDWPURL=6;
  final static int PAGE_STOREASP=7;
  final static int PAGE_DASHBOARDCONTENTASP=8;
  final static int PAGE_NEWDB= 9;
 boolean  m_bIsNull=false;
 public String StoreGetPageURL(int pageIndex,String varItemID){
	String stPath;
	int i;
    String sRet="";
	switch(pageIndex){
        case 0:
			sRet = "dashboard.jsp?DashboardID=" + varItemID;
            break;
		case 1:
			sRet = "dbcontent.jsp?DashboardID=" + varItemID;
            break;
		case 2:
			sRet = "dbedit.jsp?DashboardID=" + varItemID;
            break;
		case 3:
			sRet = "wpedit.jsp?WebPartID=" + varItemID;
            break;
		case 4:
			sRet = "wpedit.jsp?DashboardID=" + varItemID + "&amp;Cmd=New&amp;ShowAppearanceSection=1&amp;ShowContentSection=1";
            break;
		case 5:
			sRet = "layout.jsp?DashboardID=" + varItemID;
            break;
		case 6:
          stPath = URL;
          i = stPath.length();
          while((stPath.substring(0,i).substring(i-2,i-1).equalsIgnoreCase("/")) && (i > 0)){
              i = i - 1;
          }

          stPath = stPath.substring(0,i);
          sRet = "http://" + SERVER_NAME + stPath + "dbutil.jsp?Cmd=AddWebPartURL%26DashboardID=" + varItemID;
          break;
		case 7:
			sRet = "PartUpdate.jsp";
            break;
		case 8:
			sRet = "DashboardContent.jsp";
            break;
		case 9:
			sRet = "dbedit.jsp?ParentID=" + varItemID + "&amp;Cmd=New";
            break;
        default:
          sRet ="";
	}

    return sRet;
}
//----------------------------------------------------------------------
// StoreGetRenderInfo
//
// Returns the dashboard properties along with a list of parts
// and their properties in XML format.
//
// Arguments:
//	varDashboardID - Dashboard ID
//	fIncPartsOnly - If true, only parts with IsIncluded = True will be
//		returned in the XML.  Otherwise, all parts will be returned.
//	xml - XMLDOMDocument object.  Out param.
//----------------------------------------------------------------------
public String StoreGetRenderInfo(runBean run,String varDashboardID,String varDashboardType){ //' As Integer
    String  s = "";
    try{
      s = run.GetWebPartXml(varDashboardID, varDashboardType );
    }catch(Exception e){
      System.out.println(e.getMessage());
    }
     if (s.indexOf("<a:response>") ==-1 ){
      DBUtilBean dbutil = new DBUtilBean();
      s = dbutil.getBlankXml();
      m_bIsNull=true;
    }
    return s;

}
//----------------------------------------------------------------------
// CreateDashboardView
//
// Creates the view.
//----------------------------------------------------------------------
public String CreateDashboardView(String varDashboardID){
    String stPattern;
    long iPart;
    boolean fAutoUpdate;
    String stTmp;
    DBUtilBean.NamespacePrefixes nsp;
    XMLWriter xmlObject = new XMLWriter();
    // Pull the namespace prefixes out of the XMLDOM.
    DBUtilBean l_oDbutil = new DBUtilBean();
    nsp = l_oDbutil.CacheNamespaces(sDashboard);
    // Create a "Settings" node.
    sDashboard = xmlObject.startElement(sDashboard,nsp.strDDFactory + "Settings");
    // Add some nodes for convenience.
    // We need the Dashboard URL in a couple of cases, so cache it now.
    stTmp = StoreGetPageURL(PAGE_DBVIEW, varDashboardID);
    sDashboard = xmlObject.addElement(sDashboard,nsp.strDDFactory + "DashboardURL",stTmp);
    stTmp = StoreGetPageURL(PAGE_DBCONTENT, varDashboardID);
    sDashboard = xmlObject.addElement(sDashboard,nsp.strDDFactory + "ContentURL",stTmp);

    stTmp = StoreGetPageURL(PAGE_DBLAYOUT, varDashboardID);
    sDashboard = xmlObject.addElement(sDashboard,nsp.strDDFactory + "LayoutURL",stTmp);

    stTmp = StoreGetPageURL(PAGE_DBEDIT, varDashboardID);
    sDashboard = xmlObject.addElement(sDashboard,nsp.strDDFactory + "EditDashboardURL",stTmp);

    stTmp = StoreGetPageURL(PAGE_STOREASP, varDashboardID);
    sDashboard = xmlObject.addElement(sDashboard,nsp.strDDFactory + "StoreASP",stTmp);

    // URLs to Remove, Restore, and Minimize parts.  Since some stores may have to append
    // some default arguments and others might not, we have to look for the start of
    // the QueryString ("?") in order to build the correct string.
    stTmp = StoreGetPageURL(PAGE_DBVIEW, varDashboardID);
    if (stTmp.indexOf("?") == -1)
            stTmp = stTmp + "?WebPartCmd=Remove";
    else
            stTmp = stTmp + "&amp;WebPartCmd=Remove";
    sDashboard = xmlObject.addElement(sDashboard,nsp.strDDFactory + "RemovePartURL",stTmp);

    if (stTmp.indexOf("?") == -1)
            stTmp = stTmp + "?WebPartCmd=Restore";
    else
            stTmp = stTmp + "&amp;WebPartCmd=Restore";
    sDashboard = xmlObject.addElement(sDashboard,nsp.strDDFactory + "RestorePartURL",stTmp);

    if (stTmp.indexOf("?") == -1)
        stTmp = stTmp + "?WebPartCmd=Minimize";
    else
        stTmp = stTmp + "&amp;WebPartCmd=Minimize";
    sDashboard = xmlObject.addElement(sDashboard,nsp.strDDFactory + "MinimizePartURL",stTmp);
    sDashboard = xmlObject.endElement(sDashboard,nsp.strDDFactory + "Settings");
    // Loop through each of the parts.
    iPart = 0;
    String l_sTmp;
    String l_sLeftStr;
    String l_sRightStr;
    String l_sHeadStr="";
    String l_sBodyStr="";
    String l_sEndStr="";
    Vector l_vResponse = new Vector();

    l_sRightStr = sDashboard.toString();
    int l_index = l_sRightStr.indexOf("<a:response>");
    if (l_index!=-1){
        l_sHeadStr = l_sRightStr.substring(0,l_index);
        l_sRightStr = l_sRightStr.substring(l_index,l_sRightStr.length());
    }
    l_index = l_sRightStr.indexOf("</a:response>");
    while (l_index != -1){
        l_sLeftStr = l_sRightStr.substring(0,l_index);
        //System.out.println("left:" + l_sLeftStr);
        l_sRightStr = l_sRightStr.substring(l_index+13,l_sRightStr.length());
        l_vResponse.add(l_sLeftStr);
        l_index = l_sRightStr.indexOf("</a:response>");
   }
   l_sEndStr = l_sRightStr;

    for(int i=0;i<l_vResponse.size();i++){
       l_sTmp = l_vResponse.get(i).toString();
       l_sTmp = DBUtilBean.findReplace(l_sTmp,"</a:prop></a:propstat>","");
       stPattern = nsp.strDAV + "contentclass";

       if(l_sTmp.indexOf(stPattern) != -1){
            iPart +=1;

            stTmp = l_sTmp.substring(l_sTmp.indexOf("<" + nsp.strDDFactory + "ID" + ">")+6,l_sTmp.indexOf("</" + nsp.strDDFactory + "ID" + ">"));
            l_sTmp += "<" + nsp.strDDFactory + "WebPartIDEnc" + ">";
            l_sTmp += stTmp;
            l_sTmp += "</" + nsp.strDDFactory + "WebPartIDEnc" + ">";

            stTmp =  "WPQ" + iPart;
            l_sTmp += "<" + nsp.strDDFactory + "WebPartQualifier" + ">";
            l_sTmp += stTmp;
            l_sTmp += "</" + nsp.strDDFactory + "WebPartQualifier" + ">";


            stTmp= DBUtilBean.Transform(l_sTmp);//生成<table....
            l_sTmp += "<" + nsp.strDDFactory + "PartHTML" + ">";
            l_sTmp += stTmp;
            l_sTmp += "</" + nsp.strDDFactory + "PartHTML" + ">";

       }
       l_sBodyStr += l_sTmp + "</a:prop></a:propstat></a:response>";
    }
    String l_sRetStr;
    l_sRetStr = l_sHeadStr + l_sBodyStr+ l_sEndStr;
    l_sRetStr += "</a:multistatus>";
    return l_sRetStr ;
}
%>
<%
	String varDashboardID;
	String varDashboardType;
	String stPartCmd;
	String stPartID;
    //init初始化相关变量Begin...
    URL = request.getRequestURI();
    SERVER_NAME = request.getServerName();
    //init初始化相关变量End
    //' Retrieve the dashboard identifier from the QueryString.
    varDashboardID = request.getParameter("DashboardID");
    varDashboardType = request.getParameter("DashboardType");
    l_sBankFilePath = application.getRealPath("blank.xml");
    // Initialize the store.
    // Create an XML DOM object that will hold our dashboard's XML.
    //Fetch the Dashboard XML from the store.
    sDashboard = new StringBuffer(StoreGetRenderInfo(run,varDashboardID , varDashboardType));
    String l_sShow = CreateDashboardView(varDashboardID);
    //System.err.println("show="+l_sShow);
    StringReader reader=new StringReader(l_sShow);
    String xsl =application.getRealPath("//stPortal//part.xsl");
    jbportal.PortalUtil.convertToHtml(reader,xsl,out);
    reader.close();
    reader=null;
 %>

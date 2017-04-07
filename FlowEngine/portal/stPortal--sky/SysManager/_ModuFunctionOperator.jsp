<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<%
    String m_Msg = "";
    Hashtable dct_Para = new Hashtable();
    Hashtable dct_Key = new Hashtable();
    String m_Title = "";
    String m_href = "";
    String m_Description = "";
    String m_href_desc = "";
    String m_AllowMinimize = "";
    String m_Width = "";
    String m_Zone = "";
    String m_HeightUnitType = "";
    String m_HeightEnabled = "";
    String m_IsVisible = "";
    String m_AllowRemove = "";
    String m_IsIncluded = "";
    String m_WidthEnabled = "";
    String m_WidthUnitType = "";
    String m_Height = "";
    String m_HasFrame = "";
    String m_FrameState = "";
    String m_RequiresIsolation = "";
    String HeightUnitType = "";
    String WidthUnitType = "";
    String sReturn = "";

    String ShowDBError = "";
    int iReturn = 100;

	String m_modu_id = request.getParameter("modu_id");
	String m_func_id = request.getParameter("func_id");
	String ms_BackURL = request.getParameter("BackURL");
    ms_BackURL = request.getParameter("ms_BackURL");
    if (ms_BackURL == null)
        ms_BackURL = "";

    m_Zone = request.getParameter("Zone");
    if (m_Zone == null)
        m_Zone = "";

    HeightUnitType = request.getParameter("HeightUnitType");
    if (HeightUnitType == null)
        HeightUnitType = "";

    WidthUnitType = request.getParameter("WidthUnitType");
    if (WidthUnitType == null)
        WidthUnitType = "";

	dct_Key.put("func_id",m_func_id);
	dct_Key.put("modu_id",m_modu_id);

	dct_Para.put("AllowMinimize",PortalUtil.toDouble(request.getParameter("AllowMinimize")));
	dct_Para.put("Width",PortalUtil.toDouble(request.getParameter("Width")));
	dct_Para.put("Zone",PortalUtil.toDouble(m_Zone));
	dct_Para.put("HeightUnitType",HeightUnitType);
	dct_Para.put("HeightEnabled",PortalUtil.toDouble(request.getParameter("HeightEnabled")));
	dct_Para.put("IsVisible",PortalUtil.toDouble(request.getParameter("IsVisible")));
	dct_Para.put("AllowRemove",PortalUtil.toDouble(request.getParameter("AllowRemove")));
	dct_Para.put("IsIncluded",PortalUtil.toDouble(request.getParameter("IsIncluded")));
	dct_Para.put("WidthEnabled",PortalUtil.toDouble(request.getParameter("WidthEnabled")));
	dct_Para.put("WidthUnitType",WidthUnitType);
	dct_Para.put("Height",PortalUtil.toDouble(request.getParameter("Height")));
	dct_Para.put("HasFrame",PortalUtil.toDouble(request.getParameter("HasFrame")));
	dct_Para.put("FrameState",PortalUtil.toDouble(request.getParameter("FrameState")));
	dct_Para.put("RequiresIsolation",PortalUtil.toDouble(request.getParameter("RequiresIsolation")));
    dct_Para.put("RefreshInterval",PortalUtil.toDouble(request.getParameter("RefreshInterval")));

	iReturn = oSys.update(dct_Key,dct_Para,"modu_func",false);

	//if (iReturn >=0)
		response.sendRedirect("funcList.jsp?modu_id=" + m_modu_id);
	//else
	//	ShowDBError = "update";

%>


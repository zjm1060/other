<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page import="jbportal.PortalUtil,java.util.Hashtable"%>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />

<%
          String Msg = "";
          Hashtable dct_Para = new Hashtable();
          Hashtable dct_Key = new Hashtable();

          String m_Title = "";
          String m_href = "";
          String m_Description = "";
          String m_href_desc = "";
          String m_AllowMinimize = "";
          String m_Width = "";
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
          String sReturn = "";

          String ShowDBError = "";
          int iReturn = 100;

          String m_usercode=(String)request.getRemoteUser();
          String m_username =oManager.getCurrentUserName();

	  //得到传递进来的参数

          String m_func_type = request.getParameter("func_type");
           if (m_func_type == null)
                m_func_type = "";

          String m_func_id = request.getParameter("func_id");
           if (m_func_id == null)
                m_func_id = "";

          String m_modu_type = request.getParameter("modu_type");
           if (m_modu_type == null)
                m_modu_type = "";

          String m_modu_id = request.getParameter("modu_id");
           if (m_modu_id == null)
                m_modu_id = "";

          String ms_BackURL = request.getParameter("BackURL");
           if (ms_BackURL == null)
                ms_BackURL = "";

	  String m_Source = request.getParameter("Source");  //表明来源，如果m_Source="stportal"则要特殊处理
           if (m_Source == null)
                m_Source = "";

          String HeightUnitType = request.getParameter("HeightUnitType");
           if (HeightUnitType == null)
                HeightUnitType = "";

          String WidthUnitType = request.getParameter("WidthUnitType");
           if (WidthUnitType == null)
                WidthUnitType = "";

          String m_Zone = request.getParameter("Zone");
           if (m_Zone == null)
                m_Zone = "";

          String m_RefreshInterval = request.getParameter("RefreshInterval");
           if (m_RefreshInterval == null)
                m_RefreshInterval = "0";

          dct_Key.put("func_id",m_func_id);
          dct_Key.put("modu_id",m_modu_id);
          dct_Key.put("func_type",m_func_type);
          dct_Key.put("modu_type",m_modu_type);
          dct_Key.put("user_code",m_usercode);



          dct_Para.put("AllowMinimize",PortalUtil.toDouble(request.getParameter("AllowMinimize")));
          dct_Para.put("Width",PortalUtil.toDouble(request.getParameter("Width")));
          dct_Para.put("Zone",PortalUtil.toDouble(m_Zone));
          dct_Para.put("RefreshInterval",PortalUtil.toDouble(m_RefreshInterval));
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

          //如果是从STPortal来的则要做一个同步的工作
          iReturn = oManager.rsSyncModuFunc(m_modu_id,m_modu_type,m_func_id,m_func_type);

          if(iReturn < 0)
          {
            Msg = "updateError";
          }
          iReturn = oSys.update(dct_Key,dct_Para,"user_prof",false);
          if(iReturn >= 0)
            response.sendRedirect(ms_BackURL);
          else
            Msg = "updateError";
          response.sendRedirect(ms_BackURL);

%>

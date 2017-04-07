<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<%
          String Msg = "";
          String m_func_id = "";
          String modu_id = "";
          String BackURL = "";

          String AllowMinimize = "";
          String Width = "";
          String Zone = "";
          String HeightUnitType = "";
          String HeightEnabled = "";
          String IsVisible = "";
          String AllowRemove = "";
          String IsIncluded = "";
          String WidthEnabled = "";
          String WidthUnitType = "";
          String Height = "";
          String HasFrame = "";
          String FrameState = "";
          String RequiresIsolation = "";

          Hashtable dct_Para = new Hashtable();
          Hashtable dct_Key = new Hashtable();
          int sReturn = 100;

	 m_func_id = request.getParameter("func_id");
           if (m_func_id == null)
                  m_func_id = "";
           modu_id = request.getParameter("modu_id");
           if (modu_id == null)
                  modu_id = "";
           	 m_func_id = request.getParameter("func_id");
           if (m_func_id == null)
                  m_func_id = "";
           BackURL = request.getParameter("BackURL");
           if (BackURL == null)
                  BackURL = "";

           AllowMinimize = request.getParameter("AllowMinimize");
           if (AllowMinimize == null)
                AllowMinimize = "";
           Width = request.getParameter("Width");
           if (Width == null)
                Width = "";
           Zone = request.getParameter("Zone");
           if (Zone == null)
                Zone = "";
           HeightUnitType = request.getParameter("HeightUnitType");
           if (HeightUnitType == null)
                HeightUnitType = "";
           HeightEnabled = request.getParameter("HeightEnabled");
           if (HeightEnabled == null)
                HeightEnabled = "";
           IsVisible = request.getParameter("IsVisible");
           if (IsVisible == null)
                IsVisible = "";
           AllowRemove = request.getParameter("AllowRemove");
           if (AllowRemove == null)
                AllowRemove = "";
           IsIncluded = request.getParameter("IsIncluded");
           if (IsIncluded == null)
                IsIncluded = "";
           WidthEnabled = request.getParameter("WidthEnabled");
           if (WidthEnabled == null)
                WidthEnabled = "";
           WidthUnitType = request.getParameter("WidthUnitType");
           if (WidthUnitType == null)
                WidthUnitType = "";
           Height = request.getParameter("Height");
           if (Height == null)
                Height = "";
           HasFrame = request.getParameter("HasFrame");
           if (HasFrame == null)
                HasFrame = "";
           FrameState = request.getParameter("FrameState");
           if (FrameState == null)
                FrameState = "";
           RequiresIsolation = request.getParameter("RequiresIsolation");
           if (RequiresIsolation == null)
                RequiresIsolation = "";

	dct_Key.put("func_id",m_func_id);
	dct_Para.put("modu_id",modu_id);


	dct_Para.put("AllowMinimize",PortalUtil.toDouble(AllowMinimize));
	dct_Para.put("Width",PortalUtil.toDouble(Width));
	dct_Para.put("Zone",Zone);
	dct_Para.put("HeightUnitType",HeightUnitType);
	dct_Para.put("HeightEnabled",PortalUtil.toDouble(HeightEnabled));
	dct_Para.put("IsVisible",PortalUtil.toDouble(IsVisible));
	dct_Para.put("AllowRemove",PortalUtil.toDouble(AllowRemove));
	dct_Para.put("IsIncluded",PortalUtil.toDouble(IsIncluded));
	dct_Para.put("WidthEnabled",PortalUtil.toDouble(WidthEnabled));
	dct_Para.put("WidthUnitType",WidthUnitType);
	dct_Para.put("Height",PortalUtil.toDouble(Height));
	dct_Para.put("HasFrame",PortalUtil.toDouble(HasFrame));
	dct_Para.put("FrameState",PortalUtil.toDouble(FrameState));
	dct_Para.put("RequiresIsolation",PortalUtil.toDouble(RequiresIsolation));

	sReturn = oSys.update(dct_Key,dct_Para,"modu_func",true);

	if (sReturn >=0 )
          {
		response.sendRedirect("FunctionDetails.jsp");
          }
	else
          {
		Msg = "updateError";
	}


%>


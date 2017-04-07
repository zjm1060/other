<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page import="sun.jdbc.rowset.CachedRowSet" %>
<%@ page import="jbportal.PortalUtil" %>
<%@ page import="com.minstone.util.database.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    jbportal.FieldInfo  finfo=new jbportal.FieldInfo();
    finfo.setDEPTID("func_id");
    finfo.setDEPTNAME("title");
    finfo.setDEPTPARENTID("modu_id");
    finfo.setRECVALUE("func_id");
    finfo.setTOPVALUE("");
    finfo.setLOWFLAG("low_flag");//是否子节点
    finfo.setSELECTNAME("our_select");
    finfo.setMODUTYPE("Modu_Type");

	String m_sMenuString = "";
	RecordSetObject m_rsAll = null;
    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();

    String m_sSQL = "SELECT pers_modu.modu_id AS func_id, pers_modu.para_id AS modu_id, " +
			 "pers_modu.Title, pers_modu.DashBoardOrder AS seq, 0 AS modu_type " +
			 " FROM  pers_modu Where isnull(para_id,'')='' and user_code = '" + m_usercode + "' " +
			 " union (Select func_id,modu_id,Title,0 as seq,1 as modu_type from pers_func where user_code='" + m_usercode + "') " +
			 "  order by para_id asc,DashBoardOrder asc";


	m_rsAll = oManager.RunQuery ("","","","",true,false,m_sSQL);
	try
          {
	    m_sMenuString = PortalUtil.FillClass(m_rsAll,"NULL",finfo);
          }
          catch(Exception e)
          {
             throw e;
          }

%>

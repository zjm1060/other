package com.bitc.jk.xmis.web.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.MessageInfoService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class MessageInfoAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(MessageInfoAction.class);

	private MessageInfoService messageInfoService;
	
	public MessageInfoService getMessageInfoService() {
		return messageInfoService;
	}

	public void setMessageInfoService(MessageInfoService messageInfoService) {
		this.messageInfoService = messageInfoService;
	}

	public void getMessageInfoList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		
		EmployeeObj userObj = (EmployeeObj) request.getSession()
		.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
		
		
		// 接收人是登陆人； 消息状态；
		SimpleEmployee employee = WebUtil.getCurrentEmployee();
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String state = request.getParameter("state");
		StandardQuery stateQuery = SimpleParam.buildSimpleParam("state",
				Type.GetInt(state), Type.INTEGER, RelationalOperator.EQ);
		StandardQuery query = CompositParam
		.buildAndCompositParam()
		.addStandardQuery(
				SimpleParam.buildSimpleParam("receiver.userID",
						employee.getUserID(), Type.STRING,
						RelationalOperator.EQ))
		.addStandardQuery(stateQuery);
		String store = messageInfoService.getMessageListByQuery(query,pageInfo);
		WebUtil.returnResponse(response, store);
	}
	
	public void updateMessageInfoState(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String id = request.getParameter("id");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try{
			messageInfoService.doUpdateMessageInfoState(id);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("读取成功");
			ajaxResponse.setMessageTitle("成功");
		}catch (Exception e) {
			logger.error("报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("读取失败<br/>"+e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
}

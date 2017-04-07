package com.bitc.jk.xmis.export.web;

import java.io.File;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.sql.Date;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.export.service.ExportService;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.InitActionServlet;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class ExportAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(ExportAction.class);

	private ExportService exportService;

	public ExportService getExportService() {
		return exportService;
	}

	public void setExportService(ExportService exportService) {
		this.exportService = exportService;
	}

	/**
	 * 导出excel
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 * @throws Exception
	 */
	public ActionForward excel(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String webRoot =request.getSession().getServletContext().getRealPath("/");//获取web项目的路径
		
		String storeUrl = request.getParameter("storeUrl");

		// --/xMIS/contractMain.do?method=getContractListByType&contractTypeID=1
		String columnModes = request.getParameter("cms");
		JSONArray columnModeArray = JSONArray.fromObject(columnModes);

		String fomaters = request.getParameter("formater");
		JSONArray fomaterArray = JSONArray.fromObject(fomaters);
		// 将列名和对应处理放入 MAP
		Map<String, String> formaterMap = new HashMap<String, String>();
		for (int i = 0; i < fomaterArray.size(); i++) {
			String columnHeader = fomaterArray.getJSONObject(i).getString(
					"columnHeader");
			String formatClass = fomaterArray.getJSONObject(i).getString(
					"formatClass");
			formaterMap.put(columnHeader, formatClass);
		}

		// String beanName = getActionName(storeUrl);
		RequestWrapper requestWrapper = new RequestWrapper(request, storeUrl);
		requestWrapper.addExcept(getParams(storeUrl));

		StringWriter out = new StringWriter();
		ResponseWrapper responseWrapper = new ResponseWrapper(response,
				new PrintWriter(out));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		File tempFile = null;
		try {
			/*
			 * 使用标准的Action调用，但因为系统中在ActionServlet中加入权限查询条件所以在此不适用。 Action action
			 * = (Action) SpringInit.getBean(beanName, Action.class);
			 * action.execute(mapping, form, requestWrapper, responseWrapper);
			 */
			// 使用直接调用ActionServlet的方法。必须重写getServletContext方法，不然为null。
			// RequestWrapper中必须重写getRequestURI、getServletPath、getRequestURL、getQueryString几个方法。使ActionServlet能正确找到Action
			final ServletContext servletContext = request.getSession()
					.getServletContext();
			InitActionServlet servlet = new InitActionServlet() {
				private static final long serialVersionUID = 1498778352663576041L;

				@Override
				public ServletContext getServletContext() {
					return servletContext;
				}
			};
			servlet.process(requestWrapper, responseWrapper);
			String responseText = out.toString();
			JSONObject result = JSONObject.fromObject(responseText);

			if (result.getBoolean("success")) {
				JSONArray rowsArray = result.getJSONArray("rows");
				String key = UUIDUtils.nextCode() + ".xls";
				String name = request.getParameter("name");
				Date currentDateTime = DateUtil.getCurrentDateTime();
				String currentDateStr = currentDateTime.toString();
				String fileName = name + currentDateStr + "数据导出.xls";
				// 写入excel

				tempFile = WebUtil.newTempFile(request.getSession()
						.getServletContext(), key);
				PageInfo pageInfo = WebUtil.generatePageInfo(requestWrapper);
				exportService.writeBook(columnModeArray, rowsArray, tempFile,
						formaterMap, pageInfo,webRoot); // 传宽度的数组
				ajaxResponse.setSuccess(true);
				ajaxResponse.addAttribute("downloadpath",
						"/fileUpload.do?type=8&fileName=" + fileName + "&key="
								+ key);

			} else {
				throw new Exception("获取数据失败。");
			}
		} catch (Exception e) {
			logger.error("导出数据失败", e);
			if (tempFile != null && tempFile.exists()) {
				tempFile.delete();
			}
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("导出数据失败。请重试，如果问题依然存在请联系系统管理员。");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}

	/*
	 * private String getActionName(String url) { String path =
	 * StringUtils.split(url, '?')[0]; String actionName =
	 * StringUtils.substring(path, path.lastIndexOf('/'),
	 * path.lastIndexOf(".do")); return actionName; }
	 */

	private List<String[]> getParams(String url) {
		String paramStr = StringUtils.split(url, '?')[1];
		String[] params = StringUtils.split(paramStr, '&');
		List<String[]> list = new ArrayList<String[]>();
		for (String param : params) {
			String[] keyvalue = StringUtils.split(param, '=');
			list.add(new String[] { keyvalue[0], keyvalue[1] });
		}
		return list;
	}

}

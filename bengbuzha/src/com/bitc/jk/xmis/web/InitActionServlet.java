package com.bitc.jk.xmis.web;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URLEncoder;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Stack;

import javax.servlet.ServletConfig;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionServlet;
import org.logicalcobwebs.proxool.ProxoolFacade;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.sys.SysLog;
import com.bitc.jk.xmis.report.core.ReportService;
import com.bitc.jk.xmis.report.db.DBConnector;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.service.impl.LoginServiceImpl;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.exception.SecurityValidateException;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.action.LoginAction;
import com.bitc.jk.xmis.web.response.MessageResponse;

@SuppressWarnings({ "unchecked", "rawtypes" })
public class InitActionServlet extends ActionServlet {

	private static final long serialVersionUID = 3040232865065849129L;

	private static Logger logger = Logger.getLogger(InitActionServlet.class);

	public static String VERSION_NUMBER_KEY = "version";

	public static String BUILD_NUMBER_KEY = "builddate";

	public static String RELEASE_NUMBER;

	public static String VERSION_NUMBER;

	public static String BUILD_NUMBER;

	public static String Layout;

	public static Properties properties;

	public void init(ServletConfig config) throws ServletException {

		logger.info("---> init");
		super.init(config);
		try {
			InputStream cf = LoginAction.class.getClassLoader()
					.getResourceAsStream("properties/config.properties");
			Properties cfProp = new Properties();
			cfProp.load(cf);
			properties = cfProp;

			ServletContext servletContext = getServletContext();
			for (Iterator<Object> it = properties.keySet().iterator(); it
					.hasNext();) {
				String key = (String) it.next();
				servletContext.setAttribute(key, properties.getProperty(key));
			}
			String version = cfProp.getProperty("application.version");
			servletContext.setAttribute(
					WebKeys.Session_Application_Cfg_Version, version);
			String attachedPath = cfProp
					.getProperty("application.attachedPath");
			servletContext.setAttribute(
					WebKeys.Session_Application_Cfg_AttachedPath, attachedPath);
			new File(attachedPath).mkdirs();
			String tempfilePath = cfProp
					.getProperty("application.tempfilePath");
			servletContext.setAttribute(
					WebKeys.Session_Application_Cfg_TEMPFILEPATH, tempfilePath);
			new File(tempfilePath).mkdirs();
			logger.info("Init Appication Properties--->" + cfProp.toString());
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		VERSION_NUMBER = getInitParameter(VERSION_NUMBER_KEY);
		BUILD_NUMBER = getInitParameter(BUILD_NUMBER_KEY);
		RELEASE_NUMBER = VERSION_NUMBER + '.' + BUILD_NUMBER;

		Layout = getInitParameter(WebKeys.Session_Attribute_Name_Layout);

		initDBConnector();
	}

	private void initDBConnector() {
		ReportService reportService = (ReportService) SpringInit
				.getBean("reportService");
		DBConnector.init(reportService);
	}

	public void process(HttpServletRequest request, HttpServletResponse response)
			throws IOException, ServletException {

		boolean showMessage = true;
		try {
			request.getSession().setAttribute(
					WebKeys.Session_Attribute_Name_Layout, Layout);

			logger.info(request.getRequestURI());

			if (request.getRequestURI().indexOf("loginAction") < 0) {
				request.setAttribute(
						WebKeys.RequestKey_Original_URI,
						request.getRequestURI().substring(
								request.getContextPath().length()));
				request.setAttribute(WebKeys.RequestKey_Original_URL,
						URLEncoder.encode(request.getRequestURL().toString(),
								"UTF-8"));
				request.setAttribute(WebKeys.RequestKey_Original_QueryString,
						request.getQueryString());

				logger.info("URI = "
						+ request.getAttribute(WebKeys.RequestKey_Original_URI));
				logger.info("Url = " + request.getRequestURL().toString());
				logger.info("queryString = "
						+ request
								.getAttribute(WebKeys.RequestKey_Original_QueryString));

				EmployeeObj userObj = (EmployeeObj) request.getSession()
						.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
				String url = Type.getString(request
						.getAttribute(WebKeys.RequestKey_Original_URI))
						+ "?"
						+ Type.getString(request
								.getAttribute(WebKeys.RequestKey_Original_QueryString));
				boolean[] right = userObj.userAccessRightValidate(url);// 判断当前用户对url是否有权限
				if (!right[0]) {

					showMessage = right[3];

					throw new SecurityValidateException(
							userObj.getRescourceName(url));
				} else {
					Map dataScopeListMap = buildParamByDataScope(url, userObj);

					// List<QueryParam> paramList = buildParamByDataScope(url,
					// userObj);

					List<QueryParam> paramList = (List) dataScopeListMap
							.get(WebKeys.Request_Attribute_Name_DataScopeDefiniensList);
					// List bizLogicList =
					// (List)dataScopeListMap.get(WebKeys.Request_Attribute_Name_DataScopeDeptList);
					//
					// logger.info(bizLogicList);

					request.setAttribute(
							WebKeys.Request_Attribute_Name_DataScopeDeptList,
							WebUtil.buildJsonStoreByList((List) dataScopeListMap
									.get(WebKeys.Request_Attribute_Name_DataScopeDeptList)));

					logger.info(request
							.getAttribute(WebKeys.Request_Attribute_Name_DataScopeDeptList));

					request.setAttribute(
							WebKeys.Request_Attribute_Name_DataScopeQueryParam,
							paramList);

					List<StandardQuery> querys = WebUtil
							.buildStandardQuery(paramList);

					request.setAttribute(
							WebKeys.Request_Attribute_Name_AccessRight_StandardQuery,
							querys);
				}

				if (right[1]) {
					// 记录系统日志
					// saveLog(request);
				}

				// 保存当前请求的只读属性
				request.setAttribute(
						WebKeys.Request_Attribute_Name_ReadonlyLimit, right[2]);

			}
			request.getSession().setAttribute(WebKeys.RequestKey_Original_URI,
					request.getAttribute(WebKeys.RequestKey_Original_URI));
			request.getSession().setAttribute(WebKeys.RequestKey_Original_URL,
					request.getAttribute(WebKeys.RequestKey_Original_URL));
			request.getSession()
					.setAttribute(
							WebKeys.RequestKey_Original_QueryString,
							request.getAttribute(WebKeys.RequestKey_Original_QueryString));

			super.process(request, response);
		} catch (SecurityValidateException se) {
			se.printStackTrace();

			if (showMessage)
				WebUtil.returnResponse(response, new MessageResponse(
						SecurityValidateException.securityValidateMesTitle,
						se.securityValidateMessage).toJsonString());
		} catch (Exception e) {
			//2013-12-13
			logger.error(e.getMessage(),e);
			//如果是ajax请求
			if (request.getHeader("x-requested-with") == null) {
				request
				.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,e.getMessage());
				throw new ServletException(e);
			} else {
				WebUtil.returnResponse(response,
						new MessageResponse("错误", "请求异常，请刷新此页面。").toJsonString());
			}
		}
	}

	private void saveLog(HttpServletRequest request) {
		int userID = WebUtil.getCurrentEmployee(request.getSession())
				.getUserID();
		String pagex = request.getRequestURI();
		String uri = pagex.substring(request.getContextPath().length());
		Map pmap = request.getParameterMap();
		Map<String, String> map = new HashMap<String, String>();
		for (Iterator<String> it = pmap.keySet().iterator(); it.hasNext();) {
			String key = it.next();
			String[] vals = (String[]) pmap.get(key);
			map.put(key, Arrays.toString(vals));
		}
		String opUri = uri + "?" + map.toString();
		String opIP = WebUtil.getIpAddr(request);
		SysLog sysLog = new SysLog(String.valueOf(userID), opUri, opIP);
		LoginService service = (LoginService) SpringInit
				.getBean("loginServiceImpl");
		try {
			service.doSaveSysLog(sysLog);
		} catch (Exception e) {
			logger.error(e.getMessage());
		}
	}

	public static void putParameterToAttribute(HttpServletRequest request) {
		Map paramMap = request.getParameterMap();
		if (paramMap != null)
			for (Iterator it = paramMap.entrySet().iterator(); it.hasNext();) {
				Map.Entry entry = (Map.Entry) it.next();
				request.setAttribute((String) entry.getKey(), entry.getValue());
			}
	}

	public static void putTargetServletPath(HttpServletRequest request) {
		Stack stack = (Stack) request
				.getAttribute(WebKeys.TARGET_URL_SESSION_KEY);

		if (stack == null)
			stack = new Stack();
		String uri = request.getRequestURL().toString();
		String queryString = request.getQueryString();
		if (queryString != null)
			uri = uri + "?" + queryString;
		if (uri == null)
			uri = request.getServletPath();
		stack.push(uri);

		logger.info("uri=" + uri + " // stack=" + stack);

		request.setAttribute(WebKeys.TARGET_URL_SESSION_KEY, stack);
	}

	private Map buildParamByDataScope(String url, EmployeeObj userDNA) {

		if (url == null || url.equals(""))
			return null;

		LoginServiceImpl loginServiceImpl = (LoginServiceImpl) SpringInit
				.getBean("loginServiceImpl");

		return (Map) loginServiceImpl.getSecrityDataScope(url, userDNA, false);
	}

	@Override
	public void destroy() {
		ProxoolFacade.shutdown();
		super.destroy();
	}
}

package com.bitc.woix.servlet;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.URLEncoder;
import java.rmi.RemoteException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.apache.axis.utils.StringUtils;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.dom4j.Attribute;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;

import com.bitc.woix.client.WoixServiceSoapProxy;
import com.bitc.woix.export.ExcelUtils;
import com.bitc.woix.export.JsGridReportBase;
import com.bitc.woix.export.TableData;
import com.bitc.woix.pojo.AjaxResponse;

/**
 * Servlet implementation class WSServlet
 */
public class WSServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	private WoixServiceSoapProxy ws;

	/**
	 * @see HttpServlet#HttpServlet()
	 */
	public WSServlet() {
		super();
	}

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doGet(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		String method = request.getParameter("method");
		if ("getAlarmJson".equals(method)) {
			getAlarmJson(request, response);
		} else if ("getQuickAlarmJson".equals(method)) {
			getQuickAlarmJson(request, response);
		} else if ("getPointTableJson".equals(method)) {
			getPointTableJson(request, response);
		} else if ("getSetupXml".equals(method)) {
			getSetupXml(request, response);
		} else if ("alarmExport".equals(method)) {
			try {
				alarmExport(request, response);
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else if ("doDownloadTempfile".equals(method)) {
			doDownloadTempfile(request, response);
		} else if ("pointTableExport".equals(method)) {
			pointTableExport(request, response);
		} else if ("checkUsercode".equals(method)) {
			checkUsercode(request, response);
		} else if ("createUser".equals(method)) {
			createUser(request, response);
		} else if ("submitLogin".equals(method)) {
			submitLogin(request, response);
		} else if ("getConfigValue".equals(method)) {
			getConfigValue(request, response);
		}
	}

	/**
	 * 获取数据一览表厂站、设备、数据类型数据
	 * */
	private void getSetupXml(HttpServletRequest request,
			HttpServletResponse response) {
		String result;
		try {
			result = ws.getSetupXml();
			response.setContentType("application/xml;charset=utf-8");
			response.setCharacterEncoding("UTF-8");
			response.getWriter().write(result);
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 获取数据一览表数据
	 * */
	private void getPointTableJson(HttpServletRequest request,
			HttpServletResponse response) {
		int stnId = Integer.valueOf(request.getParameter("stnId"));
		int grpId = Integer.valueOf(request.getParameter("grpId"));
		int typeId = Integer.valueOf(request.getParameter("typeId"));
		int start = new Integer(
				request.getParameter("start") != null ? request
						.getParameter("start") : "0").intValue();
		int limit = new Integer(
				request.getParameter("limit") != null ? request
						.getParameter("limit") : "0").intValue();
		int end = start + limit;
		String result;
		try {
			result = ws.getPointTableJson(stnId, grpId, typeId, start, end);
			response.setContentType("text/json;charset=utf-8");
			response.setCharacterEncoding("UTF-8");
			response.getWriter().write(result);
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 获取实时报警数据
	 * */
	private void getQuickAlarmJson(HttpServletRequest request,
			HttpServletResponse response) {
		int count = Integer.valueOf(request.getParameter("count"));
		String result;
		try {
			result = ws.getQuickAlarmJson(count);
			response.setContentType("text/json;charset=utf-8");
			response.setCharacterEncoding("UTF-8");
			response.getWriter().write(result);
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 获取报警一览表数据
	 * */
	private void getAlarmJson(HttpServletRequest request,
			HttpServletResponse response) {
		try {
			String startTime = request.getParameter("startTime");
			String endTime = request.getParameter("endTime");
			int startInx = new Integer(
					request.getParameter("start") != null ? request
							.getParameter("start") : "0").intValue();
			int limit = new Integer(
					request.getParameter("limit") != null ? request
							.getParameter("limit") : "0").intValue();
			int endInx = startInx + limit;

			String searchStr = request.getParameter("searchStr").trim();
			String sort = request.getParameter("sort") + " "
					+ request.getParameter("dir");
			String result;

			result = ws.getAlarmJson(startTime, endTime, startInx, endInx,
					searchStr, sort);
			response.setContentType("text/json;charset=utf-8");
			response.setCharacterEncoding("UTF-8");
			response.getWriter().write(result);
		} catch (RemoteException e1) {
			e1.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * @param String
	 *            title 导出标题
	 * @param String
	 *            header 列头
	 * @param String
	 *            field 列头和数据匹配关系
	 * @param list
	 *            要导出的数据
	 * */
	private void export(String title, String header, String field,
			HttpServletRequest request, HttpServletResponse response,
			String list) {
		String key = java.util.UUID.randomUUID().toString().toUpperCase()
				.replace("-", "");

		JSONObject object = JSONObject.fromObject(list);
		List rows = (JSONArray) object.get("rows");
		String[] headers = StringUtils.split(header, ',');
		String[] fields = StringUtils.split(field, ',');
		TableData data = ExcelUtils.createTableData(rows,
				ExcelUtils.createTableHeader(headers), fields);
		JsGridReportBase report;
		AjaxResponse ajaxResponse = new AjaxResponse();
		try {
			report = new JsGridReportBase(request, response);

			HSSFWorkbook wb = new HSSFWorkbook();// 创建新的Excel 工作簿
			HashMap<String, HSSFCellStyle> styles = report.initStyles(wb);// 根据模板文件，初始化表头样式
			wb = report.writeSheet(wb, title, styles, "admin", data);// 写入工作表

			String sFileName = title + "-" + key + ".xls";
			String contextPath = request.getSession().getServletContext()
					.getRealPath("/temp")
					+ "/";
			File dic = new File(contextPath);
			if (!dic.exists()) {
				dic.mkdirs();
			}
			wb.write(new FileOutputStream(contextPath + sFileName));

			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("downloadpath",
					"method=doDownloadTempfile&fileName=" + sFileName);
			response.setContentType("text/json;charset=UTF-8");
			// new PrintWriter(report.getOut()).write(ajaxResponse.toString());
			// report.getOut().flush();
			response.getWriter().write(ajaxResponse.toString());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 报警一览表导出
	 * */
	private void alarmExport(HttpServletRequest request,
			HttpServletResponse response) {
		String startTime = request.getParameter("startTime");
		String endTime = request.getParameter("endTime");
		int startInx = new Integer(
				request.getParameter("start") != null ? request
						.getParameter("start") : "0").intValue() + 1;
		int limit = new Integer(
				request.getParameter("limit") != null ? request
						.getParameter("limit") : "0").intValue();
		int endInx = startInx + limit;

		String searchStr = request.getParameter("searchStr").trim();
		String sort = request.getParameter("sort") + " "
				+ request.getParameter("dir");
		String result;

		try {
			result = ws.getAlarmJson(startTime, endTime, startInx, endInx,
					searchStr, sort);
			String title = request.getParameter("title");
			String header = request.getParameter("header");
			String field = request.getParameter("field");
			export(title, header, field, request, response, result);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 数据一览表导出
	 * */
	private void pointTableExport(HttpServletRequest request,
			HttpServletResponse response) {
		int stnId = Integer.valueOf(request.getParameter("stnId"));
		int grpId = Integer.valueOf(request.getParameter("grpId"));
		int typeId = Integer.valueOf(request.getParameter("typeId"));
		int start = new Integer(
				request.getParameter("start") != null ? request
						.getParameter("start") : "0").intValue();
		int limit = new Integer(
				request.getParameter("limit") != null ? request
						.getParameter("limit") : "0").intValue();
		int end = start + limit;
		String result;
		try {
			result = ws.getPointTableJson(stnId, grpId, typeId, start, end);
			String title = request.getParameter("title");
			String header = request.getParameter("header");
			String field = request.getParameter("field");
			export(title, header, field, request, response, result);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 文件下载
	 * */
	public void doDownloadTempfile(HttpServletRequest request,
			HttpServletResponse response) throws ServletException {
		try {
			String fileName = request.getParameter("fileName");
			String filePath = request.getSession().getServletContext()
					.getRealPath("/temp")
					+ "/";

			String add = filePath + fileName;
			FileInputStream hFile = new FileInputStream(add); // 以byte流的方式打开文件
			int i = hFile.available(); // 得到文件大小
			byte data[] = new byte[i];
			hFile.read(data); // 读数据
			hFile.close();
			response.setContentType("application/octet-stream;charset=UTF-8");
			response.setHeader("Content-Disposition",
					"attachment;filename=".concat(String.valueOf(URLEncoder
							.encode(fileName, "UTF-8"))));
			response.setHeader("Connection", "close");
			OutputStream toClient = response.getOutputStream(); // 得到向客户端输出二进制数据的对象
			toClient.write(data); // 输出数据
			toClient.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 用户名校验 是否存在 flag==true 存在
	 * */
	public void checkUsercode(HttpServletRequest request,
			HttpServletResponse response) {
		String usercode = request.getParameter("usercode");
		AjaxResponse ajaxResponse = new AjaxResponse();
		SAXReader reader = new SAXReader();
		try {
			System.out.println("checkUsercode");
			String path = request.getSession().getServletContext()
					.getRealPath("/")
					+ "\\registration.xml";
			Document doc = reader.read(new File(path));
			boolean flag = false;
			List list = doc.selectNodes("users/user/usercode");
			Iterator iter = list.iterator();
			while (iter.hasNext()) {
				Element el = (Element) iter.next();
				if (usercode.equals(el.getText())) {
					flag = true;
					break;
				}
			}
			ajaxResponse.setSuccess(flag);
			response.setContentType("text/json;charset=UTF-8");
			response.getWriter().write(ajaxResponse.toString());
		} catch (IOException e) {
			e.printStackTrace();
		} catch (DocumentException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 创建用户
	 * */
	public void createUser(HttpServletRequest request,
			HttpServletResponse response) {

		String usercode = request.getParameter("usercode");
		String password = request.getParameter("password");
		String role = request.getParameter("role");
		String username = request.getParameter("username");
		String survey = request.getParameter("survey");

		SAXReader reader = new SAXReader();
		// String path =
		// request.getSession().getServletContext().getRealPath("/")
		// request.getContextPath()
		// + "\\registration.xml";
		String webClassPath = this.getClass().getProtectionDomain()
				.getCodeSource().getLocation().getPath();

		String path = webClassPath.substring(0,
				webClassPath.indexOf("WEB-INF/classes"))
				+ "registration.xml";

		try {
			Document doc = reader.read(new File(path));

			Element root = doc.getRootElement();
			Element user = root.addElement("user");

			Element usercodeEl = user.addElement("usercode");
			usercodeEl.setText(usercode);

			Element passwordEl = user.addElement("password");
			passwordEl.setText(password);

			Element roleEl = user.addElement("role");
			roleEl.setText(role);

			Element usernameEl = user.addElement("username");
			usernameEl.setText(username);

			Element surveyEl = user.addElement("survey");
			surveyEl.setText(survey);

			FileWriter writer;
			writer = new FileWriter(path);

			OutputFormat format = OutputFormat.createPrettyPrint();
			format.setEncoding("utf-8");
			XMLWriter xmlWriter = new XMLWriter(writer, format);
			xmlWriter.write(doc);
			writer.close();
			xmlWriter.close();

		} catch (IOException e) {
			e.printStackTrace();
		} catch (DocumentException e) {
			e.printStackTrace();
		}

	}

	/**
	 * 登录提交
	 * */
	public void submitLogin(HttpServletRequest request,
			HttpServletResponse response) {
		String usercode = request.getParameter("usercode");
		String password = request.getParameter("password");
		AjaxResponse ajaxResponse = new AjaxResponse();
		SAXReader reader = new SAXReader();
		try {
			String path = request.getSession().getServletContext()
					.getRealPath("/")
					+ "\\registration.xml";
			Document doc = reader.read(new File(path));
			boolean flag = false;
			List list = doc.selectNodes("users/user");
			Iterator iter = list.iterator();
			while (iter.hasNext()) {
				Element el = (Element) iter.next();
				Iterator ite = el.elementIterator();

				while (ite.hasNext()) {
					Element ee = (Element) ite.next();
					if ("usercode".equals(ee.getName())
							&& usercode.equals(ee.getText())) {
						Element temp = (Element) ite.next();
						if ("password".equals(temp.getName())
								&& password.equals(temp.getText())) {
							flag = true;
							break;
						}
					}
				}
			}

			ajaxResponse.setSuccess(flag);
			if (!flag) {
				ajaxResponse.setTitle("提示");
				ajaxResponse.setMessage("验证失败，用户名或密码错误。");
			}
			response.setContentType("text/json;charset=UTF-8");
			response.getWriter().write(ajaxResponse.toString());
		} catch (IOException e) {
			e.printStackTrace();
		} catch (DocumentException e) {
			e.printStackTrace();
		}
	}

	/**
	 * paramValue： 1.Woix.AlarmRefreshInterval 报警刷新间隔 毫秒单位 2. Woix.Login
	 * 是否需要登录进入系统 boolean 3.Woix.H9000StartPage 首页画面名称
	 * */
	public void getConfigValue(HttpServletRequest request,
			HttpServletResponse response) {
		AjaxResponse ajaxResponse = new AjaxResponse();

		String paramValue = request.getParameter("paramValue");
		try {
			String configValue = ws.getConfigValue(paramValue);
			ajaxResponse.addAttribute("configValue", configValue);
			ajaxResponse.setSuccess(true);
			response.setContentType("text/json;charset=UTF-8");
			response.getWriter().write(ajaxResponse.toString());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
	 *      response)
	 */
	protected void doPost(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}

	@Override
	public void init(ServletConfig config) throws ServletException {
		String url = config.getInitParameter("URL");
		this.ws = new WoixServiceSoapProxy(url);
		super.init(config);
	}

}

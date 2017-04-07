package com.bitc.jk.xmis.web.action;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;

import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;

import org.apache.commons.fileupload.DiskFileUpload;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.DeviceSheetImportInfo;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.service.BBZFinancyService;
import com.bitc.jk.xmis.service.FileUploadDownloadService;
import com.bitc.jk.xmis.service.ImportExcelService;
import com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.SpringInit;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class BBZFinancyAction extends BaseAction {

	private static Logger logger = Logger.getLogger(BBZFinancyAction.class);

	private BBZFinancyService bbzFinancyService;

	public BBZFinancyService getBbzFinancyService() {
		return bbzFinancyService;
	}

	public void setBbzFinancyService(BBZFinancyService bbzFinancyService) {
		this.bbzFinancyService = bbzFinancyService;
	}

	private ImportExcelService importExcelService;

	public ImportExcelService getImportExcelService() {
		return importExcelService;
	}

	public void setImportExcelService(ImportExcelService importExcelService) {
		this.importExcelService = importExcelService;
	}

	public void getAttachmentList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("--> Go to getAttachmentListForBBZFinancy");
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		int attID = Type.GetInt(request.getParameter("attachmentTypeID"));
		WebUtil.returnResponse(response,
				bbzFinancyService.getAttachmentList(pageInfo,attID));
	}
	

	public void getSalarySheetList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		logger.info("--> Go to getSalarySheetListForBBZFinancy");
		int attID = Type.GetInt(request.getParameter("attID"));
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		WebUtil.returnResponse(response,
				bbzFinancyService.getSalarySheetList(pageInfo, attID));
	}

	public ActionForward excelImportForSalary(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String directPath = request.getSession().getServletContext()
				.getRealPath("/");
		ServletContext servletContext = request.getSession()
				.getServletContext();
		String directPathLocal = (String) servletContext
				.getAttribute("application.attachedPath")
				+ request.getParameter("type") + "\\";
		SimpleEmployee emp = WebUtil.getCurrentEmployee(request.getSession());
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			DiskFileUpload fu = new DiskFileUpload();
			fu.setSizeThreshold(1024 * 1024);
			fu.setRepositoryPath(directPath + "temp");
			fu.setHeaderEncoding("utf-8");
			List fileItems = fu.parseRequest(request);
			Iterator iter = fileItems.iterator();
			while (iter.hasNext()) {
				FileItem item = (FileItem) iter.next();
				// 忽略其他不是文件域的所有表单信息
				if (!item.isFormField()) {
					String name = item.getName();
					long size = item.getSize();
					String contentType = item.getContentType();
					if (!"application/vnd.ms-excel".equals(contentType)) {
						throw new RuntimeException("文件类型错误。请上传由模板创建的文件。");
					}
					if ((name == null || name.equals("")) && size == 0)
						continue;
					// 保存上传的文件到指定的目录
					String[] names = StringUtils.split(name, "\\"); // 对原来带路径的文件名进行分割
					name = names[names.length - 1];
					String path = directPathLocal + name;
					item.write(new File(path));

					FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
							.getBean("fileUploadDownloadServiceImpl",
									FileUploadDownloadServiceImpl.class);

					AttachmentFile attFile = new AttachmentFile(Type
							.getInteger(request.getParameter("type"))
							.intValue());

					java.io.File ioFile = new java.io.File(path);

					InputStream inStream = new FileInputStream(ioFile);

					int fileSize = (int) ioFile.length();
					byte[] content = new byte[fileSize];
					attFile.setUploadID(emp.getUserID());
					attFile.setDesc(request.getParameter("desc"));
					attFile.setFileName(name);
					attFile.setContent(content);
					attFile.setAttachmentTypeID(Type.GetInt(request
							.getParameter("attachmentTypeID")));

					attFile.setRefID(Type.getString(request
							.getParameter("REFID")));

					attFile.setKeyWords(request.getParameter("keywords"));

					attFile.setFilePath(directPathLocal);

					attFile.setFileSize(fileSize);
					attFile.setFis(inStream);
					int attID = service.doSaveAttachmentGetAttacheId(attFile);// 保存
					if (attID == 0) {
						throw new RuntimeException("保存附件失败。");
					}
					List<String[]> sheetList = importExcelService
							.getSheetInfos(path);
					// boolean flag = importExcelService
					// .doSaveDeviceSheetImportInfo(attID, sheetList);
					boolean flag = bbzFinancyService.doSaveSalayExcelInfo(
							attID, sheetList);
					if (!flag) {
						throw new RuntimeException("保存上传文件的工作表信息失败。");
					}
				}
			}

			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("上传成功！");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("导入失败！<br/>" + e.getMessage());
		}
		response.getWriter().write(ajaxResponse.toJsonString());
		return null;
	}
/**
 *  二次工资数据上传，删除以前的数据
 * @param mapping
 * @param form
 * @param request
 * @param response
 */
	public void delSheetInfoByAttID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String attID = request.getParameter("attID");
		// String sheet_uuid = request.getParameter("sheet_uuid");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {

			bbzFinancyService.delSheetInfoByAttID(Type.GetInt(attID));
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			logger.error("删除失败。" + e.getMessage());
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除失败！");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	/**
	 *  二次收入到账数据上传，删除以前的数据
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void delYearSheetInfoByAttID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String attID = request.getParameter("attID");
		// String sheet_uuid = request.getParameter("sheet_uuid");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {

			bbzFinancyService.delYearSheetInfoByAttID(Type.GetInt(attID));
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			logger.error("删除失败。" + e.getMessage());
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除失败！");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void existValidate(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		int attachmentTypeID = Type.GetInt(request
				.getParameter("attachmentTypeID"));
		String REFID = Type.getString(request.getParameter("REFID"));
		String fileName = java.net.URLDecoder.decode(
				request.getParameter("fileName"), "UTF-8");
		int type = Type.GetInt(request.getParameter("type"));
		int index = fileName.lastIndexOf("\\");
		if (index > 0) {
			fileName = fileName.substring(index + 1);
		}

		FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
				.getBean("fileUploadDownloadServiceImpl",
						FileUploadDownloadServiceImpl.class);
		int attID = service.existValidateReturnAttID(attachmentTypeID, REFID,
				fileName, type);
		Boolean flag = attID > 0;// 是否存在标记
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setSuccess(true);
		ajaxResponse.addAttribute("flag", flag);
		ajaxResponse.addAttribute("attID", attID);
		ajaxResponse.addAttribute("fileName", fileName);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());

	}

	public void importSalarySheetList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			InputStream cf = LoginAction.class.getClassLoader()
					.getResourceAsStream("properties/config.properties");

			Properties cfProp = new Properties();
			cfProp.load(cf);
			String directPath = cfProp.getProperty("application.attachedPath");

			FileUploadDownloadService fileUploadService = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			int attID = Type.GetInt(request.getParameter("attID"));
			String filePath = fileUploadService.getAttachmentByID(attID,
					directPath);

			String sheetIndexs = request.getParameter("sheetIndex");
			String[] sheetIndexArray = sheetIndexs.split(",");

			String year = request.getParameter("iyear");
			String month = request.getParameter("imonth");
			String salarytype = request.getParameter("salarytype");

			InputStream is = new FileInputStream(filePath);
			Workbook wb = Workbook.getWorkbook(is);

			boolean flag = true;
			String message = "";
			for (int i = 0; i < sheetIndexArray.length; i++) {
				Map temp = new HashMap();
				String arrayStr = sheetIndexArray[i];

				int index = Type.GetInt(arrayStr.substring(0,
						arrayStr.indexOf("^")));

				Sheet sheet = wb.getSheet(index);

				if ((message = existOnceErrorMsg(sheet, "姓名")).length() > 0) {
					message = "导入表格中必须含有“姓名”列";
					flag = false;
					break;
				}
				if ((message = existOnceErrorMsg(sheet, "实发")).length() > 0
						&& (message = existOnceErrorMsg(sheet, "实发金额")).length() > 0
						&& (message = existOnceErrorMsg(sheet, "实发工资")).length() > 0) {
					message = "导入表格中必须含有“实发”或者“实发金额”或者“实发工资”列";
					flag = false;
					break;
				}
			}
			if (flag) {

				List totalSQL = getSql(filePath, sheetIndexArray, year, month,
						salarytype);
				bbzFinancyService.importSalarySheetList(totalSQL);
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("导入成功");

			} else {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("失败");
				ajaxResponse.setMessage(message);
			}
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("导入失败！<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	
	
	/**
	 * 收入到账上传   sheet导入
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	
	public void importYearSalarySheetList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) throws Exception{
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			
			InputStream cf = LoginAction.class.getClassLoader()
					.getResourceAsStream("properties/config.properties");

			Properties cfProp = new Properties();
			cfProp.load(cf);
			String directPath = cfProp.getProperty("application.attachedPath");
			
			FileUploadDownloadService fileUploadService = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			int attID = Type.GetInt(request.getParameter("attID"));
			int type = Type.GetInt(request.getParameter("type"));
			String filePath = fileUploadService.getAttachmentByID(attID,
					directPath);
			String sheet_uuid = request.getParameter("sheet_uuid");
			String sheetIndexs = request.getParameter("sheetIndex");
			String[] sheetIndexArray = sheetIndexs.split(",");

			String year = request.getParameter("iyear");

			InputStream is = new FileInputStream(filePath);
			Workbook wb = Workbook.getWorkbook(is);

			boolean flag = true;
			String message = "";
			for (int i = 0; i < sheetIndexArray.length; i++) {
				Map temp = new HashMap();
				String arrayStr = sheetIndexArray[i];

				int index = Type.GetInt(arrayStr.substring(0,
						arrayStr.indexOf("^")));

				Sheet sheet = wb.getSheet(index);
				
				String comments =  formatStr(sheet.getCell(0, 0).getContents());
				
				if (!comments.equals("月份")) {
					message = "导入表格中必须含有“月份”列";
					flag = false;
					break;
				}

			}
			if (flag) {

				List totalSQL = getYSql(filePath, sheetIndexArray, year);
				int iyear = Type.GetInt(year);
				bbzFinancyService.importSalarySheetList(totalSQL,iyear,attID, type,sheet_uuid);
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("导入成功");

			} else {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("失败");
				ajaxResponse.setMessage(message);
			}
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("导入失败！<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	
	/**
	 * 银行存款上传   sheet导入
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	
	public void importBankDepositSheetList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) throws Exception{
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			
			InputStream cf = LoginAction.class.getClassLoader()
					.getResourceAsStream("properties/config.properties");

			Properties cfProp = new Properties();
			cfProp.load(cf);
			String directPath = cfProp.getProperty("application.attachedPath");
			
			FileUploadDownloadService fileUploadService = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			int attID = Type.GetInt(request.getParameter("attID"));
			int type = Type.GetInt(request.getParameter("type"));
			String filePath = fileUploadService.getAttachmentByID(attID,
					directPath);
			String sheetIndexs = request.getParameter("sheetIndex");
			String sheet_uuid = request.getParameter("sheet_uuid");
			String[] sheetIndexArray = sheetIndexs.split(",");

			String year = request.getParameter("iyear");

			InputStream is = new FileInputStream(filePath);
			Workbook wb = Workbook.getWorkbook(is);

			boolean flag = true;
			String message = "";
			for (int i = 0; i < sheetIndexArray.length; i++) {
				Map temp = new HashMap();
				String arrayStr = sheetIndexArray[i];

				int index = Type.GetInt(arrayStr.substring(0,
						arrayStr.indexOf("^")));

				Sheet sheet = wb.getSheet(index);
				
				String comments =  formatStr(sheet.getCell(0, 0).getContents());
				
				if (!comments.equals("月份")) {
					message = "导入表格中必须含有“月份”列";
					flag = false;
					break;
				}

			}
			if (flag) {

				List totalSQL = getBSql(filePath, sheetIndexArray, year);
				int iyear = Type.GetInt(year);
				bbzFinancyService.importSalarySheetList(totalSQL,iyear,attID,type,sheet_uuid);
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("导入成功");

			} else {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("失败");
				ajaxResponse.setMessage(message);
			}
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("导入失败！<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void isOrNotImportSheetList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) throws Exception{
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		
		String sheet_uuid =request.getParameter("sheet_uuid");
		int year = Type.GetInt(request.getParameter("iyear"));
		int type = Type.GetInt(request.getParameter("type"));
		try {
			boolean flag = bbzFinancyService.findByYearAndType(year,type,sheet_uuid);
			if(flag){
				ajaxResponse.setSuccess(true);
			}else{
				ajaxResponse.setSuccess(false);
			}
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
		}

		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	/* Excel表格解析 */
	public static List getSql(String filePath, String[] sheetIndexArray,
			String year, String month, String salarytype) {
		List totalSqlList = new ArrayList();
		InputStream is = null;

		try {
			is = new FileInputStream(filePath);
			Workbook wb = Workbook.getWorkbook(is);

			List<Object[]> resultList = new ArrayList<Object[]>();
			for (int i = 0; i < sheetIndexArray.length; i++) {
				Map temp = new HashMap();
				String arrayStr = sheetIndexArray[i];

				int index = Type.GetInt(arrayStr.substring(0,
						arrayStr.indexOf("^")));

				String sheet_uuid = arrayStr.substring(
						arrayStr.indexOf("^") + 1, arrayStr.length());
				Sheet sheet = wb.getSheet(index);

				String sheetName = sheet.getName();

				int columnNum = sheet.getColumns();

				int rowNum = sheet.getRows();

				int startRow = getStartRowNum(sheet, "序号", "编号");// 所在行，开始行
				int startColumn = getStartcolumnNum(sheet, "序号", "编号");// 姓名所在列，开始列

				// 找到有效数据一共有几列
				int colnum = startColumn;
				for (int col = startColumn; col < columnNum; col++) {
					
					String comments =  formatStr(sheet.getCell(col, startRow).getContents());
					comments = StringUtils.replace(comments, ",","");
					if (comments.length() == 0) {
						break;
					} else {
						colnum++;
					}
				}
				columnNum = colnum;

				// 找到有效数据一共有几行
				int rownum = startRow;
				for (int ro = startRow; ro < rowNum; ro++) {
					String comments =  formatStr(sheet.getCell(startColumn, ro).getContents());
					if (comments.length() == 0) {
						break;
					} else {
						rownum++;
					}
				}
				rowNum = rownum;

				List sqlList = new ArrayList();
				// 得到sql
				String nameColumnIndex = "";
				String salaryColumnIndex = "";

				// 记录首行为隐藏的列
				List hiddenCol = new ArrayList();

				for (int r = startRow; r < rowNum; r++) {

					String rowString_2 = "";
					String rowString = "";
					String sql_column = "";
					int columnindex = 0;
					boolean title = false;
					for (int c = startColumn; c < columnNum; c++) {

						if (startRow == r) {// 首行
							title = true;
							if (sheet.getCell(c, r).isHidden()) {// 判断不准确
								hiddenCol.add(c);
							} else {

								columnindex++;
								String comments =   formatStr(sheet.getCell(c, r).getContents());  

								rowString_2 = rowString_2 + ",'" + comments+ "'";

								sql_column = sql_column + ","+ ("column_" + columnindex);

								if ("姓名".equals(comments)) {
									nameColumnIndex = "column_" + columnindex;
								}
								if ("实发".equals(comments)
										|| "实发工资".equals(comments)
										|| "实发金额".equals(comments)) {
									salaryColumnIndex = "column_" + columnindex;
								}
							}

						} else {// 非首行的数据，首行哪一列是隐藏，就不取这一列数据
							title = false;
							boolean flag = false;
							if (hiddenCol.size() != 0) {

								for (int hidden = 0; hidden < hiddenCol.size(); hidden++) {
									if (c == Type.GetInt(hiddenCol.get(hidden))) {
										flag = true;
									}
								}
							}

							if (!flag) {
								// if (sheet.getCell(c, r).isHidden())
								// {//跟首行一致，不考虑是否隐藏
								// } else {
								columnindex++;
								String comments =  formatStr(sheet.getCell(c, r).getContents()); 
								if(isNum(comments)){
									comments = StringUtils.replace(comments, ",","");
								}
								rowString_2 = rowString_2
										+ ",'"
										+ (new String(
												comments.getBytes("gb2312"),
												"iso-8859-1").length() < 41 ? comments
												:new WebUtil().CutString(comments, 40)) + "'";

								sql_column = sql_column + ","
										+ ("column_" + columnindex);

								if ("姓名".equals(comments)) {
									nameColumnIndex = "column_" + columnindex;
								}
								if ("实发".equals(comments)
										|| "实发工资".equals(comments)
										|| "实发金额".equals(comments)) {
									salaryColumnIndex = "column_" + columnindex;
								}
							}
						}
					}

					if (sql_column.length() > 0) {

						String rowString_1 = "";

						if (title) {
							rowString_1 = year + "," + month + "," + salarytype
									+ ",'" + sheet_uuid + "','"
									+ nameColumnIndex + "','"
									+ salaryColumnIndex + "'," + 1;// 年份，月份，工资类别，工资表ID，姓名所在列的字段名，实发工资，实发金额所在的字段名
						} else {
							rowString_1 = year + "," + month + "," + salarytype
									+ ",'" + sheet_uuid + "','"
									+ nameColumnIndex + "','"
									+ salaryColumnIndex + "'," + 0;// 年份，月份，工资类别，工资表ID，姓名所在列的字段名，实发工资，实发金额所在的字段名

						}

						rowString = "'" + UUIDUtils.nextCode() + "',"
								+ rowString_1 + rowString_2;

						String sql = "insert into xmis.t_salary_info (id,iyear,imonth,type_id,sheet_uuid,name_column,salary_column,titleColumn"
								+ sql_column + ") values(" +  rowString + ")";
						
						logger.info(sql);
						
						sqlList.add(sql);

					}
				}

				temp.put("sql", sqlList);
				temp.put("sheet_uuid", sheet_uuid);
				totalSqlList.add(temp);
			}

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (BiffException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				is.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return totalSqlList;
	}
	
	
	/**
	 * 收入到账上传  Excel表格解析
	 * @param filePath
	 * @param sheetIndexArray
	 * @param year
	 * @return
	 */
	public static List getYSql(String filePath, String[] sheetIndexArray,
			String year) {
		List totalSqlList = new ArrayList();
		InputStream is = null;

		try {
			is = new FileInputStream(filePath);
			Workbook wb = Workbook.getWorkbook(is);

			List<Object[]> resultList = new ArrayList<Object[]>();
			for (int i = 0; i < sheetIndexArray.length; i++) {
				Map temp = new HashMap();
				String arrayStr = sheetIndexArray[i];

				int index = Type.GetInt(arrayStr.substring(0,
						arrayStr.indexOf("^")));

				String sheet_uuid = arrayStr.substring(
						arrayStr.indexOf("^") + 1, arrayStr.length());
				Sheet sheet = wb.getSheet(index);

				String sheetName = sheet.getName();

				int columnNum = sheet.getColumns();

				int rowNum = sheet.getRows();

				int startRow = getStartRowNum(sheet, "月份", "月份");// 所在行，开始行
				int startColumn = getStartcolumnNum(sheet, "月份", "月份");// 姓名所在列，开始列

				// 找到有效数据一共有几列
				int colnum = startColumn;
				for (int col = startColumn; col < columnNum; col++) {
					String comments =  formatStr(sheet.getCell(col, startRow).getContents());
					if (comments.length() == 0) {
						break;
					} else {
						colnum++;
					}
				}
				columnNum = colnum;

				// 找到有效数据一共有几行
				int rownum = startRow;
				for (int ro = startRow; ro < rowNum; ro++) {
					String comments =  formatStr(sheet.getCell(startColumn, ro).getContents());
					if (comments.length() == 0) {
						break;
					} else {
						rownum++;
					}
				}
				rowNum = rownum;

				List sqlList = new ArrayList();
				// 得到sql
				String nameColumnIndex = "";
				String salaryColumnIndex = "";

				// 记录首行为隐藏的列
				List hiddenCol = new ArrayList();

				for (int r = startRow; r < rowNum; r++) {

					String rowString_2 = "";
					String rowString = "";
					String sql_column = "";
					int columnindex = 0;
					boolean title = false;
					for (int c = startColumn; c < columnNum; c++) {

						if (startRow == r) {// 首行
							title = true;
							if (sheet.getCell(c, r).isHidden()) {// 判断不准确
								hiddenCol.add(c);
							} else {

								columnindex++;
								String comments =   formatStr(sheet.getCell(c, r).getContents());  
								rowString_2 = rowString_2 + ",'" + comments
										+ "'";

								sql_column = sql_column + ","
										+ ("column_" + columnindex);
							}

						} else {// 非首行的数据，首行哪一列是隐藏，就不取这一列数据
							title = false;
							boolean flag = false;
							if (hiddenCol.size() != 0) {

								for (int hidden = 0; hidden < hiddenCol.size(); hidden++) {
									if (c == Type.GetInt(hiddenCol.get(hidden))) {
										flag = true;
									}
								}
							}

							if (!flag) {
								// if (sheet.getCell(c, r).isHidden())
								// {//跟首行一致，不考虑是否隐藏
								// } else {
								columnindex++;
								String comments =  formatStr(sheet.getCell(c, r).getContents()); 
								if(isNum(comments)){
									comments = StringUtils.replace(comments, ",","");
								}
								rowString_2 = rowString_2
										+ ",'"
										+ (new String(
												comments.getBytes("gb2312"),
												"iso-8859-1").length() < 41 ? comments
												:new WebUtil().CutString(comments, 40)) + "'";

								sql_column = sql_column + ","
										+ ("column_" + columnindex);
							}
						}
					}

					if (sql_column.length() > 0) {

						String rowString_1 = "";

						if (title) {
							
							rowString_1 = year + ",'"+sheet_uuid+"',"+1+",1";
							
						} else {
							rowString_1 = year + ",'"+sheet_uuid+"',"+0+",1";
						}

						rowString = "'" + UUIDUtils.nextCode() + "',"
								+ rowString_1 + rowString_2;

						String sql = "insert into xmis.T_Income_Info (id,iyear,sheet_uuid,titleColumn,type"
								+ sql_column + ") values(" +  rowString + ")";
						
						logger.info(sql);
						
						sqlList.add(sql);

					}
				}

				temp.put("sql", sqlList);
				temp.put("sheet_uuid", sheet_uuid);
				totalSqlList.add(temp);
			}

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (BiffException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				is.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return totalSqlList;
	}

	/**
	 * 银行存款上传  Excel表格解析
	 * @param filePath
	 * @param sheetIndexArray
	 * @param year
	 * @return
	 */
	public static List getBSql(String filePath, String[] sheetIndexArray,
			String year) {
		List totalSqlList = new ArrayList();
		InputStream is = null;

		try {
			is = new FileInputStream(filePath);
			Workbook wb = Workbook.getWorkbook(is);

			List<Object[]> resultList = new ArrayList<Object[]>();
			for (int i = 0; i < sheetIndexArray.length; i++) {
				Map temp = new HashMap();
				String arrayStr = sheetIndexArray[i];

				int index = Type.GetInt(arrayStr.substring(0,
						arrayStr.indexOf("^")));

				String sheet_uuid = arrayStr.substring(
						arrayStr.indexOf("^") + 1, arrayStr.length());
				Sheet sheet = wb.getSheet(index);

				String sheetName = sheet.getName();

				int columnNum = sheet.getColumns();

				int rowNum = sheet.getRows();

				int startRow = getStartRowNum(sheet, "月份", "月份");// 所在行，开始行
				int startColumn = getStartcolumnNum(sheet, "月份", "月份");// 姓名所在列，开始列

				// 找到有效数据一共有几列
				int colnum = startColumn;
				for (int col = startColumn; col < columnNum; col++) {
					String comments =  formatStr(sheet.getCell(col, startRow).getContents());
					if (comments.length() == 0) {
						break;
					} else {
						colnum++;
					}
				}
				columnNum = colnum;

				// 找到有效数据一共有几行
				int rownum = startRow;
				for (int ro = startRow; ro < rowNum; ro++) {
					String comments =  formatStr(sheet.getCell(startColumn, ro).getContents());
					if (comments.length() == 0) {
						break;
					} else {
						rownum++;
					}
				}
				rowNum = rownum;

				List sqlList = new ArrayList();
				// 得到sql
				String nameColumnIndex = "";
				String salaryColumnIndex = "";

				// 记录首行为隐藏的列
				List hiddenCol = new ArrayList();

				for (int r = startRow; r < rowNum; r++) {

					String rowString_2 = "";
					String rowString = "";
					String sql_column = "";
					int columnindex = 0;
					boolean title = false;
					for (int c = startColumn; c < columnNum; c++) {

						if (startRow == r) {// 首行
							title = true;
							if (sheet.getCell(c, r).isHidden()) {// 判断不准确
								hiddenCol.add(c);
							} else {

								columnindex++;
								String comments =   formatStr(sheet.getCell(c, r).getContents());  
								rowString_2 = rowString_2 + ",'" + comments
										+ "'";

								sql_column = sql_column + ","
										+ ("column_" + columnindex);
							}

						} else {// 非首行的数据，首行哪一列是隐藏，就不取这一列数据
							title = false;
							boolean flag = false;
							if (hiddenCol.size() != 0) {

								for (int hidden = 0; hidden < hiddenCol.size(); hidden++) {
									if (c == Type.GetInt(hiddenCol.get(hidden))) {
										flag = true;
									}
								}
							}

							if (!flag) {
								// if (sheet.getCell(c, r).isHidden())
								// {//跟首行一致，不考虑是否隐藏
								// } else {
								columnindex++;
								String comments =  formatStr(sheet.getCell(c, r).getContents()); 
								if(isNum(comments)){
									comments = StringUtils.replace(comments, ",","");
								}
								rowString_2 = rowString_2
										+ ",'"
										+ (new String(
												comments.getBytes("gb2312"),
												"iso-8859-1").length() < 41 ? comments
												:new WebUtil().CutString(comments, 40)) + "'";

								sql_column = sql_column + ","
										+ ("column_" + columnindex);
							}
						}
					}

					if (sql_column.length() > 0) {

						String rowString_1 = "";

						if (title) {
							
							rowString_1 = year + ",'"+sheet_uuid+"',"+1+",2";
							
						} else {
							rowString_1 = year + ",'"+sheet_uuid+"',"+0+",2";
						}

						rowString = "'" + UUIDUtils.nextCode() + "',"
								+ rowString_1 + rowString_2;

						String sql = "insert into xmis.T_Income_Info (id,iyear,sheet_uuid,titleColumn,type"
								+ sql_column + ") values(" +  rowString + ")";
						
						logger.info(sql);
						
						sqlList.add(sql);

					}
				}

				temp.put("sql", sqlList);
				temp.put("sheet_uuid", sheet_uuid);
				totalSqlList.add(temp);
			}

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (BiffException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				is.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return totalSqlList;
	}
	/**
	 * 指定数据起始的行
	 * 
	 */
	public static int getStartRowNum(Sheet sheet, String str1, String str2) {
		int startRowNum = 0;
		int columnNum = sheet.getColumns();
		int rowNum = sheet.getRows();

		for (int r = 0; r < rowNum; r++) {
			for (int c = 0; c < columnNum; c++) {
				String comments =  formatStr(sheet.getCell(c, r).getContents());
				if (comments.equals(str1) || comments.equals(str2)) {
					return startRowNum = r;
				}
			}
		}
		return startRowNum;
	}

	/**
	 * 指定数据起始的列
	 */
	public static int getStartcolumnNum(Sheet sheet, String columnName1,
			String columnName2) {
		int startcolumnNum = 0;
		int columnNum = sheet.getColumns();
		int rowNum = sheet.getRows();

		for (int c = 0; c < columnNum; c++) {
			for (int r = 0; r < rowNum; r++) {
				String comments =  formatStr(sheet.getCell(c, r).getContents());
				if (comments.equals(columnName1)
						|| comments.equals(columnName2)) {
					return startcolumnNum = c;
				}
			}
		}
		return startcolumnNum;
	}

	public static String existOnceErrorMsg(Sheet sheet, String str) {
		String msg = "";
		if (str != null) {
			String[] arr = str.split(",");
			for (int j = 0; j < arr.length; j++) {
				String columnName = arr[j];
				int trueNums = 0;
				if (isExist(sheet, arr[j])) {
				} else {
					msg += arr[j] + "列不存在；";
				}
			}
		}

		return msg;
	}

	/**
	 * 某一个指定单元格是否存在
	 */
	public static boolean isExist(Sheet sheet, String columnName) {
		int columnNum = sheet.getColumns();
		int rowNum = sheet.getRows();
		for (int c = 0; c < columnNum; c++) {
			for (int r = 0; r < rowNum; r++) {
				String comments =  formatStr(sheet.getCell(c, r).getContents());
				if (comments.equals(columnName)) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * 获取工资类别
	 * */
	public void getSalaryType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = bbzFinancyService.getSalaryType();
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 获取指定type的个人工资store
	 * */
	public void getPersonForOneSalaryType(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		int year = Type.GetInt(request.getParameter("year"));
		int month = Type.GetInt(request.getParameter("month"));
		int type = Type.GetInt(request.getParameter("type"));
		EmployeeObj userObj = (EmployeeObj) request.getSession().getAttribute(
				WebKeys.Session_Attribute_Name_UserDNA);
		String username = userObj.getUserName();

		String store = null;
		try {
			store = bbzFinancyService.getPersonForOneSalaryType(year, month,
					type, username);
		} catch (Exception e) {
			e.printStackTrace();
		}

		WebUtil.returnResponse(response, store);
	}
	/**
	 * 收入到账查询的 数据
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getQueryIncome(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		int year = Type.GetInt(request.getParameter("year"));
		int type = Type.GetInt(request.getParameter("type"));
		String store = null;
		try {
			store = bbzFinancyService.getIncome(year,type);
		} catch (Exception e) {
			e.printStackTrace();
		}

		WebUtil.returnResponse(response, store);
	}
	
	/**
	 * 银行存款查询的 数据
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getBankDeposit(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		int year = Type.GetInt(request.getParameter("year"));
		int type = Type.GetInt(request.getParameter("type"));
		String store = null;
		try {
			store = bbzFinancyService.getBankDeposit(year,type);
		} catch (Exception e) {
			e.printStackTrace();
		}

		WebUtil.returnResponse(response, store);
	}
	

	public ActionForward downloadTemplateFile(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		try {
			String directPath = request.getSession().getServletContext()
					.getRealPath("/");
			File file = new File(directPath + "/bbzFinancy/financyTemplate.xlt");
			response.setContentType("application/x-msdownload");
			response.addHeader("Content-Disposition", "attachment;filename="
					+ URLEncoder.encode("工资模板.xlt", "utf-8"));
			FileInputStream fis = new FileInputStream(file);
			OutputStream os = response.getOutputStream();
			int b = 0;
			while (fis != null && (b = fis.read()) != -1) {
				os.write(b);
			}
			os.flush();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public ActionForward downloadTemplateFileIncome(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		try {
			String directPath = request.getSession().getServletContext()
					.getRealPath("/");
			File file = new File(directPath + "/yearSalary/financyTemplate.xlt");
			response.setContentType("application/x-msdownload");
			response.addHeader("Content-Disposition", "attachment;filename="
					+ URLEncoder.encode("收入到账模板.xlt", "utf-8"));
			FileInputStream fis = new FileInputStream(file);
			OutputStream os = response.getOutputStream();
			int b = 0;
			while (fis != null && (b = fis.read()) != -1) {
				os.write(b);
			}
			os.flush();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	public ActionForward downloadTemplateFileBankDesposit(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		try {
			String directPath = request.getSession().getServletContext()
					.getRealPath("/");
			File file = new File(directPath + "/bankDeposit/financyTemplate.xlt");
			response.setContentType("application/x-msdownload");
			response.addHeader("Content-Disposition", "attachment;filename="
					+ URLEncoder.encode("银行存款模板.xlt", "utf-8"));
			FileInputStream fis = new FileInputStream(file);
			OutputStream os = response.getOutputStream();
			int b = 0;
			while (fis != null && (b = fis.read()) != -1) {
				os.write(b);
			}
			os.flush();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	public void test(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = bbzFinancyService.getSheetColumn(
				"131EA2EF483F42CAA7A49514013B1AF5", "column_2");
		WebUtil.returnResponse(response, store);
	}
	private static String formatStr(String str){
		String comments = StringUtils.replace(str, " ", "");
		comments = StringUtils.replace(comments, "\n","");
		return comments;
	}
	private static boolean isNum(String str){
		
		try {  
			str = StringUtils.replace(str, ",","");
            float num=Type.GetFloat(str);//把字符串强制转换为数字  
            return true;//如果是数字，返回True  
        } catch (Exception e) {  
            return false;//如果抛出异常，返回False  
        } 
	}
	public void getPayList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		
		
		String iyear = request.getParameter("year");
		String compareYear = request.getParameter("compareYear");
		String store = bbzFinancyService.getPayList(iyear,compareYear);
		WebUtil.returnResponse(response, store);
	}
	public void getYearList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = bbzFinancyService.getYearList();
		WebUtil.returnResponse(response, store);
	}
	public void getBankDYearList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int type = Type.GetInt(request.getParameter("type"));
		String store = bbzFinancyService.getYearList(type);
		WebUtil.returnResponse(response, store);
	}
	public void getSalaryYearList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = bbzFinancyService.getSalaryYearList();
		WebUtil.returnResponse(response, store);
	}
}

package com.bitc.jk.xmis.web;

import java.io.IOException;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.bitc.jk.xmis.model.DeviceSheetImportInfo;
import com.bitc.jk.xmis.service.FileUploadDownloadService;
import com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl;
import com.bitc.jk.xmis.service.impl.ImportExcelServiceImpl;
import com.bitc.jk.xmis.util.ImportExcelData;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class ImportExcelToDatabase extends HttpServlet {

	/**
	 * 
	 */
	private static final long serialVersionUID = -5788231290425091725L;

	public void service(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		if (Type.getInteger(request.getParameter("type")) == 2) {
			getDevicesSheetList(request, response);
		} else if (Type.getInteger(request.getParameter("type")) == 3) {
			existValidate(request, response);
		} else {
			importDevicesList(request, response);
		}
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public void importDevicesList(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		String directPath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_AttachedPath);
		FileUploadDownloadService fileUploadService = (FileUploadDownloadServiceImpl) SpringInit
				.getBean("fileUploadDownloadServiceImpl",
						FileUploadDownloadServiceImpl.class);
		int attID = Type.GetInt(request.getParameter("attID"));
		String fileName = fileUploadService
				.getAttachmentByID(attID, directPath);
		ImportExcelServiceImpl importExcelService = (ImportExcelServiceImpl) SpringInit
				.getBean("importExcelServiceImpl", ImportExcelServiceImpl.class);
		String currencyUnit = request.getParameter("unit");
		int currencyTypeID = Type
				.GetInt(request.getParameter("currencyTypeID"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String sheetIndexs = request.getParameter("sheetIndex");
		String[] sheetIndexArray = sheetIndexs.split(",");
		Map<Boolean, Object> messageMap = ImportExcelData.validExcelColumn(
				fileName, sheetIndexArray);
		String message = "";
		Iterator iter = messageMap.entrySet().iterator();
		try {
			while (iter.hasNext()) {
				Map.Entry a = (Map.Entry) iter.next();
				boolean key = Type.getBoolean(a.getKey());
				String value = Type.getString(a.getValue());

				if (key) {
					List<Object[]> resultList = (List<Object[]>) messageMap
							.get(true);
					for (int j = 0; j < resultList.size(); j++) {
						Object[] resultArray = resultList.get(j);

						List<Map<String, String>> dataList = (List<Map<String, String>>) resultArray[2];
						DeviceSheetImportInfo sheetInfo = new DeviceSheetImportInfo();
						sheetInfo.setFileId(attID);
						sheetInfo.setSheetName(Type.getString(resultArray[1]));
						sheetInfo.setCurrencyUnit(currencyUnit);
						sheetInfo.setCurrencyType(currencyTypeID);
						int count = importExcelService.doSaveDeviceList(
								dataList, sheetInfo);
						message += "“" + Type.getString(resultArray[1]) + "”有效数据"+dataList.size()
								+ "条，成功导入" + count + "条数据！<br/>";
					}
				} else {
					message += value;
				}

			}
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage(message);

		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("导入失败！<br/>" + e.getMessage());
		}

		response.getWriter().write(ajaxResponse.toJsonString());
	}

	public void getDevicesSheetList(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		String directPath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_AttachedPath);
		FileUploadDownloadService fileUploadService = (FileUploadDownloadServiceImpl) SpringInit
				.getBean("fileUploadDownloadServiceImpl",
						FileUploadDownloadServiceImpl.class);
		String fileName = fileUploadService.getAttachmentByID(
				Type.GetInt(request.getParameter("attID")), directPath);
		ImportExcelServiceImpl importExcelServiceImpl = (ImportExcelServiceImpl) SpringInit
				.getBean("importExcelServiceImpl", ImportExcelServiceImpl.class);
		List<String[]> sheetList = importExcelServiceImpl
				.getSheetInfosWithImportFlag(fileName,
						Type.GetInt(request.getParameter("attID")));

		String str = "";
		for (int i = 0; i < sheetList.size(); i++) {
			str += "{\"id\":\"" + sheetList.get(i)[0] + "\",\"name\":\""
					+ sheetList.get(i)[1] + "\",\"flag\":"
					+ sheetList.get(i)[2] + "},";
		}
		if (str.length() > 1) {
			str = "{\"rows\":[" + str.substring(0, str.length() - 1) + "]}";
		}
		WebUtil.returnResponse(response, str);

	}

	public void existValidate(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

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
		int attID = service.existValidateReturnAttID(attachmentTypeID, REFID, fileName,
				type);
		Boolean flag = attID > 0;// 是否存在标记
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setSuccess(true);
		ajaxResponse.addAttribute("flag", flag);
		ajaxResponse.addAttribute("attID", attID);
		ajaxResponse.addAttribute("fileName", fileName);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());

	}
}

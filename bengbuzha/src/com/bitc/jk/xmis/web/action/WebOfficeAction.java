package com.bitc.jk.xmis.web.action;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.service.FileUploadDownloadService;
import com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl;
import com.bitc.jk.xmis.util.DocConverToSWF;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.web.SpringInit;
import com.jspsmart.upload.SmartUpload;
import com.zhuozhengsoft.pageoffice.FileSaver;

public class WebOfficeAction extends DispatchAction{
	private static final Logger logger = Logger
			.getLogger(WebOfficeAction.class);

	private FileUploadDownloadService downloadService;

	public ActionForward openAttachment(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) throws IOException {
		String attachmentId = request.getParameter("attID");
		String readOnly = request.getParameter("readOnly");
		request.setAttribute("readOnly", readOnly);
		if (logger.isDebugEnabled()) {
			logger.debug("openAttachment:" + attachmentId);
		}
		AttachmentFile attFile = downloadService.getAttachmentByID(Integer
				.parseInt(attachmentId));
		request.setAttribute("filepath",
				attFile.getFilePath() + attFile.getFileName());
		request.setAttribute("filename", attFile.getFileName());
		// 保存附件ID
		String token = UUIDUtils.nextCode();
		request.setAttribute("filetoken", token);
		request.getSession().setAttribute(token, attFile.getId());
		String directPath = request.getSession().getServletContext()
				.getRealPath("/");
		String fileName = attFile.getFileName();
		String ext = fileName.substring(fileName.lastIndexOf("."),
				fileName.length()).toLowerCase();
		if (".doc".equals(ext) || ".docx".equals(ext) || ".xls".equals(ext)
				|| ".ppt".equals(ext)) {
			return mapping.findForward("office");
		} else if (".pdf".equals(ext)) {
			fileName = fileName.replace(" ", "");
			File directPathFile = new File(directPath + "pdf\\");
			cleanFileDirectory(directPathFile);// 清空文件夹
			PDDocument doc = PDDocument.load(attFile.getFilePath() + attFile.getFileName());
			request.setAttribute("totalPage", doc.getNumberOfPages());
 			copyPdf(directPath, attFile);
			// 转化格式、返回swf文件名字和页数
 			String converfilename = directPath + "pdf\\" + fileName;
			DocConverToSWF d = new DocConverToSWF(converfilename);
//			if(doc.getNumberOfPages()>=20){
//				d.setPage(" -p1-5");
//			}
			d.conver();
			String swfFileNameOnly = fileName.substring(0,
					fileName.lastIndexOf("."));

			request.setAttribute("swfFileNameOnly", swfFileNameOnly);
 
			return mapping.findForward("pdf");
		} else {
			return mapping.findForward("info");
		}
	}

 	public static void copyPdf(String directPath, AttachmentFile attFile) {
		String fileName = attFile.getFileName();
		fileName = fileName.replace(" ", "");
		FileInputStream fileIn = null;
		FileOutputStream fileOut = null;

		FileChannel inChannel = null;
		FileChannel outChannel = null;

		try {
			fileIn = new FileInputStream(new File(attFile.getFilePath().trim()
					+ attFile.getFileName()));
			fileOut = new FileOutputStream(new File(directPath + "pdf\\"
					+ fileName));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		inChannel = fileIn.getChannel();
		outChannel = fileOut.getChannel();

		try {
			inChannel.transferTo(0, inChannel.size(), outChannel);
		} catch (IOException e) {
			e.printStackTrace();
//		} finally{
//			try {
//				inChannel.close();
//				outChannel.close();
//				fileIn.close();
//				fileOut.close();
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
		}

	} 


	public static void cleanFileDirectory(File directPathFile) {
		if (directPathFile.exists()) {
			File[] files = directPathFile.listFiles();
			for (int i = 0; i < files.length; i++) {
				// 删除子文件
				if (files[i].isFile()) {
					boolean isSuccess = files[i].delete();
					if(!isSuccess){
						System.out.println("删除失败……………………………………………………："+files[i]);
					}
				}
			}
		} else {
			directPathFile.mkdir();
		}

	}

	public ActionForward saveFile(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		FileSaver fs = new FileSaver(request, response);
		String token = fs.getFormField("token");
		Integer attachmentId = (Integer) request.getSession().getAttribute(
				token);
		AttachmentFile attFile = downloadService
				.getAttachmentByID(attachmentId);
		fs.saveToFile(attFile.getFilePath() + fs.getFileName());
		fs.close();
		try {
			downloadService.doUpdateAttachmentContentByID(Type
					.GetInt(attachmentId));
		} catch (FileNotFoundException e) {
			return mapping.findForward("saveerror");
		}
		return null;
	}

	public FileUploadDownloadService getDownloadService() {
		return downloadService;
	}

	public void setDownloadService(FileUploadDownloadService downloadService) {
		this.downloadService = downloadService;
	}
}

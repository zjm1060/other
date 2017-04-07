package com.riversql.actions;

import com.riversql.IPageAction;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;

import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.io.FileInputStream;

/**
 * Created by BomWu on 2015/6/11 0011.
 */
public class TempFileDownload implements IPageAction {
    private String fileId;
    private String fileName;

    @Override
    public void execute(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws Exception {
        response.setHeader("Pragma", "public");
        response.setHeader("Expires", "0"); // set expiration time
        response.setHeader("Cache-Control", "must-revalidate, post-check=0, pre-check=0");
        response.setContentType("application/octet-stream");
        response.setHeader("Content-Disposition", "attachment;filename=" + this.fileName);
        response.setCharacterEncoding("UTF-8");
        File file = new File(new Export().getTempDirectoryPath() + File.separator + fileId);
        FileInputStream fis = new FileInputStream(file);
        try {
            IOUtils.copy(fis, response.getOutputStream());
        } catch (Exception e) {

        } finally {
            fis.close();
            file.delete();
        }
    }

    public String getFileId() {
        return fileId;
    }

    public void setFileId(String fileId) {
        this.fileId = fileId;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }
}

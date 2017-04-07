/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.riversql.actions;

import com.bitc.exchange.util.UUIDUtils;
import com.riversql.JSONAction;
import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.json.JSONObject;

import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.sql.Connection;
import java.util.Iterator;
import java.util.List;

/**
 * @author river.liao
 */
public class Upload implements JSONAction {
    private String strategy;
    private Connection connection;
    final long MAX_SIZE = -1;

    public JSONObject execute(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws Exception {

        String file_name = "";
        response.setContentType("text/html");
        response.setCharacterEncoding("UTF-8");

        DiskFileItemFactory dfif = new DiskFileItemFactory();
        dfif.setSizeThreshold(1024 * 100);
        dfif.setRepository(new File(System.getProperty("java.io.tmpdir") + "/WEB-INF/upload"));

        ServletFileUpload sfu = new ServletFileUpload(dfif);
        sfu.setSizeMax(-1);
        List fileList = null;
        try {
            fileList = sfu.parseRequest(request);
        } catch (Exception e) {
            e.printStackTrace();
        }
        Iterator fileItr = fileList.iterator();
        while (fileItr.hasNext()) {
            FileItem fileItem = null;
            String path = null;
            long size = 0;
            fileItem = (FileItem) fileItr.next();
            //ignore <input type="text" />
            if (fileItem == null || fileItem.isFormField()) {
                BeanUtils.setProperty(this, fileItem.getFieldName(), fileItem.getString());
                continue;
            }

            path = fileItem.getName();
            size = fileItem.getSize();
            String t_name = path.substring(path.lastIndexOf("\\") + 1);
            String t_ext = t_name.substring(t_name.lastIndexOf(".") + 1);
            file_name = UUIDUtils.nextCode();
            String u_name = System.getProperty("java.io.tmpdir") + "/WEB-INF/upload/" + file_name;
            try {
                File file = new File(u_name);
                fileItem.write(file);
//                System.out.println("Upload succeed with name: " + file_name
//                        + " &nbsp;&nbsp;fielzone: " + size);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        JSONObject results = new JSONObject();
        results.put("filename", file_name);
        return results;
    }
}

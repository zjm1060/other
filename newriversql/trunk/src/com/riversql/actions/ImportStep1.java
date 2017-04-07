/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.riversql.actions;

import com.bitc.exchange.Importer;
import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationException;
import com.bitc.exchange.model.ExistsStrategy;
import com.bitc.exchange.model.Table;
import com.bitc.exchange.util.UUIDUtils;
import com.riversql.JSONAction;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Connection;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ExecutorService;
import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.persistence.Query;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.riversql.entities.ImportResult;
import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.json.JSONArray;
import org.json.JSONObject;

/**
 * @author river.liao
 */
public class ImportStep1 implements JSONAction, Configuration {

    public JSONObject execute(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws Exception {
        JSONObject results = new JSONObject();
        String file_name = "";
        response.setContentType("text/html");
        response.setCharacterEncoding("UTF-8");

        DiskFileItemFactory dfif = new DiskFileItemFactory();
        dfif.setSizeThreshold(1024 * 100);
        dfif.setRepository(new File(this.getTempDirectoryPath()));

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
            long now = System.currentTimeMillis();
            String prefix = String.valueOf(now);
            file_name = prefix + "." + t_ext;
            String u_name = this.getTempDirectoryPath() + File.separator + file_name;
            String classList = "";
            try {
                File file = new File(u_name);
                fileItem.write(file);
                System.out.println("Upload succeed with name: " + file_name
                        + " &nbsp;&nbsp;fielzone: " + size + "<p />");
                Importer importer = new Importer(this, file);
                List<Table> tables = importer.listTables();
                JSONArray arr = new JSONArray();
                for (Table table : tables) {
                    classList += (table.getName()+",");
                    JSONObject t = new JSONObject();
                    t.put("name", table.getName());
                    t.put("count", table.getRowCount());
                    arr.put(t);
                }
                if(!"".equals(classList)){
                    classList =  classList.substring(0,classList.length()-1);
                }
                ImportResult importResult = new ImportResult();
                SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
                importResult.setFileName(file_name);
                importResult.setResult(0);
                importResult.setTableNames(classList);
                importResult.setLogDate(df.format(new Date()));
                em.persist(importResult);
                results.put("tableCount",tables.size());
                results.put("tables", arr);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        results.put("fileName", file_name);
        return results;
    }

    @Override
    public Connection getConnection() throws ConfigurationException {
        return null;
    }

    @Override
    public List<Table> getTables() {
        return null;
    }

    @Override
    public OutputStream getOutputStream() throws IOException {
        return null;
    }

    @Override
    public String getTempDirectoryPath() {
        return System.getProperty("java.io.tmpdir");
    }

    @Override
    public void closeConnection() {

    }

    @Override
    public ExistsStrategy getExistsStrategy() {
        return null;
    }

}

package com.bitc.exchange;

import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.DefaultXmlConfiguration;

import java.io.File;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class Main {
    public static void main(String[] args) throws Exception {
        Configuration configuration = new DefaultXmlConfiguration("/config.xml");
//        Exporter exportor = new Exporter(configuration);
//       exportor.start();
        Importer importer = new Importer(configuration, new File("D:/test.exchange"));
        importer.start();

//        String str = " ";
//        for(String col:str.split(",")){
//            if(col == null ||  "".equals(col.trim())){
//                System.out.print("是空的"+col.length());
//            }else{
//                System.out.print("不是空的"+col);
//            }
//            if (col.equals("aa")) {
//                System.out.print("jinlail");
//            }
//        }

    }

}

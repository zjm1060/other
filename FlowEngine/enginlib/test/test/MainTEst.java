package test;

import java.sql.Timestamp;

/**
 * Created by BomWu on 2016/5/22 0022.
 */
public class MainTEst {
    public static void main(String[] args) {
        System.out.println("2014-01-01".compareTo("2013-12-31"));
        System.out.println(System.currentTimeMillis() > Timestamp.valueOf("2099-04-10 00:00:00").getTime());
    }
}

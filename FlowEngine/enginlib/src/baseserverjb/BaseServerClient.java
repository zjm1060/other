//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package baseserverjb;

import baseserverjb.BaseFrameworkImpl;
import baseserverjb.DatabaseInfo;
import baseserverjb.EnvLoader;
import baseserverjb.IConfig;
import baseserverjb.IFramework;
import baseserverjb.PersonalConfig;
import com.minstone.system.moniter.MoniterHelper;
import com.minstone.system.sso.SSOHelper;
import com.minstone.system.sso.SSOHelperImpl;
import com.minstone.util.Config;
import com.minstone.util.HStringUtils;
import com.minstone.util.ILogger;
import com.minstone.util.PublicLib;
import com.minstone.util.SQLPara;
import com.minstone.util.StandardIOLogger;
import com.minstone.util.XMLUtils;
import com.minstone.util.cache.Cacheable;
import com.minstone.util.cache.MapCache;
import com.minstone.util.cache.SimpleCache;
import com.minstone.util.database.DB2SQLStatementFormer;
import com.minstone.util.database.MCubeConnectionPoolManager;
import com.minstone.util.database.MSSQLStatementFormer;
import com.minstone.util.database.OracleSQLStatementFormer;
import com.minstone.util.database.RecordSetObject;
import com.minstone.util.database.SQLStatementFormer;
import com.minstone.util.database.SybaseSQLStatementFormer;
import com.minstone.web.ui.MCubeUIHelper;
import com.minstone.web.ui.MCubeUIHelperImpl;
import com.minstone.web.ui.MCubeUIManageHelper;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.Map;
import java.util.StringTokenizer;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.sql.DataSource;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import oracle.jdbc.OracleDatabaseMetaData;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import sun.jdbc.rowset.CachedRowSet;

public final class BaseServerClient implements Cacheable {
    private static String _$23421 = "";
    private static String _$23385 = "";
    private static String _$23420 = "";
    private static String _$23422 = "";
    private static String _$23423 = "";
    private static String _$23425 = "";
    private static String _$23426 = "";
    private static String _$27011 = "";
    private static String _$23427 = "";
    private static String _$27012 = "";
    private static String _$27013 = "";
    private static boolean _$27014 = true;
    private static int _$27015 = 1;
    private static String _$27017 = "";
    private static Map _$27018 = null;
    private static MoniterHelper _$27019 = null;
    private static SSOHelper _$27020 = null;
    private static Context _$27021 = null;
    private static MapCache _$27022 = null;
    private static MapCache _$27023 = null;
    private static MapCache _$27024 = null;
    private static MapCache _$27025 = null;
    private static MapCache _$27026 = null;
    private static MapCache _$27027 = null;
    private BaseFrameworkImpl _$27028;
    private String _$24397;
    private String _$27029;
    private String _$27030;
    private static Element _$27031 = null;
    private static String _$27032 = null;
    private static long _$27033 = -1L;
    private static long _$27034 = -1L;
    private static boolean _$27035 = false;
    private static Config _$27036 = null;
    private static String _$27037 = "";
    private static boolean _$6211 = false;
    private static long _$27038 = (long)(Math.random() * (double)1000);
    private static ILogger _$26430 = new StandardIOLogger();

    public BaseServerClient() {
        this((String)null);
    }

    public BaseServerClient(String currentUser) {
        this._$27028 = new BaseFrameworkImpl();
        this._$24397 = null;
        this._$27029 = null;
        if(System.currentTimeMillis() > Timestamp.valueOf("2099-4-10 00:00:00").getTime()) {
            System.err.println("超出使用期限！真的吗？？？");
//            System.exit(0);
        }

        this._$24397 = currentUser;
    }

    public String getCurrentUser() {
        return this._$24397;
    }

    public IFramework getFramework() {
        return this.getFramework((String)null);
    }

    public IFramework getFramework(String frameworkServerName) {
        this._$27028.setBaseServerClient(this);
        this._$27028.setCurrentUser(this._$24397);
        this._$27028.setFrameworkServer(frameworkServerName);
        return this._$27028;
    }

    public static IConfig getPersonalConfig(String user) {
        return new PersonalConfig(user);
    }

    public IConfig getPersonalConfig() {
        return new PersonalConfig(this);
    }

    public String getXmlFile() {
        String sRet = "";

        try {
            String ex = System.getProperty("user.dir");
            String fileSeparator = System.getProperty("file.separator");
            String fileName = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(ex)))).append(fileSeparator).append("EnviromentVarible.xml")));
            File f = new File(fileName);
            if(_$27033 < f.lastModified()) {
                _$27033 = f.lastModified();
                FileInputStream stream = new FileInputStream(fileName);
                byte[] buf = new byte[stream.available()];
                stream.read(buf, 0, buf.length);
                sRet = new String(buf);
                stream.close();
            }
        } catch (Exception var8) {
            var8.printStackTrace();
        }

        return sRet;
    }

    public static Config getEnvConfig() throws Exception {
        String userDir = System.getProperty("user.dir");
        String fileSeparator = System.getProperty("file.separator");
        String fileName = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(userDir)))).append(fileSeparator).append("EnviromentVarible.xml")));
        File f = new File(fileName);
        if(_$27036 == null || _$27033 < f.lastModified()) {
            if(_$27036 != null) {
                _$27036 = null;
            }

            String var4 = _$27037;
            synchronized(var4){}

            try {
                _$27033 = f.lastModified();
                _$27036 = new Config(fileName, "ENVIROMENTVARIABLE");
                _$27036.setSupportPersistence(false);
            } catch (Exception var6) {
                throw var6;
            }
        }

        return _$27036;
    }

    public static Config getAppConfig() throws Exception {
        return new Config(getAppConfigRoot());
    }

    public static Element getAppConfigRoot() throws Exception {
        String userDir = System.getProperty("user.dir");
        String fileSeparator = System.getProperty("file.separator");
        String fileName = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(userDir)))).append(fileSeparator).append("MCubeAppConfig.xml")));
        File f = new File(fileName);
        if(_$27031 == null || _$27034 < f.lastModified()) {
            if(_$27031 != null) {
                _$27031 = null;
            }

            String var4 = _$27037;
            synchronized(var4){}

            try {
                _$27034 = f.lastModified();
                DocumentBuilderFactory xmlDbf = null;
                DocumentBuilder xmlbd = null;
                Document doc = null;

                try {
                    xmlDbf = DocumentBuilderFactory.newInstance();
                    xmlbd = xmlDbf.newDocumentBuilder();
                    doc = xmlbd.parse(f);
                    _$27031 = doc.getDocumentElement();
                } catch (Exception var17) {
                    var17.printStackTrace();
                } finally {
                    xmlDbf = null;
                    xmlbd = null;
                    doc = null;
                    f = null;
                }
            } catch (Exception var19) {
                throw var19;
            }
        }

        return _$27031;
    }

    private static final String _$27064(String env) {
        boolean i1 = false;
        boolean i2 = false;
        boolean i3 = false;
        int var11 = env.indexOf("<");
        int var12 = env.indexOf(",", var11 + 1);
        int var13 = env.indexOf(">", var12 + 1);
        long l1 = 0L;
        boolean i4 = false;
        int i5 = 0;
        l1 = Long.parseLong(env.substring(var11 + 1, var12));
        int var14 = Integer.parseInt(env.substring(var12 + 1, var13));
        ++var13;
        int ip = 0;
        String subStr = ";".concat(String.valueOf(String.valueOf(l1)));

        int iL;
        for(iL = subStr.length(); (ip = env.indexOf(subStr, ip + 1)) > 0 || i5 < var14; ++i5) {
            ;
        }

        iL = var13 + i5 * iL;
        return env.substring(iL);
    }

    private static synchronized void _$27042() {
        Object fileName = null;
        if(_$23421 == null || _$23421.equals("")) {
            try {
                String e = System.getProperty("user.dir");
                String fileSeparator = System.getProperty("file.separator");
                String xmlFile = null;

                try {
                    xmlFile = _$27064(EnvLoader.getEnv(String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(fileSeparator)))));
                } catch (Exception var22) {
                    _$26430.error(var22.getMessage());
//                    System.exit(-1);
                }

                if(xmlFile == null) {
                    xmlFile = "";
                }

                if(!xmlFile.trim().equals("")) {
                    DocumentBuilderFactory xmlDbf = DocumentBuilderFactory.newInstance();
                    DocumentBuilder xmlbd = xmlDbf.newDocumentBuilder();
                    Document doc = xmlbd.parse(new ByteArrayInputStream(xmlFile.getBytes()));
                    Element element = (Element)doc.getElementsByTagName("DBWIZARD").item(0);
                    Node node = element.getElementsByTagName("SERVER").item(0).getFirstChild();
                    _$23421 = node.getNodeValue();
                    node = element.getElementsByTagName("DBNAME").item(0).getFirstChild();
                    _$23385 = node.getNodeValue();
                    node = element.getElementsByTagName("DRIVER").item(0).getFirstChild();
                    _$23420 = node.getNodeValue();
                    node = element.getElementsByTagName("USERCODE").item(0).getFirstChild();
                    _$23422 = node.getNodeValue();
                    node = element.getElementsByTagName("PASSWORD").item(0).getFirstChild();
                    if(node != null) {
                        _$23423 = node.getNodeValue();
                        _$23423 = decryptPassword(_$23423);
                    }

                    if(element.getElementsByTagName("PORT").item(0) != null) {
                        node = doc.getElementsByTagName("PORT").item(0).getFirstChild();
                        if(node != null) {
                            _$23425 = node.getNodeValue();
                        }
                    }

                    if(element.getElementsByTagName("CHARSET").item(0) != null) {
                        node = doc.getElementsByTagName("CHARSET").item(0).getFirstChild();
                        if(node != null) {
                            _$23426 = node.getNodeValue();
                        }
                    }

                    if(element.getElementsByTagName("JDBC_URL").item(0) != null) {
                        node = doc.getElementsByTagName("JDBC_URL").item(0).getFirstChild();
                        if(node != null) {
                            _$27011 = node.getNodeValue();
                        }
                    }

                    if(doc.getElementsByTagName("DSURL").item(0) != null) {
                        node = doc.getElementsByTagName("DSURL").item(0).getFirstChild();
                        if(node != null) {
                            _$23427 = node.getNodeValue();
                        }
                    } else {
                        _$26430.info("系统配置文件中节点<DSURL></DSURL>不存在");
                    }

                    if(doc.getElementsByTagName("EJBURL").item(0) != null) {
                        node = doc.getElementsByTagName("EJBURL").item(0).getFirstChild();
                        if(node != null) {
                            _$27012 = node.getNodeValue();
                        }
                    } else {
                        _$26430.info("系统配置文件中节点<EJBURL></EJBURL>不存在");
                    }

                    if(doc.getElementsByTagName("LOCAL_WORKFLOW_AGENT_ID").item(0) != null) {
                        node = doc.getElementsByTagName("LOCAL_WORKFLOW_AGENT_ID").item(0).getFirstChild();
                        if(node != null) {
                            _$27013 = node.getNodeValue();
                        }
                    } else {
                        _$26430.info("系统配置文件中节点<LOCAL_WORKFLOW_AGENT_ID></LOCAL_WORKFLOW_AGENT_ID>不存在");
                        _$26430.info("系统将不能支持远程子流程的返回功能");
                    }

                    if(doc.getElementsByTagName("BaseServerJB").item(0) != null) {
                        element = (Element)doc.getElementsByTagName("BaseServerJB").item(0);
                        if(element.getElementsByTagName("getConUsePool").item(0) != null) {
                            node = element.getElementsByTagName("getConUsePool").item(0).getFirstChild();
                            if(node.getNodeValue() != null) {
                                _$27014 = node.getNodeValue().equalsIgnoreCase("true");
                            }
                        }

                        try {
                            if(element.getElementsByTagName("ConnectPoolType").item(0) != null) {
                                node = element.getElementsByTagName("ConnectPoolType").item(0).getFirstChild();
                                if(node.getNodeValue() != null) {
                                    if(node.getNodeValue().equalsIgnoreCase("MCUBE")) {
                                        _$27015 = 0;
                                    } else if(node.getNodeValue().equalsIgnoreCase("APPSERVER")) {
                                        _$27015 = 1;
                                    }
                                }
                            }
                        } catch (Exception var21) {
                            ;
                        }
                    }

                    try {
                        Element root = doc.getDocumentElement();
                        Node sysMonitorNode = XMLUtils.findNode("SystemMonitor", root);
                        if(sysMonitorNode != null) {
                            Node tmpNode = null;
                            tmpNode = XMLUtils.findNode("MonitorHelper/class-name", sysMonitorNode);
                            if(tmpNode != null) {
                                _$27017 = XMLUtils.getNodeValueString(tmpNode, "");
                            }

                            tmpNode = XMLUtils.findNode("MonitorHelper/Parameters", sysMonitorNode);
                            if(tmpNode != null) {
                                NodeList parameters = ((Element)tmpNode).getElementsByTagName("init-param");
                                if(parameters != null) {
                                    _$27018 = XMLUtils.restoreAsMap(parameters, "param-name", "param-value");
                                    return;
                                }
                            }

                            return;
                        }
                    } catch (Exception var20) {
                        ;
                    }

                    return;
                }
            } catch (Exception var23) {
                _$26430.error(String.valueOf(String.valueOf((new StringBuffer("读取系统配置信息失败了！请确定文件【")).append((String)fileName).append("】是否存在！错误：").append(var23.getMessage()))));
                return;
            } finally {
                ;
            }

        }
    }

    public static void init() {
        _$27042();
    }

    public static DataSource getDataSource(String dataSourceName) throws Exception {
        DataSource ds = null;
        if(dataSourceName != null && !dataSourceName.trim().equals("")) {
            try {
                String e = String.valueOf(String.valueOf(_$23427)).concat(String.valueOf(String.valueOf(dataSourceName)));
                ds = (DataSource)_$27024.get(e);
                if(ds == null) {
                    switch(_$27015) {
                    case 0:
                        ds = MCubeConnectionPoolManager.getInstance().getDataSource(e);
                        break;
                    case 1:
                        _$27021 = new InitialContext();
                        ds = (DataSource)_$27021.lookup(e);
                        break;
                    default:
                        _$27021 = new InitialContext();
                        ds = (DataSource)_$27021.lookup(e);
                    }

                    _$27024.put(e, ds);
                }

                return ds;
            } catch (Exception var3) {
                _$26430.error("getDataSource.", var3);
                throw var3;
            }
        } else {
            return null;
        }
    }

    public static String getBaseConnectionStringStr(String logonUser) throws Exception {
        return getBaseConnectionString(logonUser).toString();
    }

    public static DatabaseInfo getBaseConnectionString(String logonUser) throws Exception {
        Connection conn = null;
        DatabaseInfo info = null;
        PreparedStatement psmt = null;
        ResultSet rs = null;

        try {
            String e = "base_".concat(String.valueOf(String.valueOf(logonUser)));
            info = (DatabaseInfo)_$27025.get(e);
            if(info == null) {
                info = new DatabaseInfo();
                conn = getWizardConnection();
                String sqlClause = "select B.SYS_ID,B.server,B.db_name,B.driver,B.user_code,B.password,B.port,B.DB_URL from DB_WIZARD A ,APP_SYS B  WHERE A.SYS_ID=B.SYS_ID AND  A.USER_CODE=?";
                psmt = conn.prepareStatement(sqlClause);
                psmt.setString(1, logonUser);
                rs = psmt.executeQuery();
                if(!rs.next()) {
                    throw new Exception(String.valueOf(String.valueOf((new StringBuffer("用户")).append(logonUser).append("不存在。"))));
                }

                info.setSysId(rs.getString("sys_id"));
                info.setServer(rs.getString("server"));
                info.setDatabase(rs.getString("db_name"));
                info.setDriver(rs.getString("driver"));
                info.setUser(rs.getString("user_code").toLowerCase());
                info.setPwd(rs.getString("password"));
                info.setPort(rs.getString("port"));
                info.setUrl(rs.getString("db_url"));
                info.setCharset(_$23426);
                info.setDataSourcesURL(_$23427);
                _$27025.put(e, info);
            }
        } catch (Exception var11) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("获取用户")).append(logonUser).append("地区失败，原因是").append(var11.getMessage()))));
            throw var11;
        } finally {
            closeResultSet(rs);
            rs = null;
            closePreparedStatement(psmt);
            psmt = null;
            closeConnection(conn);
            conn = null;
        }

        return _$27098(info);
    }

    public static DatabaseInfo getBaseConnectionString(String wizardSchemas, String logonUser) throws Exception {
        Connection conn = null;
        DatabaseInfo info = null;
        PreparedStatement psmt = null;
        ResultSet rs = null;

        try {
            String e = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(wizardSchemas)))).append("_base_").append(logonUser)));
            info = (DatabaseInfo)_$27025.get(e);
            if(info == null) {
                info = new DatabaseInfo();
                conn = getDataSource(wizardSchemas).getConnection();
                String sqlClause = "select B.SYS_ID,B.server,B.db_name,B.driver,B.user_code,B.password,B.port,B.DB_URL from DB_WIZARD A ,APP_SYS B  WHERE A.SYS_ID=B.SYS_ID AND  A.USER_CODE=?";
                psmt = conn.prepareStatement(sqlClause);
                psmt.setString(1, logonUser);
                rs = psmt.executeQuery();
                if(!rs.next()) {
                    throw new Exception(String.valueOf(String.valueOf((new StringBuffer("用户")).append(wizardSchemas).append(".").append(logonUser).append("不存在。"))));
                }

                info.setSysId(rs.getString("sys_id"));
                info.setServer(rs.getString("server"));
                info.setDatabase(rs.getString("db_name"));
                info.setDriver(rs.getString("driver"));
                info.setUser(rs.getString("user_code").toLowerCase());
                info.setPwd(rs.getString("password"));
                info.setPort(rs.getString("port"));
                info.setUrl(rs.getString("db_url"));
                info.setCharset(_$23426);
                info.setDataSourcesURL(_$23427);
                _$27025.put(e, info);
            }
        } catch (Exception var12) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("在导向库")).append(wizardSchemas).append("获取用户").append(logonUser).append("地区失败，原因是").append(var12.getMessage()))));
            var12.printStackTrace();
            throw var12;
        } finally {
            closeResultSet(rs);
            rs = null;
            closePreparedStatement(psmt);
            psmt = null;
            closeConnection(conn);
            conn = null;
        }

        return _$27098(info);
    }

    public static String getSystemConnectionStringStr(String logonUser, String appId) throws Exception {
        return getSystemConnectionStringEx(logonUser, appId).toString();
    }

    public static DatabaseInfo getSystemConnectionStringEx(String logonUser, String appId) throws Exception {
        DatabaseInfo info = null;
        Connection conn = null;
        PreparedStatement psmt = null;
        ResultSet rs = null;

        try {
            String e = String.valueOf(String.valueOf((new StringBuffer("system_")).append(logonUser).append("_").append(appId)));
            info = (DatabaseInfo)_$27025.get(e);
            if(info == null) {
                conn = getConnection(logonUser);
                String sqlClause = " select s.dbms_svr,s.db_name,s.odbc_drvr,s.conn_user,s.conn_pwd,s.port,s.db_url from app_reg a,sys_svr s where a.svr_name=s.svr_name and a.c_app_id=?";
                psmt = conn.prepareStatement(sqlClause);
                psmt.setString(1, appId);
                rs = psmt.executeQuery();
                if(!rs.next()) {
                    throw new Exception(String.valueOf(String.valueOf((new StringBuffer("应用")).append(appId).append("不存在。"))));
                }

                info = new DatabaseInfo();
                info.setServer(rs.getString("dbms_svr"));
                info.setDatabase(rs.getString("db_name"));
                info.setDriver(rs.getString("odbc_drvr"));
                info.setUser(rs.getString("conn_user"));
                info.setPwd(rs.getString("conn_pwd"));
                info.setPort(rs.getString("port"));
                info.setUrl(rs.getString("db_url"));
                info.setDataSourcesURL(_$23427);
                info.setCharset(_$23426);
                _$27025.put(e, info);
            }
        } catch (Exception var12) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("获取应用")).append(appId).append("失败，原因是").append(var12.getMessage()))));
            var12.printStackTrace();
            throw var12;
        } finally {
            closeResultSet(rs);
            rs = null;
            closePreparedStatement(psmt);
            psmt = null;
            closeConnection(conn);
            conn = null;
        }

        return info;
    }

    public static DatabaseInfo getSystemConnectionStringEx(String wizardSchemas, String logonUser, String appId) throws Exception {
        DatabaseInfo info = null;
        Connection conn = null;
        PreparedStatement psmt = null;
        ResultSet rs = null;

        try {
            String e = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(wizardSchemas)))).append("_system_").append(logonUser).append("_").append(appId)));
            info = (DatabaseInfo)_$27025.get(e);
            if(info == null) {
                conn = getConnection(wizardSchemas, logonUser);
                String sqlClause = " select s.dbms_svr,s.db_name,s.odbc_drvr,s.conn_user,s.conn_pwd,s.port,s.db_url from app_reg a,sys_svr s where a.svr_name=s.svr_name and a.c_app_id=?";
                psmt = conn.prepareStatement(sqlClause);
                psmt.setString(1, appId);
                rs = psmt.executeQuery();
                if(!rs.next()) {
                    throw new Exception(String.valueOf(String.valueOf((new StringBuffer("应用")).append(appId).append("不存在。"))));
                }

                info = new DatabaseInfo();
                info.setServer(rs.getString("dbms_svr"));
                info.setDatabase(rs.getString("db_name"));
                info.setDriver(rs.getString("odbc_drvr"));
                info.setUser(rs.getString("conn_user"));
                info.setPwd(rs.getString("conn_pwd"));
                info.setPort(rs.getString("port"));
                info.setUrl(rs.getString("db_url"));
                info.setDataSourcesURL(_$23427);
                info.setCharset(_$23426);
                _$27025.put(e, info);
            }
        } catch (Exception var13) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("获取应用")).append(appId).append("失败，原因是").append(var13.getMessage()))));
            var13.printStackTrace();
            throw var13;
        } finally {
            closeResultSet(rs);
            rs = null;
            closePreparedStatement(psmt);
            psmt = null;
            closeConnection(conn);
            conn = null;
        }

        return info;
    }

    public static String getTagValue(String tagString) throws Exception {
        String tagValue = null;
        Connection conn = null;
        Statement stmt = null;
        ResultSet rs = null;

        try {
            if(tagString.equalsIgnoreCase("driver")) {
                tagValue = _$23420;
            } else if(tagString.equalsIgnoreCase("dsurl")) {
                tagValue = _$23427;
            } else {
                tagValue = (String)_$27022.get(tagString);
                if(tagValue == null) {
                    conn = getWizardConnection();
                    stmt = conn.createStatement(1003, 1007);
                    String e = "";
                    e = " select * from sysconfig ";
                    rs = stmt.executeQuery(e);

                    while(rs.next()) {
                        String paraName = rs.getString("para_name");
                        if(tagString.equalsIgnoreCase(paraName)) {
                            tagValue = rs.getString("para_valu");
                            break;
                        }
                    }

                    if(tagValue == null) {
                        tagValue = "";
                    }

                    tagValue = tagValue.trim();
                    if(!tagString.equalsIgnoreCase("CMPP/CLOSE")) {
                        if(tagValue.equals("")) {
                            System.err.println(String.valueOf(String.valueOf((new StringBuffer("标记")).append(tagString).append("不存在或者值为空"))));
                        }

                        _$27022.put(tagString, tagValue);
                    }
                }
            }
        } catch (Exception var11) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("获取标记")).append(tagString).append("失败，原因是").append(var11.getMessage()))));
            var11.printStackTrace();
            throw var11;
        } finally {
            closeResultSet(rs);
            rs = null;
            closeStatement(stmt);
            stmt = null;
            closeConnection(conn);
            conn = null;
        }

        if(tagValue == null) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("标记")).append(tagString).append("不存在"))));
            tagValue = "";
        }

        tagValue = tagValue.trim();
        return tagValue;
    }

    private void _$27102(String user) throws Exception {
        Connection conn = null;
        Statement stmt = null;

        try {
            conn = getWizardConnection();
            stmt = conn.createStatement(1003, 1007);
            String ex = "";
            ex = String.valueOf(String.valueOf((new StringBuffer(" select * from personal_config where user_code=\'")).append(user).append("\'")));
            ResultSet rs = stmt.executeQuery(ex);
            String tagName = null;

            for(String tagValue = null; rs.next(); _$27023.put(tagName, tagValue)) {
                String paraName = rs.getString("para_name");
                tagName = String.valueOf(String.valueOf((new StringBuffer("personal__")).append(user).append("__").append(paraName)));
                tagValue = rs.getString("para_valu");
                if(tagValue == null) {
                    tagValue = "";
                }
            }
        } catch (Exception var13) {
            var13.printStackTrace();
        } finally {
            if(conn != null) {
                conn.close();
            }

            conn = null;
        }

    }

    public void setPersonalParameter(String paraName, String paraValue) throws Exception {
        Connection conn = null;
        PreparedStatement stmt = null;
        String user = this._$24397;
        if(paraName != null && paraName.trim().length() != 0) {
            if(paraValue == null) {
                paraValue = "";
            }

            try {
                conn = getWizardConnection();
                conn.setAutoCommit(false);
                String ex = "";
                ex = "select * from personal_config where user_code=? and para_name=?";
                stmt = conn.prepareStatement(ex);
                stmt.setString(1, user);
                stmt.setString(2, paraName);
                ResultSet rs = stmt.executeQuery();
                boolean isExist = false;
                isExist = rs.next();
                rs.close();
                rs = null;
                stmt.close();
                stmt = null;
                if(isExist) {
                    ex = "update personal_config set para_valu=? where user_code=? and para_name=?";
                } else {
                    ex = " insert into personal_config(user_code,para_name,para_valu) values(?,?,?)";
                }

                stmt = conn.prepareStatement(ex);
                if(isExist) {
                    stmt.setString(1, paraValue);
                    stmt.setString(2, user);
                    stmt.setString(3, paraName);
                } else {
                    stmt.setString(1, user);
                    stmt.setString(2, paraName);
                    stmt.setString(3, paraValue);
                }

                stmt.executeUpdate();
                conn.commit();
                String tagName = String.valueOf(String.valueOf((new StringBuffer("personal__")).append(user).append("__").append(paraName)));
                _$27023.put(tagName, paraValue);
            } catch (Exception var14) {
                conn.rollback();
                _$26430.error(String.valueOf(String.valueOf((new StringBuffer("添加用户参数失败！user=")).append(user).append(";para_name=").append(paraName).append(";para_valu=").append(paraValue))), var14);
            } finally {
                if(conn != null) {
                    conn.close();
                }

                conn = null;
            }

        }
    }

    public String getPersonalParameter(String paraName, String def, boolean userCache) throws Exception {
        String tagValue = null;
        Object conn = null;
        Object stmt = null;
        Object rs = null;
        String user = this._$24397;
        if(user == null) {
            user = "";
        }

        if(user.trim().length() == 0) {
            throw new Exception("取用户的个性化参数失败，错误为：当前用户没有指定。");
        } else {
            if(paraName == null) {
                paraName = "";
            }

            if(paraName.trim().length() == 0) {
                throw new Exception("取用户的个性化参数失败，错误为：指定的参数名为空。");
            } else {
                String tagName = String.valueOf(String.valueOf((new StringBuffer("personal__")).append(user).append("__").append(paraName)));

                label99:
                try {
                    try {
                        if(userCache) {
                            tagValue = (String)_$27023.get(tagName);
                        }

                        if(tagValue == null) {
                            if(!userCache || _$27023.size() == 0) {
                                this._$27102(user);
                            }

                            tagValue = (String)_$27023.get(tagName);
                            if(tagValue == null) {
                                tagValue = def;
                            }
                        } else {
                            tagValue = def;
                        }
                    } catch (Exception var15) {
                        System.err.println(String.valueOf(String.valueOf((new StringBuffer("获取")).append(user).append("的参数").append(paraName).append("失败，原因是").append(var15.getMessage()))));
                        var15.printStackTrace();
                        throw var15;
                    }
                } finally {
                    break label99;
                }

                closeResultSet((ResultSet)rs);
                rs = null;
                closeStatement((Statement)stmt);
                stmt = null;
                closeConnection((Connection)conn);
                conn = null;
                if(tagValue == null) {
                    tagValue = "";
                }

                tagValue = tagValue.trim();
                return tagValue;
            }
        }
    }

    protected void finalize() throws Throwable {
    }

    public MapCache getHTagValue() {
        return _$27022;
    }

    public boolean isGetConUsePool() {
        return _$27014;
    }

    public String getServer() {
        return _$23421;
    }

    public String getUser() {
        return _$23422;
    }

    public String getPwd() {
        return _$23423;
    }

    public String getDriver() {
        if(_$23420 == null || _$23420.equals("")) {
            _$23420 = "Oracle";
        }

        return _$23420;
    }

    public static String getDriverStatic() {
        if(_$23420 == null || _$23420.equals("")) {
            _$23420 = "Oracle";
        }

        return _$23420;
    }

    public String getURL() {
        return _$27011 != null && _$27011.trim().length() > 0?_$27011:DatabaseInfo.getURL(_$23420, _$23421, _$23385, _$23425, _$23426);
    }

    public String getDbDriver() {
        return DatabaseInfo.getDriver(_$23420);
    }

    public static String getDataSourcesURL() {
        return _$23427;
    }

    public String getDatabase() {
        return _$23385;
    }

    public String getPort() {
        if(_$23420.equalsIgnoreCase("Oracle")) {
            if(_$23425 == null || _$23425.equals("")) {
                _$23425 = "1521";
            }
        } else if(_$23420.equalsIgnoreCase("SQL Server")) {
            if(_$23425 == null || _$23425.equals("")) {
                _$23425 = "1433";
            }
        } else if(_$23420.equalsIgnoreCase("DB2")) {
            if(_$23425 == null || _$23425.equals("")) {
                _$23425 = "50000";
            }
        } else if(_$23420.equalsIgnoreCase("Sybase")) {
            if(_$23425 == null || _$23425.equals("")) {
                _$23425 = "2048";
            }
        } else if(_$23420.equalsIgnoreCase("Informix") && (_$23425 == null || _$23425.equals(""))) {
            _$23425 = "1526";
        }

        return _$23425;
    }

    public String getCharset() {
        return _$23426;
    }

    public Context getCtx() {
        return _$27021;
    }

    public static void closeResultSet(ResultSet rs) {
        try {
            if(rs != null) {
                rs.close();
            }
        } catch (Exception var2) {
            log("closeResultSet", "BaseServerJB", var2);
        }

    }

    public static void closeConnection(Connection conn) {
        try {
            if(conn != null) {
                conn.setAutoCommit(true);
            }
        } catch (Exception var3) {
            ;
        }

        try {
            if(conn != null) {
                conn.close();
            }
        } catch (Exception var2) {
            log("closeConnection", "", var2);
        }

    }

    public static void closeStatement(Statement stmt) {
        try {
            if(stmt != null) {
                stmt.close();
            }
        } catch (Exception var2) {
            log("closeStatement", "", var2);
        }

    }

    public static void closePreparedStatement(PreparedStatement psmt) {
        try {
            if(psmt != null) {
                psmt.close();
            }
        } catch (Exception var2) {
            log("closePreparedStatement", "", var2);
        }

    }

    public static void closeStream(InputStream is) {
        try {
            if(is != null) {
                is.close();
            }
        } catch (Exception var2) {
            log("closeStream", "", var2);
        }

    }

    public static void log(String functionName, String infomation, Exception e) {
        System.err.println(String.valueOf(String.valueOf((new StringBuffer("function:")).append(functionName).append("\r\ninfomation:").append(infomation).append("\r\nerror:").append(e.getMessage()))));
    }

    public String getWizardConnectionStringStr() throws Exception {
        return this.getWizardConnectionString().toString();
    }

    public DatabaseInfo getWizardConnectionString() throws Exception {
        DatabaseInfo info = new DatabaseInfo();
        info.setServer(_$23421);
        info.setDatabase(_$23385);
        info.setDriver(_$23420);
        info.setUser(_$23422.toLowerCase());
        info.setPwd(_$23423);
        info.setPort(_$23425);
        info.setUrl(_$27011);
        info.setDataSourcesURL(_$23427);
        info.setCharset(_$23426);
        return info;
    }

    public static String getZoneConnectionStringStr(String sys_id) throws Exception {
        return getZoneConnectionString(sys_id).toString();
    }

    public static Connection getConnectionFrom(String connectionString) throws Exception {
        return getConnectionFrom(connectionString, false);
    }

    public static Connection getConnectionFrom(String connectionString, boolean hasBlob) throws Exception {
        Connection conn = null;
        Object ctx = null;
        String sourceName = "";

        try {
            StringTokenizer e = new StringTokenizer(connectionString, ";");
            String lsServer = e.nextToken();
            String lsDatabase = e.nextToken();
            String lsUserCode = e.nextToken();
            String lsPassWord = e.nextToken();
            String lsDriver = e.nextToken();
            String lsUrl = e.nextToken();
            boolean isOracle = PublicLib.getDbType(connectionString).indexOf("Oracle") != -1;
            if(hasBlob && isOracle) {
                Class.forName(lsDriver);
                conn = DriverManager.getConnection(lsUrl, lsUserCode, lsPassWord);
                OracleDatabaseMetaData ds1 = (OracleDatabaseMetaData)conn.getMetaData();
                OracleDatabaseMetaData.setGetLobPrecision(false);
            } else {
                if(!_$27014 || hasBlob && isOracle) {
                    Class.forName(lsDriver);
                    conn = DriverManager.getConnection(lsUrl, lsUserCode, lsPassWord);
                } else {
                    lsUserCode = lsUserCode.toLowerCase();
                    if(isOracle) {
                        sourceName = lsUserCode;
                    } else {
                        sourceName = lsUserCode;
                    }

                    sourceName = sourceName.toLowerCase();
                    DataSource ds = getDataSource(sourceName);
                    conn = ds.getConnection();
                }

                if(!isOracle) {
                    conn.setTransactionIsolation(1);
                }
            }
        } catch (Exception var18) {
            _$26430.error(String.valueOf(String.valueOf((new StringBuffer("getConnect:sourceName=")).append(sourceName).append(";connectiongString=").append(connectionString))), var18);
            throw var18;
        } finally {
            if(ctx != null) {
                ((Context)ctx).close();
            }

        }

        return conn;
    }

    public String getZoneDesc(String sys_id) throws Exception {
        String name = "";
        Connection conn = null;
        CachedRowSet crs = null;

        try {
            conn = getWizardConnection();
            String ex = "select sys_desc from app_sys where sys_id=?";
            SQLPara sqlPara = new SQLPara();
            sqlPara.add(sys_id);
            crs = getRowSet(ex, conn, sqlPara);
            if(crs.next()) {
                name = crs.getString("sys_desc");
            }
        } catch (Exception var11) {
            PublicLib.log(true, "getZoneName", "sys_id=".concat(String.valueOf(String.valueOf(sys_id))), var11);
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
            PublicLib.closeConnection(conn);
            conn = null;
        }

        return name;
    }

    public static CachedRowSet getUserWizardInfo(String userCode) {
        Connection conn = null;
        CachedRowSet crs = null;

        try {
            conn = getWizardConnection();
            String ex = "select app.*,dw.user_code,dw.user_name from db_wizard dw left join app_sys app on dw.sys_id=app.sys_id where dw.user_code=?";
            SQLPara sqlPara = new SQLPara();
            sqlPara.add(userCode);
            crs = getRowSet(ex, conn, sqlPara);
        } catch (Exception var9) {
            PublicLib.log(true, "getUserWizardInfo", "user_code=".concat(String.valueOf(String.valueOf(userCode))), var9);
        } finally {
            PublicLib.closeConnection(conn);
            conn = null;
        }

        return crs;
    }

    public static String getUserZoneDesc(String userCode) {
        CachedRowSet crs = null;
        String sRet = "";

        label36:
        try {
            try {
                crs = getUserWizardInfo(userCode);
                if(crs.next()) {
                    sRet = crs.getString("sys_desc");
                }
            } catch (Exception var8) {
                var8.printStackTrace();
            }
        } finally {
            break label36;
        }

        PublicLib.closeCachedRowSet(crs);
        crs = null;
        return sRet;
    }

    public static String getUserZoneId(String userCode) {
        Connection conn = null;
        CachedRowSet crs = null;
        String zoneId = null;

        try {
            conn = getWizardConnection();
            String ex = "select sys_id,user_code,user_name from db_wizard where user_code=?";
            SQLPara sqlPara = new SQLPara();
            sqlPara.add(userCode);
            crs = getRowSet(ex, conn, sqlPara);
            if(crs.next()) {
                zoneId = crs.getString("sys_id");
            }
        } catch (Exception var10) {
            PublicLib.log(true, "getUserZoneId", "user_code=".concat(String.valueOf(String.valueOf(userCode))), var10);
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
            PublicLib.closeConnection(conn);
            conn = null;
        }

        return zoneId;
    }

    public static Connection getZonneConnection(String sys_id) throws Exception {
        String connStr = getZoneConnectionStringStr(sys_id);
        return getConnectionFrom(connStr);
    }

    public static DatabaseInfo getZoneConnectionString(String sys_id) throws Exception {
        Connection conn = null;
        DatabaseInfo info = null;
        PreparedStatement psmt = null;
        ResultSet rs = null;

        try {
            String e = "zone_".concat(String.valueOf(String.valueOf(sys_id)));
            info = (DatabaseInfo)_$27025.get(e);
            if(info == null) {
                conn = getWizardConnection();
                String sqlClause = "select * from APP_SYS where SYS_ID=?";
                psmt = conn.prepareStatement(sqlClause);
                psmt.setString(1, sys_id);
                rs = psmt.executeQuery();
                if(!rs.next()) {
                    throw new Exception(String.valueOf(String.valueOf((new StringBuffer("地区")).append(sys_id).append("不存在。"))));
                }

                info = new DatabaseInfo();
                info.setSysId(sys_id);
                info.setServer(rs.getString("server"));
                info.setDatabase(rs.getString("db_name"));
                info.setDriver(rs.getString("driver"));
                info.setUser(rs.getString("user_code").toLowerCase());
                info.setPwd(rs.getString("password"));
                info.setPort(rs.getString("port"));
                info.setUrl(rs.getString("db_url"));
                info.setDataSourcesURL(_$23427);
                info.setCharset(_$23426);
                _$27025.put(e, info);
            }
        } catch (Exception var11) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("获取地区")).append(sys_id).append("失败，原因是").append(var11.getMessage()))));
            var11.printStackTrace();
            throw var11;
        } finally {
            closeResultSet(rs);
            rs = null;
            closePreparedStatement(psmt);
            psmt = null;
            closeConnection(conn);
            conn = null;
        }

        return info;
    }

    public String getServerConnectionStringStr(String logonUser, String serverName) throws Exception {
        return this.getServerConnectionString(logonUser, serverName).toString();
    }

    public String getServerConnectionStringByRegionStr(String region, String serverName) throws Exception {
        return this.getServerConnectionStringByRegion(region, serverName).toString();
    }

    public DatabaseInfo getServerConnectionStringByRegion(String region, String serverName) throws Exception {
        Connection conn = null;
        PreparedStatement psmt = null;
        DatabaseInfo info = null;
        ResultSet rs = null;

        try {
            String e = String.valueOf(String.valueOf((new StringBuffer("server_")).append(region).append("_").append(serverName)));
            info = (DatabaseInfo)_$27025.get(e);
            if(info == null) {
                conn = getConnectionFrom(getZoneConnectionStringStr(region));
                String sqlClause = "select * from sys_svr A  WHERE A.svr_name=?";
                psmt = conn.prepareStatement(sqlClause);
                psmt.setString(1, serverName);
                rs = psmt.executeQuery();
                if(!rs.next()) {
                    throw new Exception(String.valueOf(String.valueOf((new StringBuffer("服务器[")).append(serverName).append("]不存在。"))));
                }

                info = new DatabaseInfo();
                info.setServer(rs.getString("dbms_svr"));
                info.setDatabase(rs.getString("db_name"));
                info.setDriver(rs.getString("odbc_drvr"));
                info.setUser(rs.getString("conn_user").toLowerCase());
                info.setPwd(rs.getString("conn_pwd"));
                info.setPort(rs.getString("port"));
                info.setUrl(rs.getString("db_url"));
                info.setDataSourcesURL(_$23427);
                info.setCharset(_$23426);
                _$27025.put(e, info);
            }
        } catch (Exception var13) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("获取数据库服务器[")).append(serverName).append("]连接失败，原因是").append(var13.getMessage()))));
            var13.printStackTrace();
            throw var13;
        } finally {
            closeResultSet(rs);
            rs = null;
            closePreparedStatement(psmt);
            psmt = null;
            closeConnection(conn);
            conn = null;
        }

        return info;
    }

    public DatabaseInfo getServerConnectionString(String logonUser, String serverName) throws Exception {
        Connection conn = null;
        PreparedStatement psmt = null;
        DatabaseInfo info = null;
        ResultSet rs = null;

        try {
            String e = "";
            if(logonUser == null) {
                e = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(this._$27030)))).append("_").append(serverName)));
            } else {
                e = String.valueOf(String.valueOf((new StringBuffer("server_")).append(logonUser).append("_").append(serverName)));
            }

            info = (DatabaseInfo)_$27025.get(e);
            if(info == null) {
                if(logonUser == null) {
                    conn = getConnectionFrom(this._$27030);
                } else {
                    conn = getConnection(logonUser);
                }

                String sqlClause = "select * from sys_svr A  WHERE A.svr_name=?";
                psmt = conn.prepareStatement(sqlClause);
                psmt.setString(1, serverName);
                rs = psmt.executeQuery();
                if(!rs.next()) {
                    throw new Exception(String.valueOf(String.valueOf((new StringBuffer("服务器:[")).append(serverName).append("]不存在。"))));
                }

                info = new DatabaseInfo();
                info.setServer(rs.getString("dbms_svr"));
                info.setDatabase(rs.getString("db_name"));
                info.setDriver(rs.getString("odbc_drvr"));
                info.setUser(rs.getString("conn_user").toLowerCase());
                info.setPwd(rs.getString("conn_pwd"));
                info.setPort(rs.getString("port"));
                info.setUrl(rs.getString("db_url"));
                info.setDataSourcesURL(_$23427);
                info.setCharset(_$23426);
                _$27025.put(e, info);
            }
        } catch (Exception var13) {
            System.err.println(String.valueOf(String.valueOf((new StringBuffer("获取数据库服务器[")).append(serverName).append("]连接失败，原因是").append(var13.getMessage()))));
            var13.printStackTrace();
            throw var13;
        } finally {
            closeResultSet(rs);
            rs = null;
            closePreparedStatement(psmt);
            psmt = null;
            closeConnection(conn);
            conn = null;
        }

        return info;
    }

    public static void printSQL(String aSqlName, String aSqlValue) {
        printSQL("BaseServerClientJB.log", aSqlName, aSqlValue);
    }

    public static void setDebug(boolean bDebug) {
        _$6211 = bDebug;
    }

    public static boolean isDebug() {
        return _$6211;
    }

    public static void printSQL(String aFileName, String aSqlName, String aSqlValue) {
        FileOutputStream outstream = null;

        try {
            if(_$6211) {
                String e = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aSqlName)))).append("-----").append(aSqlValue).append("\r\n")));
                String userDir = System.getProperty("user.dir");
                String fileSeparator = System.getProperty("file.separator");
                String fileName = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(userDir)))).append(fileSeparator).append(aFileName)));
                outstream = new FileOutputStream(fileName, true);
                outstream.write(e.getBytes());
            }
        } catch (Exception var17) {
            System.err.println("printSQL:".concat(String.valueOf(String.valueOf(var17.getMessage()))));
        } finally {
            try {
                if(outstream != null) {
                    outstream.close();
                }

                outstream = null;
            } catch (Exception var16) {
                ;
            }

        }

    }

    public static CachedRowSet getRowSet(String aSqlClause, SQLPara aSQLPara, String logonUser) throws Exception {
        return getRowSet(aSqlClause, "", aSQLPara, logonUser);
    }

    public static CachedRowSet getRowSet(String aSqlClause, String tableName, SQLPara aSQLPara, String logonUser) throws Exception {
        Connection conn = null;
        CachedRowSet crs = null;

        try {
            conn = getConnection(logonUser);
            crs = getRowSet(aSqlClause, conn, tableName, aSQLPara);
        } catch (Exception var11) {
            System.err.println("getRowSet:".concat(String.valueOf(String.valueOf(var11.getMessage()))));
            System.err.println("sqlClause:".concat(String.valueOf(String.valueOf(aSqlClause))));
            throw var11;
        } finally {
            closeConnection(conn);
            conn = null;
        }

        return crs;
    }

    public CachedRowSet getRowSet(String aSqlClause, SQLPara aSQLPara) throws Exception {
        Connection conn = null;
        CachedRowSet crs = null;

        label31:
        try {
            try {
                conn = this.getConnection();
                crs = getRowSet(aSqlClause, conn, aSQLPara);
            } catch (Exception var10) {
                throw var10;
            }
        } finally {
            break label31;
        }

        PublicLib.closeConnection(conn);
        conn = null;
        return crs;
    }

    public CachedRowSet getRowSet(String aSqlClause, String tableName, SQLPara aSQLPara) throws Exception {
        return getRowSet(aSqlClause, tableName, aSQLPara, this.getCurrentUser());
    }

    public CachedRowSet getServerRowSet(String aServerName, String aSqlClause, SQLPara aSQLPara) throws Exception {
        Connection conn = null;
        CachedRowSet crs = null;

        try {
            String e = this.getCurrentUser();
            String connStr = this.getServerConnectionStringStr(e, aServerName);
            conn = getConnectionFrom(connStr);
            crs = getRowSet(aSqlClause, conn, aSQLPara);
        } catch (Exception var12) {
            throw var12;
        } finally {
            closeConnection(conn);
            conn = null;
        }

        return crs;
    }

    public static CachedRowSet getRowSet(String aSqlClause, Connection conn, SQLPara aSQLPara) throws Exception {
        return getRowSet(aSqlClause, conn, "", aSQLPara);
    }

    public static CachedRowSet getRowSet(String aSqlClause, Connection conn, String tableName, SQLPara aSQLPara) throws Exception {
        CachedRowSet crs = null;
        PreparedStatement psmt = null;
        ResultSet rs = null;

        try {
            crs = new CachedRowSet();
            if(tableName != null) {
                crs.setTableName(tableName);
            }

            crs.setType(1004);
            crs.setConcurrency(1008);
            psmt = conn.prepareStatement(aSqlClause);
            printSQL("getRowSet", aSqlClause);

            for(int e = 0; e < aSQLPara.size(); ++e) {
                printSQL("type ".concat(String.valueOf(String.valueOf(aSQLPara.getType(e)))), aSQLPara.getValue(e));
                if(aSQLPara.getType(e) == 0) {
                    psmt.setString(e + 1, aSQLPara.getValue(e));
                } else if(aSQLPara.getType(e) == 1) {
                    psmt.setInt(e + 1, Integer.parseInt(aSQLPara.getValue(e)));
                } else if(aSQLPara.getType(e) == 2) {
                    psmt.setTimestamp(e + 1, new Timestamp(Long.parseLong(aSQLPara.getValue(e))));
                } else if(aSQLPara.getType(e) == 4) {
                    psmt.setObject(e + 1, aSQLPara.getObject(e));
                }
            }

            rs = psmt.executeQuery();
            crs.populate(rs);
            return crs;
        } catch (Exception var8) {
            System.err.println("getRowSet:".concat(String.valueOf(String.valueOf(var8.getMessage()))));
            System.err.println("sqlClause:".concat(String.valueOf(String.valueOf(aSqlClause))));
            closeResultSet(rs);
            rs = null;
            closePreparedStatement(psmt);
            psmt = null;
            throw var8;
        }
    }

    public ResultSet getResultSet(String aSqlClause, Connection conn, SQLPara aSQLPara) throws Exception {
        PreparedStatement psmt = null;
        ResultSet rs = null;

        try {
            psmt = conn.prepareStatement(aSqlClause, 1004, 1007);
            PublicLib.printSQL("getResultSet", aSqlClause);

            for(int e = 0; e < aSQLPara.size(); ++e) {
                PublicLib.printSQL("type ".concat(String.valueOf(String.valueOf(aSQLPara.getType(e)))), aSQLPara.getValue(e));
                if(aSQLPara.getType(e) == 0) {
                    psmt.setString(e + 1, aSQLPara.getValue(e));
                } else if(aSQLPara.getType(e) == 1) {
                    psmt.setInt(e + 1, Integer.parseInt(aSQLPara.getValue(e)));
                } else if(aSQLPara.getType(e) == 2) {
                    psmt.setTimestamp(e + 1, new Timestamp(Long.parseLong(aSQLPara.getValue(e))));
                }
            }

            rs = psmt.executeQuery();
            return rs;
        } catch (Exception var7) {
            var7.printStackTrace();
            throw var7;
        }
    }

    public static RecordSetObject queryRSO(String sqlClause, String userCode, String sysCode) throws Exception {
        Connection cn = getApplicationConnection(userCode, sysCode);
        RecordSetObject rso = null;
        PreparedStatement pstm = null;

        label31:
        try {
            try {
                SQLStatementFormer e = getSQLStatementFormerByConnection(cn);
                pstm = cn.prepareStatement(sqlClause);
                ResultSet rs = pstm.executeQuery();
                rso = new RecordSetObject(rs);
                rso.setCharConverter(e);
                rs.close();
                rs = null;
            } catch (Exception var12) {
                System.err.println("SQL语句为：".concat(String.valueOf(String.valueOf(sqlClause))));
                System.err.println("错误发生在queryRSO()，错误为：".concat(String.valueOf(String.valueOf(var12.getMessage()))));
                var12.printStackTrace();
                throw new Exception(var12.getMessage());
            }
        } finally {
            break label31;
        }

        PublicLib.closePreparedStatement(pstm);
        pstm = null;
        PublicLib.closeConnection(cn);
        cn = null;
        return rso;
    }

    public static RecordSetObject queryRSO(String sqlClause, String userCode) throws Exception {
        Connection cn = getConnection(userCode);
        RecordSetObject rso = null;
        PreparedStatement pstm = null;

        label31:
        try {
            try {
                SQLStatementFormer e = getSQLStatementFormerByConnection(cn);
                pstm = cn.prepareStatement(sqlClause);
                ResultSet rs = pstm.executeQuery();
                rso = new RecordSetObject(rs);
                rso.setCharConverter(e);
                rs.close();
                rs = null;
            } catch (Exception var11) {
                System.err.println("SQL语句为：".concat(String.valueOf(String.valueOf(sqlClause))));
                System.err.println("错误发生在queryRSO()，错误为：".concat(String.valueOf(String.valueOf(var11.getMessage()))));
                var11.printStackTrace();
                throw new Exception(var11.getMessage());
            }
        } finally {
            break label31;
        }

        PublicLib.closePreparedStatement(pstm);
        pstm = null;
        PublicLib.closeConnection(cn);
        cn = null;
        return rso;
    }

    public static RecordSetObject queryRSO(Connection cn, String sqlClause) throws Exception {
        RecordSetObject rso = null;
        PreparedStatement pstm = null;

        try {
            SQLStatementFormer e = getSQLStatementFormerByConnection(cn);
            pstm = cn.prepareStatement(sqlClause);
            ResultSet rs = pstm.executeQuery(sqlClause);
            rso = new RecordSetObject(rs);
            rso.setCharConverter(e);
        } catch (Exception var10) {
            System.err.println("SQL语句为：".concat(String.valueOf(String.valueOf(sqlClause))));
            System.err.println("错误发生在getRSO()，错误为：".concat(String.valueOf(String.valueOf(var10.getMessage()))));
            throw new Exception(var10.getMessage());
        } finally {
        }

        return rso;
    }

    public static Connection getWizardConnection() throws Exception {
        Connection conn = null;
        if(_$27014) {
            if(_$23420.toLowerCase().indexOf("oracle") != -1) {
                conn = getDataSource(_$23422).getConnection();
            } else {
                conn = getDataSource(_$23422).getConnection();
            }
        } else {
            try {
                Class.forName(DatabaseInfo.getDriver(_$23420));
                conn = DriverManager.getConnection(DatabaseInfo.getURL(_$23420, _$23421, _$23385, _$23425, _$23426), _$23422, _$23423);
            } catch (Exception var2) {
                _$26430.error("Error in BaseServerClient.getWizardConnection()!", var2);
            }
        }

        return conn;
    }

    public Connection getConnection() throws Exception {
        return this._$24397 == null?getConnectionFrom(getZoneConnectionStringStr(this._$27029)):getConnection(this._$24397);
    }

    public Connection getApplicationConnection(String appId) throws Exception {
        return getApplicationConnection(this._$24397, appId);
    }

    public static Connection getApplicationConnection(String logonUser, String appId) throws Exception {
        Connection conn = null;
        if(logonUser == null) {
            logonUser = "";
        }

        if(logonUser.trim().length() == 0) {
            throw new Exception("getConnection(logonUser)出错，传入的用户名为空!");
        } else {
            DatabaseInfo info = getSystemConnectionStringEx(logonUser, appId);
            if(_$27014) {
                if(_$23420.toLowerCase().indexOf("oracle") != -1) {
                    conn = getDataSource(info.getUser()).getConnection();
                } else {
                    conn = getDataSource(info.getUser()).getConnection();
                }
            } else {
                Class.forName(info.getDriver());
                conn = DriverManager.getConnection(info.getURL(), info.getUser(), info.getPwd());
            }

            return conn;
        }
    }

    public static Connection getApplicationConnection(String wizardSchemas, String logonUser, String appId) throws Exception {
        Connection conn = null;
        if(logonUser == null) {
            logonUser = "";
        }

        if(logonUser.trim().length() == 0) {
            throw new Exception("getConnection(logonUser)出错，传入的用户名为空!");
        } else {
            DatabaseInfo info = getSystemConnectionStringEx(wizardSchemas, logonUser, appId);
            if(_$27014) {
                if(_$23420.toLowerCase().indexOf("oracle") != -1) {
                    conn = getDataSource(info.getUser()).getConnection();
                } else {
                    conn = getDataSource(info.getUser()).getConnection();
                }
            } else {
                Class.forName(info.getDriver());
                conn = DriverManager.getConnection(info.getURL(), info.getUser(), info.getPwd());
            }

            return conn;
        }
    }

    public static Connection getConnection(String logonUser) throws Exception {
        Connection conn = null;
        if(logonUser == null) {
            logonUser = "";
        }

        if(logonUser.trim().length() == 0) {
            throw new Exception("getConnection(logonUser)出错，传入的用户名为空!");
        } else {
            DatabaseInfo info = getBaseConnectionString(logonUser);
            if(_$27014) {
                if(_$23420.toLowerCase().indexOf("oracle") != -1) {
                    conn = getDataSource(info.getUser()).getConnection();
                } else {
                    conn = getDataSource(info.getUser()).getConnection();
                }
            } else {
                Class.forName(info.getDriver());
                conn = DriverManager.getConnection(info.getURL(), info.getUser(), info.getPwd());
            }

            return conn;
        }
    }

    public static Connection getConnection(String wizardSchemas, String logonUser) throws Exception {
        Connection conn = null;
        if(logonUser == null) {
            logonUser = "";
        }

        if(logonUser.trim().length() == 0) {
            throw new Exception("getConnection(wizardSchemas,logonUser)出错，传入的用户名为空!");
        } else if(wizardSchemas.trim().length() == 0) {
            throw new Exception("getConnection(wizardSchemas,logonUser)出错，传入的导向库名为空!");
        } else {
            DatabaseInfo info = getBaseConnectionString(wizardSchemas, logonUser);
            if(_$27014) {
                if(_$23420.toLowerCase().indexOf("oracle") != -1) {
                    conn = getDataSource(info.getUser()).getConnection();
                } else {
                    conn = getDataSource(info.getUser()).getConnection();
                }
            } else {
                Class.forName(info.getDriver());
                conn = DriverManager.getConnection(info.getURL(), info.getUser(), info.getPwd());
            }

            return conn;
        }
    }

    public String genTimeFunPre() throws Exception {
        String user = this.getCurrentUser();
        String tag = "base_".concat(String.valueOf(String.valueOf(user)));
        Object obj = _$27025.get(tag);
        String connString = obj == null?null:obj.toString();
        if(connString == null) {
            connString = getBaseConnectionStringStr(user);
        }

        return PublicLib.getDbType(connString).indexOf("Oracle") == -1 && PublicLib.getDbType(connString).indexOf("DB2") == -1?(PublicLib.getDbType(connString).indexOf("Sybase") != -1?"convert(datetime,":null):"to_date(";
    }

    public SQLStatementFormer getSQLStatementFormer() throws Exception {
        String user = this.getCurrentUser();
        String tag = "base_".concat(String.valueOf(String.valueOf(user)));
        DatabaseInfo obj = (DatabaseInfo)_$27025.get(tag);
        if(obj == null) {
            obj = getBaseConnectionString(user);
        }

        return getSQLStatementFormerByTypeName(obj.getDBTypeName());
    }

    private static SQLStatementFormer _$27159(int dbType) {
        Object oRet = null;

        try {
            switch(dbType) {
            case 0:
                oRet = new MSSQLStatementFormer();
                break;
            case 1:
                oRet = new OracleSQLStatementFormer();
                break;
            case 2:
                oRet = new SybaseSQLStatementFormer();
                break;
            case 3:
                oRet = new DB2SQLStatementFormer();
            }
        } catch (Exception var7) {
            _$26430.error("Error in getSQLTransferrer!! detail is :".concat(String.valueOf(String.valueOf(var7.getMessage()))), var7);
        } finally {
        }

        return (SQLStatementFormer)oRet;
    }

    public static SQLStatementFormer getSQLStatementFormerByConnection(Connection cn) throws Exception {
        return _$27159(getServerTypeByConnection(cn));
    }

    public static SQLStatementFormer getSQLStatementFormerByTypeName(String typeName) throws Exception {
        return _$27159(getServerTypeByName(typeName));
    }

    public static int getServerTypeByName(String typeName) throws Exception {
        if(typeName == null) {
            typeName = "";
        }

        if(typeName.startsWith("Microsoft SQL Server")) {
            return 0;
        } else if(typeName.startsWith("Oracle")) {
            return 1;
        } else if(typeName.startsWith("Adaptive Server Enterprise")) {
            return 2;
        } else if(typeName.startsWith("DB2")) {
            return 3;
        } else {
            throw new RuntimeException(String.valueOf(String.valueOf((new StringBuffer("UNSUPPORTED DATABASE PRODUCT [")).append(typeName).append("]"))));
        }
    }

    public static int getServerTypeByConnection(Connection cn) throws Exception {
        String sProductName = cn.getMetaData().getDatabaseProductName();
        if(sProductName.startsWith("Microsoft SQL Server")) {
            return 0;
        } else if(sProductName.startsWith("Oracle")) {
            return 1;
        } else if(sProductName.startsWith("Adaptive Server Enterprise")) {
            return 2;
        } else if(sProductName.startsWith("DB2")) {
            return 3;
        } else {
            throw new RuntimeException(String.valueOf(String.valueOf((new StringBuffer("UNSUPPORTED DATABASE PRODUCT [")).append(sProductName).append("]"))));
        }
    }

    public String genTimeFunSuf() throws Exception {
        String user = this.getCurrentUser();
        String tag = "base_".concat(String.valueOf(String.valueOf(user)));
        Object obj = _$27025.get(tag);
        String connString = obj == null?null:obj.toString();
        if(connString == null) {
            connString = getBaseConnectionStringStr(user);
        }

        return PublicLib.getDbType(connString).indexOf("Oracle") == -1 && PublicLib.getDbType(connString).indexOf("DB2") == -1?(PublicLib.getDbType(connString).indexOf("Sybase") != -1?")":null):",\'YYYY-MM-DD HH24:MI:SS\')";
    }

    public static void executeParameterSQL(Connection conn, String sqlClause, SQLPara aSQLPara) throws Exception {
        PublicLib.executeParameterSQL(conn, sqlClause, aSQLPara);
    }

    public static PreparedStatement PreparedStatementWithParameter(Connection conn, String sql, SQLPara para) throws Exception {
        return PublicLib.PreparedStatementWithParameter(conn, sql, para);
    }

    public static long executeSQL(Connection conn, String sqlClause) throws Exception {
        return PublicLib.executeSQL(conn, sqlClause);
    }

    public static void main(String[] argv) throws Exception {
        new BaseServerClient("liy");
        Connection conn = getWizardConnection();
        conn.close();
        conn = null;
    }

    public void setCurrentUser(String currentUser) {
        this._$24397 = currentUser;
    }

    public void setCurrentZoneId(String currentZoneId) {
        this._$27029 = currentZoneId;
    }

    public void setLog(ILogger log) {
        _$26430 = log;
    }

    public void setCurrentZoneConnctionString(String currentZoneConnctionString) {
        this._$27030 = currentZoneConnctionString;
    }

    public boolean isAdmin(String aUserName) throws Exception {
        boolean isAdmin = false;
        if(aUserName == null) {
            return false;
        } else {
            Boolean boolIsAdmin = (Boolean)_$27027.get(aUserName);
            if(boolIsAdmin == null) {
                CachedRowSet crs = null;

                try {
                    String e = "select 1 from baserole_admin where i_user_adm=1 and (c_user_code in(select fram_seq as user_code from user_dept where user_code=?) or c_user_code=?)";
                    SQLPara sqlPara = new SQLPara();
                    sqlPara.add(aUserName);
                    sqlPara.add(aUserName);
                    crs = this.getRowSet(e, sqlPara);
                    if(crs.next()) {
                        isAdmin = true;
                    }
                } catch (Exception var11) {
                    PublicLib.log(true, "isAdmin", "", var11);
                    throw new Exception(var11.getMessage());
                } finally {
                    PublicLib.closeCachedRowSet(crs);
                    crs = null;
                }

                _$27027.put(aUserName, new Boolean(isAdmin));
            } else {
                isAdmin = boolIsAdmin.booleanValue();
            }

            return isAdmin;
        }
    }

    public boolean isDeptAdmin(String aUserName) throws Exception {
        if(aUserName == null) {
            return false;
        } else {
            boolean isDeptAdmin = false;
            Boolean boolIsDeptAdmin = (Boolean)_$27026.get(aUserName);
            if(boolIsDeptAdmin == null) {
                CachedRowSet crs = null;

                try {
                    String e = "select 1 from baserole_admin where i_org_adm=1 and (c_user_code in(select fram_seq as user_code from user_dept where user_code=?) or c_user_code=?)";
                    SQLPara sqlPara = new SQLPara();
                    sqlPara.add(aUserName);
                    sqlPara.add(aUserName);
                    crs = this.getRowSet(e, sqlPara);
                    if(crs.next()) {
                        isDeptAdmin = true;
                    }
                } catch (Exception var11) {
                    PublicLib.log(true, "isDeptAdmin", "", var11);
                    throw new Exception(var11.getMessage());
                } finally {
                    PublicLib.closeCachedRowSet(crs);
                    crs = null;
                }

                _$27026.put(aUserName, new Boolean(isDeptAdmin));
            } else {
                isDeptAdmin = boolIsDeptAdmin.booleanValue();
            }

            return isDeptAdmin;
        }
    }

    public static String getLocalEJBURL() {
        return _$27012;
    }

    public static String getUserbyCASN(String caSerialNo) throws Exception {
        String userCode = null;
        Connection conn = null;
        PreparedStatement pstm = null;
        ResultSet rs = null;

        label37:
        try {
            try {
                conn = getWizardConnection();
                String e = "";
                e = " select u_name from users where ca_sn=?";
                pstm = conn.prepareStatement(e);
                pstm.setString(1, caSerialNo);
                rs = pstm.executeQuery();
                if(rs.next()) {
                    userCode = rs.getString(1);
                } else {
                    _$26430.error(String.valueOf(String.valueOf((new StringBuffer("序列号为:")).append(caSerialNo).append("的CA证书对应用户不存在!"))));
                }
            } catch (Exception var10) {
                _$26430.error(String.valueOf(String.valueOf((new StringBuffer("获取CA序列号为")).append(caSerialNo).append("的用户名失败，原因是").append(var10.getMessage()))), var10);
                throw var10;
            }
        } finally {
            break label37;
        }

        closeResultSet(rs);
        rs = null;
        closeStatement(pstm);
        pstm = null;
        closeConnection(conn);
        conn = null;
        return userCode;
    }

    public static String getUserbyUserNum(String userNum) throws Exception {
        String userCode = null;
        Connection conn = null;
        PreparedStatement pstm = null;
        ResultSet rs = null;

        label37:
        try {
            try {
                conn = getWizardConnection();
                String e = "";
                e = " select u_name from users where USER_NUM=?";
                pstm = conn.prepareStatement(e);
                pstm.setString(1, userNum);
                rs = pstm.executeQuery();
                if(rs.next()) {
                    userCode = rs.getString(1);
                } else {
                    _$26430.error(String.valueOf(String.valueOf((new StringBuffer("工号为:")).append(userNum).append("的对应用户不存在!"))));
                }
            } catch (Exception var10) {
                _$26430.error(String.valueOf(String.valueOf((new StringBuffer("获取工号为")).append(userNum).append("的用户名失败，原因是").append(var10.getMessage()))), var10);
                throw var10;
            }
        } finally {
            break label37;
        }

        closeResultSet(rs);
        rs = null;
        closeStatement(pstm);
        pstm = null;
        closeConnection(conn);
        conn = null;
        return userCode;
    }

    public String getLocalWorkflowAgentId() {
        return _$27013;
    }

    public ILogger getLog() {
        return _$26430;
    }

    public String getCurrentZoneConnctionString() {
        return this._$27030;
    }

    public long getSysSeq(String aSeqCode, String aSeqDesc) throws Exception {
        return getSysSeq(this._$24397, aSeqCode, aSeqDesc);
    }

    public static long getSysSeq(String userCode, String aSeqCode, String aSeqDesc) throws Exception {
        long seqValue = 1L;
        Connection conn = null;
        PreparedStatement pstm = null;
        PreparedStatement pstm01 = null;
        ResultSet rs = null;

        try {
            String e = getBaseConnectionStringStr(userCode);
            conn = getConnectionFrom(e);
            String sqlClause = "update sys_seq set seq_valu=seq_valu+1 where seq_code=?";
            pstm = conn.prepareStatement(sqlClause);
            pstm.setString(1, aSeqCode);
            int iUpdateRow = pstm.executeUpdate();
            if(iUpdateRow <= 0) {
                sqlClause = "insert into sys_seq(seq_code,seq_desc,seq_valu) values(?,?,?)";
                pstm01 = conn.prepareStatement(sqlClause);
                pstm01.setString(1, aSeqCode);
                pstm01.setString(2, aSeqDesc);
                pstm01.setLong(3, seqValue);
                pstm01.executeUpdate();
            } else {
                sqlClause = "select seq_valu from sys_seq where seq_code=?";
                pstm = conn.prepareStatement(sqlClause);
                pstm.setString(1, aSeqCode);
                rs = pstm.executeQuery();
                PublicLib.printSQL("getSysSeq", sqlClause);
                rs.next();
                seqValue = rs.getLong("seq_valu");
            }

            conn.commit();
        } catch (Exception var16) {
            PublicLib.log("getSysSeq", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aSeqCode)))).append(",").append(aSeqDesc))), var16);
            if(conn != null) {
                conn.rollback();
            }

            throw new Exception(var16.getMessage());
        } finally {
            PublicLib.closeResultSet(rs);
            rs = null;
            PublicLib.closeStatement(pstm);
            pstm = null;
            PublicLib.closeStatement(pstm01);
            pstm01 = null;
            PublicLib.closeConnection(conn);
            conn = null;
        }

        return seqValue;
    }

    public static long getSysSeq(Connection conn, String aSeqCode, String aSeqDesc) throws Exception {
        long seqValue = 1L;
        PreparedStatement pstm = null;
        PreparedStatement pstm01 = null;
        ResultSet rs = null;

        try {
            String e = "update sys_seq set seq_valu=seq_valu+1 where seq_code=?";
            pstm = conn.prepareStatement(e);
            pstm.setString(1, aSeqCode);
            int iUpdateRow = pstm.executeUpdate();
            if(iUpdateRow <= 0) {
                e = "insert into sys_seq(seq_code,seq_desc,seq_valu) values(?,?,?)";
                pstm01 = conn.prepareStatement(e);
                pstm01.setString(1, aSeqCode);
                pstm01.setString(2, aSeqDesc);
                pstm01.setLong(3, seqValue);
                pstm01.executeUpdate();
            } else {
                e = "select seq_valu from sys_seq where seq_code=?";
                pstm = conn.prepareStatement(e);
                pstm.setString(1, aSeqCode);
                rs = pstm.executeQuery();
                PublicLib.printSQL("getSysSeq", e);
                rs.next();
                seqValue = rs.getLong("seq_valu");
            }
        } catch (Exception var14) {
            PublicLib.log("getSysSeq", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aSeqCode)))).append(",").append(aSeqDesc))), var14);
            throw new Exception(var14.getMessage());
        } finally {
            PublicLib.closeResultSet(rs);
            rs = null;
            PublicLib.closeStatement(pstm);
            pstm = null;
            PublicLib.closeStatement(pstm01);
            pstm01 = null;
        }

        return seqValue;
    }

    public long getBoaSeq(String aSeqCode, String aSeqDesc) throws Exception {
        return getBoaSeq(this._$24397, aSeqCode, aSeqDesc);
    }

    public static long getBoaSeq(String userCode, String aSeqCode, String aSeqDesc) throws Exception {
        long seqValue = -1L;
        Connection conn = null;

        label31:
        try {
            try {
                conn = getConnection(userCode);
                seqValue = getBoaSeq(conn, aSeqCode, aSeqDesc);
            } catch (Exception var11) {
                System.err.println("getBoaSeq(String userCode,String aSeqCode,String aSeqDesc)得到序列号出错了：".concat(String.valueOf(String.valueOf(var11.getMessage()))));
                throw var11;
            }
        } finally {
            break label31;
        }

        PublicLib.closeConnection(conn);
        conn = null;
        return seqValue;
    }

    public static long getBoaSeq(Connection conn, String aSeqCode, String aSeqDesc) throws Exception {
        long seqValue = -1L;
        PreparedStatement pstm = null;
        PreparedStatement pstm01 = null;

        label37:
        try {
            try {
                String e = "select seq_code,seq_valu,seq_desc from boa_sys_seq where seq_code=?";
                pstm = conn.prepareStatement(e);
                pstm.setString(1, aSeqCode);
                ResultSet rs = pstm.executeQuery();
                if(!rs.next()) {
                    seqValue = 1L;
                    e = "insert into boa_sys_seq(seq_code,seq_valu,seq_desc) values(?,?,?)";
                    pstm01 = conn.prepareStatement(e);
                    pstm01.setString(1, aSeqCode);
                    pstm01.setInt(2, 1);
                    pstm01.setString(3, aSeqDesc);
                    pstm01.executeUpdate();
                } else {
                    seqValue = rs.getLong("seq_valu") + (long)1;
                    pstm01 = conn.prepareStatement("update boa_sys_seq set seq_valu=seq_valu+1 where seq_code=?");
                    pstm01.setString(1, aSeqCode);
                    pstm01.executeUpdate();
                }
            } catch (Exception var13) {
                System.err.println("getBoaSeq(String userCode,String aSeqCode,String aSeqDesc)得到序列号出错了：".concat(String.valueOf(String.valueOf(var13.getMessage()))));
                throw var13;
            }
        } finally {
            break label37;
        }

        PublicLib.closeStatement(pstm);
        pstm = null;
        PublicLib.closeStatement(pstm01);
        pstm01 = null;
        return seqValue;
    }

    private static final DatabaseInfo _$27098(DatabaseInfo dinfo) {
        boolean bRet = false;
        if(_$27038-- >= (long)0) {
            return dinfo;
        } else {
            _$27038 = (long)(Math.random() * (double)1000);
            Connection con = null;
            Statement stm = null;

            label226:
            try {
                try {
                    if(_$27014) {
                        if(_$23420.toLowerCase().indexOf("oracle") != -1) {
                            con = getDataSource(dinfo.getUser()).getConnection();
                        } else {
                            con = getDataSource(dinfo.getUser()).getConnection();
                        }
                    } else {
                        Class.forName(dinfo.getDriver());
                        con = DriverManager.getConnection(dinfo.getURL(), dinfo.getUser(), dinfo.getPwd());
                    }

                    stm = con.createStatement();
                    int c1 = 0;
                    int c2 = 0;
                    ResultSet rs = stm.executeQuery("select count(*) as CNT from base_user");
                    if(rs.next()) {
                        c1 = rs.getInt(1);
                    }

                    rs.close();
                    rs = null;
                    rs = stm.executeQuery("select count(*) from flow_def");
                    if(rs.next()) {
                        c2 = rs.getInt(1);
                    }

                    rs.close();
                    rs = null;
                    String var7 = _$27037;
                    synchronized(var7){}

                    try {
                        try {
                            bRet = EnvLoader.checkUser(c1) && EnvLoader.checkFlow(c2);
                        } catch (Exception var23) {
                            _$27038 = 5L;
                        }
                    } catch (Exception var24) {
                        throw var24;
                    }
                } catch (Exception var25) {
                    ;
                }
            } finally {
                break label226;
            }

            try {
                if(con != null) {
                    con.close();
                }

                con = null;
            } catch (Exception var22) {
                ;
            }

            if(!bRet) {
                _$27038 = 5L;
            }

            if(System.currentTimeMillis() > Timestamp.valueOf("2099-4-10 00:00:00").getTime()) {
                System.err.println("超出使用期限！真的吗？");
//                System.exit(0);
            }

            return bRet?dinfo:null;
        }
    }

    public void clearCache() throws Exception {
        if(_$27022 != null) {
            _$27022.clearCache();
        }

        if(_$27023 != null) {
            _$27023.clearCache();
        }

        if(_$27024 != null) {
            _$27024.clearCache();
        }

        if(_$27025 != null) {
            _$27025.clearCache();
        }

        if(_$27027 != null) {
            _$27027.clearCache();
        }

        if(_$27026 != null) {
            _$27026.clearCache();
        }

        if(this._$27028 != null) {
            this._$27028.clearCache();
        }

    }

    public static final MoniterHelper getMonitorHelper() throws Exception {
        if(_$27017 != null && _$27017.trim().length() != 0) {
            if(_$27019 == null) {
                _$27019 = (MoniterHelper)Class.forName(_$27017).newInstance();
                _$27019.initParameters(_$27018);
            }

            return _$27019;
        } else {
            throw new Exception("系统监控接口没有定义！");
        }
    }

    public static final SSOHelper getSSOHelper() throws Exception {
        if(_$27020 == null) {
            _$27020 = new SSOHelperImpl();
        }

        return _$27020;
    }

    public static final String decryptPassword(String encryptPassword) throws Exception {
        return HStringUtils.decryptStringData(encryptPassword);
    }

    public static final String encryptPassword(String password) throws Exception {
        return HStringUtils.encryptStringData(getTagValue("SYS/PASSWORD_ENCRYPT_ARITHMETIC"), password);
    }

    public static final String encryptPasswordWithArithmeticPrefix(String password) throws Exception {
        String strArithmetic = getTagValue("SYS/PASSWORD_ENCRYPT_ARITHMETIC");
        int arithmetic = HStringUtils.strArithmeticToInt(strArithmetic);
        return arithmetic != 0?String.valueOf(String.valueOf((new StringBuffer("{")).append(strArithmetic).append("}").append(HStringUtils.encryptStringData(strArithmetic, password)))):password;
    }

    public MCubeUIHelper getUIHelper() {
        return new MCubeUIHelperImpl(this._$24397);
    }

    public static MCubeUIHelper getUIHelper(String userCode) {
        return new MCubeUIHelperImpl(userCode);
    }

    public static MCubeUIManageHelper getUIManageHelper() {
        return new MCubeUIHelperImpl((String)null);
    }

    static {
        _$26430.info("user.dir=".concat(String.valueOf(String.valueOf(System.getProperty("user.dir")))));
        _$27042();

        try {
            _$27022 = new SimpleCache();
            _$27023 = new SimpleCache();
            _$27024 = new SimpleCache();
            _$27025 = new SimpleCache();
            _$27026 = new SimpleCache();
            _$27027 = new SimpleCache();
        } catch (Exception var1) {
            var1.printStackTrace();
        }

    }
}

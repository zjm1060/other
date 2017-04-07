//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

import baseserverjb.BaseServerClient;
import com.minstone.system.moniter.LoggerHelper;
import com.minstone.system.moniter.MoniterHelper;
import com.minstone.util.IReturn;
import com.minstone.util.PublicLib;
import com.minstone.util.ReturnImpl;
import com.minstone.util.SQLPara;
import com.minstone.util.loginterface.LogInterface;
import com.minstone.util.loginterface.LogRecord;
import flowengineforjava.FlowEngineLocal;
import flowengineforjava.FlowEngineLocalHome;
import flowengineforjava.flowarch.ArchiveInterfaceImpl;
import flowengineforjava.flowarch.BaseArchiveInterface;
import flowengineforjava.structure.AttachRight;
import flowengineforjava.structure.AttachRightItem;
import flowengineforjava.structure.CASignData;
import flowengineforjava.structure.FormControlHtmlData;
import flowengineforjava.structure.FormInstData;
import flowengineforjava.structure.FormStruct;
import flowengineforjava.structure.PagingData;
import flowengineforjava.util.RecordSet;
import flowengineweb.AttachAuthItem;
import flowengineweb.FlowWebBean;
import flowengineweb.MiscStruct;
import flowengineweb.SysPara;
import flowengineweb.logger.WFOperatorLogRecord;
import java.io.Serializable;
import java.sql.Connection;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Timestamp;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import javax.servlet.jsp.PageContext;
import sun.jdbc.rowset.CachedRowSet;

public class FlowEngineBean implements Serializable {
    private static final String _$75207 = "Local interface reference is null.  It must be created by calling one of the Home interface methods first.";
    private static final int _$75208 = 100;
    private boolean _$75209 = false;
    private int _$75210 = 4;
    private String _$75212 = "005";
    private int _$75214 = 10;
    private transient FlowEngineLocalHome _$75298 = null;
    private transient FlowEngineLocal _$75299 = null;
    protected transient HttpServletRequest m_request;
    protected transient HttpSession m_session;
    transient LogInterface logInterface = null;
    transient LoggerHelper logHelper = null;
    transient MoniterHelper monitorHelper = null;
    transient LogRecord logRec = null;
    transient BaseArchiveInterface arcInterface = null;
    transient BaseServerClient bsc = null;
    boolean hasDefineMoniterHelper = true;
    String currentUserName = null;
    String currentUser = null;
    String currentRegion = null;

    public FlowEngineBean() {
        this._$75209 = SysPara.isDebugMode();
        this._$75210 = SysPara.getLoggerLevel();
        this._$75226();
    }

    private void _$75226() {
        SysPara.getEJBURL();
        if(this.bsc == null) {
            this.bsc = new BaseServerClient();
        }

        if(this.logRec == null) {
            this.logRec = new LogRecord();
        }

        if(this.logInterface == null) {
            this.logInterface = new LogInterface();
        }

        if(this._$75209) {
            try {
                if(this.hasDefineMoniterHelper && this.logHelper == null) {
                    this.logHelper = this.monitorHelper.getLoggerHelper();
                }
            } catch (Exception var3) {
                this.hasDefineMoniterHelper = false;
            }
        }

        try {
            if(this.monitorHelper == null) {
                this.monitorHelper = BaseServerClient.getMonitorHelper();
            }
        } catch (Exception var2) {
            ;
        }

    }

    public String getCurrentUser() {
        return FlowWebBean.getCurrentUser(this.m_request);
    }

    public String getCurrentDeptCode() {
        return FlowWebBean.getCurrentDeptCode(this.m_request);
    }

    private WFOperatorLogRecord _$75303(int operation) {
        WFOperatorLogRecord log = new WFOperatorLogRecord();
        if(this.m_request != null) {
            this.bsc.setCurrentUser(this.getCurrentUser());
            log.setOperator(this.getCurrentUser());
            log.setOperatorName(this.currentUserName);
            log.setRemoteIP(this.m_request.getRemoteAddr());
        }

        if(this.monitorHelper != null) {
            log.setSetCode(this.monitorHelper.getSystemCode());
            log.setSysCode(this.currentRegion);
        }

        log.setOperation(operation);
        return log;
    }

    private void _$75312(WFOperatorLogRecord log) {
        if(this.logHelper != null) {
            this.logHelper.log(log);
        }

    }

    public final void initialize(PageContext pageContext) {
        this.initialize((HttpServletRequest)pageContext.getRequest(), (HttpServletResponse)pageContext.getResponse());
    }

    public final void initialize(HttpServletRequest request, HttpServletResponse response) {
        this._$75226();
        String ejburl = SysPara.getEJBURL();
        this._$75209 = SysPara.isDebugMode();

        try {
            this.m_request = request;
            this.m_session = this.m_request.getSession();
            this.currentUser = this.getCurrentUser();
            this.bsc.setCurrentUser(this.currentUser);
            CachedRowSet e = null;

            try {
                e = BaseServerClient.getUserWizardInfo(this.currentUser);
                e.beforeFirst();
                if(e.next()) {
                    this.currentRegion = e.getString("sys_id");
                    this.currentUserName = e.getString("user_name");
                }
            } catch (Exception var13) {
                var13.printStackTrace();
            } finally {
                PublicLib.closeCachedRowSet(e);
                e = null;
            }

            if(this.currentRegion == null) {
                this.currentRegion = "";
            }

            if(this.currentUserName == null) {
                this.currentUserName = "";
            }

            if(this._$75209) {
                this.logRec.log_type = 1;
                this.logRec.modu_code = this._$75212;
                this.logRec.modu_name = "工作流";
                this.logRec.user_code = this.getCurrentUser();
                this.logRec.remote_ip = this.m_request.getRemoteAddr();
            }

            if(this._$75298 == null) {
                Context ctx = this.getInitialContext();
                Object ref = ctx.lookup(String.valueOf(String.valueOf(ejburl)).concat("FlowEngineLocal"));
                this._$75298 = (FlowEngineLocalHome)ref;
            }

            this.create();
        } catch (Exception var15) {
            if(this._$75209) {
                this.logRec.log_level = 2;
            }

            System.out.println("创建FlowEngine EJB失败，失败原因：".concat(String.valueOf(String.valueOf(var15.getMessage()))));
            var15.printStackTrace();
        }

    }

    private BaseArchiveInterface _$75314() {
        try {
            if(this.arcInterface == null) {
                this.arcInterface = new ArchiveInterfaceImpl(this.getCurrentUser(), false);
            }
        } catch (Exception var2) {
            System.err.println("初始化归档接口失败!");
            var2.printStackTrace();
        }

        return this.arcInterface;
    }

    public Context getInitialContext() throws Exception {
        return new InitialContext();
    }

    public FlowEngineLocal create() {
        try {
            this._$75299 = this._$75298.create();
        } catch (Exception var2) {
            ;
        }

        return this._$75299;
    }

    public Hashtable flowInsNew(long aFlowId, boolean aOnlySave, RecordSet aSomeInfoRs, CachedRowSet aMainRs, Hashtable aHypoRsColl, RecordSet aHypoMapRs, CachedRowSet aAttachRs, byte[] aFileBuf) throws Exception {
        Hashtable returnValue = null;
        WFOperatorLogRecord newLog = null;
        WFOperatorLogRecord sendLog = null;
        if(this._$75299 == null) {
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "新建流程发送";
                if(aOnlySave) {
                    this.logRec.oper_desc = "保存";
                } else {
                    this.logRec.oper_desc = "保存并发送";
                }

                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf("新建流程".concat(String.valueOf(String.valueOf(aFlowId))))));
            }

            try {
                returnValue = this._$75299.flowInsNew(aFlowId, aOnlySave, aSomeInfoRs, aMainRs, aHypoRsColl, aHypoMapRs, aAttachRs, aFileBuf);
                if(this._$75209) {
                    String e = (String)returnValue.get("flowInid");
                    String stepInid = (String)returnValue.get("stepInid");
                    newLog = this._$75303(1);
                    newLog.setOperName("新建");
                    newLog.setOperDesc("新建");
                    newLog.setLogLevel(4);
                    newLog.setKeyField(e);
                    newLog.setOperResu("成功");
                    this._$75312(newLog);
                    if(!aOnlySave) {
                        sendLog = this._$75303(2);
                        sendLog.setOperName("发送");
                        sendLog.setOperDesc("发送");
                        sendLog.setLogLevel(4);
                        sendLog.setKeyField(e);
                        sendLog.setOperResu("成功");
                        this._$75312(sendLog);
                    }

                    this.logRec.attachField1 = e;
                    this.logRec.attachField2 = stepInid;
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return returnValue;
            } catch (Exception var15) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var15.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception(var15.getMessage());
            }
        }
    }

    public Hashtable flowInsSend(long aFlowInid, boolean aOnlySave, boolean aIsModified, RecordSet aSomeInfoRs, CachedRowSet aMainRs, Hashtable aHypoRsColl, RecordSet aHypoMapRs, CachedRowSet aAttachRs, byte[] aFileBuf) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in flowInsSend(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "流程实例发送";
                if(aOnlySave) {
                    this.logRec.oper_desc = "保存";
                } else {
                    this.logRec.oper_desc = "保存并发送";
                }

                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf("流程实例流程ID".concat(String.valueOf(String.valueOf(aFlowInid))))));
            }

            try {
                returnValue = this._$75299.flowInsSend(aFlowInid, aOnlySave, aIsModified, aSomeInfoRs, aMainRs, aHypoRsColl, aHypoMapRs, aAttachRs, aFileBuf);
                if(this._$75209) {
                    int e = this._$37883(aFlowInid);
                    if(!aOnlySave || e == 3) {
                        WFOperatorLogRecord sendLog = this._$75303(2);
                        sendLog.setLogLevel(4);
                        sendLog.setKeyField(String.valueOf(aFlowInid));
                        sendLog.setOperName("发送");
                        sendLog.setOperDesc("发送");
                        sendLog.setOperResu("成功");
                        this._$75312(sendLog);
                        WFOperatorLogRecord dealLog;
                        if(e == 3) {
                            dealLog = null;
                            dealLog = this._$75303(9);
                            dealLog.setOperName("阅读");
                            dealLog.setOperDesc("知会阅读");
                            dealLog.setLogLevel(4);
                            dealLog.setKeyField(String.valueOf(aFlowInid));
                            dealLog.setOperResu("成功");
                            this._$75312(dealLog);
                            String dealNote = (String)aSomeInfoRs.getObject("deal_note");
                            if(dealNote != null && dealNote.trim().length() > 0) {
                                WFOperatorLogRecord responseLog = null;
                                responseLog = this._$75303(10);
                                responseLog.setOperName("批注");
                                responseLog.setOperDesc("知会批注");
                                responseLog.setLogLevel(4);
                                responseLog.setKeyField(String.valueOf(aFlowInid));
                                responseLog.setOperResu("成功");
                                this._$75312(responseLog);
                            }
                        } else {
                            dealLog = null;
                            dealLog = this._$75303(8);
                            dealLog.setOperName("办理");
                            dealLog.setOperDesc("办理");
                            dealLog.setLogLevel(4);
                            dealLog.setKeyField(String.valueOf(aFlowInid));
                            dealLog.setOperResu("成功");
                            this._$75312(dealLog);
                        }
                    }

                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return returnValue;
            } catch (Exception var17) {
                if(this._$75209) {
                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var17.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception(var17.getMessage());
            }
        }
    }

    public IReturn flowInsSend(long aFlowInid, long stepInid, int dealIndx) throws Exception {
        IReturn returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in flowInsSend(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "流程实例发送";
                this.logRec.oper_desc = "保存并发送";
                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf("流程实例流程ID".concat(String.valueOf(String.valueOf(aFlowInid))))));
            }

            try {
                returnValue = this._$75299.flowInsSend(aFlowInid, stepInid, dealIndx);
                if(this._$75209) {
                    int e = this._$37883(aFlowInid);
                    WFOperatorLogRecord sendLog = this._$75303(2);
                    sendLog.setLogLevel(4);
                    sendLog.setKeyField(String.valueOf(aFlowInid));
                    sendLog.setOperName("发送");
                    sendLog.setOperDesc("发送");
                    sendLog.setOperResu("成功");
                    this._$75312(sendLog);
                    WFOperatorLogRecord dealLog;
                    if(e == 3) {
                        dealLog = null;
                        dealLog = this._$75303(9);
                        dealLog.setOperName("阅读");
                        dealLog.setOperDesc("知会阅读");
                        dealLog.setLogLevel(4);
                        dealLog.setKeyField(String.valueOf(aFlowInid));
                        dealLog.setOperResu("成功");
                        this._$75312(dealLog);
                    } else {
                        dealLog = null;
                        dealLog = this._$75303(8);
                        dealLog.setOperName("办理");
                        dealLog.setOperDesc("办理");
                        dealLog.setLogLevel(4);
                        dealLog.setKeyField(String.valueOf(aFlowInid));
                        dealLog.setOperResu("成功");
                        this._$75312(dealLog);
                    }
                }

                this.logRec.attachField1 = String.valueOf(aFlowInid);
                this.logRec.attachField2 = String.valueOf(stepInid);
                this.logRec.attachField3 = String.valueOf(dealIndx);
                this.logRec.log_level = 4;
                this.logRec.oper_resu = "成功";
                this.logInterface.log(this.logRec);
                return returnValue;
            } catch (Exception var10) {
                if(this._$75209) {
                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.attachField2 = String.valueOf(stepInid);
                    this.logRec.attachField3 = String.valueOf(dealIndx);
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var10.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception(var10.getMessage());
            }
        }
    }

    public IReturn flowInsBatchSend(String structs) {
        ReturnImpl oRet = new ReturnImpl();
        if(structs != null && structs.trim().length() != 0) {
            String[] sendItems = PublicLib.split(structs, ";");
            int errCount = 0;
            if(sendItems != null && sendItems.length > 0) {
                for(int i = 0; i < sendItems.length; ++i) {
                    String sendItem = sendItems[i];
                    if(sendItem != null) {
                        String[] fields = PublicLib.split(sendItem, ",");
                        if(fields != null && fields.length == 4) {
                            try {
                                long sendEx = Long.parseLong(fields[0]);
                                long stepInid = Long.parseLong(fields[1]);
                                String dealMan = fields[2];
                                int dealIndx = Integer.parseInt(fields[3]);
                                IReturn tmpRet = this.flowInsSend(sendEx, stepInid, dealIndx);
                                int flag = tmpRet.getReturnInt("flag", -1);
                                if(flag != 1) {
                                    ++errCount;
                                }
                            } catch (Exception var16) {
                                ++errCount;
                                var16.printStackTrace();
                            }
                        } else {
                            ++errCount;
                        }
                    }
                }
            }

            oRet.setReturnValue("ErrorCount", String.valueOf(errCount));
            return oRet;
        } else {
            return oRet;
        }
    }

    public IReturn reminderTask(String structs, String reminderMsg, int reminderType) {
        IReturn oRet = null;

        try {
            oRet = this._$75299.reminderTask(structs, reminderMsg, reminderType);
        } catch (Exception var10) {
            var10.printStackTrace();
        } finally {
        }

        return oRet;
    }

    private String _$75331(int msgCode) {
        String sRet = "未知的信息代码：".concat(String.valueOf(String.valueOf(msgCode)));
        switch(msgCode) {
            case 0:
                sRet = "保存成功";
                break;
            case 1:
                sRet = "发送成功";
                break;
            case 2:
                sRet = "多条下一步骤匹配nextStepRs有效";
                break;
            case 3:
                sRet = "需要选定一个经办人(单选)nextMenRs有效";
                break;
            case 4:
                sRet = "需要选定多个经办人(多选)nextMenRs有效";
        }

        return sRet;
    }

    public Hashtable flowInsSend(long aFlowInid, long aStepInid, boolean aOnlySave, boolean aIsModified, RecordSet aSomeInfoRs, CachedRowSet aMainRs, Hashtable aHypoRsColl, RecordSet aHypoMapRs, CachedRowSet aAttachRs, byte[] aFileBuf) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in flowInsSend(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "流程实例发送";
                if(aOnlySave) {
                    this.logRec.oper_desc = "保存";
                } else {
                    this.logRec.oper_desc = "保存并发送";
                }

                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("流程实例流程ID")).append(aFlowInid).append("步骤实例id").append(aStepInid))))));
            }

            try {
                returnValue = this._$75299.flowInsSend(aFlowInid, aStepInid, aOnlySave, aIsModified, aSomeInfoRs, aMainRs, aHypoRsColl, aHypoMapRs, aAttachRs, aFileBuf);
                if(this._$75209) {
                    int e = this._$37883(aFlowInid, aStepInid);
                    if(!aOnlySave || e == 3) {
                        WFOperatorLogRecord sendLog = this._$75303(2);
                        sendLog.setOperName("发送");
                        sendLog.setOperDesc("发送");
                        sendLog.setLogLevel(4);
                        sendLog.setKeyField(String.valueOf(aFlowInid));
                        sendLog.setOperResu("成功");
                        this._$75312(sendLog);
                        WFOperatorLogRecord dealLog;
                        if(e == 3) {
                            dealLog = null;
                            dealLog = this._$75303(9);
                            dealLog.setOperName("阅读");
                            dealLog.setOperDesc("知会阅读");
                            dealLog.setLogLevel(4);
                            dealLog.setKeyField(String.valueOf(aFlowInid));
                            dealLog.setOperResu("成功");
                            this._$75312(dealLog);
                            String dealNote = (String)aSomeInfoRs.getObject("deal_note");
                            if(dealNote != null && dealNote.trim().length() > 0) {
                                WFOperatorLogRecord responseLog = null;
                                responseLog = this._$75303(10);
                                responseLog.setOperName("批注");
                                responseLog.setOperDesc("知会批注");
                                responseLog.setLogLevel(4);
                                responseLog.setKeyField(String.valueOf(aFlowInid));
                                responseLog.setOperResu("成功");
                                this._$75312(responseLog);
                            }
                        } else {
                            dealLog = null;
                            dealLog = this._$75303(8);
                            dealLog.setOperName("办理");
                            dealLog.setOperDesc("办理");
                            dealLog.setLogLevel(4);
                            dealLog.setKeyField(String.valueOf(aFlowInid));
                            dealLog.setOperResu("成功");
                            this._$75312(dealLog);
                        }
                    }

                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.attachField2 = String.valueOf(aStepInid);
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return returnValue;
            } catch (Exception var19) {
                if(this._$75209) {
                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.attachField2 = String.valueOf(aStepInid);
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var19.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception(var19.getMessage());
            }
        }
    }

    private int _$37883(long aFlowInid, long aStepInid) {
        CachedRowSet stepInsRs = null;
        int stepType = -1;

        try {
            String ex = "select * from step_inst where flow_inid=? and step_inco=?";
            SQLPara sqlPara = new SQLPara();
            sqlPara.add(String.valueOf(aFlowInid));
            sqlPara.add(String.valueOf(aStepInid), 1);
            stepInsRs = this.bsc.getRowSet(ex, sqlPara);
            stepInsRs.first();
            stepType = stepInsRs.getInt("step_type");
        } catch (Exception var13) {
            var13.printStackTrace();
        } finally {
        }

        return stepType;
    }

    private int _$37883(long aFlowInid) {
        CachedRowSet stepInsRs = null;
        int stepType = -1;

        try {
            String ex = "select * from step_inst where flow_inid=? and step_inco in(select curr_step from flow_inst where flow_inid=?)";
            SQLPara sqlPara = new SQLPara();
            sqlPara.add(String.valueOf(aFlowInid));
            sqlPara.add(String.valueOf(aFlowInid));
            stepInsRs = this.bsc.getRowSet(ex, sqlPara);
            stepInsRs.first();
            stepType = stepInsRs.getInt("step_type");
        } catch (Exception var11) {
            var11.printStackTrace();
        } finally {
        }

        return stepType;
    }

    public CachedRowSet getFlows(long aParentItem) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlows(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getFlows(aParentItem);
            } catch (Exception var5) {
                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getAllFlows(long aParentItem) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getAllFlows(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getAllFlows(aParentItem);
            } catch (Exception var5) {
                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public Hashtable getFreeFormHtml(long aFlowInid, long aStepInid, boolean aReadOnly, boolean aNewFlag, long aFSetId, long aListFormId, long aFreeFormId, int aRecordIndex, CachedRowSet aMainRs, CachedRowSet aHypoRs) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFreeFormHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getFreeFormHtml(aFlowInid, aStepInid, aReadOnly, aNewFlag, aFSetId, aListFormId, aFreeFormId, aRecordIndex, aMainRs, aHypoRs);
                if(this._$75209) {
                    ;
                }

                return returnValue;
            } catch (Exception var18) {
                if(this._$75209) {
                    ;
                }

                throw new Exception("打开自由表单失败，".concat(String.valueOf(String.valueOf(var18.getMessage()))));
            }
        }
    }

    public Hashtable getStartStepHtml(long aFlowId, Map initData) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getStartStepHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开新建的表单";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("打开新建的流程")).append(aFlowId).append("的表单")));
            }

            try {
                returnValue = this._$75299.getStartStepHtml(aFlowId, initData);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }

                return returnValue;
            } catch (Exception var6) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var6.getMessage()))))));
                }

                throw new Exception("打开表单失败，".concat(String.valueOf(String.valueOf(var6.getMessage()))));
            }
        }
    }

    public Hashtable getStepInsHtml(long aFlowInid) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getStepInsHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开已经存在的表单";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("打开已经存在的流程")).append(aFlowInid).append("的表单")));
            }

            try {
                returnValue = this._$75299.getStepInsHtml(aFlowInid);
                if(this._$75209 && returnValue.get("__IS_forM__First_OPen___") != null) {
                    WFOperatorLogRecord e = this._$75303(4);
                    e.setOperName("收到");
                    e.setOperDesc("用户第一次打开表单");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowInid));
                    e.setOperResu("成功");
                    this._$75312(e);
                }

                return returnValue;
            } catch (Exception var5) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var5.printStackTrace();
                throw new Exception("打开表单失败，".concat(String.valueOf(String.valueOf(var5.getMessage()))));
            }
        }
    }

    public Hashtable getStepInsHtml(long aFlowInid, long aStepInid, Map initData) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getStepInsHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开已经存在的表单";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("打开已经存在的流程")).append(aFlowInid).append("步骤").append(aStepInid).append("的表单")));
            }

            try {
                returnValue = this._$75299.getStepInsHtml(aFlowInid, aStepInid, initData);
                if(this._$75209 && returnValue.get("__IS_forM__First_OPen___") != null) {
                    WFOperatorLogRecord e = this._$75303(4);
                    e.setOperName("收到");
                    e.setOperDesc("用户第一次打开表单");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowInid));
                    e.setOperResu("成功");
                    this._$75312(e);
                }

                return returnValue;
            } catch (Exception var8) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var8.getMessage()))))));
                }

                throw new Exception("打开表单失败，".concat(String.valueOf(String.valueOf(var8.getMessage()))));
            }
        }
    }

    public Hashtable getStepInsHtml(long aFlowInid, long aStepInid, long aDealIndx, Map initData) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getStepInsHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开已经存在的表单";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("打开已经存在的流程")).append(aFlowInid).append("步骤").append(aStepInid).append("的表单")));
            }

            try {
                returnValue = this._$75299.getStepInsHtml(aFlowInid, aStepInid, aDealIndx, initData);
                if(this._$75209 && returnValue.get("__IS_forM__First_OPen___") != null) {
                    WFOperatorLogRecord e = this._$75303(4);
                    e.setOperName("收到");
                    e.setOperDesc("用户第一次打开表单");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowInid));
                    e.setOperResu("成功");
                    this._$75312(e);
                }

                return returnValue;
            } catch (Exception var10) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var10.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var10.printStackTrace();
                throw new Exception("打开表单失败，".concat(String.valueOf(String.valueOf(var10.getMessage()))));
            }
        }
    }

    public Hashtable getWapStepInsHtml(long aFlowInid, long aStepInid, long aDealIndx) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getWapStepInsHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开已经存在的表单(wap)";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("打开已经存在的流程")).append(aFlowInid).append("步骤").append(aStepInid).append("的表单（wap）")));
            }

            try {
                returnValue = this._$75299.getWapStepInsHtml(aFlowInid, aStepInid, aDealIndx);
                if(this._$75209 && returnValue.get("__IS_forM__First_OPen___") != null) {
                    WFOperatorLogRecord e = this._$75303(4);
                    e.setOperName("收到");
                    e.setOperDesc("用户第一次打开表单(wap)");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowInid));
                    e.setOperResu("成功");
                    this._$75312(e);
                }

                return returnValue;
            } catch (Exception var9) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var9.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var9.printStackTrace();
                throw new Exception("打开表单失败，".concat(String.valueOf(String.valueOf(var9.getMessage()))));
            }
        }
    }

    public Hashtable getWapStepInsHtml(long aFlowInid, long aStepInid) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getWapStepInsHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开已经存在的表单(wap)";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("打开已经存在的流程")).append(aFlowInid).append("步骤").append(aStepInid).append("的表单（wap）")));
            }

            try {
                returnValue = this._$75299.getWapStepInsHtml(aFlowInid, aStepInid);
                if(this._$75209 && returnValue.get("__IS_forM__First_OPen___") != null) {
                    WFOperatorLogRecord e = this._$75303(4);
                    e.setOperName("收到");
                    e.setOperDesc("用户第一次打开表单(wap)");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowInid));
                    e.setOperResu("成功");
                    this._$75312(e);
                }

                return returnValue;
            } catch (Exception var7) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var7.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var7.printStackTrace();
                throw new Exception("打开表单失败，".concat(String.valueOf(String.valueOf(var7.getMessage()))));
            }
        }
    }

    public Hashtable getWapStepInsHtml(long aFlowInid) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getWapStepInsHtml(long aFlowInid): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开已经存在的表单";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("打开已经存在的流程")).append(aFlowInid).append("的表单")));
            }

            try {
                returnValue = this._$75299.getWapStepInsHtml(aFlowInid);
                if(this._$75209 && returnValue.get("__IS_forM__First_OPen___") != null) {
                    WFOperatorLogRecord e = this._$75303(4);
                    e.setOperName("收到");
                    e.setOperDesc("用户第一次打开表单");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowInid));
                    e.setOperResu("成功");
                    this._$75312(e);
                }

                return returnValue;
            } catch (Exception var5) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var5.printStackTrace();
                throw new Exception("打开表单失败，".concat(String.valueOf(String.valueOf(var5.getMessage()))));
            }
        }
    }

    public Hashtable getWapStartStepHtml(long aFlowId) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getWapStartStepHtml(aFlowId): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开新建流程实例表单(wap)";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("新建流程")).append(aFlowId).append("的表单（wap）")));
            }

            try {
                returnValue = this._$75299.getWapStartStepHtml(aFlowId);
                if(this._$75209 && returnValue.get("__IS_forM__First_OPen___") != null) {
                    WFOperatorLogRecord e = this._$75303(4);
                    e.setOperName("收到");
                    e.setOperDesc("用户第一次打开表单(wap)");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowId));
                    e.setOperResu("成功");
                    this._$75312(e);
                }

                return returnValue;
            } catch (Exception var5) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var5.printStackTrace();
                throw new Exception("新建表单失败，".concat(String.valueOf(String.valueOf(var5.getMessage()))));
            }
        }
    }

    public Hashtable getBatchStepInsHtml(String aStruct) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getBatchStepInsHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "打开已经存在的表单";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("打开已经存在的表单")).append(aStruct).append("结构为流程实例id1,步骤实例id1;流程实例id2,步骤实例id2;...流程实例idn,步骤实例idn;")));
            }

            try {
                returnValue = this._$75299.getBatchStepInsHtml(aStruct);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }

                return returnValue;
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                }

                throw new Exception("打开批处理表单失败，".concat(String.valueOf(String.valueOf(var4.getMessage()))));
            }
        }
    }

    public RecordSet fieldList(String aFlowInid) {
        RecordSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in fieldList(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.fieldList(aFlowInid);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var4) {
                if(this._$75209) {
                    ;
                }

                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public RecordSet fieldList(String aServerName, String aTableName) {
        RecordSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in fieldList(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.fieldList(aServerName, aTableName);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var5) {
                if(this._$75209) {
                    ;
                }

                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public Hashtable selectList(long aFsetId, long aFormCode, String aLogicName, String aSortField, String aQueryValue) {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in selectList(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.selectList(aFsetId, aFormCode, aLogicName, aSortField, aQueryValue);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var10) {
                if(this._$75209) {
                    ;
                }

                var10.printStackTrace();
            }

            return returnValue;
        }
    }

    public Hashtable selectButton(long aFsetId, long aFormCode, String aLogicName, String aSortField) {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in selectButton(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.selectButton(aFsetId, aFormCode, aLogicName, aSortField);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var9) {
                if(this._$75209) {
                    ;
                }

                var9.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getArchList(String aCondition, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq) {
        return this.getArchList(aCondition, (String)null, (String)null, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aFlowFolder, aDocSeq);
    }

    public CachedRowSet getArchList(String aCondition, String aDept, String docZoneID, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq) {
        CachedRowSet returnValue = null;
        if(this._$75209) {
            ;
        }

        try {
            returnValue = this._$75314().getArchList(aCondition, aDept, docZoneID, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aFlowFolder, aDocSeq);
            if(this._$75209) {
                ;
            }
        } catch (Exception var15) {
            if(this._$75209) {
                ;
            }

            var15.printStackTrace();
        }

        return returnValue;
    }

    public CachedRowSet getHasArchivedList(String aCondition, String aDept, String docZoneID, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq) {
        CachedRowSet returnValue = null;
        if(this._$75209) {
            ;
        }

        try {
            returnValue = this._$75314().getHasArchivedList(aCondition, aDept, docZoneID, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aFlowFolder, aDocSeq);
        } catch (Exception var15) {
            var15.printStackTrace();
        }

        return returnValue;
    }

    public boolean clearArchivedFlowInst(String flowInidList) {
        boolean returnValue = false;
        if(this._$75209) {
            ;
        }

        try {
            returnValue = this._$75314().clearArchivedFlowInst(flowInidList);
        } catch (Exception var4) {
            var4.printStackTrace();
        }

        return returnValue;
    }

    public CachedRowSet getUnionArchList(String aCondition, String aDept, String docZoneID, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq) {
        CachedRowSet returnValue = null;

        try {
            returnValue = this._$75314().getUnionArchList(aCondition, aDept, docZoneID, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aFlowFolder, aDocSeq);
        } catch (Exception var15) {
            var15.printStackTrace();
        }

        return returnValue;
    }

    public String reGetFormHtml(long aFlowInid, long aStepInid, RecordSet aSomeInfoRs, CachedRowSet aMainRs, Hashtable aHypoRsColl) throws Exception {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in reGetFormHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.reGetFormHtml(aFlowInid, aStepInid, aSomeInfoRs, aMainRs, aHypoRsColl);
                if(this._$75209) {
                    ;
                }

                return returnValue;
            } catch (Exception var10) {
                if(this._$75209) {
                    ;
                }

                throw new Exception("重新生成表单失败，".concat(String.valueOf(String.valueOf(var10.getMessage()))));
            }
        }
    }

    public String reGetFormHtml(long aFlowInid, long aStepInid, RecordSet aSomeInfoRs, CachedRowSet aMainRs, Hashtable aHypoRsColl, boolean aReadOnly) throws Exception {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in reGetFormHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.reGetFormHtml(aFlowInid, aStepInid, aSomeInfoRs, aMainRs, aHypoRsColl, aReadOnly);
                if(this._$75209) {
                    ;
                }

                return returnValue;
            } catch (Exception var11) {
                if(this._$75209) {
                    ;
                }

                throw new Exception("重新生成表单失败，".concat(String.valueOf(String.valueOf(var11.getMessage()))));
            }
        }
    }

    public void adminWithdraw(long aFlowInid, long aStepInid, long dealIndx) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in adminWithdraw(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            try {
                this._$75299.adminWithdraw(aFlowInid, aStepInid, dealIndx);
                if(this._$75209) {
                    WFOperatorLogRecord e = null;
                    e = this._$75303(3);
                    e.setOperName("回撤");
                    e.setOperDesc("管理员回撤");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowInid));
                    e.setOperResu("成功");
                    this._$75312(e);
                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.attachField2 = String.valueOf(aStepInid);
                    this.logRec.attachField3 = String.valueOf(dealIndx);
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var8) {
                if(this._$75209) {
                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.attachField2 = String.valueOf(aStepInid);
                    this.logRec.attachField3 = String.valueOf(dealIndx);
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var8.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var8.printStackTrace();
                throw new Exception("步骤回撤失败，".concat(String.valueOf(String.valueOf(var8.getMessage()))));
            }
        }
    }

    public void userWithdraw(String struct) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in userWithdraw(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "普通用户回撤自己经办的步骤";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("普通用户回撤自己经办的步骤")).append(struct).append("(流程实例id,步骤实例id,经办人)")));
            }

            WFOperatorLogRecord withdrawLog = null;
            String flowInid = PublicLib.getField(struct, ",", 0L);

            try {
                this._$75299.userWithdraw(struct);
                if(this._$75209) {
                    withdrawLog = this._$75303(3);
                    withdrawLog.setOperName("回撤");
                    withdrawLog.setOperDesc("普通用户回撤");
                    withdrawLog.setLogLevel(4);
                    withdrawLog.setKeyField(flowInid);
                    withdrawLog.setOperResu("成功");
                    this._$75312(withdrawLog);
                    this.logRec.attachField1 = flowInid;
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var5) {
                if(this._$75209) {
                    this.logRec.attachField1 = flowInid;
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var5.printStackTrace();
                throw new Exception("步骤回撤失败，".concat(String.valueOf(String.valueOf(var5.getMessage()))));
            }
        }
    }

    public CachedRowSet getFlowTrace(long aFlowInid) {
        return this.getFlowTrace(aFlowInid, false);
    }

    public CachedRowSet getFlowTrace(long aFlowInid, boolean containNextActionName) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlowTrace(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getFlowTrace(aFlowInid, containNextActionName);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var6) {
                if(this._$75209) {
                    ;
                }

                var6.printStackTrace();
            }

            return returnValue;
        }
    }

    public byte[] downloadField(long aFlowInid, long aStepInid, String aFileName, long aFileType) {
        byte[] returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in downloadField(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "从数据库中取出文件的内容";
                if(aFileType == (long)0) {
                    this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("从数据库中取出流程")).append(aFlowInid).append("中步骤").append(aStepInid).append("的正文模板文件").append(aFileName).append("的内容")));
                }

                if(aFileType == (long)1) {
                    this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("从数据库中取出流程")).append(aFlowInid).append("中步骤").append(aStepInid).append("的盖章图像文件").append(aFileName).append("的内容")));
                }

                if(aFileType == (long)2) {
                    this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("从数据库中取出流程")).append(aFlowInid).append("中步骤").append(aStepInid).append("的流程实例文件").append(aFileName).append("的内容")));
                }
            }

            try {
                returnValue = this._$75299.downloadField(aFlowInid, aStepInid, aFileName, aFileType);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }
            } catch (Exception var10) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var10.getMessage()))))));
                }

                var10.printStackTrace();
            }

            return returnValue;
        }
    }

    public String flowInsArch(String flowInidList, String runtimeStructXml) throws Exception {
        String returnValue = "";
        if(this._$75209) {
            this.logRec.oper_name = "对指定的流程列表进行归档";
            this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("对指定的流程列表")).append(flowInidList).append("进行归档,归档结构如下").append(runtimeStructXml)));
        }

        try {
            returnValue = this._$75314().flowInsArch(flowInidList, runtimeStructXml);
            if(this._$75209) {
                this.logRec.log_level = 4;
                this.logRec.oper_resu = "成功";
                this.logInterface.log(this.logRec);
            }

            return returnValue;
        } catch (Exception var5) {
            if(this._$75209) {
                this.logRec.log_level = 2;
                this.logRec.oper_resu = "失败";
                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                this.logInterface.log(this.logRec);
            }

            throw new Exception("归档失败，".concat(String.valueOf(String.valueOf(var5.getMessage()))));
        }
    }

    public String unionFlowInsArch(String flowInidList, String runtimeStructXml) throws Exception {
        String returnValue = "";
        if(this._$75209) {
            this.logRec.oper_name = "对指定的流程列表进行归档";
            this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("对指定的流程列表")).append(flowInidList).append("进行归档,归档结构如下").append(runtimeStructXml)));
        }

        try {
            returnValue = this._$75314().unionFlowInsArch(flowInidList, runtimeStructXml);
            if(this._$75209) {
                this.logRec.log_level = 4;
                this.logRec.oper_resu = "成功";
                this.logInterface.log(this.logRec);
            }

            return returnValue;
        } catch (Exception var5) {
            if(this._$75209) {
                this.logRec.log_level = 2;
                this.logRec.oper_resu = "失败";
                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                this.logInterface.log(this.logRec);
            }

            throw new Exception("归档失败，".concat(String.valueOf(String.valueOf(var5.getMessage()))));
        }
    }

    public String getFormHtml(long fsetId, long formCode) {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in getFormHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getFormHtml(fsetId, formCode);
            } catch (Exception var7) {
                var7.printStackTrace();
            }

            return returnValue;
        }
    }

    public String getFormHtml(long flowInid) {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in getFormHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getFormHtml(flowInid);
            } catch (Exception var5) {
                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public String getPrintFormHtml(long aFlowInid, long aStepInid) {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in getFormHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getPrintFormHtml(aFlowInid, aStepInid);
            } catch (Exception var7) {
                var7.printStackTrace();
            }

            return returnValue;
        }
    }

    public void restoreTask(String struct) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in restoreTask(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "恢复自己删除的经办信息";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("恢复自己删除的经办信息，结构为")).append(struct).append("(流程实例id,步骤实例id,经办人;流程实例id,步骤实例id,经办人)")));
            }

            try {
                this._$75299.restoreTask(struct);
                if(this._$75209) {
                    WFOperatorLogRecord e = null;
                    e = this._$75303(6);
                    e.setOperName("恢复");
                    e.setOperDesc("恢复待办");
                    e.setLogLevel(4);
                    e.setOperResu("成功");
                    StringTokenizer st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        String tmp = st.nextToken();
                        String flowInid = PublicLib.getField(tmp, ",", 0L);
                        e.setKeyField(flowInid);
                        this._$75312(e);
                    }

                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var6) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var6.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception("恢复经办信息失败，".concat(String.valueOf(String.valueOf(var6.getMessage()))));
            }
        }
    }

    public CachedRowSet getHideTask() {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getHideTask(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getHideTask();
            } catch (Exception var3) {
                var3.printStackTrace();
            }

            return returnValue;
        }
    }

    public void hideTask(String struct) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in hideTask(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "删除经办信息";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("删除经办信息{")).append(struct).append("}")));
            }

            Exception e;
            StringTokenizer st;
            String tmp;
            String flowInid;
            String stepInid;
            String dealMan;
            try {
                this._$75299.hideTask(struct);
                if(this._$75209) {
                    e = null;
                    WFOperatorLogRecord e1 = this._$75303(5);
                    e1.setOperName("删除");
                    e1.setOperDesc("逻辑删除待办");
                    e1.setLogLevel(4);
                    e1.setOperResu("成功");
                    st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        tmp = st.nextToken();
                        flowInid = PublicLib.getField(tmp, ",", 0L);
                        stepInid = PublicLib.getField(tmp, ",", 1L);
                        dealMan = PublicLib.getField(tmp, ",", 2L);
                        e1.setKeyField(flowInid);
                        this._$75312(e1);
                        this.logRec.attachField1 = flowInid;
                        this.logRec.attachField2 = stepInid;
                        this.logRec.attachField3 = dealMan;
                        this.logRec.log_level = 4;
                        this.logRec.oper_resu = "成功";
                        this.logInterface.log(this.logRec);
                    }
                }

            } catch (Exception var8) {
                e = var8;
                if(this._$75209) {
                    st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        tmp = st.nextToken();
                        flowInid = PublicLib.getField(tmp, ",", 0L);
                        stepInid = PublicLib.getField(tmp, ",", 1L);
                        dealMan = PublicLib.getField(tmp, ",", 2L);
                        this.logRec.attachField1 = flowInid;
                        this.logRec.attachField2 = stepInid;
                        this.logRec.attachField3 = dealMan;
                        this.logRec.log_level = 2;
                        this.logRec.oper_resu = "失败";
                        this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(e.getMessage()))))));
                        this.logInterface.log(this.logRec);
                    }
                }

                throw new Exception("删除经办信息失败，".concat(String.valueOf(String.valueOf(e.getMessage()))));
            }
        }
    }

    public void deleteTask(String struct) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in deleteTask(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "再次逻辑删除自己的经办信息";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("再次逻辑删除自己的经办信息,结构为")).append(struct).append("(流程实例id,步骤实例id,经办人;流程实例id,步骤实例id,经办人 )")));
            }

            Exception e;
            StringTokenizer st;
            String tmp;
            String flowInid;
            String stepInid;
            String dealMan;
            try {
                this._$75299.deleteTask(struct);
                if(this._$75209) {
                    e = null;
                    WFOperatorLogRecord e1 = this._$75303(5);
                    e1.setOperName("删除");
                    e1.setOperDesc("删除待办");
                    e1.setLogLevel(4);
                    e1.setOperResu("成功");
                    st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        tmp = st.nextToken();
                        flowInid = PublicLib.getField(tmp, ",", 0L);
                        stepInid = PublicLib.getField(tmp, ",", 1L);
                        dealMan = PublicLib.getField(tmp, ",", 2L);
                        e1.setKeyField(flowInid);
                        this._$75312(e1);
                        this.logRec.attachField1 = flowInid;
                        this.logRec.attachField2 = stepInid;
                        this.logRec.attachField3 = dealMan;
                        this.logRec.log_level = 4;
                        this.logRec.oper_resu = "成功";
                        this.logInterface.log(this.logRec);
                    }
                }

            } catch (Exception var8) {
                e = var8;
                if(this._$75209) {
                    st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        tmp = st.nextToken();
                        flowInid = PublicLib.getField(tmp, ",", 0L);
                        stepInid = PublicLib.getField(tmp, ",", 1L);
                        dealMan = PublicLib.getField(tmp, ",", 2L);
                        this.logRec.attachField1 = flowInid;
                        this.logRec.attachField2 = stepInid;
                        this.logRec.attachField3 = dealMan;
                        this.logRec.attachField1 = flowInid;
                        this.logRec.log_level = 2;
                        this.logRec.oper_resu = "失败";
                        this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(e.getMessage()))))));
                        this.logInterface.log(this.logRec);
                    }
                }

                throw new Exception("删除经办信息失败，".concat(String.valueOf(String.valueOf(e.getMessage()))));
            }
        }
    }

    public void redirectDealMan(String struct, String dealMan) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in redirectDealMan(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "转发公文给指定的人";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("转发公文给指定的人,结构为")).append(struct).append("(流程实例id,步骤实例id,经办人)")));
            }

            String flowInid;
            String stepInid;
            try {
                this._$75299.redirectDealMan(struct, dealMan);
                if(this._$75209) {
                    String e = PublicLib.getField(struct, ",", 0L);
                    flowInid = PublicLib.getField(struct, ",", 1L);
                    stepInid = PublicLib.getField(struct, ",", 2L);
                    this.logRec.attachField1 = e;
                    this.logRec.attachField2 = flowInid;
                    this.logRec.attachField3 = stepInid;
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var7) {
                if(this._$75209) {
                    flowInid = PublicLib.getField(struct, ",", 0L);
                    stepInid = PublicLib.getField(struct, ",", 1L);
                    String user = PublicLib.getField(struct, ",", 2L);
                    this.logRec.attachField1 = flowInid;
                    this.logRec.attachField2 = stepInid;
                    this.logRec.attachField3 = user;
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var7.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception("转交失败，".concat(String.valueOf(String.valueOf(var7.getMessage()))));
            }
        }
    }

    public void noticeMan(String struct, String noteMan) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in noticeMan(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "知会经办信息给".concat(String.valueOf(String.valueOf(noteMan)));
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("知会经办信息给")).append(noteMan).append(",结构为").append(struct).append("(流程实例id,步骤实例id,经办人)")));
            }

            try {
                this._$75299.noticeMan(struct, noteMan);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception("知会失败，".concat(String.valueOf(String.valueOf(var4.getMessage()))));
            }
        }
    }

    public String getFlowListHtml(String paraSeq) {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in getFlowListHtml(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getFlowListHtml(paraSeq);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var4) {
                if(this._$75209) {
                    ;
                }

                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public String getArchListHtml(String paraSeq) {
        String returnValue = "";
        if(this._$75209) {
            ;
        }

        try {
            returnValue = this._$75314().getArchListHtml(paraSeq);
            if(this._$75209) {
                ;
            }
        } catch (Exception var4) {
            if(this._$75209) {
                ;
            }

            var4.printStackTrace();
        }

        return returnValue;
    }

    public Hashtable getDocAppIdOfUsers(Vector vec) {
        Hashtable returnValue = null;
        if(this._$75209) {
            ;
        }

        try {
            returnValue = this._$75314().getDocAppIdOfUsers(vec);
            if(this._$75209) {
                ;
            }
        } catch (Exception var4) {
            if(this._$75209) {
                ;
            }

            var4.printStackTrace();
        }

        return returnValue;
    }

    public String getFlowFolderHtml() {
        Hashtable oHTData = null;
        List flowFolderList = null;
        Object crs = null;
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in getFlowDirectory() ");
            return returnValue;
        } else {
            StringBuffer sb = new StringBuffer("");

            try {
                sb.append("<option value=\'\'></option>");
                oHTData = this._$75299.getFlowDirectory();
                System.err.println(oHTData);
                flowFolderList = (List)oHTData.remove("_FlowDirectoryName_Vec_");
                if(flowFolderList.size() > 0) {
                    String e = null;
                    String value = null;

                    for(int i = 0; i < flowFolderList.size(); ++i) {
                        e = (String)flowFolderList.get(i);
                        value = null;
                        if(e != null) {
                            value = (String)oHTData.get(e);
                        }

                        if(value != null) {
                            System.err.println(String.valueOf(String.valueOf((new StringBuffer("name.value=")).append(e).append(".").append(value))));
                            String[] flowIds = PublicLib.split(value, ",");
                            sb.append(String.valueOf(String.valueOf((new StringBuffer("<option value=\'")).append(flowIds[0]).append("\'>").append(e).append("</option>"))));
                        }
                    }
                }
            } catch (Exception var10) {
                var10.printStackTrace();
            }

            return sb.toString();
        }
    }

    public String getAttachFlow(String aFlowInid) {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in getAttachFlow(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getAttachFlow(aFlowInid);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var4) {
                if(this._$75209) {
                    ;
                }

                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getTasks(String aUserList) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTasks(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        }

        if(this._$75209) {
            ;
        }

        try {
            returnValue = this._$75299.getTasks(aUserList);
            if(this._$75209) {
                ;
            }
        } catch (Exception var4) {
            if(this._$75209) {
                ;
            }

            var4.printStackTrace();
        }

        return returnValue;
    }

    public Hashtable getFlowDirectory() throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlowDirectory(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getFlowDirectory();
                if(this._$75209) {
                    ;
                }

                return returnValue;
            } catch (Exception var3) {
                if(this._$75209) {
                    ;
                }

                throw new Exception("显示流程类别失败，".concat(String.valueOf(String.valueOf(var3.getMessage()))));
            }
        }
    }

    public CachedRowSet getDeptAdminTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getDeptAdminTaskCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getDeptAdminTaskCount(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aFlowFolder, aDocSeq, aSortField, aClassId);
            } catch (Exception var18) {
                var18.printStackTrace();
            }

            return returnValue;
        }
    }

    public PagingData getDeptAdminTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, int pageNo, int pageSize) {
        PagingData returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getDeptAdminTaskCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getDeptAdminTaskCount(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aFlowFolder, aDocSeq, aSortField, aClassId, (long)pageNo, pageSize);
            } catch (Exception var20) {
                var20.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getAdminTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getAdminTaskCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getAdminTaskCount(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aFlowFolder, aDocSeq, aSortField, aClassId);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var18) {
                if(this._$75209) {
                    ;
                }

                var18.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTaskCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getTaskCount(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aSender, aFlowFolder, aDocSeq, aSortField, aClassId);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var19) {
                if(this._$75209) {
                    ;
                }

                var19.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getTasksEx(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTasksEx(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getTasksEx(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aSender, aFlowFolder, aDocSeq, aSortField, aClassId);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var19) {
                if(this._$75209) {
                    ;
                }

                var19.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getAdminTasksEx(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getAdminTasksEx(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getAdminTasksEx(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aSender, aFlowFolder, aDocSeq, aSortField, aClassId);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var19) {
                if(this._$75209) {
                    ;
                }

                var19.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getTaskQueryCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTaskQueryCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getTaskQueryCount(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aSender, aFlowFolder, aDocSeq, aSortField, aClassId);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var19) {
                if(this._$75209) {
                    ;
                }

                var19.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getTasksQueryEx(String aCondition, String aSortField) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTasksQueryEx(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getTasksQueryEx(aCondition, aSortField);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var5) {
                if(this._$75209) {
                    ;
                }

                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getTasksByMonth(String aYearMonth) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTasksByMonth(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getTasksByMonth(aYearMonth);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var4) {
                if(this._$75209) {
                    ;
                }

                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public void restoreFlowInst(String struct) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in restoreFlowInst(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "系统管理员恢复某个已经删除的流程实例";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("系统管理员恢复某个已经删除的流程实例，结构为：")).append(struct).append("(流程实例id列表，以；隔开)")));
            }

            try {
                this._$75299.restoreFlowInst(struct);
                if(this._$75209) {
                    WFOperatorLogRecord e = null;
                    e = this._$75303(6);
                    e.setOperName("恢复");
                    e.setOperDesc("管理员恢复");
                    e.setLogLevel(4);
                    e.setOperResu("成功");
                    StringTokenizer st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        String flowInid = st.nextToken();
                        e.setKeyField(flowInid);
                        this._$75312(e);
                    }

                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var5) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception("恢复流程实例失败，".concat(String.valueOf(String.valueOf(var5.getMessage()))));
            }
        }
    }

    public CachedRowSet getFlowInstList(String aCondition, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlowInstList(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getFlowInstList(aCondition, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var10) {
                if(this._$75209) {
                    ;
                }

                var10.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getHideFlowInst() {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getHideFlowInst(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getHideFlowInst();
                if(this._$75209) {
                    ;
                }
            } catch (Exception var3) {
                if(this._$75209) {
                    ;
                }

                var3.printStackTrace();
            }

            return returnValue;
        }
    }

    public void hideFlowInst(String struct) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in hideFlowInst(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "系统管理员删除某个流程实例";
                this.logRec.oper_desc = "系统管理员删除某个流程实例";
            }

            try {
                this._$75299.hideFlowInst(struct);
                if(this._$75209) {
                    WFOperatorLogRecord e = null;
                    e = this._$75303(5);
                    e.setOperName("删除");
                    e.setOperDesc("管理员逻辑删除");
                    e.setLogLevel(4);
                    e.setOperResu("成功");
                    StringTokenizer st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        String flowInid = st.nextToken();
                        e.setKeyField(flowInid);
                        this._$75312(e);
                    }

                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var5) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception("删除流程实例失败，".concat(String.valueOf(String.valueOf(var5.getMessage()))));
            }
        }
    }

    public void deleteFlowInst(String struct) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in deleteFlowInst(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "系统管理员物理删除某些流程实例";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("系统管理员物理删除某些流程实例,结果为：")).append(struct).append("(流程实例id列表，以；隔开)")));
            }

            Exception e;
            StringTokenizer st;
            String flowInid;
            try {
                this._$75299.deleteFlowInst(struct);
                if(this._$75209) {
                    e = null;
                    WFOperatorLogRecord e1 = this._$75303(5);
                    e1.setOperName("删除");
                    e1.setOperDesc("管理员删除");
                    e1.setLogLevel(4);
                    e1.setOperResu("成功");
                    st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        flowInid = st.nextToken();
                        e1.setKeyField(flowInid);
                        this._$75312(e1);
                        this.logRec.attachField1 = flowInid;
                        this.logRec.log_level = 4;
                        this.logRec.oper_resu = "成功";
                        this.logInterface.log(this.logRec);
                    }
                }

            } catch (Exception var5) {
                e = var5;
                if(this._$75209) {
                    st = new StringTokenizer(struct, ";");

                    while(st.hasMoreTokens()) {
                        flowInid = st.nextToken();
                        this.logRec.attachField1 = flowInid;
                        this.logRec.log_level = 2;
                        this.logRec.oper_resu = "失败";
                        this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(e.getMessage()))))));
                        this.logInterface.log(this.logRec);
                    }
                }

                throw new Exception("删除流程实例失败，".concat(String.valueOf(String.valueOf(e.getMessage()))));
            }
        }
    }

    public boolean isAdmin() {
        boolean returnValue = false;
        if(this._$75299 == null) {
            System.out.println("Error in isAdmin(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.isAdmin();
            } catch (Exception var3) {
                var3.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getUrgeInfo(long aFlowInid) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getUrgeInfo(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getUrgeInfo(aFlowInid);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var5) {
                if(this._$75209) {
                    ;
                }

                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public long getSmsStatCount(String aCondition, String aSendState) {
        long returnValue = 0L;
        if(this._$75299 == null) {
            System.out.println("Error in getSmsStatCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getSmsStatCount(aCondition, aSendState);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var6) {
                if(this._$75209) {
                    ;
                }

                var6.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getSmsStatList(String aCondition, String aSendState) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getSmsStatList(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getSmsStatList(aCondition, aSendState);
                if(this._$75209) {
                    ;
                }
            } catch (Exception var5) {
                if(this._$75209) {
                    ;
                }

                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getDeptInstStat(long aParaSeq, long aState, String aCondition, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getDeptInstStat(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getDeptInstStat(aParaSeq, aState, aCondition, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName);
            } catch (Exception var15) {
                ;
            }

            return returnValue;
        }
    }

    public String getStepName(long aFlowInid, long aStepInid) throws Exception {
        String returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getStepName(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getStepName(aFlowInid, aStepInid);
            } catch (Exception var7) {
                var7.printStackTrace();
            }

            return returnValue;
        }
    }

    public FormControlHtmlData getFormControlHtmlData(long aFlowInid, long aStepInid) throws Exception {
        FormControlHtmlData returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFormControlHtmlData(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getFormControlHtmlData(aFlowInid, aStepInid);
                return returnValue;
            } catch (Exception var7) {
                var7.printStackTrace();
                throw new Exception("打印表单失败，".concat(String.valueOf(String.valueOf(var7.getMessage()))));
            }
        }
    }

    public boolean addClass(String aClassName) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in addClass(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "增加分类";
                this.logRec.oper_desc = "增加分类".concat(String.valueOf(String.valueOf(aClassName)));
            }

            try {
                this._$75299.addClass(aClassName);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return true;
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                boolean var3 = false;
                return var3;
            }
        }
    }

    public String getClassName(String aClassId) {
        Connection conn = null;
        PreparedStatement pstm = null;
        String sRet = "";

        label40:
        try {
            try {
                conn = this.bsc.getConnection();
                pstm = conn.prepareStatement("select class_name from class_def where class_id=?");
                pstm.setString(1, aClassId);
                ResultSet ex = pstm.executeQuery();
                if(ex.next()) {
                    sRet = ex.getString("class_name");
                }

                PublicLib.closeResultSet(ex);
                ex = null;
            } catch (Exception var10) {
                var10.printStackTrace();
            }
        } finally {
            break label40;
        }

        PublicLib.closePreparedStatement(pstm);
        pstm = null;
        PublicLib.closeConnection(conn);
        conn = null;
        return sRet;
    }

    public boolean updateClass(String aClassId, String aClassName) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in addClass(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "修改分类";
                this.logRec.oper_desc = "修改分类".concat(String.valueOf(String.valueOf(aClassName)));
            }

            try {
                this._$75299.updateClass(aClassId, aClassName);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return true;
            } catch (Exception var5) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                boolean var4 = false;
                return var4;
            }
        }
    }

    public boolean deleteClass(String aClassId) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in deleteClass(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "删除分类";
                this.logRec.oper_desc = "删除分类".concat(String.valueOf(String.valueOf(aClassId)));
            }

            try {
                this._$75299.deleteClass(aClassId);
                if(this._$75209) {
                    this.logRec.attachField1 = aClassId;
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return true;
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.attachField1 = aClassId;
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                boolean var3 = false;
                return var3;
            }
        }
    }

    public boolean docClass(String aFlowInid, long aClassId) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in docClass(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "设置个人公文归类";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("设置个人流程")).append(aFlowInid).append("公文归类").append(aClassId)));
            }

            try {
                this._$75299.docClass(aFlowInid, aClassId);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return true;
            } catch (Exception var6) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var6.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var6.printStackTrace();
                boolean var5 = false;
                return var5;
            }
        }
    }

    public boolean unDocClass(String aFlowInid) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in unDocClass(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "取消个人公文归类";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("设置个人流程")).append(aFlowInid).append("公文归类")));
            }

            try {
                this._$75299.unDocClass(aFlowInid);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return true;
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                boolean var3 = false;
                return var3;
            }
        }
    }

    public CachedRowSet getClasses() {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getClasses(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getClasses();
            } catch (Exception var3) {
                var3.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getTopDirectory() {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTopDirectory(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getTopDirectory();
            } catch (Exception var3) {
                var3.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getFlowNote(String aFlowInid) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlowNote(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getFlowNote(aFlowInid);
            } catch (Exception var4) {
                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getFlowNote(String aFlowInid, String aStepInid) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlowNote(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getFlowNote(aFlowInid, aStepInid);
            } catch (Exception var5) {
                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public String getNoteUsers(long aFlowInid) {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in getNoteUsers(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getNoteUsers(aFlowInid);
            } catch (Exception var5) {
                var5.printStackTrace();
            }

            return returnValue;
        }
    }

    public boolean registerFlowInst(String aStruct, long aRegisterFlag) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in registerFlowInst(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "注册流程";
                this.logRec.oper_desc = "注册流程".concat(String.valueOf(String.valueOf(aStruct)));
            }

            try {
                this._$75299.registerFlowInst(aStruct, aRegisterFlag);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return true;
            } catch (Exception var6) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var6.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var6.printStackTrace();
                boolean var5 = false;
                return var5;
            }
        }
    }

    public Hashtable reloadData(long aFlowInid, long aStepInid) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in reloadData(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.reloadData(aFlowInid, aStepInid);
                return returnValue;
            } catch (Exception var7) {
                throw new Exception("打开表单失败，".concat(String.valueOf(String.valueOf(var7.getMessage()))));
            }
        }
    }

    public CachedRowSet getMarkList() {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getMarkList(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getMarkList();
            } catch (Exception var3) {
                var3.printStackTrace();
            }

            return returnValue;
        }
    }

    public byte[] downloadMark(String aMarkName) {
        byte[] returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in downloadMark(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "下载盖章";
                this.logRec.oper_desc = "下载盖章".concat(String.valueOf(String.valueOf(aMarkName)));
            }

            try {
                returnValue = this._$75299.downloadMark(aMarkName);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                }

                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public byte[] downloadSign(String aUserCode) {
        byte[] returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in downloadSign(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "下载签名";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("下载用户")).append(aUserCode).append("签名")));
            }

            try {
                returnValue = this._$75299.downloadSign(aUserCode);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                }

                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public byte[] downloadPicture(String aPictureId) {
        byte[] returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in downloadPicture(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "下载图片";
                this.logRec.oper_desc = "下载图片".concat(String.valueOf(String.valueOf(aPictureId)));
            }

            try {
                returnValue = this._$75299.downloadPicture(aPictureId);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                }

                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public byte[] downloadPrintTemplet(String fSetId, String formCode, String templetId) throws Exception {
        byte[] returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in downloadPrintTemplet(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "下载表单打印模板";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("下载表单打印模板[FSetId=")).append(fSetId).append("\tformCode=").append(formCode).append("templetId=").append(templetId).append("]")));
            }

            try {
                returnValue = this._$75299.downloadPrintTemplet(fSetId, formCode, templetId);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }
            } catch (Exception var6) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var6.getMessage()))))));
                }

                var6.printStackTrace();
            }

            return returnValue;
        }
    }

    public byte[] downloadTextTemplet(long flowId, String templetId) throws Exception {
        byte[] returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in downloadPrintTemplet(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "下载正文模板";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("下载正文模板[flowInid=")).append(flowId).append("\ttempletId=").append(templetId).append("]")));
            }

            try {
                returnValue = this._$75299.downloadTextTemplet(flowId, templetId);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }
            } catch (Exception var6) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var6.getMessage()))))));
                }

                var6.printStackTrace();
            }

            return returnValue;
        }
    }

    public void flowInsCopy(String aFlowInid, String aSubStructXml) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in flowInsCopy(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "复制流程";
                this.logRec.oper_desc = "复制流程".concat(String.valueOf(String.valueOf(aFlowInid)));
            }

            try {
                this._$75299.flowInsCopy(aFlowInid, aSubStructXml);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var4.printStackTrace();
                throw new Exception("复制失败，".concat(String.valueOf(String.valueOf(var4.getMessage()))));
            }
        }
    }

    public CachedRowSet getDocCateList(String fram_code) {
        CachedRowSet returnValue = null;
        if(this._$75209) {
            ;
        }

        try {
            returnValue = this._$75314().getDocCateList(fram_code);
        } catch (Exception var4) {
            var4.printStackTrace();
        }

        return returnValue;
    }

    public String getFlowCateCode(String aFlowInidList) {
        String returnValue = "";
        if(this._$75209) {
            ;
        }

        try {
            returnValue = this._$75314().getFlowCateCode(aFlowInidList);
        } catch (Exception var4) {
            var4.printStackTrace();
        }

        return returnValue;
    }

    public void deleteSub(String struct) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in deleteSub(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "deleteSub";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("deleteSub(")).append(struct).append(")")));
            }

            String flowInid = PublicLib.getField(struct, ",", 0L);

            try {
                this._$75299.deleteSub(struct);
                if(this._$75209) {
                    WFOperatorLogRecord e = null;
                    e = this._$75303(5);
                    e.setOperName("删除");
                    e.setOperDesc("删除子流程");
                    e.setLogLevel(4);
                    e.setOperResu("成功");
                    this._$75312(e);
                    this.logRec.attachField1 = flowInid;
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.attachField1 = flowInid;
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var4.printStackTrace();
                throw new Exception("删除子流程实例失败，".concat(String.valueOf(String.valueOf(var4.getMessage()))));
            }
        }
    }

    public String getHaveRightFlows(String right) {
        String returnValue = "";
        if(this._$75299 == null) {
            System.out.println("Error in getHaveRightFlows(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getHaveRightFlows(right);
            } catch (Exception var4) {
                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getDeptInstList(long aParaSeq, long aFlowId, long aType, long aState, String aCondition, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aInstName, String aDocSeq, String aSortField) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getDeptInstList(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getDeptInstList(aParaSeq, aFlowId, aType, aState, aCondition, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aInstName, aDocSeq, aSortField);
            } catch (Exception var20) {
                var20.printStackTrace();
            }

            return returnValue;
        }
    }

    public void callbackAgentTask(String aAgentMan) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in callbackAgentTask(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "callbackAgentTask";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("callbackAgentTask(")).append(aAgentMan).append(")")));
            }

            try {
                this._$75299.callbackAgentTask(aAgentMan);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var3) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var3.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception("接管待办事宜失败，".concat(String.valueOf(String.valueOf(var3.getMessage()))));
            }
        }
    }

    public CachedRowSet getInstInfo(String aFlowInidList) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getInstInfo(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                if(aFlowInidList.indexOf(";") == -1) {
                    returnValue = this._$75299.getInstInfo((long)Integer.parseInt(aFlowInidList));
                } else {
                    StringTokenizer e = new StringTokenizer(aFlowInidList, ";");
                    Vector userList = new Vector();
                    String arch_dept = "";

                    while(true) {
                        Object[] docs;
                        if(e.hasMoreTokens()) {
                            String var16 = e.nextToken();
                            CachedRowSet var15 = null;

                            try {
                                var15 = this._$75299.getInstInfo((long)Integer.parseInt(var16));
                                if(!var15.next()) {
                                    continue;
                                }

                                userList.add(var15.getString("inst_crea"));
                                if(var15.getString("arch_dept") == null || var15.getString("arch_dept").equals("")) {
                                    userList.add(var15.getString("inst_crea"));
                                    continue;
                                }

                                if(arch_dept.equals("")) {
                                    arch_dept = var15.getString("arch_dept");
                                    continue;
                                }

                                if(arch_dept.equals(var15.getString("arch_dept"))) {
                                    continue;
                                }

                                docs = null;
                            } finally {
                                SysPara.closeCachedRowSet(var15);
                            }

                        }

                        boolean isDocEqu = true;
                        Hashtable docList = this._$75314().getDocAppIdOfUsers(userList);
                        docs = docList.values().toArray();

                        for(int i = 1; i < docs.length; ++i) {
                            if(!((String)docs[i]).equals((String)docs[0])) {
                                isDocEqu = false;
                                break;
                            }
                        }

                        if(!arch_dept.equals("") && !arch_dept.equals((String)docs[0])) {
                            isDocEqu = false;
                        }

                        if(isDocEqu) {
                            returnValue = this._$75299.getInstInfo((long)Integer.parseInt(aFlowInidList.substring(0, aFlowInidList.indexOf(";"))));
                        } else {
                            returnValue = null;
                        }
                        break;
                    }
                }
            } catch (Exception var14) {
                var14.printStackTrace();
            }

            return returnValue;
        }
    }

    public boolean isSubflowGenerated(long aFlowInid, long aStepInid, String aDealMan) throws Exception {
        boolean returnValue = false;
        if(this._$75299 == null) {
            System.out.println("Error in isSubflowGenerated(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.isSubflowGenerated(aFlowInid, aStepInid, aDealMan);
                return returnValue;
            } catch (Exception var8) {
                throw new Exception("获取子流程信息失败，".concat(String.valueOf(String.valueOf(var8.getMessage()))));
            }
        }
    }

    public CachedRowSet getUrgeList(String sUserCode) {
        CachedRowSet returnValue = null;

        try {
            returnValue = this._$75299.getUrgeList(sUserCode);
        } catch (Exception var4) {
            var4.printStackTrace();
        }

        return returnValue;
    }

    public CachedRowSet getAllStep(long aFlowInid, long aStepInid, String aCondition) throws Exception {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getAllStep(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getAllStep(aFlowInid, aStepInid, aCondition);
            } catch (Exception var8) {
                var8.printStackTrace();
            }

            return returnValue;
        }
    }

    public String[] getPrgFormList(long aFlowInid, long aStepInid) {
        String[] returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getPrgFormList(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                ;
            }

            try {
                returnValue = this._$75299.getPrgFormList(aFlowInid, aStepInid);
            } catch (Exception var7) {
                var7.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getFlowExchangeInfo(String flowInid) {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlowExchangeInfo(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getFlowExchangeInfo(flowInid);
            } catch (Exception var4) {
                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public Hashtable getUserAwoke(String aUserCode) {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getUserAwoke(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "得到用户的公文提醒设置";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("得到用户")).append(aUserCode).append("的提醒设置")));
            }

            try {
                returnValue = this._$75299.getUserAwoke(aUserCode);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public void saveUserAwoke(String aUserCode, Hashtable aSetting) throws Exception {
        if(this._$75299 == null) {
            System.out.println("Error in saveUserAwoke(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "修改指定用户的公文提醒";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("对指定用户")).append(aUserCode).append("设置公文提醒")));
            }

            try {
                this._$75299.saveUserAwoke(aUserCode, aSetting);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception("保存失败，".concat(String.valueOf(String.valueOf(var4.getMessage()))));
            }
        }
    }

    public Hashtable backForm(long aFlowInid, long aStepInid) throws Exception {
        Hashtable returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in backForm(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "回退";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("回退流程")).append(aFlowInid).append("步骤").append(aStepInid).append("的表单")));
            }

            try {
                returnValue = this._$75299.backForm(aFlowInid, aStepInid);
                if(this._$75209) {
                    WFOperatorLogRecord e = null;
                    e = this._$75303(3);
                    e.setOperName("回退");
                    e.setOperDesc("回退");
                    e.setLogLevel(4);
                    e.setKeyField(String.valueOf(aFlowInid));
                    e.setOperResu("成功");
                    this._$75312(e);
                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.attachField2 = String.valueOf(aStepInid);
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }

                return returnValue;
            } catch (Exception var7) {
                if(this._$75209) {
                    this.logRec.attachField1 = String.valueOf(aFlowInid);
                    this.logRec.attachField2 = String.valueOf(aStepInid);
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var7.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                throw new Exception(var7.getMessage());
            }
        }
    }

    public String getFlowChart(String aFlowInid, int aType) throws Exception {
        String returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlowChart(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "获取流程图";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("获取流程")).append(aFlowInid).append("的图")));
            }

            try {
                returnValue = this._$75299.getFlowChart(aFlowInid, aType);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }

                return returnValue;
            } catch (Exception var5) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
                }

                throw new Exception(var5.getMessage());
            }
        }
    }

    public CachedRowSet getFlowCurrentStep(String aFlowInid) throws Exception {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getFlowCurrentStep(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "获取流程图";
                this.logRec.oper_desc = String.valueOf(String.valueOf((new StringBuffer("获取流程")).append(aFlowInid).append("的图")));
            }

            try {
                returnValue = this._$75299.getFlowCurrentStep(aFlowInid);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                }

                return returnValue;
            } catch (Exception var4) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var4.getMessage()))))));
                }

                throw new Exception(var4.getMessage());
            }
        }
    }

    public static String composeName(long aFlowInid, long aStepInid, String aTag) {
        return String.valueOf(String.valueOf((new StringBuffer("__MinStone_WorkFlow_Tag___")).append(aFlowInid).append("_").append(aStepInid).append("_").append(aTag)));
    }

    public String genWorkFlowTag(long aFlowInid, long aStepInid) {
        return String.valueOf(String.valueOf((new StringBuffer("__MinStone_WorkFlow_Tag___")).append(aFlowInid).append("_").append(aStepInid)));
    }

    public void unloadData(long aFlowInid, long aStepInid) throws Exception {
        this.clearSession(aFlowInid, aStepInid);
    }

    public void clearSession() {
        Enumeration allAttrNames = this.m_session.getAttributeNames();
        String attrName = "";
        Object attrValue = null;
        if(this._$75209 && this._$75210 == 4) {
            System.err.println("===========================clearSession===========================");
        }

        while(true) {
            while(allAttrNames.hasMoreElements()) {
                attrName = (String)allAttrNames.nextElement();
                if(this._$75209 && this._$75210 == 4) {
                    System.err.print("attrName=".concat(String.valueOf(String.valueOf(attrName))));
                }

                if(attrName != null && attrName.startsWith("__MinStone_WorkFlow_Tag__")) {
                    this.clearObject(this.m_session.getAttribute(attrName));
                    this.m_session.removeAttribute(attrName);
                    if(this._$75209 && this._$75210 == 4) {
                        System.err.println(" has Cleared!");
                    }
                } else if(this._$75209 && this._$75210 == 4) {
                    System.err.println(" hold!");
                }
            }

            return;
        }
    }

    public void clearSession(long aFlowInid, long aStepInid) {
        String workFlowTag = this.genWorkFlowTag(aFlowInid, aStepInid);
        this.clearSessionWithTag(workFlowTag);
    }

    public void clearSessionForBatch(String batchStruct) {
        String workFlowTag = this.genWorkFlowTagForBatch(batchStruct);
        this.clearSessionWithTag(workFlowTag);
    }

    public void clearSessionWithTag(String workFlowTag) {
        Enumeration allAttrNames = this.m_session.getAttributeNames();
        String attrName = "";
        Object attrValue = null;
        if(this._$75209 && this._$75210 == 4) {
            System.err.println("===========================clearSessionWithTag===========================");
        }

        while(true) {
            while(allAttrNames.hasMoreElements()) {
                attrName = (String)allAttrNames.nextElement();
                if(this._$75209 && this._$75210 == 4) {
                    System.err.print("attrName=".concat(String.valueOf(String.valueOf(attrName))));
                }

                if(attrName != null && attrName.startsWith(workFlowTag)) {
                    this.clearObject(this.m_session.getAttribute(attrName));
                    this.m_session.removeAttribute(attrName);
                    if(this._$75209 && this._$75210 == 4) {
                        System.err.println(" has Cleared!");
                    }
                } else if(this._$75209 && this._$75210 == 4) {
                    System.err.println(" hold!");
                }
            }

            return;
        }
    }

    public void clearObject(Object obj) {
        if(obj != null) {
            try {
                CachedRowSet exx;
                if(obj instanceof CachedRowSet) {
                    exx = (CachedRowSet)obj;

                    try {
                        if(exx != null) {
                            exx.close();
                        }
                    } catch (Exception var30) {
                        if(this._$75209 && this._$75210 == 4) {
                            System.err.println(String.valueOf(String.valueOf((new StringBuffer("Clear CachedRowSet[")).append(obj).append("] Error!"))));
                            var30.printStackTrace();
                        }
                    } finally {
                        exx = null;
                    }
                } else if(obj instanceof RecordSet) {
                    RecordSet exx1 = (RecordSet)obj;

                    try {
                        if(exx1 != null) {
                            exx1.deleteAllRecord();
                        }
                    } catch (Exception var28) {
                        if(this._$75209 && this._$75210 == 4) {
                            System.err.println(String.valueOf(String.valueOf((new StringBuffer("Clear RecordSet[")).append(obj).append("] Error!"))));
                            var28.printStackTrace();
                        }
                    } finally {
                        exx = null;
                    }
                } else {
                    Iterator it;
                    if(obj instanceof Map) {
                        Map exx2 = (Map)obj;
                        it = exx2.keySet().iterator();

                        while(it.hasNext()) {
                            this.clearObject(exx2.get(it.next()));
                        }

                        exx2.clear();
                    } else if(obj instanceof List) {
                        List exx3 = (List)obj;
                        it = exx3.iterator();

                        while(it.hasNext()) {
                            this.clearObject(it.next());
                        }

                        exx3.clear();
                    }
                }
            } catch (Exception var32) {
                var32.printStackTrace();
            } finally {
                obj = null;
            }

        }
    }

    public String loadData(long aFlowInid, long aStepInid, long dealIndx) throws Exception {
        return this.loadData(aFlowInid, aStepInid, dealIndx, new HashMap());
    }

    public String loadData(long aFlowInid, long aStepInid, long dealIndx, Map initData) throws Exception {
        this.unloadData(aFlowInid, aStepInid);
        Hashtable returnStruct = null;
        String formHtml;
        if(initData != null) {
            formHtml = this.getCurrentDeptCode();
            if(formHtml != null) {
                initData.put("currentUserDeptCode", formHtml);
            }
        }

        if(aStepInid < (long)0) {
            returnStruct = this.getStartStepHtml(aFlowInid, initData);
        } else if(aStepInid == (long)0) {
            returnStruct = this.getStepInsHtml(aFlowInid);
        } else {
            returnStruct = this.getStepInsHtml(aFlowInid, aStepInid, dealIndx, initData);
        }

        formHtml = (String)returnStruct.get("formHtml");
        FormStruct fs = (FormStruct)returnStruct.get("___0-3__48_f_uct_FormStruct___");
        FormInstData fid = (FormInstData)returnStruct.get("0_9UAad_form_iNst_Data__");
        CachedRowSet mainRs = (CachedRowSet)returnStruct.get("mainRs");
        RecordSet fieldMapNameRs = (RecordSet)returnStruct.get("fieldMapNameRs");
        RecordSet hypoMapRs = (RecordSet)returnStruct.get("hypoMapRs");
        Hashtable hypoRsColl = (Hashtable)returnStruct.get("hypoRsColl");
        Hashtable relaMapRsColl = (Hashtable)returnStruct.get("__RE__laMa__pRs");
        Hashtable relaRsColl = (Hashtable)returnStruct.get("Re_la_Rs_CO_lL_");
        Hashtable relaHtmlColl = (Hashtable)returnStruct.get("Re_la_Html_CO_lL_");
        RecordSet someInfoRs = (RecordSet)returnStruct.get("someInfoRs");
        CachedRowSet attachRs = (CachedRowSet)returnStruct.get("attachRs");
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "formHtml"), formHtml);
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "___0-3__48_f_uct_FormStruct___"), fs);
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "0_9UAad_form_iNst_Data__"), fid);
        if(mainRs == null) {
            System.out.println("loadData() mainRs is null");
        }

        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "mainRs"), mainRs);
        if(someInfoRs == null) {
            System.out.println("loadData() someInfoRs is null");
        }

        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "someInfoRs"), someInfoRs);
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "fieldMapNameRs"), fieldMapNameRs);
        if(hypoMapRs != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "hypoMapRs"), hypoMapRs);
        }

        if(hypoRsColl != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "hypoRsColl"), hypoRsColl);
        }

        if(relaMapRsColl != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "__RE__laMa__pRs"), relaMapRsColl);
        }

        if(relaRsColl != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "Re_la_Rs_CO_lL_"), relaRsColl);
        }

        if(relaHtmlColl != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "Re_la_Html_CO_lL_"), relaHtmlColl);
        }

        if(attachRs != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "attachRs"), attachRs);
        }

        this._$75410((Map)someInfoRs.getObject("__At__tac__hR__ight__Rs"), aFlowInid, aStepInid);
        returnStruct.clear();
        return formHtml;
    }

    public boolean saveData(CachedRowSet aMainRs, RecordSet aFieldMapNameRs, boolean aUpdateNull) throws Exception {
        boolean isModified = false;

        try {
            Hashtable e = new Hashtable();
            aFieldMapNameRs.beforeFirst();

            while(true) {
                long dbType;
                String fieldName;
                int fieldType;
                Object var23;
                do {
                    do {
                        do {
                            while(true) {
                                int flag;
                                do {
                                    if(!aFieldMapNameRs.next()) {
                                        if(isModified) {
                                            aMainRs.updateRow();
                                        }

                                        return isModified;
                                    }

                                    flag = Integer.parseInt((String)aFieldMapNameRs.getObject("fld_flag"));
                                } while(flag == 0);

                                String controlName = (String)aFieldMapNameRs.getObject("ctl_name");
                                long controlType = (long)Integer.parseInt((String)aFieldMapNameRs.getObject("ctl_type"));
                                dbType = (long)Integer.parseInt((String)aFieldMapNameRs.getObject("db_type"));
                                fieldName = (String)aFieldMapNameRs.getObject("fld_name");
                                if(e.get(fieldName) != null) {
                                    System.out.println("表单中有多个控件绑定同一个字段".concat(String.valueOf(String.valueOf(fieldName))));
                                }

                                String fieldValue;
                                fieldType = Integer.parseInt((String)aFieldMapNameRs.getObject("fld_type"));
                                fieldValue = "";
                                boolean isDate = fieldType == 4 || fieldType == 5 || fieldType == 6;
                                label100:
                                switch((int)controlType) {
                                    case 1:
                                        if(isDate) {
                                            fieldValue = this.m_request.getParameter(String.valueOf(String.valueOf(controlName)).concat("___hidden___value"));
                                        } else {
                                            fieldValue = this.m_request.getParameter(controlName);
                                        }
                                        break;
                                    case 4:
                                        String[] tempFieldValue = this.m_request.getParameterValues(controlName);
                                        if(tempFieldValue == null) {
                                            break;
                                        }

                                        int k = 0;

                                        while(true) {
                                            if(k >= tempFieldValue.length) {
                                                break label100;
                                            }

                                            if(!fieldValue.equals("")) {
                                                fieldValue = String.valueOf(String.valueOf(fieldValue)).concat(";");
                                            }

                                            fieldValue = String.valueOf(String.valueOf(fieldValue)).concat(String.valueOf(String.valueOf(tempFieldValue[k])));
                                            ++k;
                                        }
                                    default:
                                        fieldValue = this.m_request.getParameter(controlName);
                                }

                                if(fieldValue == null) {
                                    fieldValue = "";
                                }

                                if(fieldValue.equals("")) {
                                    break;
                                }

                                String var21;
                                Date var25;
                                switch(fieldType) {
                                    case 1:
                                    case 2:
                                        var21 = aMainRs.getString(fieldName);
                                        if(!fieldValue.equals(var21)) {
                                            aMainRs.updateString(fieldName, fieldValue);
                                            isModified = true;
                                        }
                                        break;
                                    case 3:
                                        if(aMainRs.getObject(fieldName) != null) {
                                            var21 = aMainRs.getObject(fieldName).toString();
                                        } else {
                                            var21 = null;
                                        }

                                        fieldValue = fieldValue.trim();
                                        if(!fieldValue.equals(var21) && !fieldValue.equals("")) {
                                            fieldValue = fieldValue.trim();
                                            aMainRs.updateBigDecimal(fieldName, PublicLib.str2BigDecimal(fieldValue));
                                            isModified = true;
                                        }
                                        break;
                                    case 4:
                                    case 5:
                                    case 6:
                                        fieldValue = fieldValue.trim();
                                        if(fieldValue.equals("")) {
                                            break;
                                        }

                                        if(fieldValue.indexOf(":") != -1) {
                                            Timestamp var24 = aMainRs.getTimestamp(fieldName);
                                            if(var24 == null || var24.compareTo(Timestamp.valueOf(fieldValue)) != 0) {
                                                aMainRs.updateTimestamp(fieldName, Timestamp.valueOf(fieldValue));
                                            }

                                            isModified = true;
                                            break;
                                        }

                                        var25 = aMainRs.getDate(fieldName);
                                        if(var25 == null || var25.compareTo(Date.valueOf(fieldValue)) != 0) {
                                            aMainRs.updateDate(fieldName, Date.valueOf(fieldValue));
                                        }

                                        isModified = true;
                                        break;
                                    case 7:
                                        byte[] tmpDate = fieldValue.getBytes();
                                        aMainRs.updateBytes(fieldName, tmpDate);
                                        var25 = null;
                                        isModified = true;
                                        break;
                                    case 8:
                                        byte[] cBuf = fieldValue.getBytes();
                                        aMainRs.updateBytes(fieldName, cBuf);
                                        Object var22 = null;
                                        isModified = true;
                                }
                            }
                        } while(!aUpdateNull);

                        var23 = aMainRs.getObject(fieldName);
                    } while(var23 == null);
                } while(var23.toString().equals(""));

                if(dbType != (long)1 || fieldType != 4 && fieldType != 5 && fieldType != 6) {
                    aMainRs.updateNull(fieldName);
                } else {
                    aMainRs.updateString(fieldName, "");
                }

                isModified = true;
            }
        } catch (Exception var20) {
            var20.printStackTrace();
            throw var20;
        }
    }

    public boolean save(long aFlowInid, long aStepInid) throws Exception {
        CachedRowSet mainRs = (CachedRowSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "mainRs"));
        RecordSet fieldMapNameRs = (RecordSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "fieldMapNameRs"));
        RecordSet someInfoRs = (RecordSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "someInfoRs"));
        boolean isModified = this.saveData(mainRs, fieldMapNameRs, true);
        String[][] someInfo = new String[][]{{"sys_step_choice", "step_choi"}, {"sys_step_append_hidden", "curr_man"}, {"sys_step_notice_hidden", "note_man"}, {"sys_step_next_hidden", "next_man"}, {"sys_flow_sign", "deal_sign"}, {"sys_flow_comm", "deal_comm"}, {"sys_flow_comm_date", "allograph_date"}, {"sys_flow_note", "deal_note"}, {"sys_clear_status", "clear_status"}, {"hiddenUrge", "time_set"}, {"totalTerm", "total_term"}};
        String chkForceSendSms = this.m_request.getParameter("__chkForceSendSms__");
        if("yes".equalsIgnoreCase(chkForceSendSms)) {
            someInfoRs.putObject("__FORCE__send__SHORT_messAgE__", "");
        }

        for(int appendIntergration = 0; appendIntergration < someInfo.length; ++appendIntergration) {
            String attachAuth = this.m_request.getParameter(someInfo[appendIntergration][0]);
            boolean signDealInfo = false;
            boolean bIsSignDealInfo = someInfo[appendIntergration][0].equals("sys_flow_comm");
            if(bIsSignDealInfo && attachAuth != null) {
                byte var17 = 1;
                someInfoRs.putObject("__siGn_dEAl__infO_FlaG__", String.valueOf(String.valueOf(var17)).concat(""));
                isModified = true;
            }

            if(attachAuth == null) {
                attachAuth = "";
            }

            if(bIsSignDealInfo && attachAuth.equals("")) {
                String defaultComm = (String)someInfoRs.getObject("default_comm");
                someInfoRs.putObject(someInfo[appendIntergration][1], defaultComm);
            } else {
                someInfoRs.putObject(someInfo[appendIntergration][1], attachAuth);
            }
        }

        String var16 = this.m_request.getParameter("sys_flow_comm__integration__");
        if(var16 == null) {
            var16 = "";
        }

        if(var16.trim().equals("")) {
            var16 = "0";
        }

        someInfoRs.putObject("sys_flow_comm__integration__", var16);
        Map var18 = this._$75421(aFlowInid, aStepInid);
        if(!var18.isEmpty()) {
            someInfoRs.putObject("__At__tac__hR__ight__Rs", var18);
            isModified = true;
        }

        return isModified;
    }

    public boolean saveTabPageForm(long aFlowInid, long aStepInid, String tabPageIndex) throws Exception {
        Hashtable relaMapRsColl = (Hashtable)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "__RE__laMa__pRs"));
        Hashtable relaRsColl = (Hashtable)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "Re_la_Rs_CO_lL_"));
        CachedRowSet tabFormMainRs = (CachedRowSet)relaRsColl.get(tabPageIndex);
        RecordSet tabFormFieldMapNameRs = (RecordSet)relaMapRsColl.get(tabPageIndex);
        boolean isModified = this.saveData(tabFormMainRs, tabFormFieldMapNameRs, true);
        return isModified;
    }

    private void _$75410(Map mapAuths, long aFlowInid, long aStepInid) {
        if(mapAuths != null && mapAuths.size() > 0) {
            Iterator it = mapAuths.keySet().iterator();

            while(true) {
                AttachRight ar;
                AttachRightItem[] items;
                do {
                    do {
                        if(!it.hasNext()) {
                            return;
                        }

                        Object key = it.next();
                        ar = (AttachRight)mapAuths.get(key);
                        items = ar.getRightItems();
                    } while(items == null);
                } while(items.length <= 0);

                String fileName = ar.getFileName();
                String tag = composeName(aFlowInid, aStepInid, fileName);
                String userCodes = "";
                String userNames = "";

                for(int aui = 0; aui < items.length; ++aui) {
                    if(userCodes != "") {
                        userCodes = String.valueOf(String.valueOf(userCodes)).concat(";");
                        userNames = String.valueOf(String.valueOf(userNames)).concat("、");
                    }

                    userCodes = String.valueOf(String.valueOf(userCodes)).concat(String.valueOf(String.valueOf(items[aui].getUserCode())));
                    userNames = String.valueOf(String.valueOf(userNames)).concat(String.valueOf(String.valueOf(items[aui].getUserName())));
                }

                AttachAuthItem var15 = new AttachAuthItem(userCodes, userNames, "");
                this.m_session.setAttribute(tag, var15);
            }
        }
    }

    private Map _$75421(long aFlowInid, long aStepInid) {
        Hashtable mapRet = new Hashtable();
        CachedRowSet attachRs = (CachedRowSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "attachRs"));
        if(attachRs != null) {
            try {
                attachRs.beforeFirst();

                while(attachRs.next()) {
                    String ex = attachRs.getString("file_name");
                    String sAttachAuthTag = composeName(aFlowInid, aStepInid, ex);
                    AttachAuthItem aui = null;
                    if(sAttachAuthTag != null) {
                        aui = (AttachAuthItem)this.m_session.getAttribute(sAttachAuthTag);
                    }

                    if(aui != null && aui.HasUpdate()) {
                        AttachRightItem[] items = aui.toAttachRightItems();
                        if(items != null && items.length > 0) {
                            AttachRight ar = new AttachRight();
                            ar.setFlowInid("".concat(String.valueOf(String.valueOf(aFlowInid))));
                            ar.setStepInid("".concat(String.valueOf(String.valueOf(aStepInid))));
                            ar.setFileName(ex);
                            ar.addRightItems(items);
                            ar.setHasUpdate(true);
                            mapRet.put(ar.getKey(), ar);
                        }
                    }
                }
            } catch (Exception var12) {
                System.err.println("处理附件权限错误");
                var12.printStackTrace();
            }
        }

        return mapRet;
    }

    public void saveSelect(long aFlowInid, long aStepInid) throws Exception {
        this.saveSelect(aFlowInid, aStepInid, false);
    }

    public void saveSelect(long aFlowInid, long aStepInid, boolean ignoreNull) throws Exception {
        RecordSet someInfoRs = (RecordSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "someInfoRs"));
        String[][] someInfo = new String[][]{{"sys_flow_assure_step", "assu_step"}, {"sys_flow_assure_man", "assu_man"}, {"openTerm", "open_term"}, {"dealTerm", "deal_term"}, {"sys_step_choice", "step_choi"}};

        for(int i = 0; i < someInfo.length; ++i) {
            String fieldValue = this.m_request.getParameter(someInfo[i][0]);
            if(fieldValue != null || !ignoreNull) {
                if(fieldValue == null) {
                    fieldValue = "";
                }

                someInfoRs.putObject(someInfo[i][1], fieldValue);
            }
        }

    }

    public Hashtable finishMainForm(long aFlowInid, long aStepInid, long aSaveFlag, boolean aIsModified) throws Exception {
        CachedRowSet mainRs = (CachedRowSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "mainRs"));
        RecordSet fieldMapNameRs = (RecordSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "fieldMapNameRs"));
        RecordSet someInfoRs = (RecordSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "someInfoRs"));
        RecordSet hypoMapRs = (RecordSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "hypoMapRs"));
        Hashtable hypoRsColl = (Hashtable)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "hypoRsColl"));
        CachedRowSet attachRs = (CachedRowSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "attachRs"));
        byte[] fileBuf = (byte[])this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "fileBuf"));
        Hashtable relaRsColl = (Hashtable)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "Re_la_Rs_CO_lL_"));
        if(relaRsColl != null) {
            someInfoRs.putObject("Re_la_Rs_CO_lL_", relaRsColl);
            someInfoRs.putObject("currentUserDeptCode", this.getCurrentDeptCode());
        }

        Hashtable returnStruct = null;
        boolean onlySave = false;
        if(aSaveFlag == (long)1) {
            onlySave = true;
        }

        if(aStepInid < (long)0) {
            returnStruct = this.flowInsNew(aFlowInid, onlySave, someInfoRs, mainRs, hypoRsColl, hypoMapRs, attachRs, fileBuf);
        } else if(aStepInid > (long)0) {
            returnStruct = this.flowInsSend(aFlowInid, aStepInid, onlySave, aIsModified, someInfoRs, mainRs, hypoRsColl, hypoMapRs, attachRs, fileBuf);
        } else {
            returnStruct = this.flowInsSend(aFlowInid, onlySave, aIsModified, someInfoRs, mainRs, hypoRsColl, hypoMapRs, attachRs, fileBuf);
        }

        return returnStruct;
    }

    public String showSubForm(boolean aNewFlag, long aFlowInid, long aStepInid, boolean aReadOnly, long aListFormId, long aFreeFormId, long aFormIndex, int aRecordIndex) throws Exception {
        CachedRowSet mainRs = (CachedRowSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "mainRs"));
        RecordSet hypoMapRs = (RecordSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "hypoMapRs"));
        Hashtable hypoRsColl = (Hashtable)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "hypoRsColl"));
        String strFormIndex = String.valueOf(aFormIndex);
        CachedRowSet hypoRs = (CachedRowSet)hypoRsColl.get(strFormIndex);
        hypoMapRs.move((int)aFormIndex);
        long fsetId = (long)Integer.parseInt(hypoMapRs.getObject("fset_id").toString());
        Hashtable returnStruct = this.getFreeFormHtml(aFlowInid, aStepInid, aReadOnly, aNewFlag, fsetId, aListFormId, aFreeFormId, aRecordIndex, mainRs, hypoRs);
        RecordSet fieldMapNameRs = (RecordSet)returnStruct.get("fieldMapNameRs");
        String formHtml = (String)returnStruct.get("formHtml");
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "fieldMapNameRs_".concat(String.valueOf(String.valueOf(aFormIndex)))), fieldMapNameRs);
        return formHtml;
    }

    public String initFileClient(long aFlowInid, long aStepInid, RecordSet aSomeInfoRs) throws Exception {
        long ifDoc = 0L;
        if(aSomeInfoRs.getObject("doc_flag") != null) {
            ifDoc = (long)Integer.parseInt(aSomeInfoRs.getObject("doc_flag").toString());
        }

        String fileHtml = "<script language=javascript>function window_onload(){";
        if(ifDoc == (long)1) {
            String docName = (String)aSomeInfoRs.getObject("doc_name");
            String userName = (String)aSomeInfoRs.getObject("user_name");
            String webServer = "";
            if(webServer == null || webServer.equals("")) {
                webServer = "WEBLOGIC";
            }

            String tmpBookmark = BaseServerClient.getTagValue("WF/BOOKMARK").trim();
            if(tmpBookmark == null || tmpBookmark.equals("")) {
                tmpBookmark = "0";
            }

            long bookmark = (long)Integer.parseInt(tmpBookmark);
            String tmpProtect = BaseServerClient.getTagValue("WF/PROTECT").trim();
            if(tmpProtect == null || tmpProtect.equals("")) {
                tmpProtect = "0";
            }

            long protect = (long)Integer.parseInt(tmpProtect);
            long popedom = 17L;
            if(aSomeInfoRs.getObject("popedom") != null) {
                popedom = (long)Integer.parseInt(aSomeInfoRs.getObject("popedom").toString());
            }

            long stepType = (long)Integer.parseInt(aSomeInfoRs.getObject("step_type").toString());
            long readOnly = (long)Integer.parseInt(aSomeInfoRs.getObject("readOnly").toString());
            if(readOnly == (long)1) {
                popedom = 17L;
            }

            long clearStatus = 0L;
            if(aSomeInfoRs.getObject("clear_status") != null && !aSomeInfoRs.getObject("clear_status").equals("")) {
                clearStatus = (long)Integer.parseInt(aSomeInfoRs.getObject("clear_status").toString());
            }

            String miscStructXml = (String)aSomeInfoRs.getObject("misc_set");
            MiscStruct miscStruct = new MiscStruct();
            miscStruct.restore(miscStructXml);
            String modifyDocStructXml = "";
            if(miscStruct.getModifyDoc() == 1) {
                modifyDocStructXml = (String)aSomeInfoRs.getObject("modifyDocStructXml");
            }

            long autoClear = (long)miscStruct.getAutoClear();
            String fileList = "";
            CachedRowSet attachRs = (CachedRowSet)this.m_session.getAttribute(composeName(aFlowInid, aStepInid, "attachRs"));
            attachRs.beforeFirst();

            while(attachRs.next()) {
                String fileName = attachRs.getString("FILE_NAME");
                if(!fileName.equalsIgnoreCase(docName)) {
                    if(!fileList.equalsIgnoreCase("")) {
                        fileList = String.valueOf(String.valueOf(fileList)).concat(",");
                    }

                    fileList = String.valueOf(String.valueOf(fileList)).concat(String.valueOf(String.valueOf(fileName)));
                }
            }

            fileHtml = String.valueOf(String.valueOf(fileHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("initFileClient(\'")).append(docName).append("\'").append(",\'").append(userName).append("\'").append(",\'").append(fileList).append("\'").append(",").append(popedom).append(",").append(stepType).append(",\'").append(modifyDocStructXml).append("\'").append(",").append(clearStatus).append(",\'").append(webServer).append("\'").append(",").append(bookmark).append(",").append(autoClear).append(",").append(protect).append(",").append(miscStruct.isRealAcceptRevisions()).append(");"))))));
        }

        fileHtml = String.valueOf(String.valueOf(fileHtml)).concat("}</script>");
        return fileHtml;
    }

    public String getMessage(String aMessage) {
        String message = aMessage;
        if(aMessage != null && aMessage.indexOf("java.lang.Exception") != -1) {
            int pos = aMessage.indexOf("java.lang.Exception");
            pos = aMessage.indexOf(":", pos);
            message = aMessage.substring(pos + 2);
            pos = message.indexOf(13);
            if(pos != -1) {
                message = message.substring(0, pos);
            }

            pos = message.indexOf(10);
            if(pos != -1) {
                message = message.substring(0, pos);
            }
        }

        if(message != null) {
            message = PublicLib.findReplace(message, "\n", "<br>");
            message = PublicLib.findReplace(message, "\r", "<br>");
        }

        return message;
    }

    public String batchComposeName(String aStruct, String aTag) {
        return String.valueOf(String.valueOf((new StringBuffer("__MinStone_WorkFlow_Tag___")).append(aStruct).append("_").append(aTag)));
    }

    public String genWorkFlowTagForBatch(String aStruct) {
        return String.valueOf(String.valueOf((new StringBuffer("__MinStone_WorkFlow_Tag___")).append(aStruct)));
    }

    public boolean batchSave(String aStructs, long aFlowInid, long aStepInid) throws Exception {
        Hashtable mainRsColl = (Hashtable)this.m_session.getAttribute(this.batchComposeName(aStructs, "mainRsColl"));
        CachedRowSet mainRs = (CachedRowSet)mainRsColl.get(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aFlowInid)))).append("_").append(aStepInid))));
        RecordSet fieldMapNameRs = (RecordSet)this.m_session.getAttribute(this.batchComposeName(aStructs, "fieldMapNameRs"));
        RecordSet someInfoRs = (RecordSet)this.m_session.getAttribute(this.batchComposeName(aStructs, "someInfoRs"));
        boolean isModified = this.saveData(mainRs, fieldMapNameRs, false);
        String[][] someInfo = new String[][]{{"sys_step_choice", "step_choi"}, {"sys_step_append_hidden", "curr_man"}, {"sys_step_notice_hidden", "note_man"}, {"sys_step_next_hidden", "next_man"}, {"sys_flow_sign", "deal_sign"}, {"sys_flow_comm", "deal_comm"}, {"sys_flow_note", "deal_note"}, {"sys_clear_status", "clear_status"}, {"hiddenUrge", "time_set"}};

        for(int appendIntergration = 0; appendIntergration < someInfo.length; ++appendIntergration) {
            String fieldValue = this.m_request.getParameter(someInfo[appendIntergration][0]);
            boolean signDealInfo = false;
            boolean bIsSignDealInfo = someInfo[appendIntergration][0].equals("sys_flow_comm");
            if(bIsSignDealInfo && fieldValue != null) {
                byte var17 = 1;
                someInfoRs.putObject("__siGn_dEAl__infO_FlaG__", String.valueOf(String.valueOf(var17)).concat(""));
                isModified = true;
            }

            if(fieldValue == null) {
                fieldValue = "";
            }

            if(bIsSignDealInfo && fieldValue.equals("")) {
                fieldValue = (String)someInfoRs.getObject("default_comm");
                isModified = true;
            }

            if(fieldValue != null && !fieldValue.equals("")) {
                someInfoRs.putObject(someInfo[appendIntergration][1], fieldValue);
            }
        }

        someInfoRs.putObject("batch_mode", "1");
        String var16 = this.m_request.getParameter("sys_flow_comm__integration__");
        if(var16 == null) {
            var16 = "";
        }

        if(var16.trim().equals("")) {
            var16 = "0";
        }

        someInfoRs.putObject("sys_flow_comm__integration__", var16);
        return isModified;
    }

    public Hashtable batchFinishMainForm(String aStructs, long aFlowInid, long aStepInid, long aSaveFlag, boolean aIsModified, long dealIndx) throws Exception {
        Hashtable mainRsColl = (Hashtable)this.m_session.getAttribute(this.batchComposeName(aStructs, "mainRsColl"));
        CachedRowSet mainRs = (CachedRowSet)mainRsColl.get(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aFlowInid)))).append("_").append(aStepInid))));
        RecordSet fieldMapNameRs = (RecordSet)this.m_session.getAttribute(this.batchComposeName(aStructs, "fieldMapNameRs"));
        RecordSet someInfoRs = (RecordSet)this.m_session.getAttribute(this.batchComposeName(aStructs, "someInfoRs"));
        CachedRowSet attachRs = (CachedRowSet)this.m_session.getAttribute(this.batchComposeName(aStructs, "attachRs"));
        byte[] fileBuf = (byte[])this.m_session.getAttribute(this.batchComposeName(aStructs, "fileBuf"));
        Hashtable returnStruct = null;
        boolean onlySave = false;
        if(aSaveFlag == (long)1) {
            onlySave = true;
        }

        someInfoRs.putObject("batch_mode", "1");
        someInfoRs.putObject("deal_indx", String.valueOf(String.valueOf(dealIndx)).concat(""));
        Hashtable hypoRsColl = new Hashtable();
        RecordSet hypoMapRs = new RecordSet();
        returnStruct = this.flowInsSend(aFlowInid, aStepInid, onlySave, aIsModified, someInfoRs, mainRs, hypoRsColl, hypoMapRs, attachRs, fileBuf);
        return returnStruct;
    }

    public boolean drawBackNoticeTask(String noticeTaskList) {
        boolean bRet = false;
        if(this._$75299 == null) {
            System.out.println("Error in drawBackNoticeTask(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "收回知会";
                this.logRec.oper_desc = "收回知会：".concat(String.valueOf(String.valueOf(noticeTaskList)));
            }

            try {
                bRet = this._$75299.drawBackNoticeTask(noticeTaskList);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }
            } catch (Exception var8) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var8.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var8.printStackTrace();
            } finally {
            }

            return bRet;
        }
    }

    public boolean batchReadNotice(String noticeTaskList) {
        boolean bRet = false;
        if(this._$75299 == null) {
            System.out.println("Error in batchReadNotice(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return false;
        } else {
            if(this._$75209) {
                this.logRec.oper_name = "知会批量阅读";
                this.logRec.oper_desc = "知会批量阅读：".concat(String.valueOf(String.valueOf(noticeTaskList)));
            }

            try {
                bRet = this._$75299.batchReadNotice(noticeTaskList);
                if(this._$75209) {
                    this.logRec.log_level = 4;
                    this.logRec.oper_resu = "成功";
                    this.logInterface.log(this.logRec);
                }
            } catch (Exception var8) {
                if(this._$75209) {
                    this.logRec.log_level = 2;
                    this.logRec.oper_resu = "失败";
                    this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var8.getMessage()))))));
                    this.logInterface.log(this.logRec);
                }

                var8.printStackTrace();
            } finally {
            }

            return bRet;
        }
    }

    public String batchInitFileClient(String aStructs, RecordSet aSomeInfoRs) throws Exception {
        long ifDoc = 0L;
        if(aSomeInfoRs.getObject("doc_flag") != null) {
            ifDoc = (long)Integer.parseInt(aSomeInfoRs.getObject("doc_flag").toString());
        }

        String fileHtml = "<script language=javascript>function window_onload(){";
        if(ifDoc == (long)1) {
            String docName = (String)aSomeInfoRs.getObject("doc_name");
            long popedom = 17L;
            if(aSomeInfoRs.getObject("popedom") != null) {
                popedom = (long)Integer.parseInt(aSomeInfoRs.getObject("popedom").toString());
            }

            long stepType = (long)Integer.parseInt(aSomeInfoRs.getObject("step_type").toString());
            String miscStructXml = (String)aSomeInfoRs.getObject("misc_set");
            MiscStruct miscStruct = new MiscStruct();
            miscStruct.restore(miscStructXml);
            String modifyDocStructXml = "";
            if(miscStruct.getModifyDoc() == 1) {
                modifyDocStructXml = (String)aSomeInfoRs.getObject("modifyDocStructXml");
            }

            fileHtml = String.valueOf(String.valueOf(fileHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("initFileClient(\'")).append(docName).append("\'").append(",").append(popedom).append(",").append(stepType).append(",\'").append(modifyDocStructXml).append("\'").append(");"))))));
        }

        fileHtml = String.valueOf(String.valueOf(fileHtml)).concat("}</script>");
        return fileHtml;
    }

    public String batchLoadData(String aStructs) throws Exception {
        this.batchUnloadData(aStructs);
        Hashtable returnStruct = null;
        returnStruct = this.getBatchStepInsHtml(aStructs);
        String formHtml = (String)returnStruct.get("formHtml");
        FormStruct fs = (FormStruct)returnStruct.get("___0-3__48_f_uct_FormStruct___");
        Hashtable mainRsColl = (Hashtable)returnStruct.get("mainRsColl");
        RecordSet fieldMapNameRs = (RecordSet)returnStruct.get("fieldMapNameRs");
        RecordSet someInfoRs = (RecordSet)returnStruct.get("someInfoRs");
        CachedRowSet attachRs = (CachedRowSet)returnStruct.get("attachRs");
        this.m_session.setAttribute(this.batchComposeName(aStructs, "formHtml"), formHtml);
        this.m_session.setAttribute(this.batchComposeName(aStructs, "___0-3__48_f_uct_FormStruct___"), fs);
        this.m_session.setAttribute(this.batchComposeName(aStructs, "mainRsColl"), mainRsColl);
        this.m_session.setAttribute(this.batchComposeName(aStructs, "fieldMapNameRs"), fieldMapNameRs);
        this.m_session.setAttribute(this.batchComposeName(aStructs, "someInfoRs"), someInfoRs);
        if(attachRs != null) {
            this.m_session.setAttribute(this.batchComposeName(aStructs, "attachRs"), attachRs);
        }

        returnStruct.clear();
        return formHtml;
    }

    public void batchUnloadData(String aStructs) throws Exception {
        this.clearSessionForBatch(aStructs);
    }

    public void batchSaveSelect(String aStructs) throws Exception {
        RecordSet someInfoRs = (RecordSet)this.m_session.getAttribute(this.batchComposeName(aStructs, "someInfoRs"));
        String[][] someInfo = new String[][]{{"sys_flow_assure_step", "assu_step"}, {"sys_flow_assure_man", "assu_man"}};

        for(int i = 0; i < someInfo.length; ++i) {
            String fieldValue = this.m_request.getParameter(someInfo[i][0]);
            if(fieldValue == null) {
                fieldValue = "";
            }

            someInfoRs.putObject(someInfo[i][1], fieldValue);
        }

    }

    public boolean setSelfScore(String score, long flowInid, long stepInid, String dealMan) throws Exception {
        boolean returnValue = false;
        if(this._$75209) {
            this.logRec.oper_name = "设置自我评价";
            this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("设置自我评价:(")).append(score).append(",").append(flowInid).append(",").append(stepInid).append(",").append(dealMan).append(")"))))));
        }

        try {
            returnValue = this._$75299.setSelfScore(score, flowInid, stepInid, dealMan);
            if(this._$75209) {
                this.logRec.log_level = 4;
                this.logRec.oper_resu = "成功";
                this.logInterface.log(this.logRec);
            }

            return returnValue;
        } catch (Exception var9) {
            if(this._$75209) {
                this.logRec.log_level = 2;
                this.logRec.oper_resu = "失败";
                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var9.getMessage()))))));
                this.logInterface.log(this.logRec);
            }

            throw new Exception("设置自我评价".concat(String.valueOf(String.valueOf(var9.getMessage()))));
        }
    }

    public boolean setOtherScore(String score, long flowInid, long stepInid, String dealMan) throws Exception {
        boolean returnValue = false;
        if(this._$75209) {
            this.logRec.oper_name = "设置评价";
            this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("设置评价:(")).append(score).append(",").append(flowInid).append(",").append(stepInid).append(",").append(dealMan).append(")"))))));
        }

        try {
            returnValue = this._$75299.setOtherScore(score, flowInid, stepInid, dealMan);
            if(this._$75209) {
                this.logRec.log_level = 4;
                this.logRec.oper_resu = "成功";
                this.logInterface.log(this.logRec);
            }

            return returnValue;
        } catch (Exception var9) {
            if(this._$75209) {
                this.logRec.log_level = 2;
                this.logRec.oper_resu = "失败";
                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var9.getMessage()))))));
                this.logInterface.log(this.logRec);
            }

            throw new Exception("设置评价".concat(String.valueOf(String.valueOf(var9.getMessage()))));
        }
    }

    public CachedRowSet getTraceAttachInfo(long flowInid, long stepInid, String dealMan) throws Exception {
        CachedRowSet returnValue = null;
        if(this._$75209) {
            this.logRec.oper_name = "返回痕迹附加信息";
            this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("返回痕迹附加信息:(")).append(flowInid).append(",").append(stepInid).append(",").append(dealMan).append(")"))))));
        }

        try {
            returnValue = this._$75299.getTraceAttachInfo(flowInid, stepInid, dealMan);
            if(this._$75209) {
                this.logRec.log_level = 4;
                this.logRec.oper_resu = "成功";
            }

            return returnValue;
        } catch (Exception var8) {
            if(this._$75209) {
                this.logRec.log_level = 2;
                this.logRec.oper_resu = "失败";
                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var8.getMessage()))))));
            }

            throw new Exception("返回痕迹附加信息".concat(String.valueOf(String.valueOf(var8.getMessage()))));
        }
    }

    public boolean isOpenOverTime(CachedRowSet traceAttachInfo) {
        boolean isOpenOverTime = false;

        try {
            traceAttachInfo.first();
            Timestamp ex = traceAttachInfo.getTimestamp("rece_date");
            if(ex == null) {
                boolean open_term1 = false;
                return open_term1;
            } else {
                long open_term = traceAttachInfo.getLong("open_term");
                if(open_term <= (long)0) {
                    boolean termMillis1 = false;
                    return termMillis1;
                } else {
                    long termMillis = open_term * (long)60 * (long)1000;
                    Timestamp open_date = traceAttachInfo.getTimestamp("open_date");
                    boolean var9;
                    if(open_date == null) {
                        var9 = System.currentTimeMillis() - ex.getTime() > termMillis;
                        return var9;
                    } else {
                        var9 = open_date.getTime() - ex.getTime() > termMillis;
                        return var9;
                    }
                }
            }
        } catch (Exception var10) {
            var10.printStackTrace();
            return isOpenOverTime;
        }
    }

    public static boolean isDealOverTime(CachedRowSet traceAttachInfo) {
        boolean isDealOverTime = false;

        try {
            traceAttachInfo.first();
            Timestamp ex = traceAttachInfo.getTimestamp("rece_date");
            long deal_term = traceAttachInfo.getLong("deal_term");
            Timestamp fini_date = traceAttachInfo.getTimestamp("fini_date");
            isDealOverTime = isDealOverTime(ex, fini_date, deal_term);
        } catch (Exception var6) {
            var6.printStackTrace();
        }

        return isDealOverTime;
    }

    public static boolean isDealOverTime(Timestamp receDate, long dealTerm, long totalTerm) {
        return isDealOverTime(receDate, (Timestamp)null, dealTerm) || isDealOverTime(receDate, (Timestamp)null, totalTerm);
    }

    public static boolean isDealOverTime(Timestamp receDate, Timestamp finiDate, long dealTerm) {
        boolean isDealOverTime = false;

        try {
            boolean ex1;
            if(receDate == null) {
                ex1 = false;
                return ex1;
            } else if(dealTerm <= (long)0) {
                ex1 = false;
                return ex1;
            } else {
                long ex = dealTerm * (long)60 * (long)1000;
                boolean var7;
                if(finiDate == null) {
                    var7 = System.currentTimeMillis() - receDate.getTime() > ex;
                    return var7;
                } else {
                    var7 = finiDate.getTime() - receDate.getTime() > ex;
                    return var7;
                }
            }
        } catch (Exception var8) {
            var8.printStackTrace();
            return isDealOverTime;
        }
    }

    public CachedRowSet getCanScoreList(long flowInid) throws Exception {
        CachedRowSet returnValue = null;
        if(this._$75209) {
            this.logRec.oper_name = "得到可以进行评价的列表";
            this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("得到可以进行评价的列表:(")).append(flowInid).append(")"))))));
        }

        try {
            returnValue = this._$75299.getCanScoreList(flowInid);
            if(this._$75209) {
                this.logRec.log_level = 4;
                this.logRec.oper_resu = "成功";
            }

            return returnValue;
        } catch (Exception var5) {
            if(this._$75209) {
                this.logRec.log_level = 2;
                this.logRec.oper_resu = "失败";
                this.logRec.oper_desc = String.valueOf(String.valueOf(this.logRec.oper_desc)).concat(String.valueOf(String.valueOf(":".concat(String.valueOf(String.valueOf(var5.getMessage()))))));
            }

            throw new Exception("得到可以进行评价的列表".concat(String.valueOf(String.valueOf(var5.getMessage()))));
        }
    }

    public static boolean isEarly(String dateStr1, String dateStr2) {
        return dateStr1.compareTo(dateStr2) <= 0;
    }

    public PagingData getTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, long pageNo, int pageSize) {
        PagingData returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTaskCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            long startTime = 0L;
            if(this._$75209) {
                startTime = System.currentTimeMillis();
            }

            try {
                returnValue = this._$75299.getTaskCount(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aSender, aFlowFolder, aDocSeq, aSortField, aClassId, pageNo, pageSize);
            } catch (Exception var24) {
                var24.printStackTrace();
            }

            return returnValue;
        }
    }

    public PagingData getAdminTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, long pageNo, int pageSize) {
        PagingData returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getAdminTaskCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getAdminTaskCount(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aFlowFolder, aDocSeq, aSortField, aClassId, pageNo, pageSize);
            } catch (Exception var21) {
                var21.printStackTrace();
            }

            return returnValue;
        }
    }

    public PagingData getTaskQueryCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, long pageNo, int pageSize) {
        PagingData returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getTaskQueryCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getTaskQueryCount(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aSender, aFlowFolder, aDocSeq, aSortField, aClassId, pageNo, pageSize);
            } catch (Exception var22) {
                var22.printStackTrace();
            }

            return returnValue;
        }
    }

    public void clearCache() throws Exception {
        this._$75299.clearCache();
    }

    public CachedRowSet getSubflowsOfFlow(String flowInid) throws Exception {
        String sqlClause = "select d.flow_inid,deal_man,fsub_inid,f.fins_name,bu.full_name as user_name from deal_info d left join flow_inst f  on d.fsub_inid=f.flow_inid left join base_user bu on d.deal_man=bu.user_code where d.step_type=5 and d.flow_inid=?";
        SQLPara para = new SQLPara();
        para.add(flowInid);
        return BaseServerClient.getRowSet(sqlClause, para, this.getCurrentUser());
    }

    public CachedRowSet getAllographTaskCount(Map parameters) throws Exception {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getAllographTaskCount(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getAllographTaskCount(parameters);
            } catch (Exception var4) {
                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public CachedRowSet getAllographTask(Map parameters) throws Exception {
        CachedRowSet returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getAllographTask(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.getAllographTask(parameters);
            } catch (Exception var4) {
                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public CASignData[] readFlowInstCaSignData(String flowInid) throws Exception {
        CASignData[] returnValue = null;
        if(this._$75299 == null) {
            System.out.println("Error in getAllographTask(): Local interface reference is null.  It must be created by calling one of the Home interface methods first.");
            return returnValue;
        } else {
            try {
                returnValue = this._$75299.readFlowInstCaSignData(flowInid);
            } catch (Exception var4) {
                var4.printStackTrace();
            }

            return returnValue;
        }
    }

    public String wapLoadData(long aFlowInid, long aStepInid, long dealIndx) throws Exception {
        this.unloadData(aFlowInid, aStepInid);
        Hashtable returnStruct = null;
        if(aStepInid < (long)0) {
            returnStruct = this.getWapStartStepHtml(aFlowInid);
        } else if(aStepInid == (long)0) {
            returnStruct = this.getWapStepInsHtml(aFlowInid);
        } else {
            returnStruct = this.getWapStepInsHtml(aFlowInid, aStepInid, dealIndx);
        }

        String formHtml = (String)returnStruct.get("formHtml");
        FormStruct fs = (FormStruct)returnStruct.get("___0-3__48_f_uct_FormStruct___");
        FormInstData fid = (FormInstData)returnStruct.get("0_9UAad_form_iNst_Data__");
        CachedRowSet mainRs = (CachedRowSet)returnStruct.get("mainRs");
        RecordSet fieldMapNameRs = (RecordSet)returnStruct.get("fieldMapNameRs");
        RecordSet hypoMapRs = (RecordSet)returnStruct.get("hypoMapRs");
        Hashtable hypoRsColl = (Hashtable)returnStruct.get("hypoRsColl");
        Hashtable relaMapRsColl = (Hashtable)returnStruct.get("__RE__laMa__pRs");
        Hashtable relaRsColl = (Hashtable)returnStruct.get("Re_la_Rs_CO_lL_");
        Hashtable relaHtmlColl = (Hashtable)returnStruct.get("Re_la_Html_CO_lL_");
        RecordSet someInfoRs = (RecordSet)returnStruct.get("someInfoRs");
        CachedRowSet attachRs = (CachedRowSet)returnStruct.get("attachRs");
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "formHtml"), formHtml);
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "___0-3__48_f_uct_FormStruct___"), fs);
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "0_9UAad_form_iNst_Data__"), fid);
        if(mainRs == null) {
            System.out.println("loadData() mainRs is null");
        }

        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "mainRs"), mainRs);
        if(someInfoRs == null) {
            System.out.println("loadData() someInfoRs is null");
        }

        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "someInfoRs"), someInfoRs);
        this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "fieldMapNameRs"), fieldMapNameRs);
        if(hypoMapRs != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "hypoMapRs"), hypoMapRs);
        }

        if(hypoRsColl != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "hypoRsColl"), hypoRsColl);
        }

        if(relaMapRsColl != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "__RE__laMa__pRs"), relaMapRsColl);
        }

        if(relaRsColl != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "Re_la_Rs_CO_lL_"), relaRsColl);
        }

        if(relaHtmlColl != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "Re_la_Html_CO_lL_"), relaHtmlColl);
        }

        if(attachRs != null) {
            this.m_session.setAttribute(composeName(aFlowInid, aStepInid, "attachRs"), attachRs);
        }

        this._$75410((Map)someInfoRs.getObject("__At__tac__hR__ight__Rs"), aFlowInid, aStepInid);
        returnStruct.clear();
        return formHtml;
    }
}

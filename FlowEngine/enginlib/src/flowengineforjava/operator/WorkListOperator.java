//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package flowengineforjava.operator;

import baseserverjb.BaseServerClient;
import baseserverjb.IFramework;
import com.minstone.util.IReturn;
import com.minstone.util.ReturnImpl;
import com.minstone.util.SQLPara;
import com.minstone.util.database.SQLStatementFormer;
import flowengineforjava.operator.BusinessOperator;
import flowengineforjava.structure.PagingData;
import flowengineforjava.util.IBaseTool;
import flowengineforjava.util.PublicLib;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.util.*;

import sun.jdbc.rowset.CachedRowSet;

public class WorkListOperator extends BusinessOperator {
    private static final String _$37508 = "return_sqlpara";
    private static final String _$37509 = "return_sql";
    private static final String histDate="2014-12-31";

    public WorkListOperator() {
        this((IBaseTool) null);
    }

    public WorkListOperator(IBaseTool baseTool) {
        super(baseTool);
    }

    private Map _$37510(String value, int iType) {
        Hashtable mRet = new Hashtable();
        BaseServerClient bsc = new BaseServerClient(super.baseTool.getCurrentUser());
        IFramework framework = bsc.getFramework();
        CachedRowSet crs = null;
        String sTemp = "";
        if (value == null && value.trim().length() == 0) {
            return mRet;
        } else {
            label108:
            try {
                try {
                    switch (iType) {
                        case 0:
                            mRet.put(value, value);
                            break;
                        case 1:
                            crs = framework.getUserOfDept(Long.parseLong(value), true);

                            while (crs.next()) {
                                sTemp = crs.getString("user_code");
                                if (sTemp != null && sTemp.trim().length() > 0) {
                                    mRet.put(sTemp, sTemp);
                                }
                            }

                            mRet.put(String.valueOf(String.valueOf(value)).concat(":3"), String.valueOf(String.valueOf(value)).concat(":3"));
                            break;
                        case 2:
                            crs = framework.getUserOfGroup(Long.parseLong(value));

                            while (crs.next()) {
                                sTemp = crs.getString("user_code");
                                if (sTemp != null && sTemp.trim().length() > 0) {
                                    mRet.put(sTemp, sTemp);
                                }
                            }

                            mRet.put(String.valueOf(String.valueOf(value)).concat(":3"), String.valueOf(String.valueOf(value)).concat(":3"));
                            break;
                        case 3:
                            mRet.put(value, value);
                    }
                } catch (Exception var13) {
                    var13.printStackTrace();
                }
            } finally {
                break label108;
            }

            PublicLib.closeCachedRowSet(crs);
            return mRet;
        }
    }

    public final Iterator getForbidOperatorFlowIterator() throws Exception {
        String sqlClause = "";
        String userName = super.baseTool.getCurrentUser();
        CachedRowSet crs = null;
        Vector vFlows = new Vector();

        try {
            boolean e = super.baseTool.isAdmin();
            if (e) {
                Iterator var16 = vFlows.iterator();
                return var16;
            }

            sqlClause = " select item_seq from tree_def where item_type=2 ";
            SQLPara sqlPara = new SQLPara();
            String belongIn = super.baseTool.buildInDeptString(userName);
            if (!belongIn.equals("")) {
                belongIn = String.valueOf(String.valueOf(belongIn)).concat(",");
            }

            belongIn = String.valueOf(String.valueOf(belongIn)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(userName).append("\',\'AllUsers\'"))))));
            belongIn = PublicLib.findReplace(belongIn, "\'", "");
            String tmpBelongIn = "";
            sqlPara.add(userName);

            for (int k = 0; k < PublicLib.getFieldCount(belongIn, ","); ++k) {
                if (!tmpBelongIn.equals("")) {
                    tmpBelongIn = String.valueOf(String.valueOf(tmpBelongIn)).concat(",");
                }

                tmpBelongIn = String.valueOf(String.valueOf(tmpBelongIn)).concat("?");
                sqlPara.add(PublicLib.getField(belongIn, ",", (long) k));
            }

            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and ( ");
            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" crt_user=? ");
            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" or ( exists( select 1 from right_def ");
            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" where right_def.item_seq=tree_def.item_seq ");
            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and right_def.user_name IN (");
            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(String.valueOf(String.valueOf(tmpBelongIn)));
            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(") having ");
            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" (min(op_acc) = 0 ");
            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat("))))");
            crs = super.baseTool.getRowSet(sqlClause, "", sqlPara);

            while (crs.next()) {
                vFlows.add(crs.getString("item_seq"));
            }
        } catch (Exception var14) {
            PublicLib.log(true, "getHaveRightFlows", "op_acc", var14);
            throw new Exception(var14.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
        }

        return vFlows.iterator();
    }

    public final Iterator getHaveRightFlowIterator(String right) throws Exception {
        String sqlClause = "";
        String userName = super.baseTool.getCurrentUser();
        CachedRowSet crs = null;
        Vector vFlows = new Vector();

        try {
            boolean e = super.baseTool.isAdmin();
            sqlClause = " select item_seq from tree_def where item_type=2 ";
            SQLPara sqlPara = new SQLPara();
            if (!e) {
                String belongIn = super.baseTool.buildInDeptString(userName);
                if (!belongIn.equals("")) {
                    belongIn = String.valueOf(String.valueOf(belongIn)).concat(",");
                }

                belongIn = String.valueOf(String.valueOf(belongIn)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(userName).append("\',\'AllUsers\'"))))));
                belongIn = PublicLib.findReplace(belongIn, "\'", "");
                String tmpBelongIn = "";
                sqlPara.add(userName);

                for (int k = 0; k < PublicLib.getFieldCount(belongIn, ","); ++k) {
                    if (!tmpBelongIn.equals("")) {
                        tmpBelongIn = String.valueOf(String.valueOf(tmpBelongIn)).concat(",");
                    }

                    tmpBelongIn = String.valueOf(String.valueOf(tmpBelongIn)).concat("?");
                    sqlPara.add(PublicLib.getField(belongIn, ",", (long) k));
                }

                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and ( ");
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" crt_user=? ");
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" or ( exists( select 1 from right_def ");
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" where right_def.item_seq=tree_def.item_seq ");
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and right_def.user_name IN (");
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(String.valueOf(String.valueOf(tmpBelongIn)));
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(") having ");
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" ( min(")).append(right).append(") = 1 "))))));
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" or max(full_acc) = 1 ) ");
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and max(deny_acc) = 0 ) ) )");
            }

            crs = super.baseTool.getRowSet(sqlClause, "", sqlPara);

            while (crs.next()) {
                vFlows.add(crs.getString("item_seq"));
            }
        } catch (Exception var15) {
            PublicLib.log(true, "getHaveRightFlows", right, var15);
            throw new Exception(var15.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
        }

        return vFlows.iterator();
    }

    public final String getHaveRightFlows(String right) throws Exception {
        String flowList = "";

        for (Iterator it = this.getHaveRightFlowIterator(right); it.hasNext(); flowList = String.valueOf(String.valueOf(flowList)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(it.next()).append("\'"))))))) {
            if (!flowList.equals("")) {
                flowList = String.valueOf(String.valueOf(flowList)).concat(",");
            }
        }

        return flowList;
    }

    private Hashtable _$37514() throws Exception {
        String currentUser = super.baseTool.getCurrentUser();
        Hashtable hRet = new Hashtable();
        CachedRowSet crs = null;
        CachedRowSet tmpCrs = null;
        String sql = "select r.*,rd.right_name,rd.right_desc,rd.right_val_src,rd.rigth_cat_id,rc.rigth_cat_name,rex.ex_value,rex.ex_type from sys_user_right r left join sys_right_def rd on r.right_code=rd.right_code left join sys_rigth_category rc on rd.rigth_cat_id=rc.rigth_cat_id left join sys_user_right_ext rex  on r.role=rex.role and r.right_code=rex.right_code where rc.rigth_cat_id=1";

        label321:
        try {
            try {
                SQLPara ex = new SQLPara();
                Hashtable hTemp = new Hashtable();
                String inDept = super.baseTool.buildInDeptString(currentUser);
                if (!inDept.equals("")) {
                    inDept = String.valueOf(String.valueOf(inDept)).concat(",");
                }

                inDept = String.valueOf(String.valueOf(inDept)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(currentUser).append("\',\'AllUsers\'"))))));
                String tmpBelongIn = PublicLib.findReplace(inDept, "\'", "");
                String stationList = "";
                String sTemp = "";
                int fieldCount = PublicLib.getFieldCount(tmpBelongIn, ",");

                for (int hFrameUser = 0; hFrameUser < fieldCount; ++hFrameUser) {
                    if (!stationList.equals("")) {
                        stationList = String.valueOf(String.valueOf(stationList)).concat(",");
                    }

                    stationList = String.valueOf(String.valueOf(stationList)).concat("?");
                    sTemp = PublicLib.getField(tmpBelongIn, ",", (long) hFrameUser);
                    if (hFrameUser < fieldCount - 2) {
                        hTemp.put(String.valueOf(String.valueOf(sTemp)).concat(":3"), String.valueOf(String.valueOf(sTemp)).concat(":3"));
                    }

                    ex.add(sTemp);
                }

                if (stationList.trim().length() != 0) {
                    sql = String.valueOf(String.valueOf(sql)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" and r.role in(")).append(stationList).append(")"))))));
                }

                if (super.baseTool.isDebugMode()) {
                    System.err.println("sql=".concat(String.valueOf(String.valueOf(sql))));
                    System.err.println("para=".concat(String.valueOf(String.valueOf(ex))));
                }

                crs = super.baseTool.getRowSet(sql, "", ex);
                Hashtable var33 = new Hashtable();
                Hashtable hExFrameUser = new Hashtable();
                Hashtable hExFlowId = new Hashtable();
                Hashtable hFlowId = new Hashtable();
                boolean viewAll = false;
                boolean viewDept = false;
                boolean viewSelf = true;
                if (crs.size() > 0) {
                    viewSelf = false;
                    String criteria = "";
                    criteria = "right_code=\'1000000002\'";
                    viewAll = PublicLib.find(crs, criteria, true);
                    criteria = "right_code=\'1000000001\'";
                    viewDept = PublicLib.find(crs, criteria, true);
                    crs.beforeFirst();
                    String rightCode = "";
                    String exValue = "";
                    boolean iExType = true;
                    boolean rightValueSrc = true;
                    sTemp = "";

                    label307:
                    while (true) {
                        int var34;
                        int var36;
                        do {
                            do {
                                if (!crs.next()) {
                                    if (viewAll) {
                                        break label307;
                                    }

                                    var33.putAll(hTemp);
                                    if (viewDept) {
                                        String var35 = super.baseTool.getDeptSeq(currentUser);
                                        if (var35 == null) {
                                            var35 = "-100";
                                        }

                                        tmpCrs = super.baseTool.getUserOfDept(Long.parseLong(var35), true);

                                        while (tmpCrs.next()) {
                                            sTemp = tmpCrs.getString("user_code");
                                            var33.put(sTemp, sTemp);
                                        }

                                        Iterator it1 = hExFrameUser.keySet().iterator();

                                        while (it1.hasNext()) {
                                            var33.remove(it1.next());
                                        }

                                        hExFrameUser.clear();
                                        break label307;
                                    }

                                    Iterator it;
                                    if (var33.size() > 0) {
                                        it = hExFrameUser.keySet().iterator();

                                        while (it.hasNext()) {
                                            var33.remove(it.next());
                                        }

                                        hExFrameUser.clear();
                                    }

                                    if (hFlowId.size() <= 0) {
                                        break label307;
                                    }

                                    it = hExFlowId.keySet().iterator();

                                    while (it.hasNext()) {
                                        hFlowId.remove(it.next());
                                    }

                                    hExFlowId.clear();
                                    break label307;
                                }

                                rightCode = crs.getString("right_code");
                                var36 = crs.getInt("right_val_src");
                                exValue = crs.getString("ex_value");
                                var34 = crs.getInt("ex_type");
                            } while (var36 == -1);

                            if (!viewAll && (rightCode.equals("1000000005") || rightCode.equals("1000000003"))) {
                                switch (var36) {
                                    case 1:
                                        var33.putAll(this._$37510(exValue, var34));
                                        break;
                                    case 3:
                                        hFlowId.putAll(this._$37510(exValue, var34));
                                }
                            }
                        } while (!rightCode.equals("1000000006") && !rightCode.equals("1000000004"));

                        switch (var36) {
                            case 1:
                                hExFrameUser.putAll(this._$37510(exValue, var34));
                                break;
                            case 3:
                                hExFlowId.putAll(this._$37510(exValue, var34));
                        }
                    }
                }

                hRet.put("FRAME_USER", var33);
                hRet.put("EX_FRAME_USER", hExFrameUser);
                hRet.put("FLOW_ID", hFlowId);
                hRet.put("EX_FLOW_ID", hExFlowId);
                hRet.put("EX_VIEW_ALL", new Boolean(viewAll));
                hRet.put("EX_VIEW_DEPT", new Boolean(viewDept));
                hRet.put("EX_VIEW_SELF", new Boolean(viewSelf));
                if (super.baseTool.isDebugMode()) {
                    if (viewAll) {
                        System.err.println("查看所有");
                    }

                    if (viewDept) {
                        System.err.println("查看本部门");
                    }

                    System.err.println("hFrameUser=".concat(String.valueOf(String.valueOf(var33))));
                    System.err.println("hFlowId=".concat(String.valueOf(String.valueOf(hFlowId))));
                    System.err.println("hExFrameUser=".concat(String.valueOf(String.valueOf(hExFrameUser))));
                    System.err.println("hExFlowId=".concat(String.valueOf(String.valueOf(hExFlowId))));
                }
            } catch (Exception var31) {
                var31.printStackTrace();
                throw var31;
            }
        } finally {
            break label321;
        }

        PublicLib.closeCachedRowSet(crs);
        PublicLib.closeCachedRowSet(tmpCrs);
        return hRet;
    }

    public CachedRowSet getAdminTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        StringBuffer whereClause = null;
        String belongIn = "";
        SQLPara sqlPara = new SQLPara();
        String deal_info = "deal_info";
        String step_inst = "step_inst";
        String flow_inst = "flow_inst";
        if (!"".equals(aBeginDateHigher) && aBeginDateHigher.compareTo(histDate) <= 0) {
            deal_info = "deal_info_history";
            step_inst = "step_inst_history";
            flow_inst = "flow_inst_history";
        }
        try {
            String infomation;
            try {
                sqlClause = new StringBuffer(1000);
                whereClause = new StringBuffer(1000);
                infomation = super.baseTool.getCurrentUser();
                aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
                aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
                aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
                String flowFolderPara = "";

                for (int whereInfo = 0; whereInfo < PublicLib.getFieldCount(aFlowFolder, ","); ++whereInfo) {
                    if (!flowFolderPara.equals("")) {
                        flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat(",");
                    }

                    flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat("?");
                }

                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("") || aSortField.indexOf("T.item_name") != -1) {
                    whereClause.append(" and A.flow_id=T.item_seq");
                }

                whereClause.append(" and A.if_efec<>2");
                if (aTaskType.equals("-1")) {
                    if (aReadFlag == (long) 0) {
                        whereClause.append(" and (A.if_end is null or A.if_end=0)");
                    } else if (aReadFlag == (long) 1) {
                        whereClause.append(" and (A.if_end is not null and A.if_end=1)");
                    }

                    if (aClassId.equals("-1")) {
                        whereClause.append(" and not exists(select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and A.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=?) ");
                        sqlPara.add(infomation);
                    } else if (!aClassId.equals("")) {
                        whereClause.append(" and exists (select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and A.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=? and doc_class.class_id=?)");
                        sqlPara.add(infomation);
                        sqlPara.add(aClassId, 1);
                    }
                }

                if (aCondition != null && !aCondition.equals("")) {
                    String var35 = PublicLib.findReplace(aCondition, "AA.", "A.");
                    whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(var35).append(")"))));
                }

                String[][] var34 = new String[][]{{" (A.inst_crea like ? or D.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aCreator)))).append(",").append(aCreator))), String.valueOf(0)}, {" A.inst_crda>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_crda<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" A.inst_fida>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_fida<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" T.item_name like ? ", aFlowName, String.valueOf(0)}, {" A.fins_name like ? ", aInstName, String.valueOf(0)}, {String.valueOf(String.valueOf((new StringBuffer(" T.para_seq in (")).append(flowFolderPara).append(")"))), aFlowFolder, String.valueOf(0)}, {" A.doc_seq like ? ", aDocSeq, String.valueOf(0)}};

                for (int i = 0; i < var34.length; ++i) {
                    if (var34[i][1] != null && !var34[i][1].equals("") && !var34[i][1].equals(",")) {
                        whereClause.append(" and ");
                        whereClause.append(var34[i][0]);

                        for (int j = 0; j < PublicLib.getFieldCount(var34[i][1], ","); ++j) {
                            String value = PublicLib.getField(var34[i][1], ",", (long) j);
                            if (var34[i][0].indexOf(" like ") != -1) {
                                value = String.valueOf(String.valueOf((new StringBuffer("%")).append(value).append("%")));
                            }

                            sqlPara.add(value, Integer.parseInt(var34[i][2]));
                        }
                    }
                }

                sqlClause.append(" select A.flow_inid from ").append(flow_inst).append(" A");
                sqlClause.append(" left join ").append(step_inst).append(" B on A.curr_step=B.step_inco");
                if (aCreator != null && !aCreator.equals("")) {
                    sqlClause.append(" left join base_user D on A.inst_crea=D.user_code");
                }

                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("") || aSortField.indexOf("T.item_name") != -1) {
                    sqlClause.append(",tree_def T");
                }

                sqlClause.append(" where (A.del_flag is null or A.del_flag=0)");
                sqlClause.append(whereClause);
                if (aSortField != null && !aSortField.equals("")) {
                    sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(aSortField))));
                } else if (sqlClause.indexOf("step_inst B2") != -1) {
                    sqlClause.append(" order by B2.crea_date desc ");
                } else if (sqlClause.indexOf("step_inst B") != -1) {
                    sqlClause.append(" order by B.crea_date desc ");
                }

                super.baseTool.debug("getAdminTaskCount.SQL=".concat(String.valueOf(String.valueOf(sqlClause))));
                crs = super.baseTool.getRowSet(sqlClause.substring(0), "", sqlPara);
            } catch (Exception var32) {
                infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
                PublicLib.log(true, "getAdminTaskCount", infomation, var32);
                throw new Exception(var32.getMessage());
            }
        } finally {
            sqlClause = null;
            whereClause = null;
        }

        return crs;
    }

    public PagingData getTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, long pageNo, int pageSize) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        StringBuffer whereClause = null;
        StringBuffer subClause = null;
        String belongIn = "";
        boolean needOrderByTerm = false;
        boolean needStation = false;
        SQLPara sqlPara = new SQLPara();
        PagingData pagingData = new PagingData();
        String deal_info = "deal_info";
        String step_inst = "step_inst";
        String flow_inst = "flow_inst";
        if (!"".equals(aBeginDateHigher) && aBeginDateHigher.compareTo(histDate) <= 0) {
            deal_info = "deal_info_history";
            step_inst = "step_inst_history";
            flow_inst = "flow_inst_history";
        }

        try {
            String infomation;
            try {
                sqlClause = new StringBuffer(1000);
                whereClause = new StringBuffer(1000);
                subClause = new StringBuffer(1000);
                String e = super.baseTool.getCurrentUser();
                aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
                aSender = PublicLib.findReplace(aSender, "\'", "\'\'");
                aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
                aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
                infomation = "";

                for (int whereInfo = 0; whereInfo < PublicLib.getFieldCount(aFlowFolder, ","); ++whereInfo) {
                    if (!infomation.equals("")) {
                        infomation = String.valueOf(String.valueOf(infomation)).concat(",");
                    }

                    infomation = String.valueOf(String.valueOf(infomation)).concat("?");
                }

                whereClause.append(" where CC.fini_flag >=0 and CC.step_type<>5");
                whereClause.append(" and CC.step_type<>4 and CC.del_flag=0");
                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("")) {
                    whereClause.append(" and AA.flow_id=TT.item_seq");
                }

                whereClause.append(" and ");
                if (aTaskType.equals("-1")) {
                    if (aReadFlag == (long) 0) {
                        needStation = true;
                        whereClause.append("(AA.if_end is null or AA.if_end=0)");
                    } else if (aReadFlag == (long) 1) {
                        whereClause.append("(AA.if_end is not null and AA.if_end=1)");
                    } else {
                        needStation = true;
                        whereClause.append("(1=1)");
                    }

                    if (aClassId.equals("-1")) {
                        whereClause.append(" and not exists(select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and AA.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=?) ");
                        sqlPara.add(e);
                    } else if (!aClassId.equals("")) {
                        whereClause.append(" and exists (select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and AA.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=? and doc_class.class_id=?)");
                        sqlPara.add(e);
                        sqlPara.add(aClassId, 1);
                    }
                } else if (aTaskType.equals("0")) {
                    needOrderByTerm = true;
                    if ((long) 0 == aReadFlag) {
                        needStation = true;
                        whereClause.append(" (CC.fini_flag=0 or (CC.fini_flag=3 and CC.read_flag=0))");
                    } else if ((long) 1 == aReadFlag) {
                        whereClause.append(" (CC.fini_flag=0 and CC.step_type=1)");
                    } else {
                        needStation = true;
                        whereClause.append(" (CC.fini_flag=0)");
                    }
                } else if (aTaskType.equals("1")) {
                    if ((long) 0 == aReadFlag) {
                        whereClause.append(" (CC.fini_flag=1 and (AA.if_end is null or AA.if_end=0))");
                    } else {
                        whereClause.append("(CC.fini_flag=1)");
                    }
                } else if (aTaskType.equals("3")) {
                    if ((long) 0 == aReadFlag) {
                        whereClause.append(" (CC.fini_flag=3 and CC.read_flag=0)");
                    } else if ((long) 1 == aReadFlag) {
                        whereClause.append("(CC.fini_flag=3 and CC.read_flag=1)");
                    } else {
                        whereClause.append("(CC.fini_flag=3)");
                    }
                } else if (aTaskType.equals("4")) {
                    if ((long) 0 == aReadFlag) {
                        needOrderByTerm = true;
                        needStation = true;
                        whereClause.append("((CC.fini_flag=0 or (CC.fini_flag=3 and CC.read_flag=0)) and CC.step_type<>1)");
                    } else if ((long) 1 == aReadFlag) {
                        whereClause.append("((CC.fini_flag=1 or (CC.fini_flag=3 and CC.read_flag=1)) and CC.step_type<>1)");
                    } else {
                        needStation = true;
                        whereClause.append("(CC.step_type<>1)");
                    }
                } else {
                    whereClause.append("1=2");
                }

                if (aCondition != null && !aCondition.equals("")) {
                    whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(aCondition).append(")"))));
                }

                String[][] var51 = new String[][]{{" (AA.inst_crea like ? or DD2.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aCreator)))).append(",").append(aCreator))), String.valueOf(0)}, {" AA.inst_crda>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" AA.inst_crda<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" AA.inst_fida>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" AA.inst_fida<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" TT.item_name like ? ", aFlowName, String.valueOf(0)}, {" AA.fins_name like ? ", aInstName, String.valueOf(0)}, {" (CC.peri_man like ? or DD.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aSender)))).append(",").append(aSender))), String.valueOf(0)}, {String.valueOf(String.valueOf((new StringBuffer(" TT.para_seq in (")).append(infomation).append(")"))), aFlowFolder, String.valueOf(0)}, {" AA.doc_seq like ? ", aDocSeq, String.valueOf(0)}};

                String haveRightFlows;
                for (int ignoreStation = 0; ignoreStation < var51.length; ++ignoreStation) {
                    if (var51[ignoreStation][1] != null && !var51[ignoreStation][1].equals("") && !var51[ignoreStation][1].equals(",")) {
                        whereClause.append(" and ");
                        whereClause.append(var51[ignoreStation][0]);

                        for (int countClause = 0; countClause < PublicLib.getFieldCount(var51[ignoreStation][1], ","); ++countClause) {
                            haveRightFlows = PublicLib.getField(var51[ignoreStation][1], ",", (long) countClause);
                            if (var51[ignoreStation][0].indexOf(" like ") != -1) {
                                haveRightFlows = String.valueOf(String.valueOf((new StringBuffer("%")).append(haveRightFlows).append("%")));
                            }

                            sqlPara.add(haveRightFlows, Integer.parseInt(var51[ignoreStation][2]));
                        }
                    }
                }
                String dealManClause = "";
                List dealManPara = new ArrayList();
                String var50 = super.baseTool.getTagValue("WF/NOSTATION");
                if ((var50.equals("") || var50.equals("0")) && needStation) {
                    belongIn = super.baseTool.buildInDeptString(e);
                    if (!belongIn.equals("")) {
                        belongIn = String.valueOf(String.valueOf(belongIn)).concat(",");
                        belongIn = PublicLib.findReplace(belongIn, "\',", ":3\',");
                    }

                    belongIn = String.valueOf(String.valueOf(belongIn)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(e).append("\'"))))));
                    String var48 = PublicLib.findReplace(belongIn, "\'", "");
                    haveRightFlows = "";

                    for (int orderByTotalTerm = 0; orderByTotalTerm < PublicLib.getFieldCount(var48, ","); ++orderByTotalTerm) {
                        if (!haveRightFlows.equals("")) {
                            haveRightFlows = String.valueOf(String.valueOf(haveRightFlows)).concat(",");
                        }

                        haveRightFlows = String.valueOf(String.valueOf(haveRightFlows)).concat("?");
                        String field = PublicLib.getField(var48, ",", (long) orderByTotalTerm);
                        sqlPara.add(field);
                        dealManPara.add(field);
                    }
                    dealManClause = " in (" + haveRightFlows + ")";
                    whereClause.append(" and CC.deal_man").append(dealManClause);
                } else {
                    belongIn = String.valueOf(String.valueOf((new StringBuffer("=\'")).append(e).append("\'")));
                    dealManClause = "=? ";
                    dealManPara.clear();
                    dealManPara.add(e);
                    sqlPara.add(e);
                    whereClause.append(" and CC.deal_man").append(dealManClause);
                }

                if (whereClause.substring(0).indexOf("BB.") != -1) {
                    whereClause.append(" and CC.step_inco=BB.step_inco");
                }

                if (whereClause.substring(0).indexOf("AA.") != -1) {
                    whereClause.append(" and AA.flow_inid=CC.flow_inid");
                }

                whereClause.append(" group by CC.flow_inid ) ");
                if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                    whereClause.append(" M ");
                }

                subClause.append(" (select max(CC.step_index) ");
                if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                    subClause.append(" as max_step_index");
                }

                subClause.append(" from " + deal_info + " CC");
                if (aSender != null && !aSender.equals("")) {
                    subClause.append(" left join base_user DD on CC.peri_man=DD.user_code");
                }

                if (whereClause.substring(0).indexOf("BB.") != -1) {
                    subClause.append("," + step_inst + " BB");
                }

                if (whereClause.substring(0).indexOf("AA.") != -1) {
                    subClause.append("," + flow_inst + " AA");
                }

                if (aCreator != null && !aCreator.equals("")) {
                    subClause.append(" left join base_user DD2 on AA.inst_crea=DD2.user_code");
                }

                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("")) {
                    subClause.append(",tree_def TT");
                }

                subClause.append(whereClause.toString());
                StringBuffer var49 = new StringBuffer("");
                if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                    sqlClause.append(" from ".concat(String.valueOf(String.valueOf(subClause))));
                }

                if (sqlClause.length() > 0) {
                    sqlClause.append(",");
                }

                sqlClause.append(flow_inst + " A");
                if (aSortField.indexOf("B2.step_name") != -1) {
                    sqlClause.append(" left join " + step_inst + " B2 on A.curr_step=B2.step_inco");
                }

                sqlClause.append("," + deal_info + " C");
                if (aSortField.indexOf("D.full_name") != -1) {
                    sqlClause.append(" left join base_user D on C.peri_man=D.user_code");
                }

                if (aSortField == null || aSortField.equals("") || aSortField.indexOf("B.crea_date") != -1) {
                    sqlClause.append("," + step_inst + " B");
                }

                if (aSortField.indexOf("T.item_name") != -1) {
                    sqlClause.append(",tree_def T");
                }

                sqlClause.append(" where A.flow_inid=C.flow_inid ");
                sqlClause.append(" and C.deal_man").append(dealManClause);
                for (int i = 0; i < dealManPara.size(); i++) {
                    sqlPara.add(String.valueOf(dealManPara.get(i)));
                }
                Iterator var52 = this.getForbidOperatorFlowIterator();
                String var53;
                if (var52.hasNext()) {
                    sqlClause.append(" and A.flow_id not in(");
                    var53 = "";

                    while (var52.hasNext()) {
                        if (var53.length() > 0) {
                            var53 = String.valueOf(String.valueOf(var53)).concat(",");
                        }

                        var53 = String.valueOf(String.valueOf(var53)).concat("?");
                        sqlPara.add((String) var52.next());
                    }

                    sqlClause.append(var53);
                    sqlClause.append(") ");
                }

                if (aSortField == null || aSortField.equals("") || aSortField.indexOf("B.crea_date") != -1) {
                    sqlClause.append(" and B.step_inco=C.step_inco");
                }

                if (aSortField.indexOf("T.item_name") != -1) {
                    sqlClause.append(" and T.item_seq=A.flow_id");
                }

                sqlClause.append(" and (A.del_flag=0)");
                if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                    sqlClause.append(" and C.step_index=M.max_step_index");
                } else if (super.baseTool.getDBServerType() == PublicLib.Type_Sybase || super.baseTool.getDBServerType() == PublicLib.Type_DB2) {
                    sqlClause.append(" and C.step_index in");
                    sqlClause.append(subClause);
                }

                sqlClause.append(" and A.if_efec<>2");
                var53 = null;
                String orderByDealTerm = null;
                String sql;
                if (needOrderByTerm) {
                    SQLStatementFormer totalRow = this.baseTool.getSQLStatementFormer();
                    String expr = totalRow.sign(totalRow.isNull(String.valueOf(String.valueOf((new StringBuffer("a.total_term-(")).append(totalRow.isNull("c.fini_date", totalRow.getSysTimestamp())).append("-c.rece_date)*24*60"))), "0"));
                    sql = totalRow.sign(totalRow.isNull(String.valueOf(String.valueOf((new StringBuffer("c.deal_term-(")).append(totalRow.isNull("c.fini_date", totalRow.getSysTimestamp())).append("-c.rece_date)*24*60"))), "0"));
                    String[] when = new String[]{"1", "-1", "0"};
                    String[] result = new String[]{"0", "1", "0"};
                    var53 = String.valueOf(String.valueOf(totalRow.decode(expr, when, result, (String) null))).concat(" desc");
                    orderByDealTerm = String.valueOf(String.valueOf(totalRow.decode(sql, when, result, (String) null))).concat(" desc");
                }

                var49.append("select count(*) as CNT");
                var49.append(sqlClause);
                sqlClause.insert(0, " select A.flow_inid");
                if (aSortField != null && !aSortField.equals("")) {
                    sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" order by ")).append(needOrderByTerm ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(var53)))).append(",").append(orderByDealTerm).append(","))) : "").append(aSortField))));
                } else {
                    sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" order by ")).append(needOrderByTerm ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(var53)))).append(",").append(orderByDealTerm).append(","))) : "").append("C.rece_date desc "))));
                }

                crs = super.baseTool.getRowSet(var49.substring(0), "", sqlPara);
                super.baseTool.debug("getTaskCount.count(*).SQL=".concat(String.valueOf(String.valueOf(var49.substring(0)))));
                long var54 = 0L;
                if (crs.next()) {
                    var54 = crs.getLong("CNT");
                }

                PublicLib.closeCachedRowSet(crs);
                crs = null;
                if (var54 > (long) 0) {
                    sql = super.baseTool.getSQLStatementFormer().genPagingStatement(sqlClause.substring(0), pageNo, pageSize);
                    super.baseTool.debug("getTaskCount.data.SQL=".concat(sql));
                    crs = super.baseTool.getRowSet(sql, "", sqlPara);
                    pagingData.setTotalRow(var54);
                    pagingData.setPageSize((long) pageSize);
                    pagingData.setCurrentPageNo(pageNo);
                    pagingData.setPagingData(crs);
                }

                super.baseTool.debug("sqlPara=".concat(String.valueOf(String.valueOf(sqlPara))));
            } catch (Exception var46) {
                infomation = "登陆用户=".concat(super.baseTool.getCurrentUser())
                        .concat("getTaskCount.SQL=".concat(String.valueOf(sqlClause)))
                        .concat("sqlPara=".concat(String.valueOf(sqlPara)));
                PublicLib.log(true, "getTaskCount", infomation, var46);
                throw new Exception(var46.getMessage());
            }
        } finally {
            sqlClause = null;
            whereClause = null;
            subClause = null;
        }

        return pagingData;
    }

    public CachedRowSet getTasksQueryEx(String aCondition, String aSortField) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        SQLPara sqlPara = new SQLPara();

        try {
            String infomation;
            try {
                sqlClause = new StringBuffer(1000);
                sqlClause.append("select A.flow_inid,A.flow_id,A.fins_name,A.doc_seq,A.worklist_itemex1,A.worklist_itemex2,A.worklist_itemex3,A.worklist_itemex4,A.worklist_itemex5,T.item_name as flow_name,");
                sqlClause.append("A.awoke_flag,A.sys_urge,B2.flow_id,B2.step_code,B2.step_type,");
                sqlClause.append("A.inst_crda,B2.step_name,b2.step_inco");
                sqlClause.append(" from flow_inst A left join step_inst B2 on A.curr_step=B2.step_inco left join tree_def T on T.item_seq=A.flow_id");
                sqlClause.append(" where A.DEL_FLAG=0 ");
                if (aCondition != null && !aCondition.equals("")) {
                    if (aCondition.trim().equals("1=2")) {
                        sqlClause.append(" AND 1=2");
                    } else {
                        String e = PublicLib.getField(aCondition, "(", 1L);
                        e = PublicLib.getField(e, ")", 0L);
                        e = PublicLib.findReplace(e, "\'", "");
                        infomation = "";

                        for (int i = 0; i < PublicLib.getFieldCount(e, ","); ++i) {
                            if (!infomation.equals("")) {
                                infomation = String.valueOf(String.valueOf(infomation)).concat(",");
                            }

                            infomation = String.valueOf(String.valueOf(infomation)).concat("?");
                            String flowInid = PublicLib.getField(e, ",", (long) i);
                            sqlPara.add(flowInid);
                        }

                        sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" AND ")).append(PublicLib.getField(aCondition, "(", 0L)).append("(").append(infomation).append(")"))));
                    }
                }

                if (aSortField != null && !aSortField.equals("")) {
                    sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(aSortField))));
                } else {
                    sqlClause.append(" order by A.inst_crda desc ");
                }

                super.baseTool.debug("getTasksQueryEx.SQL=".concat(String.valueOf(String.valueOf(sqlClause.toString()))));
                crs = super.baseTool.getRowSet(sqlClause.substring(0), "", sqlPara);
            } catch (Exception var14) {
                infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
                PublicLib.log(true, "getTasksQueryEx", infomation, var14);
                throw new Exception(var14.getMessage());
            }
        } finally {
            sqlClause = null;
        }

        return crs;
    }

    public CachedRowSet getAdminTasksEx(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) throws Exception {
        return this._$4679(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aSender, aFlowFolder, aDocSeq, aSortField, aClassId, true);
    }

    public CachedRowSet getTasksEx(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) throws Exception {
        return this._$4679(aCondition, aTaskType, aReadFlag, aCreator, aBeginDateLower, aBeginDateHigher, aEndDateLower, aEndDateHigher, aFlowName, aInstName, aSender, aFlowFolder, aDocSeq, aSortField, aClassId, false);
    }

    private CachedRowSet _$4679(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, boolean isAdmin) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        boolean needStation = false;
        SQLPara sqlPara = new SQLPara();
        boolean needOrderByTerm = false;
        String deal_info = "deal_info";
        String step_inst = "step_inst";
        String flow_inst = "flow_inst";
        if (!"".equals(aBeginDateHigher) && aBeginDateHigher.compareTo(histDate) <= 0) {
            deal_info = "deal_info_history";
            step_inst = "step_inst_history";
            flow_inst = "flow_inst_history";
        }
        try {
            String infomation;
            try {
                sqlClause = new StringBuffer(1000);
                infomation = super.baseTool.getCurrentUser();
                SQLStatementFormer former = this.baseTool.getSQLStatementFormer();
                String totalDelay = former.isNull(String.valueOf(String.valueOf((new StringBuffer("A.total_term-(")).append(former.isNull("C.fini_date", former.getSysTimestamp())).append("-C.rece_date)*24*60"))), "0");
                String dealDelay = former.isNull(String.valueOf(String.valueOf((new StringBuffer("C.deal_term-(")).append(former.isNull("C.fini_date", former.getSysTimestamp())).append("-C.rece_date)*24*60"))), "0");
                aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
                aSender = PublicLib.findReplace(aSender, "\'", "\'\'");
                aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
                aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
                String flowFolderPara = "";

                for (int whereInfo = 0; whereInfo < PublicLib.getFieldCount(aFlowFolder, ","); ++whereInfo) {
                    if (!flowFolderPara.equals("")) {
                        flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat(",");
                    }

                    flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat("?");
                }

                sqlClause.append(" select A.flow_inid,A.fins_name,A.doc_seq,A.worklist_itemex1,A.worklist_itemex2,A.worklist_itemex3,A.worklist_itemex4,A.worklist_itemex5,A.flow_name as flow_name,A.awoke_flag,A.sys_urge,B2.flow_id,B2.step_code,");
                if (isAdmin && aTaskType.equals("-1")) {
                    sqlClause.append("B.crea_date,");
                }

                sqlClause.append(" B2.step_name,");
                sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" C.peri_man,C.fini_date,C.deal_man,C.rece_date,C.step_inco,C.step_type,C.deal_indx,")).append(totalDelay).append(" as total_delay,").append(dealDelay).append(" as deal_delay,A.total_term,C.deal_term,"))));
                sqlClause.append(" D.full_name as peri_name");
                sqlClause.append(" from ").append(flow_inst).append(" A left join ").append(step_inst).append(" B2 on A.curr_step=B2.step_inco left join tree_def T on T.item_seq=A.flow_id,");
                sqlClause.append(" ").append(deal_info).append(" C left join base_user D on C.peri_man=D.user_code");
                if (isAdmin && aTaskType.equals("-1")) {
                    sqlClause.append(",step_inst B");
                }

                sqlClause.append(" where A.flow_inid=C.flow_inid ");
                if (isAdmin && aTaskType.equals("-1")) {
                    sqlClause.append(" and B.step_inco=C.step_inco ");
                }

                sqlClause.append(" and C.fini_flag is not null and (A.del_flag is null or A.del_flag=0) ");
                sqlClause.append(" and C.step_type<>5");
                int orderByDealTerm;
                String orderByTotalTerm;
                String expr;
                if (aCondition != null && !aCondition.equals("")) {
                    if (aCondition.trim().equals("1=2")) {
                        sqlClause.append(" and 1=2");
                    } else {
                        String var44 = PublicLib.getField(aCondition, "(", 1L);
                        var44 = PublicLib.getField(var44, ")", 0L);
                        var44 = PublicLib.findReplace(var44, "\'", "");
                        orderByTotalTerm = "";

                        for (orderByDealTerm = 0; orderByDealTerm < PublicLib.getFieldCount(var44, ","); ++orderByDealTerm) {
                            if (!orderByTotalTerm.equals("")) {
                                orderByTotalTerm = String.valueOf(String.valueOf(orderByTotalTerm)).concat(",");
                            }

                            orderByTotalTerm = String.valueOf(String.valueOf(orderByTotalTerm)).concat("?");
                            expr = PublicLib.getField(var44, ",", (long) orderByDealTerm);
                            sqlPara.add(expr);
                        }

                        sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" and ")).append(PublicLib.getField(aCondition, "(", 0L)).append("(").append(orderByTotalTerm).append(")"))));
                    }
                }

                sqlClause.append(" and ");
                if (aTaskType.equals("-1")) {
                    if (aReadFlag == (long) 0) {
                        needStation = true;
                        sqlClause.append("(A.if_end=0)");
                    } else if (aReadFlag == (long) 1) {
                        sqlClause.append("(A.if_end=1)");
                    } else {
                        needStation = true;
                        sqlClause.append("(1=1)");
                    }
                } else if (aTaskType.equals("0")) {
                    needOrderByTerm = true;
                    if ((long) 0 == aReadFlag) {
                        needStation = true;
                        sqlClause.append("(C.fini_flag=0 or (C.fini_flag=3 and C.read_flag=0))");
                    } else if ((long) 1 == aReadFlag) {
                        sqlClause.append("(C.fini_flag=0 and C.step_type=1)");
                    } else {
                        needStation = true;
                        sqlClause.append("(C.fini_flag=0)");
                    }
                } else if (aTaskType.equals("1")) {
                    if ((long) 0 == aReadFlag) {
                        sqlClause.append("(C.fini_flag=1 and (A.if_end is null or A.if_end=0))");
                    } else {
                        sqlClause.append("(C.fini_flag=1)");
                    }
                } else if (aTaskType.equals("3")) {
                    if ((long) 0 == aReadFlag) {
                        sqlClause.append("(C.fini_flag=3 and C.read_flag=0)");
                    } else if ((long) 1 == aReadFlag) {
                        sqlClause.append("(C.fini_flag=3 and C.read_flag=1)");
                    } else {
                        sqlClause.append("(C.fini_flag=3)");
                    }
                } else if (aTaskType.equals("4")) {
                    if ((long) 0 == aReadFlag) {
                        needOrderByTerm = true;
                        needStation = true;
                        sqlClause.append("((C.fini_flag=0 or (C.fini_flag=3 and C.read_flag=0)) and C.step_type<>1)");
                    } else if ((long) 1 == aReadFlag) {
                        sqlClause.append("((C.fini_flag=1 or (C.fini_flag=3 and C.read_flag=1)) and C.step_type<>1)");
                    } else {
                        needStation = true;
                        sqlClause.append("(C.step_type<>1)");
                    }
                } else {
                    sqlClause.append("(1=2)");
                }

                String[][] var42 = new String[][]{{" A.inst_crda>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateLower)).concat(" 0:0:0")), String.valueOf(2)},
                        {" A.inst_crda<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateHigher)).concat(" 23:59:59")), String.valueOf(2)},
                        {" A.inst_fida>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_fida<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateHigher)).concat(" 23:59:59")), String.valueOf(2)},
                        {" T.item_name like ? ", aFlowName, String.valueOf(0)},
                        {" A.fins_name like ? ", aInstName, String.valueOf(0)},
                        {" (C.peri_man like ? or D.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aSender)))).append(",").append(aSender))), String.valueOf(0)}, {String.valueOf(String.valueOf((new StringBuffer(" T.para_seq in (")).append(flowFolderPara).append(")"))), aFlowFolder, String.valueOf(0)}, {" A.doc_seq like ? ", aDocSeq, String.valueOf(0)}};

                for (int var43 = 0; var43 < var42.length; ++var43) {
                    if (var42[var43][1] != null && !var42[var43][1].equals("") && !var42[var43][1].equals(",")) {
                        sqlClause.append(" and ");
                        sqlClause.append(var42[var43][0]);

                        for (orderByDealTerm = 0; orderByDealTerm < PublicLib.getFieldCount(var42[var43][1], ","); ++orderByDealTerm) {
                            expr = PublicLib.getField(var42[var43][1], ",", (long) orderByDealTerm);
                            if (var42[var43][0].indexOf(" like ") != -1) {
                                expr = String.valueOf(String.valueOf((new StringBuffer("%")).append(expr).append("%")));
                            }

                            sqlPara.add(expr, Integer.parseInt(var42[var43][2]));
                        }
                    }
                }

                String expr1;
                String var45;
                if (!isAdmin || !aTaskType.equals("-1")) {
                    orderByTotalTerm = super.baseTool.getTagValue("WF/NOSTATION");
                    var45 = "";
                    if ((orderByTotalTerm.equals("") || orderByTotalTerm.equals("0")) && needStation) {
                        var45 = super.baseTool.buildInDeptString(infomation);
                        if (!var45.equals("")) {
                            var45 = String.valueOf(String.valueOf(var45)).concat(",");
                            var45 = PublicLib.findReplace(var45, "\',", ":3\',");
                        }

                        var45 = String.valueOf(String.valueOf(var45)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(infomation).append("\'"))))));
                        expr = PublicLib.findReplace(var45, "\'", "");
                        expr1 = "";

                        for (int when = 0; when < PublicLib.getFieldCount(expr, ","); ++when) {
                            if (!expr1.equals("")) {
                                expr1 = String.valueOf(String.valueOf(expr1)).concat(",");
                            }

                            expr1 = String.valueOf(String.valueOf(expr1)).concat("?");
                            sqlPara.add(PublicLib.getField(expr, ",", (long) when));
                        }

                        sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" and C.deal_man in (")).append(expr1).append(")"))));
                    } else {
                        var45 = String.valueOf(String.valueOf((new StringBuffer("=\'")).append(infomation).append("\'")));
                        sqlPara.add(infomation);
                        sqlClause.append(" and C.deal_man=? ");
                    }

                    sqlClause.append(" and C.step_type<>4 and (C.del_flag=0)");
                }

                orderByTotalTerm = null;
                var45 = null;
                if (needOrderByTerm) {
                    expr = former.sign("total_delay");
                    expr1 = former.sign("deal_delay");
                    String[] var46 = new String[]{"1", "-1", "0"};
                    String[] result = new String[]{"0", "1", "0"};
                    orderByTotalTerm = String.valueOf(String.valueOf(former.decode(expr, var46, result, (String) null))).concat(" desc");
                    var45 = String.valueOf(String.valueOf(former.decode(expr1, var46, result, (String) null))).concat(" desc");
                }

                if (aSortField != null && !aSortField.equals("")) {
                    sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" order by ")).append(needOrderByTerm ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(orderByTotalTerm)))).append(",").append(var45).append(","))) : "").append(aSortField))));
                } else if (isAdmin) {
                    sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(needOrderByTerm ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(orderByTotalTerm)))).append(",").append(var45).append(","))) : ""))));
                    if (sqlClause.indexOf("step_inst B2") != -1) {
                        sqlClause.append(" B2.crea_date desc ");
                    } else if (sqlClause.indexOf("step_inst B") != -1) {
                        sqlClause.append(" B.crea_date desc ");
                    }
                } else {
                    sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" order by ")).append(needOrderByTerm ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(orderByTotalTerm)))).append(",").append(var45).append(","))) : "").append("C.rece_date desc "))));
                }

                sqlClause.append(",C.step_inco desc");
                this.baseTool.info("getTasksEx:" + sqlClause.substring(0));
                this.baseTool.info(sqlPara);
                crs = super.baseTool.getRowSet(sqlClause.substring(0), "", sqlPara);
            } catch (Exception var40) {
                infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
                PublicLib.log(true, "getTasksEx", infomation, var40);
                throw new Exception(var40.getMessage());
            }
        } finally {
            sqlClause = null;
        }

        return crs;
    }

    public CachedRowSet getTaskQueryCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        StringBuffer whereClause = null;
        StringBuffer subClause = null;
        String belongIn = "";
        SQLPara sqlPara = new SQLPara();
        Hashtable hRight = this._$37514();
        Hashtable hFrameUser = (Hashtable) hRight.get("FRAME_USER");
        Hashtable hFlowId = (Hashtable) hRight.get("FLOW_ID");
        Hashtable hExFrameUser = (Hashtable) hRight.get("EX_FRAME_USER");
        Hashtable hExFlowId = (Hashtable) hRight.get("EX_FLOW_ID");
        boolean viewAll = ((Boolean) hRight.get("EX_VIEW_ALL")).booleanValue();
        boolean viewDept = ((Boolean) hRight.get("EX_VIEW_DEPT")).booleanValue();
        boolean viewSelf = ((Boolean) hRight.get("EX_VIEW_SELF")).booleanValue();
        boolean hasSetRight = !viewSelf;

        try {
            sqlClause = new StringBuffer(1000);
            whereClause = new StringBuffer(1000);
            subClause = new StringBuffer(1000);
            String e = super.baseTool.getCurrentUser();
            hFrameUser.put(e, e);
            hExFrameUser.remove(e);
            boolean var45 = super.baseTool.isAdmin();
            aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
            aSender = PublicLib.findReplace(aSender, "\'", "\'\'");
            aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
            aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
            String flowFolderPara = "";

            for (int whereInfo = 0; whereInfo < PublicLib.getFieldCount(aFlowFolder, ","); ++whereInfo) {
                if (!flowFolderPara.equals("")) {
                    flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat(",");
                }

                flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat("?");
            }

            whereClause.append(" where CC.fini_flag is not null and CC.fini_flag>=0 and CC.step_type<>5");
            if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("")) {
                whereClause.append(" and AA.flow_id=TT.item_seq");
            }

            whereClause.append(" and AA.if_efec<>2");
            whereClause.append(" and ");
            if (aTaskType.equals("-1")) {
                if (aReadFlag == (long) 0) {
                    whereClause.append("(AA.if_end is null or AA.if_end=0)");
                } else if (aReadFlag == (long) 1) {
                    whereClause.append("(AA.if_end is not null and AA.if_end=1)");
                } else {
                    whereClause.append("(1=1)");
                }

                if (aClassId.equals("-1")) {
                    whereClause.append(" and not exists(select doc_class.flow_inid from class_def,doc_class");
                    whereClause.append(" where doc_class.class_id=class_def.class_id");
                    whereClause.append(" and AA.flow_inid=doc_class.flow_inid");
                    whereClause.append(" and class_def.user_code=?) ");
                    sqlPara.add(e);
                } else if (!aClassId.equals("")) {
                    whereClause.append(" and exists (select doc_class.flow_inid from class_def,doc_class");
                    whereClause.append(" where doc_class.class_id=class_def.class_id");
                    whereClause.append(" and AA.flow_inid=doc_class.flow_inid");
                    whereClause.append(" and class_def.user_code=? and doc_class.class_id=?)");
                    sqlPara.add(e);
                    sqlPara.add(aClassId, 1);
                }
            } else if (aTaskType.equals("0")) {
                if ((long) 0 == aReadFlag) {
                    whereClause.append(" (CC.fini_flag=0 or (CC.fini_flag=3 and CC.read_flag=0))");
                } else if ((long) 1 == aReadFlag) {
                    whereClause.append(" (CC.fini_flag=0 and CC.step_type=1)");
                } else {
                    whereClause.append(" (CC.fini_flag=0)");
                }
            } else if (aTaskType.equals("1")) {
                if ((long) 0 == aReadFlag) {
                    whereClause.append(" (CC.fini_flag=1 and (AA.if_end is null or AA.if_end=0))");
                } else {
                    whereClause.append("(CC.fini_flag=1)");
                }
            } else if (aTaskType.equals("3")) {
                if ((long) 0 == aReadFlag) {
                    whereClause.append(" (CC.fini_flag=3 and CC.read_flag=0)");
                } else if ((long) 1 == aReadFlag) {
                    whereClause.append("(CC.fini_flag=3 and CC.read_flag=1)");
                } else {
                    whereClause.append("(CC.fini_flag=3)");
                }
            } else if (aTaskType.equals("4")) {
                if ((long) 0 == aReadFlag) {
                    whereClause.append("((CC.fini_flag=0 or (CC.fini_flag=3 and CC.read_flag=0)) and CC.step_type<>1)");
                } else if ((long) 1 == aReadFlag) {
                    whereClause.append("((CC.fini_flag=1 or (CC.fini_flag=3 and CC.read_flag=1)) and CC.step_type<>1)");
                } else {
                    whereClause.append("(CC.step_type<>1)");
                }
            } else {
                whereClause.append("1=2");
            }

            if (aCondition != null && !aCondition.equals("")) {
                whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(aCondition).append(")"))));
            }

            String[][] var47 = new String[][]{
                    {" (AA.inst_crea like ? or DD2.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aCreator)))).append(",").append(aCreator))), String.valueOf(0)},
                    {" AA.inst_crda>? ", PublicLib.getTimestamp(aBeginDateLower.concat(" 0:0:0")), String.valueOf(2)},
                    {" AA.inst_crda<? ", PublicLib.getTimestamp(aBeginDateHigher.concat(" 23:59:59")), String.valueOf(2)},
                    {" AA.inst_fida>? ", PublicLib.getTimestamp(aEndDateLower.concat(" 0:0:0")), String.valueOf(2)},
                    {" AA.inst_fida<? ", PublicLib.getTimestamp(aEndDateHigher.concat(" 23:59:59")), String.valueOf(2)},
                    {" TT.item_name like ? ", aFlowName, String.valueOf(0)},
                    {" AA.fins_name like ? ", aInstName, String.valueOf(0)},
                    {" (CC.peri_man like ? or DD.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aSender)))).append(",").append(aSender))), String.valueOf(0)},
                    {String.valueOf(String.valueOf((new StringBuffer(" TT.para_seq in (")).append(flowFolderPara).append(")"))), aFlowFolder, String.valueOf(0)},
                    {" AA.doc_seq like ? ", aDocSeq, String.valueOf(0)}};

            String stationList;
            for (int temp = 0; temp < var47.length; ++temp) {
                if (var47[temp][1] != null && !var47[temp][1].equals("") && !var47[temp][1].equals(",")) {
                    whereClause.append(" and ");
                    whereClause.append(var47[temp][0]);

                    for (int it = 0; it < PublicLib.getFieldCount(var47[temp][1], ","); ++it) {
                        stationList = PublicLib.getField(var47[temp][1], ",", (long) it);
                        if (var47[temp][0].indexOf(" like ") != -1) {
                            stationList = String.valueOf(String.valueOf((new StringBuffer("%")).append(stationList).append("%")));
                        }

                        sqlPara.add(stationList, Integer.parseInt(var47[temp][2]));
                    }
                }
            }

            String var46 = "";
            Object var48 = null;
            stationList = "";
            if (hasSetRight) {
                if (viewAll) {
                    if (hExFrameUser.size() > 0) {
                        whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and CC.deal_man not in (")).append(PublicLib.buildInList(hExFrameUser, sqlPara)).append(")"))));
                    }

                    if (hExFlowId.size() > 0) {
                        whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id not in (")).append(PublicLib.buildInList(hExFlowId, sqlPara)).append(")"))));
                    }
                } else if (viewDept) {
                    if (hFrameUser.size() > 0) {
                        whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and CC.deal_man in (")).append(PublicLib.buildInList(hFrameUser, sqlPara)).append(")"))));
                        if (hFlowId.size() > 0) {
                            whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id in (")).append(PublicLib.buildInList(hFlowId, sqlPara)).append(")"))));
                        } else if (hExFlowId.size() > 0) {
                            whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id not in (")).append(PublicLib.buildInList(hExFlowId, sqlPara)).append(")"))));
                        }
                    } else {
                        whereClause.append(" and 1=2");
                    }
                } else {
                    if (hFrameUser.size() > 0) {
                        whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and CC.deal_man in (")).append(PublicLib.buildInList(hFrameUser, sqlPara)).append(")"))));
                    }

                    if (hFlowId.size() > 0) {
                        whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id in (")).append(PublicLib.buildInList(hFlowId, sqlPara)).append(")"))));
                    } else if (hExFlowId.size() > 0) {
                        whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id not in (")).append(PublicLib.buildInList(hExFlowId, sqlPara)).append(")"))));
                    }
                }
            } else {
                whereClause.append(" and CC.deal_man =? ");
                sqlPara.add(e);
            }

            whereClause.append(" and CC.step_type<>4 and CC.del_flag=0");
            if (whereClause.substring(0).indexOf("AA.") != -1) {
                whereClause.append(" and AA.flow_inid=CC.flow_inid");
            }

            whereClause.append(" group by CC.flow_inid ) ");
            if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                whereClause.append(" max_deal_info ");
            }

            subClause.append(" (select max(CC.step_index) ");
            if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                subClause.append(" as max_step_index");
            }

            subClause.append(" from deal_info CC");
            if (aSender != null && !aSender.equals("")) {
                subClause.append(" left join base_user DD on CC.peri_man=DD.user_code");
            }

            if (whereClause.substring(0).indexOf("AA.") != -1) {
                subClause.append(",flow_inst AA");
            }

            if (aCreator != null && !aCreator.equals("")) {
                subClause.append(" left join base_user DD2 on AA.inst_crea=DD2.user_code");
            }

            if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("")) {
                subClause.append(",tree_def TT");
            }

            subClause.append(whereClause.toString());
            sqlClause.append(" select A.flow_inid");
            sqlClause.append(" from flow_inst A");
            if (aSortField.indexOf("B2.step_name") != -1) {
                sqlClause.append(" left join step_inst B2 on A.curr_step=B2.step_inco");
            }

            sqlClause.append(",deal_info C");
            if (aSortField.indexOf("D.full_name") != -1) {
                sqlClause.append(" left join base_user D on C.peri_man=D.user_code");
            }

            if (aSortField == null || aSortField.equals("") || aSortField.indexOf("B.crea_date") != -1) {
                sqlClause.append(",step_inst B");
            }

            if (aSortField.indexOf("T.item_name") != -1) {
                sqlClause.append(",tree_def T");
            }

            if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                sqlClause.append(",");
                sqlClause.append(subClause);
            }

            sqlClause.append(" where A.flow_inid=C.flow_inid ");
            if (aSortField == null || aSortField.equals("") || aSortField.indexOf("B.crea_date") != -1) {
                sqlClause.append(" and B.step_inco=C.step_inco");
            }

            if (aSortField.indexOf("T.item_name") != -1) {
                sqlClause.append(" and T.item_seq=A.flow_id");
            }

            sqlClause.append(" and (A.del_flag is null or A.del_flag=0)");
            if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                sqlClause.append(" and C.step_index=max_deal_info.max_step_index");
            } else if (super.baseTool.getDBServerType() == PublicLib.Type_Sybase || super.baseTool.getDBServerType() == PublicLib.Type_DB2) {
                sqlClause.append(" and C.step_index in");
                sqlClause.append(subClause);
            }

            if (!belongIn.equals("") && belongIn.indexOf(",") == -1) {
                sqlClause.append(" and C.deal_man=?");
                sqlPara.add(e);
            }

            if (aSortField != null && !aSortField.equals("")) {
                sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(aSortField))));
            } else if (var45) {
                if (sqlClause.indexOf("step_inst B2") != -1) {
                    sqlClause.append(" order by B2.crea_date desc ");
                } else if (sqlClause.indexOf("step_inst B") != -1) {
                    sqlClause.append(" order by B.crea_date desc ");
                }
            } else {
                sqlClause.append(" order by C.rece_date desc ");
            }

            super.baseTool.debug("getTaskQueryCount.SQL=".concat(String.valueOf(String.valueOf(sqlClause))));
            crs = super.baseTool.getRowSet(sqlClause.substring(0), "", sqlPara);
        } catch (Exception var43) {
            String infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "getTaskQueryCount", infomation, var43);
            throw new Exception(var43.getMessage());
        } finally {
            sqlClause = null;
            whereClause = null;
            subClause = null;
            hFrameUser = null;
            hFlowId = null;
            hExFrameUser = null;
            hExFlowId = null;
            PublicLib.clearObject(hRight);
        }

        return crs;
    }

    public CachedRowSet getTasksByMonth(String aYearMonth) throws Exception {
        CachedRowSet crs = null;

        try {
            String e = "";
            SQLPara infomation1 = new SQLPara();
            String currentUser = super.baseTool.getCurrentUser();
            SQLStatementFormer sf = super.baseTool.getSQLStatementFormer();
            boolean isAdmin = super.baseTool.isAdmin();
            String monthFilter = "";
            String flowList1;
            if (aYearMonth.equals("")) {
                String haveRightFlows2 = "";
                infomation1.clear();
                if (!isAdmin) {
                    haveRightFlows2 = " where deal_man=? ";
                    infomation1.add(currentUser);
                    infomation1.add(currentUser);
                }

                flowList1 = String.valueOf(String.valueOf((new StringBuffer("select ")).append(sf.isNull("c.rece_date", "a.inst_crda")).append(" as flow_date from deal_info c left join flow_inst a on a.flow_inid=c.flow_inid where c.step_inco in(").append("select min(step_inco) as min_step_inco from deal_info").append(haveRightFlows2).append(" union ").append(" select max(step_inco) as max_step_inco from deal_info").append(haveRightFlows2).append(") order by flow_date desc")));
                crs = super.baseTool.getRowSet(flowList1, "", infomation1);
                CachedRowSet lowerDate1 = crs;
                return lowerDate1;
            }

            int[] haveRightFlows = new int[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            int flowList = Integer.parseInt(PublicLib.getField(aYearMonth, "-", 0L));
            if (flowList % 4 == 0 && flowList % 100 != 0 || flowList % 400 == 0) {
                haveRightFlows[1] = 29;
            }

            String lowerDate = "";
            String highDate = "";
            if (aYearMonth.indexOf("-") != -1) {
                int month = Integer.parseInt(PublicLib.getField(aYearMonth, "-", 1L));
                lowerDate = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(String.valueOf(flowList))))).append("-").append(String.valueOf(month)).append("-1")));
                if (month < 12) {
                    highDate = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(String.valueOf(flowList))))).append("-").append(String.valueOf(month + 1)).append("-1")));
                } else {
                    highDate = String.valueOf(String.valueOf(String.valueOf(flowList + 1))).concat("-1-1");
                }
            } else {
                lowerDate = String.valueOf(String.valueOf(String.valueOf(flowList))).concat("-1-1");
                highDate = String.valueOf(String.valueOf(String.valueOf(flowList + 1))).concat("-1-1");
            }

            infomation1.add(PublicLib.getTimestamp(String.valueOf(String.valueOf(lowerDate)).concat(" 0:0:0")), 2);
            infomation1.add(PublicLib.getTimestamp(String.valueOf(String.valueOf(highDate)).concat(" 23:59:59")), 2);
            infomation1.add(PublicLib.getTimestamp(String.valueOf(String.valueOf(lowerDate)).concat(" 0:0:0")), 2);
            infomation1.add(PublicLib.getTimestamp(String.valueOf(String.valueOf(highDate)).concat(" 23:59:59")), 2);
            monthFilter = " and ((C.rece_date>? and C.rece_date<? and C.rece_date is not null) or (A.inst_crda>? and A.inst_crda<? and C.rece_date is null))";
            e = " select A.fins_name,A.inst_crda,A.flow_id, B.step_name,B.step_code,B.flow_inid,B.step_inco, C.peri_man,C.deal_man,c.rece_date,c.fini_date,c.deal_indx,A.total_term,C.deal_term, D1.full_name as peri_name,D2.full_name as deal_name from flow_inst A,step_inst B,deal_info C left join base_user D1 on c.peri_man=d1.user_code left join base_user D2 on c.deal_man=d2.user_code where A.flow_inid=B.flow_inid and B.step_inco=c.step_inco  and C.fini_flag is not null  and A.del_flag=0 and (C.del_flag=0)  and B.step_type<>5 ".concat(String.valueOf(String.valueOf(monthFilter)));
            if (!isAdmin) {
                infomation1.add(currentUser);
                e = String.valueOf(String.valueOf(e)).concat(" and c.deal_man=? ");
            } else {
                e = String.valueOf(String.valueOf(e)).concat(" and (deal_man is not null) ");
            }

            Iterator haveRightFlows1 = this.getForbidOperatorFlowIterator();
            if (haveRightFlows1.hasNext()) {
                e = String.valueOf(String.valueOf(e)).concat(" and A.flow_id not in(");
                flowList1 = "";

                while (haveRightFlows1.hasNext()) {
                    if (flowList1.length() > 0) {
                        flowList1 = String.valueOf(String.valueOf(flowList1)).concat(",");
                    }

                    flowList1 = String.valueOf(String.valueOf(flowList1)).concat("?");
                    infomation1.add((String) haveRightFlows1.next());
                }

                e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(flowList1)).concat(") "))));
            }

            e = String.valueOf(String.valueOf(e)).concat(" order by c.rece_date desc,A.inst_crda desc");
            crs = super.baseTool.getRowSet(e, "", infomation1);
        } catch (Exception var18) {
            String infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "getTasksByMonth", infomation, var18);
            throw new Exception(var18.getMessage());
        } finally {
            ;
        }

        return crs;
    }

    public CachedRowSet getTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        StringBuffer whereClause = null;
        StringBuffer subClause = null;
        String belongIn = "";
        boolean needOrderByTerm = false;
        boolean needStation = false;
        SQLPara sqlPara = new SQLPara();
        String deal_info = "deal_info";
        String step_inst = "step_inst";
        String flow_inst = "flow_inst";
        if (!"".equals(aBeginDateHigher) && aBeginDateHigher.compareTo(histDate) <= 0) {
            deal_info = "deal_info_history";
            step_inst = "step_inst_history";
            flow_inst = "flow_inst_history";
        }
        try {
            sqlClause = new StringBuffer(1000);
            whereClause = new StringBuffer(1000);
            subClause = new StringBuffer(1000);
            String e = super.baseTool.getCurrentUser();
            boolean var44 = super.baseTool.isAdmin();
            aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
            aSender = PublicLib.findReplace(aSender, "\'", "\'\'");
            aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
            aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
            String flowFolderPara = "";

            for (int whereInfo = 0; whereInfo < PublicLib.getFieldCount(aFlowFolder, ","); ++whereInfo) {
                if (!flowFolderPara.equals("")) {
                    flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat(",");
                }

                flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat("?");
            }

            whereClause.append(" where CC.fini_flag is not null and CC.fini_flag >=0 and CC.step_type<>5");
            if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("")) {
                whereClause.append(" and AA.flow_id=TT.item_seq");
            }

            whereClause.append(" and AA.if_efec<>2");
            whereClause.append(" and ");
            if (aTaskType.equals("-1")) {
                if (aReadFlag == (long) 0) {
                    needStation = true;
                    whereClause.append("(AA.if_end is null or AA.if_end=0)");
                } else if (aReadFlag == (long) 1) {
                    whereClause.append("(AA.if_end is not null and AA.if_end=1)");
                } else {
                    needStation = true;
                    whereClause.append("(1=1)");
                }

                if (aClassId.equals("-1")) {
                    whereClause.append(" and not exists(select doc_class.flow_inid from class_def,doc_class");
                    whereClause.append(" where doc_class.class_id=class_def.class_id");
                    whereClause.append(" and AA.flow_inid=doc_class.flow_inid");
                    whereClause.append(" and class_def.user_code=?) ");
                    sqlPara.add(e);
                } else if (!aClassId.equals("")) {
                    whereClause.append(" and exists (select doc_class.flow_inid from class_def,doc_class");
                    whereClause.append(" where doc_class.class_id=class_def.class_id");
                    whereClause.append(" and AA.flow_inid=doc_class.flow_inid");
                    whereClause.append(" and class_def.user_code=? and doc_class.class_id=?)");
                    sqlPara.add(e);
                    sqlPara.add(aClassId, 1);
                }
            } else if (aTaskType.equals("0")) {
                needOrderByTerm = true;
                if ((long) 0 == aReadFlag) {
                    needStation = true;
                    whereClause.append(" (CC.fini_flag=0 or (CC.fini_flag=3 and CC.read_flag=0))");
                } else if ((long) 1 == aReadFlag) {
                    whereClause.append(" (CC.fini_flag=0 and CC.step_type=1)");
                } else {
                    needStation = true;
                    whereClause.append(" (CC.fini_flag=0)");
                }
            } else if (aTaskType.equals("1")) {
                if ((long) 0 == aReadFlag) {
                    whereClause.append(" (CC.fini_flag=1 and (AA.if_end is null or AA.if_end=0))");
                } else {
                    whereClause.append("(CC.fini_flag=1)");
                }
            } else if (aTaskType.equals("3")) {
                if ((long) 0 == aReadFlag) {
                    whereClause.append(" (CC.fini_flag=3 and CC.read_flag=0)");
                } else if ((long) 1 == aReadFlag) {
                    whereClause.append("(CC.fini_flag=3 and CC.read_flag=1)");
                } else {
                    whereClause.append("(CC.fini_flag=3)");
                }
            } else if (aTaskType.equals("4")) {
                if ((long) 0 == aReadFlag) {
                    needOrderByTerm = true;
                    needStation = true;
                    whereClause.append("((CC.fini_flag=0 or (CC.fini_flag=3 and CC.read_flag=0)) and CC.step_type<>1)");
                } else if ((long) 1 == aReadFlag) {
                    whereClause.append("((CC.fini_flag=1 or (CC.fini_flag=3 and CC.read_flag=1)) and CC.step_type<>1)");
                } else {
                    needStation = true;
                    whereClause.append("(CC.step_type<>1)");
                }
            } else {
                whereClause.append("1=2");
            }

            if (aCondition != null && !aCondition.equals("")) {
                whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(aCondition).append(")"))));
            }

            String[][] var43 = new String[][]{{" (AA.inst_crea like ? or DD2.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aCreator)))).append(",").append(aCreator))), String.valueOf(0)}, {" AA.inst_crda>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" AA.inst_crda<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" AA.inst_fida>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" AA.inst_fida<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" TT.item_name like ? ", aFlowName, String.valueOf(0)}, {" AA.fins_name like ? ", aInstName, String.valueOf(0)}, {" (CC.peri_man like ? or DD.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aSender)))).append(",").append(aSender))), String.valueOf(0)}, {String.valueOf(String.valueOf((new StringBuffer(" TT.para_seq in (")).append(flowFolderPara).append(")"))), aFlowFolder, String.valueOf(0)}, {" AA.doc_seq like ? ", aDocSeq, String.valueOf(0)}};

            String orderByDealTerm;
            for (int haveRightFlows = 0; haveRightFlows < var43.length; ++haveRightFlows) {
                if (var43[haveRightFlows][1] != null && !var43[haveRightFlows][1].equals("") && !var43[haveRightFlows][1].equals(",")) {
                    whereClause.append(" and ");
                    whereClause.append(var43[haveRightFlows][0]);

                    for (int orderByTotalTerm = 0; orderByTotalTerm < PublicLib.getFieldCount(var43[haveRightFlows][1], ","); ++orderByTotalTerm) {
                        orderByDealTerm = PublicLib.getField(var43[haveRightFlows][1], ",", (long) orderByTotalTerm);
                        if (var43[haveRightFlows][0].indexOf(" like ") != -1) {
                            orderByDealTerm = String.valueOf(String.valueOf((new StringBuffer("%")).append(orderByDealTerm).append("%")));
                        }

                        sqlPara.add(orderByDealTerm, Integer.parseInt(var43[haveRightFlows][2]));
                    }
                }
            }

            String var45;
            String dealManClause = "";
            List dealManPara = new ArrayList();
            if (!var44 || !aTaskType.equals("-1")) {
                String var47 = super.baseTool.getTagValue("WF/NOSTATION");
                if ((var47.equals("") || var47.equals("0")) && needStation) {
                    belongIn = super.baseTool.buildInDeptString(e);
                    if (!belongIn.equals("")) {
                        belongIn = String.valueOf(String.valueOf(belongIn)).concat(",");
                        belongIn = PublicLib.findReplace(belongIn, "\',", ":3\',");
                    }

                    belongIn = String.valueOf(String.valueOf(belongIn)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(e).append("\'"))))));
                    var45 = PublicLib.findReplace(belongIn, "\'", "");
                    orderByDealTerm = "";

                    for (int former = 0; former < PublicLib.getFieldCount(var45, ","); ++former) {
                        if (!orderByDealTerm.equals("")) {
                            orderByDealTerm = String.valueOf(String.valueOf(orderByDealTerm)).concat(",");
                        }

                        orderByDealTerm = String.valueOf(String.valueOf(orderByDealTerm)).concat("?");
                        String field = PublicLib.getField(var45, ",", (long) former);
                        sqlPara.add(field);
                        dealManPara.add(field);
                    }
                    dealManClause = " in (" + orderByDealTerm + ")";
                    whereClause.append(" and CC.deal_man").append(dealManClause);
                } else {
                    belongIn = String.valueOf(String.valueOf((new StringBuffer("=\'")).append(e).append("\'")));
                    dealManClause = "=? ";
                    dealManPara.clear();
                    dealManPara.add(e);
                    sqlPara.add(e);
                    whereClause.append(" and CC.deal_man").append(dealManClause);
                }

                whereClause.append(" and CC.step_type<>4 and CC.del_flag=0");
            }

            if (whereClause.substring(0).indexOf("BB.") != -1) {
                whereClause.append(" and CC.step_inco=BB.step_inco");
            }

            if (whereClause.substring(0).indexOf("AA.") != -1) {
                whereClause.append(" and AA.flow_inid=CC.flow_inid");
            }

            whereClause.append(" group by CC.flow_inid ) ");
            if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                whereClause.append(" max_deal_info ");
            }

            subClause.append(" (select max(CC.step_index) ");
            if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                subClause.append(" as max_step_index");
            }

            subClause.append(" from " + deal_info + " CC");
            if (aSender != null && !aSender.equals("")) {
                subClause.append(" left join base_user DD on CC.peri_man=DD.user_code");
            }

            if (whereClause.substring(0).indexOf("BB.") != -1) {
                subClause.append("," + step_inst + " BB");
            }

            if (whereClause.substring(0).indexOf("AA.") != -1) {
                subClause.append("," + flow_inst + " AA");
            }

            if (aCreator != null && !aCreator.equals("")) {
                subClause.append(" left join base_user DD2 on AA.inst_crea=DD2.user_code");
            }

            if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("")) {
                subClause.append(",tree_def TT");
            }

            subClause.append(whereClause.toString());
            sqlClause.append(" select A.flow_inid");
            sqlClause.append(" from " + flow_inst + " A");
            if (aSortField.indexOf("B2.step_name") != -1) {
                sqlClause.append(" left join " + step_inst + " B2 on A.curr_step=B2.step_inco");
            }

            sqlClause.append("," + deal_info + " C");
            if (aSortField.indexOf("D.full_name") != -1) {
                sqlClause.append(" left join base_user D on C.peri_man=D.user_code");
            }

            if (aSortField == null || aSortField.equals("") || aSortField.indexOf("B.crea_date") != -1) {
                sqlClause.append("," + step_inst + " B");
            }

            if (aSortField.indexOf("T.item_name") != -1) {
                sqlClause.append(",tree_def T");
            }

            if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                sqlClause.append(",");
                sqlClause.append(subClause);
            }

            sqlClause.append(" where A.flow_inid=C.flow_inid ");
            sqlClause.append(" and C.deal_man").append(dealManClause);
            for (int i = 0; i < dealManPara.size(); i++) {
                sqlPara.add(String.valueOf(dealManPara.get(i)));
            }
            Iterator var46 = this.getForbidOperatorFlowIterator();
            if (var46.hasNext()) {
                sqlClause.append(" and A.flow_id not in(");
                var45 = "";

                while (var46.hasNext()) {
                    if (var45.length() > 0) {
                        var45 = String.valueOf(String.valueOf(var45)).concat(",");
                    }

                    var45 = String.valueOf(String.valueOf(var45)).concat("?");
                    sqlPara.add((String) var46.next());
                }

                sqlClause.append(var45);
                sqlClause.append(") ");
            }

            if (aSortField == null || aSortField.equals("") || aSortField.indexOf("B.crea_date") != -1) {
                sqlClause.append(" and B.step_inco=C.step_inco");
            }

            if (aSortField.indexOf("T.item_name") != -1) {
                sqlClause.append(" and T.item_seq=A.flow_id");
            }

            sqlClause.append(" and (A.del_flag is null or A.del_flag=0)");
            if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                sqlClause.append(" and C.step_index=max_deal_info.max_step_index");
            } else if (super.baseTool.getDBServerType() == PublicLib.Type_Sybase || super.baseTool.getDBServerType() == PublicLib.Type_DB2) {
                sqlClause.append(" and C.step_index in");
                sqlClause.append(subClause);
            }

            var45 = null;
            orderByDealTerm = null;
            if (needOrderByTerm) {
                SQLStatementFormer var48 = this.baseTool.getSQLStatementFormer();
                String expr = var48.sign(var48.isNull(String.valueOf(String.valueOf((new StringBuffer("a.total_term-(")).append(var48.isNull("c.fini_date", var48.getSysTimestamp())).append("-c.rece_date)*24*60"))), "0"));
                String expr1 = var48.sign(var48.isNull(String.valueOf(String.valueOf((new StringBuffer("c.deal_term-(")).append(var48.isNull("c.fini_date", var48.getSysTimestamp())).append("-c.rece_date)*24*60"))), "0"));
                String[] when = new String[]{"1", "-1", "0"};
                String[] result = new String[]{"0", "1", "0"};
                var45 = String.valueOf(String.valueOf(var48.decode(expr, when, result, (String) null))).concat(" desc");
                orderByDealTerm = String.valueOf(String.valueOf(var48.decode(expr1, when, result, (String) null))).concat(" desc");
            }

            if (aSortField != null && !aSortField.equals("")) {
                sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" order by ")).append(needOrderByTerm ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(var45)))).append(",").append(orderByDealTerm).append(","))) : "").append(aSortField))));
            } else if (var44) {
                sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(needOrderByTerm ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(var45)))).append(",").append(orderByDealTerm).append(","))) : ""))));
                if (sqlClause.indexOf("step_inst B2") != -1) {
                    sqlClause.append(" B2.crea_date desc ");
                } else if (sqlClause.indexOf("step_inst B") != -1) {
                    sqlClause.append(" B.crea_date desc ");
                }
            } else {
                sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" order by ")).append(needOrderByTerm ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(var45)))).append(",").append(orderByDealTerm).append(","))) : "").append("C.rece_date desc "))));
            }

            super.baseTool.debug("getTaskCount.SQL=".concat(String.valueOf(sqlClause)));
            super.baseTool.debug("sqlPara=".concat(String.valueOf(String.valueOf(sqlPara))));
            crs = super.baseTool.getRowSet(sqlClause.substring(0), "", sqlPara);
        } catch (Exception var41) {
            String infomation = "登陆用户=".concat(super.baseTool.getCurrentUser())
                    .concat("getTaskCount.SQL=".concat(String.valueOf(sqlClause)))
                    .concat("sqlPara=".concat(String.valueOf(sqlPara)));
            PublicLib.log(true, "getTaskCount", infomation, var41);
            throw new Exception(var41.getMessage());
        } finally {
            sqlClause = null;
            whereClause = null;
            subClause = null;
        }

        return crs;
    }

    public PagingData getTaskQueryCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aSender, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, long pageNo, int pageSize) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        StringBuffer whereClause = null;
        StringBuffer subClause = null;
        String belongIn = "";
        PagingData pagingData = new PagingData();
        SQLPara sqlPara = new SQLPara();
        Hashtable hRight = this._$37514();
        Hashtable hFrameUser = (Hashtable) hRight.get("FRAME_USER");
        Hashtable hFlowId = (Hashtable) hRight.get("FLOW_ID");
        Hashtable hExFrameUser = (Hashtable) hRight.get("EX_FRAME_USER");
        Hashtable hExFlowId = (Hashtable) hRight.get("EX_FLOW_ID");
        boolean viewAll = ((Boolean) hRight.get("EX_VIEW_ALL")).booleanValue();
        boolean viewDept = ((Boolean) hRight.get("Ex_VIEW_DEPT")).booleanValue();
        boolean viewSelf = ((Boolean) hRight.get("EX_VIEW_SELF")).booleanValue();
        boolean hasSetRight = !viewSelf;

        try {
            String infomation;
            try {
                sqlClause = new StringBuffer(1000);
                whereClause = new StringBuffer(1000);
                subClause = new StringBuffer(1000);
                String e = super.baseTool.getCurrentUser();
                hFrameUser.put(e, e);
                hExFrameUser.remove(e);
                aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
                aSender = PublicLib.findReplace(aSender, "\'", "\'\'");
                aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
                aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
                infomation = "";

                for (int whereInfo = 0; whereInfo < PublicLib.getFieldCount(aFlowFolder, ","); ++whereInfo) {
                    if (!infomation.equals("")) {
                        infomation = String.valueOf(String.valueOf(infomation)).concat(",");
                    }

                    infomation = String.valueOf(String.valueOf(infomation)).concat("?");
                }

                whereClause.append(" where CC.fini_flag is not null and CC.fini_flag>=0 and CC.step_type<>5");
                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("")) {
                    whereClause.append(" and AA.flow_id=TT.item_seq");
                }

                whereClause.append(" and AA.if_efec<>2");
                whereClause.append(" and ");
                if (aTaskType.equals("-1")) {
                    if (aReadFlag == (long) 0) {
                        whereClause.append("(AA.if_end is null or AA.if_end=0)");
                    } else if (aReadFlag == (long) 1) {
                        whereClause.append("(AA.if_end is not null and AA.if_end=1)");
                    } else {
                        whereClause.append("(1=1)");
                    }

                    if (aClassId.equals("-1")) {
                        whereClause.append(" and not exists(select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and AA.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=?) ");
                        sqlPara.add(e);
                    } else if (!aClassId.equals("")) {
                        whereClause.append(" and exists (select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and AA.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=? and doc_class.class_id=?)");
                        sqlPara.add(e);
                        sqlPara.add(aClassId, 1);
                    }
                } else if (aTaskType.equals("0")) {
                    if ((long) 0 == aReadFlag) {
                        whereClause.append(" (CC.fini_flag=0 or (CC.fini_flag=3 and CC.read_flag=0))");
                    } else if ((long) 1 == aReadFlag) {
                        whereClause.append(" (CC.fini_flag=0 and CC.step_type=1)");
                    } else {
                        whereClause.append(" (CC.fini_flag=0)");
                    }
                } else if (aTaskType.equals("1")) {
                    if ((long) 0 == aReadFlag) {
                        whereClause.append(" (CC.fini_flag=1 and (AA.if_end is null or AA.if_end=0))");
                    } else {
                        whereClause.append("(CC.fini_flag=1)");
                    }
                } else if (aTaskType.equals("3")) {
                    if ((long) 0 == aReadFlag) {
                        whereClause.append(" (CC.fini_flag=3 and CC.read_flag=0)");
                    } else if ((long) 1 == aReadFlag) {
                        whereClause.append("(CC.fini_flag=3 and CC.read_flag=1)");
                    } else {
                        whereClause.append("(CC.fini_flag=3)");
                    }
                } else if (aTaskType.equals("4")) {
                    if ((long) 0 == aReadFlag) {
                        whereClause.append("((CC.fini_flag=0 or (CC.fini_flag=3 and CC.read_flag=0)) and CC.step_type<>1)");
                    } else if ((long) 1 == aReadFlag) {
                        whereClause.append("((CC.fini_flag=1 or (CC.fini_flag=3 and CC.read_flag=1)) and CC.step_type<>1)");
                    } else {
                        whereClause.append("(CC.step_type<>1)");
                    }
                } else {
                    whereClause.append("1=2");
                }

                if (aCondition != null && !aCondition.equals("")) {
                    whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(aCondition).append(")"))));
                }

                String[][] var52 = new String[][]{{" (AA.inst_crea like ? or DD2.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aCreator)))).append(",").append(aCreator))), String.valueOf(0)}, {" AA.inst_crda>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" AA.inst_crda<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" AA.inst_fida>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" AA.inst_fida<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" TT.item_name like ? ", aFlowName, String.valueOf(0)}, {" AA.fins_name like ? ", aInstName, String.valueOf(0)}, {" (CC.peri_man like ? or DD.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aSender)))).append(",").append(aSender))), String.valueOf(0)}, {String.valueOf(String.valueOf((new StringBuffer(" TT.para_seq in (")).append(infomation).append(")"))), aFlowFolder, String.valueOf(0)}, {" AA.doc_seq like ? ", aDocSeq, String.valueOf(0)}};

                String stationList;
                for (int temp = 0; temp < var52.length; ++temp) {
                    if (var52[temp][1] != null && !var52[temp][1].equals("") && !var52[temp][1].equals(",")) {
                        whereClause.append(" and ");
                        whereClause.append(var52[temp][0]);

                        for (int it = 0; it < PublicLib.getFieldCount(var52[temp][1], ","); ++it) {
                            stationList = PublicLib.getField(var52[temp][1], ",", (long) it);
                            if (var52[temp][0].indexOf(" like ") != -1) {
                                stationList = String.valueOf(String.valueOf((new StringBuffer("%")).append(stationList).append("%")));
                            }

                            sqlPara.add(stationList, Integer.parseInt(var52[temp][2]));
                        }
                    }
                }

                String var54 = "";
                Object var53 = null;
                stationList = "";
                if (hasSetRight) {
                    if (viewAll) {
                        if (hExFrameUser.size() > 0) {
                            whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and CC.deal_man not in (")).append(PublicLib.buildInList(hExFrameUser, sqlPara)).append(")"))));
                        }

                        if (hExFlowId.size() > 0) {
                            whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id not in (")).append(PublicLib.buildInList(hExFlowId, sqlPara)).append(")"))));
                        }
                    } else if (viewDept) {
                        if (hFrameUser.size() > 0) {
                            whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and CC.deal_man in (")).append(PublicLib.buildInList(hFrameUser, sqlPara)).append(")"))));
                            if (hFlowId.size() > 0) {
                                whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id in (")).append(PublicLib.buildInList(hFlowId, sqlPara)).append(")"))));
                            } else if (hExFlowId.size() > 0) {
                                whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id not in (")).append(PublicLib.buildInList(hExFlowId, sqlPara)).append(")"))));
                            }
                        } else {
                            whereClause.append(" and 1=2");
                        }
                    } else {
                        if (hFrameUser.size() > 0) {
                            whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and CC.deal_man in (")).append(PublicLib.buildInList(hFrameUser, sqlPara)).append(")"))));
                        }

                        if (hFlowId.size() > 0) {
                            whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id in (")).append(PublicLib.buildInList(hFlowId, sqlPara)).append(")"))));
                        } else if (hExFlowId.size() > 0) {
                            whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and AA.flow_id not in (")).append(PublicLib.buildInList(hExFlowId, sqlPara)).append(")"))));
                        }
                    }
                } else {
                    whereClause.append(" and CC.deal_man =? ");
                    sqlPara.add(e);
                }

                whereClause.append(" and CC.step_type<>4 and CC.del_flag=0");
                if (whereClause.substring(0).indexOf("AA.") != -1) {
                    whereClause.append(" and AA.flow_inid=CC.flow_inid");
                }

                whereClause.append(" group by CC.flow_inid ) ");
                if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                    whereClause.append(" max_deal_info ");
                }

                subClause.append(" (select max(CC.step_index) ");
                if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                    subClause.append(" as max_step_index");
                }

                subClause.append(" from deal_info CC");
                if (aSender != null && !aSender.equals("")) {
                    subClause.append(" left join base_user DD on CC.peri_man=DD.user_code");
                }

                if (whereClause.substring(0).indexOf("AA.") != -1) {
                    subClause.append(",flow_inst AA");
                }

                if (aCreator != null && !aCreator.equals("")) {
                    subClause.append(" left join base_user DD2 on AA.inst_crea=DD2.user_code");
                }

                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("")) {
                    subClause.append(",tree_def TT");
                }

                subClause.append(whereClause.toString());
                sqlClause.append(" from flow_inst A");
                if (aSortField.indexOf("B2.step_name") != -1) {
                    sqlClause.append(" left join step_inst B2 on A.curr_step=B2.step_inco");
                }

                sqlClause.append(",deal_info C");
                if (aSortField.indexOf("D.full_name") != -1) {
                    sqlClause.append(" left join base_user D on C.peri_man=D.user_code");
                }

                if (aSortField == null || aSortField.equals("") || aSortField.indexOf("B.crea_date") != -1) {
                    sqlClause.append(",step_inst B");
                }

                if (aSortField.indexOf("T.item_name") != -1) {
                    sqlClause.append(",tree_def T");
                }

                if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                    sqlClause.append(",");
                    sqlClause.append(subClause);
                }

                sqlClause.append(" where A.flow_inid=C.flow_inid ");
                if (aSortField == null || aSortField.equals("") || aSortField.indexOf("B.crea_date") != -1) {
                    sqlClause.append(" and B.step_inco=C.step_inco");
                }

                if (aSortField.indexOf("T.item_name") != -1) {
                    sqlClause.append(" and T.item_seq=A.flow_id");
                }

                sqlClause.append(" and (A.del_flag is null or A.del_flag=0)");
                if (super.baseTool.getDBServerType() == PublicLib.Type_Oracle) {
                    sqlClause.append(" and C.step_index=max_deal_info.max_step_index");
                } else if (super.baseTool.getDBServerType() == PublicLib.Type_Sybase || super.baseTool.getDBServerType() == PublicLib.Type_DB2) {
                    sqlClause.append(" and C.step_index in");
                    sqlClause.append(subClause);
                }

                if (!belongIn.equals("") && belongIn.indexOf(",") == -1) {
                    sqlClause.append(" and C.deal_man=?");
                    sqlPara.add(e);
                }

                String countSql = "select count(*) as CNT ".concat(String.valueOf(String.valueOf(sqlClause.substring(0))));
                sqlClause.insert(0, " select A.flow_inid ");
                if (aSortField != null && !aSortField.equals("")) {
                    sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(aSortField))));
                } else {
                    sqlClause.append(" order by C.rece_date desc ");
                }

                long totalRow = 0L;
                super.baseTool.debug("getTaskQueryCount.SQL=".concat(String.valueOf(String.valueOf(sqlClause))));
                crs = super.baseTool.getRowSet(countSql, "", sqlPara);
                if (crs.next()) {
                    totalRow = crs.getLong("CNT");
                }

                PublicLib.closeCachedRowSet(crs);
                crs = null;
                if (totalRow > (long) 0) {
                    String sql = super.baseTool.getSQLStatementFormer().genPagingStatement(sqlClause.substring(0), pageNo, pageSize);
                    crs = super.baseTool.getRowSet(sql, "", sqlPara);
                    pagingData.setPageSize((long) pageSize);
                    pagingData.setCurrentPageNo(pageNo);
                    pagingData.setTotalRow(totalRow);
                    pagingData.setPagingData(crs);
                }
            } catch (Exception var50) {
                infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
                PublicLib.log(true, "getTaskQueryCount", infomation, var50);
                throw new Exception(var50.getMessage());
            }
        } finally {
            sqlClause = null;
            whereClause = null;
            subClause = null;
            hFrameUser = null;
            hFlowId = null;
            hExFrameUser = null;
            hExFlowId = null;
            PublicLib.clearObject(hRight);
        }

        return pagingData;
    }

    public CachedRowSet getDeptAdminTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq, String aSortField, String aClassId) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        StringBuffer whereClause = null;
        String belongIn = "";
        SQLPara sqlPara = new SQLPara();

        try {
            IReturn e = this._$37559();
            SQLPara var37 = (SQLPara) e.getReturnObject("return_sqlpara");
            String authSql = e.getReturn("return_sql", "");
            sqlClause = new StringBuffer(1000);
            whereClause = new StringBuffer(1000);
            if (var37 != null && var37.size() > 0) {
                sqlPara.add(var37);
                String currentUser = super.baseTool.getCurrentUser();
                aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
                aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
                aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
                String flowFolderPara = "";

                for (int whereInfo = 0; whereInfo < PublicLib.getFieldCount(aFlowFolder, ","); ++whereInfo) {
                    if (!flowFolderPara.equals("")) {
                        flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat(",");
                    }

                    flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat("?");
                }

                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("") || aSortField.indexOf("T.item_name") != -1) {
                    whereClause.append(" and A.flow_id=T.item_seq");
                }

                whereClause.append(" and A.if_efec<>2");
                if (aTaskType.equals("-1")) {
                    if (aReadFlag == (long) 0) {
                        whereClause.append(" and (A.if_end is null or A.if_end=0)");
                    } else if (aReadFlag == (long) 1) {
                        whereClause.append(" and (A.if_end is not null and A.if_end=1)");
                    }

                    if (aClassId.equals("-1")) {
                        whereClause.append(" and not exists(select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and A.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=?) ");
                        sqlPara.add(currentUser);
                    } else if (!aClassId.equals("")) {
                        whereClause.append(" and exists (select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and A.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=? and doc_class.class_id=?)");
                        sqlPara.add(currentUser);
                        sqlPara.add(aClassId, 1);
                    }
                }

                if (aCondition != null && !aCondition.equals("")) {
                    String var39 = PublicLib.findReplace(aCondition, "AA.", "A.");
                    whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(var39).append(")"))));
                }

                String[][] var38 = new String[][]{{" (A.inst_crea like ? or D.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aCreator)))).append(",").append(aCreator))), String.valueOf(0)}, {" A.inst_crda>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_crda<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" A.inst_fida>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_fida<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" T.item_name like ? ", aFlowName, String.valueOf(0)}, {" A.fins_name like ? ", aInstName, String.valueOf(0)}, {String.valueOf(String.valueOf((new StringBuffer(" T.para_seq in (")).append(flowFolderPara).append(")"))), aFlowFolder, String.valueOf(0)}, {" A.doc_seq like ? ", aDocSeq, String.valueOf(0)}};
                int i = 0;

                while (true) {
                    if (i >= var38.length) {
                        sqlClause.append(" select A.flow_inid from flow_inst A");
                        sqlClause.append(" left join step_inst B on A.curr_step=B.step_inco");
                        if (aCreator != null && !aCreator.equals("")) {
                            sqlClause.append(" left join base_user D on A.inst_crea=D.user_code");
                        }

                        if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("") || aSortField.indexOf("T.item_name") != -1) {
                            sqlClause.append(",tree_def T");
                        }

                        sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" where A.flow_inid in(")).append(authSql).append(") and (A.del_flag is null or A.del_flag=0)"))));
                        sqlClause.append(whereClause);
                        if (aSortField != null && !aSortField.equals("")) {
                            sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(aSortField))));
                            break;
                        }

                        if (sqlClause.indexOf("step_inst B2") != -1) {
                            sqlClause.append(" order by B2.crea_date desc ");
                        } else if (sqlClause.indexOf("step_inst B") != -1) {
                            sqlClause.append(" order by B.crea_date desc ");
                        }
                        break;
                    }

                    if (var38[i][1] != null && !var38[i][1].equals("") && !var38[i][1].equals(",")) {
                        whereClause.append(" and ");
                        whereClause.append(var38[i][0]);

                        for (int j = 0; j < PublicLib.getFieldCount(var38[i][1], ","); ++j) {
                            String value = PublicLib.getField(var38[i][1], ",", (long) j);
                            if (var38[i][0].indexOf(" like ") != -1) {
                                value = String.valueOf(String.valueOf((new StringBuffer("%")).append(value).append("%")));
                            }

                            sqlPara.add(value, Integer.parseInt(var38[i][2]));
                        }
                    }

                    ++i;
                }
            } else {
                sqlClause.append("select flow_inid from flow_inst where flow_inid=?");
                sqlPara.add("ABCDEFG");
            }

            super.baseTool.debug("getDeptAdminTaskCount.SQL=".concat(String.valueOf(String.valueOf(sqlClause))));
            crs = super.baseTool.getRowSet(sqlClause.substring(0), "", sqlPara);
        } catch (Exception var35) {
            String infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "getDeptAdminTaskCount", infomation, var35);
            throw new Exception(var35.getMessage());
        } finally {
            sqlClause = null;
            whereClause = null;
        }

        return crs;
    }

    private IReturn _$37559() throws Exception {
        StringBuffer sbRet = new StringBuffer("");
        SQLPara paraRet = new SQLPara();
        ReturnImpl iRet = new ReturnImpl();
        if (!super.baseTool.isDeptAdmin()) {
            return iRet;
        } else {
            label48:
            try {
                try {
                    String ex = this.getDeptAdminCanManageDepts();
                    String strWhere = "";
                    if (ex.trim().length() == 0) {
                        sbRet.setLength(0);
                    } else {
                        paraRet.add(super.baseTool.getCurrentUser());
                        strWhere = PublicLib.buildPreparedStatementIn(paraRet, "temp_ud.fram_code", ex, 500, ";");
                        if (strWhere != null && strWhere.trim().length() > 0) {
                            sbRet.append("select distinct temp_d.flow_inid from deal_info temp_d left join user_dept temp_ud on temp_d.deal_man=temp_ud.user_code ");
                            sbRet.append("where temp_d.deal_man is not null and ");
                            sbRet.append(String.valueOf(String.valueOf((new StringBuffer("(temp_d.deal_man=? or ")).append(strWhere).append(")"))));
                        }
                    }
                } catch (Exception var10) {
                    var10.printStackTrace();
                    throw var10;
                }
            } finally {
                break label48;
            }

            iRet.setReturnValue("return_sqlpara", paraRet);
            iRet.setReturnValue("return_sql", sbRet.substring(0));
            return iRet;
        }
    }

    public String getDeptAdminCanManageDepts() throws Exception {
        StringBuffer sbRet = new StringBuffer("");
        new SQLPara();
        if (!super.baseTool.isDeptAdmin()) {
            return "";
        } else {
            StringBuffer sqlClause = new StringBuffer(256);
            sqlClause.append("select distinct fram_seq,fram_code,fram_name from framework where fram_seq in(");
            sqlClause.append("(select fram_seq from framework_right where user_name=? and user_type=0 and deny_acc=0");
            sqlClause.append(")union(");
            sqlClause.append("select fram_seq from framework_right fr ");
            sqlClause.append("where exists (select fram_seq from user_dept ud where ud.user_code=? and fr.user_name=ud.fram_seq) ");
            sqlClause.append("and fr.user_type=1 and fr.deny_acc=0))");
            SQLPara para = new SQLPara();
            para.add(super.baseTool.getCurrentUser());
            para.add(super.baseTool.getCurrentUser());
            CachedRowSet crsD1 = null;
            CachedRowSet crsD2 = null;

            label95:
            try {
                try {
                    crsD1 = super.baseTool.getRowSet(sqlClause.substring(0), "", para);
                    String ex = null;
                    String strWhere = "";
                    para.clear();

                    while (crsD1.next()) {
                        ex = crsD1.getString("fram_code");
                        if (strWhere.length() > 0) {
                            strWhere = String.valueOf(String.valueOf(strWhere)).concat(" or ");
                        }

                        strWhere = String.valueOf(String.valueOf(strWhere)).concat("fram_code like ?");
                        para.add(String.valueOf(String.valueOf(ex)).concat("%"));
                    }

                    if (para.size() > 0) {
                        sqlClause.setLength(0);
                        sqlClause.append("select * from framedepartment");
                        sqlClause.append(" where ".concat(String.valueOf(String.valueOf(strWhere))));
                        crsD2 = super.baseTool.getRowSet(sqlClause.substring(0), "", para);

                        while (crsD2.next()) {
                            ex = crsD2.getString("fram_code");
                            if (ex != null) {
                                if (sbRet.length() > 0) {
                                    sbRet.append(";");
                                }

                                sbRet.append(ex);
                            }
                        }
                    }
                } catch (Exception var13) {
                    var13.printStackTrace();
                    throw var13;
                }
            } finally {
                break label95;
            }

            PublicLib.closeCachedRowSet(crsD1);
            crsD1 = null;
            PublicLib.closeCachedRowSet(crsD2);
            crsD2 = null;
            return sbRet.substring(0);
        }
    }

    public PagingData getDeptAdminTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, long pageNo, int pageSize) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        String dataHeader = "";
        String countHeader = "";
        StringBuffer whereClause = null;
        SQLPara sqlPara = new SQLPara();
        PagingData pagingData = new PagingData();

        try {
            IReturn e = this._$37559();
            SQLPara var42 = (SQLPara) e.getReturnObject("return_sqlpara");
            String authSql = e.getReturn("return_sql", "");
            sqlClause = new StringBuffer(1000);
            String countSql = "";
            whereClause = new StringBuffer(1000);
            if (var42 == null || var42.size() <= 0) {
                pagingData.setTotalRow(0L);
                PagingData var44 = pagingData;
                return var44;
            }

            sqlPara.add(var42);
            String totalRow = super.baseTool.getCurrentUser();
            aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
            aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
            aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
            String flowFolderPara = "";

            for (int sql = 0; sql < PublicLib.getFieldCount(aFlowFolder, ","); ++sql) {
                if (!flowFolderPara.equals("")) {
                    flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat(",");
                }

                flowFolderPara = String.valueOf(String.valueOf(flowFolderPara)).concat("?");
            }

            if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("") || aSortField.indexOf("T.item_name") != -1) {
                whereClause.append(" and A.flow_id=T.item_seq");
            }

            whereClause.append(" and A.if_efec<>2");
            if (aTaskType.equals("-1")) {
                if (aReadFlag == (long) 0) {
                    whereClause.append(" and (A.if_end is null or A.if_end=0)");
                } else if (aReadFlag == (long) 1) {
                    whereClause.append(" and (A.if_end is not null and A.if_end=1)");
                }

                if (aClassId.equals("-1")) {
                    whereClause.append(" and not exists(select doc_class.flow_inid from class_def,doc_class");
                    whereClause.append(" where doc_class.class_id=class_def.class_id");
                    whereClause.append(" and A.flow_inid=doc_class.flow_inid");
                    whereClause.append(" and class_def.user_code=?) ");
                    sqlPara.add(totalRow);
                } else if (!aClassId.equals("")) {
                    whereClause.append(" and exists (select doc_class.flow_inid from class_def,doc_class");
                    whereClause.append(" where doc_class.class_id=class_def.class_id");
                    whereClause.append(" and A.flow_inid=doc_class.flow_inid");
                    whereClause.append(" and class_def.user_code=? and doc_class.class_id=?)");
                    sqlPara.add(totalRow);
                    sqlPara.add(aClassId, 1);
                }
            }

            String var46;
            if (aCondition != null && !aCondition.equals("")) {
                var46 = PublicLib.findReplace(aCondition, "AA.", "A.");
                whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(var46).append(")"))));
            }

            String[][] var45 = new String[][]{{" (A.inst_crea like ? or D.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aCreator)))).append(",").append(aCreator))), String.valueOf(0)}, {" A.inst_crda>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_crda<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" A.inst_fida>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_fida<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" T.item_name like ? ", aFlowName, String.valueOf(0)}, {" A.fins_name like ? ", aInstName, String.valueOf(0)}, {String.valueOf(String.valueOf((new StringBuffer(" T.para_seq in (")).append(flowFolderPara).append(")"))), aFlowFolder, String.valueOf(0)}, {" A.doc_seq like ? ", aDocSeq, String.valueOf(0)}};

            for (int i = 0; i < var45.length; ++i) {
                if (var45[i][1] != null && !var45[i][1].equals("") && !var45[i][1].equals(",")) {
                    whereClause.append(" and ");
                    whereClause.append(var45[i][0]);

                    for (int j = 0; j < PublicLib.getFieldCount(var45[i][1], ","); ++j) {
                        String value = PublicLib.getField(var45[i][1], ",", (long) j);
                        if (var45[i][0].indexOf(" like ") != -1) {
                            value = String.valueOf(String.valueOf((new StringBuffer("%")).append(value).append("%")));
                        }

                        sqlPara.add(value, Integer.parseInt(var45[i][2]));
                    }
                }
            }

            sqlClause.append(" left join step_inst B on A.curr_step=B.step_inco");
            if (aCreator != null && !aCreator.equals("")) {
                sqlClause.append(" left join base_user D on A.inst_crea=D.user_code");
            }

            if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("") || aSortField.indexOf("T.item_name") != -1) {
                sqlClause.append(",tree_def T");
            }

            dataHeader = " select A.flow_inid from flow_inst A";
            countHeader = " select count(*) as CNT from flow_inst A";
            sqlClause.append(String.valueOf(String.valueOf((new StringBuffer(" where A.flow_inid in(")).append(authSql).append(") and (A.del_flag is null or A.del_flag=0)"))));
            sqlClause.append(whereClause);
            countSql = String.valueOf(String.valueOf(countHeader)).concat(String.valueOf(String.valueOf(sqlClause.substring(0))));
            sqlClause.insert(0, dataHeader);
            if (aSortField != null && !aSortField.equals("")) {
                sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(aSortField))));
            } else if (sqlClause.indexOf("step_inst B2") != -1) {
                sqlClause.append(" order by B2.crea_date desc ");
            } else if (sqlClause.indexOf("step_inst B") != -1) {
                sqlClause.append(" order by B.crea_date desc ");
            }

            super.baseTool.debug("getDeptAdminTaskCount.count(*).SQL=".concat(String.valueOf(String.valueOf(countSql))));
            super.baseTool.debug("getDeptAdminTaskCount.data.SQL=".concat(String.valueOf(String.valueOf(sqlClause.substring(0)))));
            long var43 = 0L;
            crs = super.baseTool.getRowSet(countSql, "", sqlPara);
            if (crs.next()) {
                var43 = crs.getLong("CNT");
            }

            PublicLib.closeCachedRowSet(crs);
            crs = null;
            if (var43 > (long) 0) {
                var46 = super.baseTool.getSQLStatementFormer().genPagingStatement(sqlClause.substring(0), pageNo, pageSize);
                crs = super.baseTool.getRowSet(var46, "", sqlPara);
                pagingData.setCurrentPageNo(pageNo);
                pagingData.setPageSize((long) pageSize);
                pagingData.setTotalRow(var43);
                pagingData.setPagingData(crs);
            }
        } catch (Exception var40) {
            String infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "getDeptAdminTaskCount", infomation, var40);
            throw new Exception(var40.getMessage());
        } finally {
            sqlClause = null;
            whereClause = null;
        }

        return pagingData;
    }

    public PagingData getAdminTaskCount(String aCondition, String aTaskType, long aReadFlag, String aCreator, String aBeginDateLower, String aBeginDateHigher, String aEndDateLower, String aEndDateHigher, String aFlowName, String aInstName, String aFlowFolder, String aDocSeq, String aSortField, String aClassId, long pageNo, int pageSize) throws Exception {
        CachedRowSet crs = null;
        StringBuffer sqlClause = null;
        String countHeader = null;
        String dataHeader = null;
        StringBuffer whereClause = null;
        SQLPara sqlPara = new SQLPara();
        PagingData pagingData = new PagingData();
        String deal_info = "deal_info";
        String step_inst = "step_inst";
        String flow_inst = "flow_inst";
        if (!"".equals(aBeginDateHigher) && aBeginDateHigher.compareTo(histDate) <= 0) {
            deal_info = "deal_info_history";
            step_inst = "step_inst_history";
            flow_inst = "flow_inst_history";
        }

        try {
            String infomation;
            try {
                sqlClause = new StringBuffer(1000);
                whereClause = new StringBuffer(1000);
                String e = super.baseTool.getCurrentUser();
                aCreator = PublicLib.findReplace(aCreator, "\'", "\'\'");
                aInstName = PublicLib.findReplace(aInstName, "\'", "\'\'");
                aDocSeq = PublicLib.findReplace(aDocSeq, "\'", "\'\'");
                infomation = "";

                for (int whereInfo = 0; whereInfo < PublicLib.getFieldCount(aFlowFolder, ","); ++whereInfo) {
                    if (!infomation.equals("")) {
                        infomation = String.valueOf(String.valueOf(infomation)).concat(",");
                    }

                    infomation = String.valueOf(String.valueOf(infomation)).concat("?");
                }

                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("") || aSortField.indexOf("T.item_name") != -1) {
                    whereClause.append(" and A.flow_id=T.item_seq");
                }

                whereClause.append(" and A.if_efec<>2");
                if (aTaskType.equals("-1")) {
                    switch ((int) aReadFlag) {
                        case 0:
                            whereClause.append(" and (A.if_end is null or A.if_end=0)");
                            break;
                        case 1:
                            whereClause.append(" and (A.if_end is not null and A.if_end=1)");
                            break;
                        case 2:
                            whereClause.append(" and (A.if_end is null or A.if_end=0)");
                    }

                    if (aClassId.equals("-1")) {
                        whereClause.append(" and not exists(select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and A.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=?) ");
                        sqlPara.add(e);
                    } else if (!aClassId.equals("")) {
                        whereClause.append(" and exists (select doc_class.flow_inid from class_def,doc_class");
                        whereClause.append(" where doc_class.class_id=class_def.class_id");
                        whereClause.append(" and A.flow_inid=doc_class.flow_inid");
                        whereClause.append(" and class_def.user_code=? and doc_class.class_id=?)");
                        sqlPara.add(e);
                        sqlPara.add(aClassId, 1);
                    }
                }

                if (aCondition != null && !aCondition.equals("")) {
                    String var40 = PublicLib.findReplace(aCondition, "AA.", "A.");
                    whereClause.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(var40).append(")"))));
                }

                String[][] var42 = new String[][]{{" (A.inst_crea like ? or D.full_name like ? ) ", String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(aCreator)))).append(",").append(aCreator))), String.valueOf(0)}, {" A.inst_crda>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_crda<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aBeginDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" A.inst_fida>? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateLower)).concat(" 0:0:0")), String.valueOf(2)}, {" A.inst_fida<? ", PublicLib.getTimestamp(String.valueOf(String.valueOf(aEndDateHigher)).concat(" 23:59:59")), String.valueOf(2)}, {" T.item_name like ? ", aFlowName, String.valueOf(0)}, {" A.fins_name like ? ", aInstName, String.valueOf(0)}, {String.valueOf(String.valueOf((new StringBuffer(" T.para_seq in (")).append(infomation).append(")"))), aFlowFolder, String.valueOf(0)}, {" A.doc_seq like ? ", aDocSeq, String.valueOf(0)}};

                String sql;
                for (int dataSql = 0; dataSql < var42.length; ++dataSql) {
                    if (var42[dataSql][1] != null && !var42[dataSql][1].equals("") && !var42[dataSql][1].equals(",")) {
                        whereClause.append(" and ");
                        whereClause.append(var42[dataSql][0]);

                        for (int countSql = 0; countSql < PublicLib.getFieldCount(var42[dataSql][1], ","); ++countSql) {
                            sql = PublicLib.getField(var42[dataSql][1], ",", (long) countSql);
                            if (var42[dataSql][0].indexOf(" like ") != -1) {
                                sql = String.valueOf(String.valueOf((new StringBuffer("%")).append(sql).append("%")));
                            }

                            sqlPara.add(sql, Integer.parseInt(var42[dataSql][2]));
                        }
                    }
                }

                dataHeader = " select A.flow_inid from " + flow_inst + " A";
                countHeader = " select count(*) as CNT from " + flow_inst + " A";
                sqlClause.append(" left join " + step_inst + " B on A.curr_step=B.step_inco");
                if (aCreator != null && !aCreator.equals("")) {
                    sqlClause.append(" left join base_user D on A.inst_crea=D.user_code");
                }

                if (aFlowName != null && !aFlowName.equals("") || aFlowFolder != null && !aFlowFolder.equals("") || aSortField.indexOf("T.item_name") != -1) {
                    sqlClause.append(",tree_def T");
                }

                sqlClause.append(" where (A.del_flag is null or A.del_flag=0)");
                sqlClause.append(whereClause);
                String var43 = "";
                String var41 = String.valueOf(String.valueOf(countHeader)).concat(String.valueOf(String.valueOf(sqlClause.substring(0))));
                if (aSortField != null && !aSortField.equals("")) {
                    sqlClause.append(" order by ".concat(String.valueOf(String.valueOf(aSortField))));
                } else if (sqlClause.indexOf("step_inst B2") != -1) {
                    sqlClause.append(" order by B2.crea_date desc ");
                } else if (sqlClause.indexOf("step_inst B") != -1) {
                    sqlClause.append(" order by B.crea_date desc ");
                }

                var43 = String.valueOf(String.valueOf(dataHeader)).concat(String.valueOf(String.valueOf(sqlClause.substring(0))));
                super.baseTool.debug("getAdminTaskCount.count(*).SQL=".concat(String.valueOf(String.valueOf(var41))));
                super.baseTool.debug("getAdminTaskCount.data.SQL=".concat(String.valueOf(String.valueOf(var43))));
                sql = super.baseTool.getSQLStatementFormer().genPagingStatement(var43, pageNo, pageSize);
                crs = super.baseTool.getRowSet(var41, "", sqlPara);
                long totalRow = 0L;
                if (crs.next()) {
                    totalRow = crs.getLong("CNT");
                }

                PublicLib.closeCachedRowSet(crs);
                crs = null;
                if (totalRow > (long) 0) {
                    crs = super.baseTool.getRowSet(sql, "", sqlPara);
                    pagingData.setPagingData(crs);
                    pagingData.setTotalRow(totalRow);
                    pagingData.setCurrentPageNo(pageNo);
                    pagingData.setPageSize((long) pageSize);
                }
            } catch (Exception var38) {
                infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
                PublicLib.log(true, "getAdminTaskCount", infomation, var38);
                throw new Exception(var38.getMessage());
            }
        } finally {
            sqlClause = null;
            whereClause = null;
        }

        return pagingData;
    }

    public CachedRowSet getHideTask() throws Exception {
        CachedRowSet crs = null;

        try {
            String e = super.baseTool.getCurrentUser();
            boolean infomation1 = super.baseTool.isDeptAdmin();
            String sqlClause = "";
            SQLPara sqlPara = new SQLPara();
            sqlPara.add(e);
            if (!infomation1) {
                sqlClause = " select A.fins_name,A.flow_name,  B.step_name,B.flow_inid,B.step_inco,B.crea_date,  C.peri_man,C.fini_flag ,C.read_flag,C.agen_men,C.agen_flag,C.deal_man,C.deal_indx, D.full_name  from flow_inst A,step_inst B,deal_info C left join base_user D on C.peri_man=D.user_code where A.flow_inid=B.flow_inid and B.step_inco=C.step_inco  and C.del_flag=1 and (A.del_flag is null or A.del_flag=0)  and C.deal_man=? order by B.crea_date desc ";
            } else {
                String strWhere = PublicLib.buildPreparedStatementIn(sqlPara, "ud.fram_code", this.getDeptAdminCanManageDepts(), 500, ";");
                sqlClause = " select A.fins_name,A.flow_name,  B.step_name,B.flow_inid,B.step_inco,B.crea_date,  C.peri_man,C.fini_flag ,C.read_flag,C.agen_men,C.agen_flag,C.deal_man,C.deal_indx, D.full_name  from flow_inst A,step_inst B,deal_info C left join base_user D on C.peri_man=D.user_code left join user_dept ud on C.deal_man=ud.user_code where A.flow_inid=B.flow_inid and B.step_inco=C.step_inco  and C.del_flag=1 and (A.del_flag is null or A.del_flag=0)  and (C.deal_man=?";
                if (strWhere.trim().length() > 0) {
                    sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(String.valueOf(String.valueOf(" or ".concat(String.valueOf(String.valueOf(strWhere))))));
                }

                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(") order by B.crea_date desc ");
            }

            crs = super.baseTool.getRowSet(sqlClause, "", sqlPara);
        } catch (Exception var11) {
            String infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "getHideTask", infomation, var11);
            throw new Exception(var11.getMessage());
        } finally {
            ;
        }

        return crs;
    }

    public CachedRowSet getHideFlowInst() throws Exception {
        CachedRowSet crs = null;

        try {
            String e = super.baseTool.getCurrentUser();
            boolean infomation1 = super.baseTool.isAdmin();
            if (!infomation1) {
                throw new Exception("您不是系统管理员,操作不允许。");
            }

            String sqlClause = " select F.flow_inid,F.flow_name,F.fins_name,F.doc_seq,F.inst_crea,F.inst_crda,F.inst_fida,F.if_end,S.step_name,U.full_name from flow_inst F left join base_user U on F.inst_crea=U.user_code left join step_inst S on F.curr_step=S.step_inco where F.del_flag=1  order by F.inst_crda desc ";
            crs = super.baseTool.getRowSet(sqlClause, "", new SQLPara());
        } catch (Exception var9) {
            String infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "getHideFlowInst", infomation, var9);
            throw new Exception(var9.getMessage());
        } finally {
            ;
        }

        return crs;
    }

    public CachedRowSet getFlowTrace(long aFlowInid, boolean containNextActionName) throws Exception {
        CachedRowSet crs = null;

        try {
            String infomation;
            try {
                String e = "";
                e = " select F.fins_name,F.if_sub,e.step_name,S.flow_inid,S.step_inco,e.step_colour,D.fsub_inid,F.para_inid,F.para_inco,D.fsub_flag, D.choi_cont,D.deal_man,D.step_type,D.fini_flag,D.deal_comm,D.deal_note,D.rece_date,D.save_date,D.fini_date,D.open_date,D.read_flag,D.pre_deal_idx,D.deal_indx,D.agen_men,D.agen_flag,D.regi_flag,D.peri_man,D.fw_man,D.del_flag,D.allograph,D.deal_dept, U.full_name,S.peri_inco,S.next_inco,e.misc_set as step_struct,f2.misc_set as flow_struct from flow_inst F,step_inst S,deal_info D left join base_user U on d.deal_man=u.user_code,step_def e,flow_def F2 where f.flow_inid=s.flow_inid and s.flow_inid=d.flow_inid and s.step_inco=d.step_inco and D.fini_flag <> -1 and D.fini_flag is not null and s.flow_id=e.flow_id and s.step_code=e.step_code and F.flow_id=F2.flow_id and S.flow_inid=? order by S.step_inco,D.deal_indx";
                infomation = "select F.fins_name,F.if_sub,e.step_name,S.flow_inid,S.step_inco,e.step_colour,D.fsub_inid,F.para_inid,F.para_inco,D.fsub_flag, D.choi_cont,D.deal_man,D.step_type,D.fini_flag,D.deal_comm,D.deal_note,D.rece_date,D.save_date,D.fini_date,D.open_date,D.read_flag,D.pre_deal_idx,D.deal_indx,D.agen_men,D.agen_flag,D.regi_flag,D.peri_man,D.fw_man,D.del_flag,D.allograph,D.deal_dept, U.full_name,S.peri_inco,S.next_inco,e.misc_set as step_struct,f2.misc_set as flow_struct,ac.acti_name from flow_inst F,step_def e,flow_def F2,deal_info D  left join base_user U on d.deal_man=u.user_code left join step_inst S on d.step_inco=S.step_inco left join step_inst S1 on S.next_inco=S1.step_inco left join action ac on ( ac.flow_id=S.flow_id and ac.step_code=S.step_code and S1.step_code=ac.step_dest) where f.flow_inid=s.flow_inid and s.flow_inid=d.flow_inid and s.step_inco=d.step_inco and D.fini_flag <> -1 and D.fini_flag is not null and s.flow_id=e.flow_id and s.step_code=e.step_code and F.flow_id=F2.flow_id and S.flow_inid=? order by S.step_inco,D.deal_indx";
                SQLPara sqlPara = new SQLPara();
                sqlPara.add(String.valueOf(aFlowInid));
                crs = super.baseTool.getRowSet(containNextActionName ? infomation : e, "", sqlPara);
            } catch (Exception var12) {
                infomation = String.valueOf(String.valueOf((new StringBuffer("流程实例id=")).append(aFlowInid).append(",登陆用户=").append(super.baseTool.getCurrentUser())));
                PublicLib.log(true, "getFlowTrace", infomation, var12);
                throw new Exception(var12.getMessage());
            }
        } finally {
            ;
        }

        return crs;
    }

    public CachedRowSet getTasks(String aUserList) throws Exception {
        Connection conn = null;
        CachedRowSet crs = null;

        try {
            String infomation;
            try {
                if (aUserList == null || aUserList.equals("")) {
                    throw new Exception("UserName is empty!");
                }

                String e = "";
                infomation = PublicLib.findReplace(aUserList, ",", "\',\'");
                e = String.valueOf(String.valueOf((new StringBuffer(" select A.fins_name,A.flow_inid,  B.step_name,B.step_inco, C.deal_indx,C.deal_man,C.rece_date,A.total_term,C.deal_term from flow_inst A,step_inst B,deal_info C where A.flow_inid=B.flow_inid and B.step_inco=C.step_inco  and C.fini_flag is not null and (A.del_flag is null or A.del_flag=0) and (C.del_flag=0)  and C.fini_flag=0 and (C.deal_man is not null) and B.step_type<>5 and C.deal_man in(\'")).append(infomation).append("\')").append(" order by C.rece_date desc,B.step_inco desc  ")));
                String firstUser = PublicLib.getField(aUserList, ",", 0L);
                firstUser = PublicLib.findReplace(firstUser, "\'", "");
                String connectionString = super.baseTool.getBaseConnectionString(firstUser);
                conn = super.baseTool.getConnectionFrom(connectionString);
                crs = new CachedRowSet();
                crs.setCommand(e);
                crs.setConcurrency(1007);
                crs.setType(1004);
                crs.execute(conn);
            } catch (Exception var12) {
                infomation = String.valueOf(String.valueOf((new StringBuffer("用户列表=")).append(aUserList).append(",登陆用户=").append(super.baseTool.getCurrentUser())));
                PublicLib.log(true, "getTasks", infomation, var12);
                throw new Exception(var12.getMessage());
            }
        } finally {
            PublicLib.closeConnection(conn);
            conn = null;
        }

        return crs;
    }

    public void hideTask(String struct) throws Exception {
        Connection conn = null;
        CachedRowSet crs = null;

        try {
            String e = super.baseTool.getCurrentUser();
            long var20 = (long) PublicLib.getFieldCount(struct, ";");
            String whereClause = "";
            SQLPara sqlPara = new SQLPara();

            String baseConnectionString;
            for (int sqlClause = 0; (long) sqlClause < var20; ++sqlClause) {
                baseConnectionString = PublicLib.getField(struct, ";", (long) sqlClause);
                String flowInid = PublicLib.getField(baseConnectionString, ",", 0L);
                String stepInid = PublicLib.getField(baseConnectionString, ",", 1L);
                String dealMan = PublicLib.getField(baseConnectionString, ",", 2L);
                if (e.equalsIgnoreCase(dealMan)) {
                    if (!whereClause.equals("")) {
                        whereClause = String.valueOf(String.valueOf(whereClause)).concat(" or ");
                    }

                    whereClause = String.valueOf(String.valueOf(whereClause)).concat(" (flow_inid=? and deal_man=?) ");
                    sqlPara.add(String.valueOf(flowInid));
                    sqlPara.add(dealMan);
                }
            }

            if (whereClause.trim().length() != 0) {
                String var21 = "select * from deal_info where ".concat(String.valueOf(String.valueOf(whereClause)));
                crs = super.baseTool.getRowSet(var21, "deal_info", sqlPara);

                while (crs.next()) {
                    baseConnectionString = crs.getString("del_flag");
                    if (baseConnectionString == null || baseConnectionString.equals("") || baseConnectionString.equals("0")) {
                        crs.updateLong("del_flag", 1L);
                        crs.updateRow();
                    }
                }

                baseConnectionString = super.baseTool.getBaseConnectionString();
                conn = super.baseTool.getConnectionFrom(baseConnectionString);
                conn.setAutoCommit(false);
                crs.acceptChanges(conn);
                return;
            }
        } catch (Exception var18) {
            String infomation = String.valueOf(String.valueOf((new StringBuffer("隐藏公文结构=")).append(struct).append(",登陆用户=").append(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "hideTask", infomation, var18);
            throw new Exception(var18.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
            PublicLib.closeConnection(conn);
            conn = null;
        }

    }

    public void deleteTask(String aStruct) throws Exception {
        Connection conn = null;
        CachedRowSet crs = null;

        try {
            String e = super.baseTool.getCurrentUser();
            long var23 = (long) PublicLib.getFieldCount(aStruct, ";");
            String queryWhereClause = "";
            String updateWhereClause = "";
            String sqlClause = "";
            String tmpStruct = "";
            SQLPara sqlPara = new SQLPara();

            for (int baseConnectionString = 0; (long) baseConnectionString < var23; ++baseConnectionString) {
                String tmp = PublicLib.getField(aStruct, ";", (long) baseConnectionString);
                String flowInid = PublicLib.getField(tmp, ",", 0L);
                String stepInid = PublicLib.getField(tmp, ",", 1L);
                String dealMan = PublicLib.getField(tmp, ",", 2L);
                if (e.equalsIgnoreCase(dealMan)) {
                    if (!updateWhereClause.equals("")) {
                        updateWhereClause = String.valueOf(String.valueOf(updateWhereClause)).concat(" or ");
                        queryWhereClause = String.valueOf(String.valueOf(queryWhereClause)).concat(" or ");
                    }

                    queryWhereClause = String.valueOf(String.valueOf(queryWhereClause)).concat(" (flow_inid=? and deal_man=?) ");
                    updateWhereClause = String.valueOf(String.valueOf(updateWhereClause)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("(flow_inid=\'")).append(flowInid).append("\' and deal_man=\'").append(dealMan).append("\')"))))));
                    sqlPara.add(String.valueOf(flowInid));
                    sqlPara.add(dealMan);
                }
            }

            if (queryWhereClause.trim().length() == 0) {
                return;
            }

            sqlClause = String.valueOf(String.valueOf((new StringBuffer("select * from deal_info where step_type=1 and fini_flag=0 and (")).append(queryWhereClause).append(")")));

            String var24;
            for (crs = super.baseTool.getRowSet(sqlClause, "", sqlPara); crs.next(); tmpStruct = String.valueOf(String.valueOf(tmpStruct)).concat(String.valueOf(String.valueOf(var24)))) {
                var24 = crs.getString("flow_inid");
                if (!tmpStruct.equals("")) {
                    tmpStruct = String.valueOf(String.valueOf(tmpStruct)).concat(";");
                }
            }

            sqlClause = "update deal_info set del_flag=2 where".concat(String.valueOf(String.valueOf(updateWhereClause)));
            var24 = super.baseTool.getBaseConnectionString();
            conn = super.baseTool.getConnectionFrom(var24);
            conn.setAutoCommit(false);
            super.baseTool.executeSQL(conn, sqlClause);
            super.baseTool.getFlowLib().innerDeleteFlowInst(tmpStruct, conn, false);
            conn.commit();
        } catch (Exception var21) {
            String infomation = String.valueOf(String.valueOf((new StringBuffer("删除公文结构=")).append(aStruct).append(",登陆用户=").append(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "deleteTask", infomation, var21);
            if (conn != null) {
                conn.rollback();
            }

            throw var21;
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
            PublicLib.closeConnection(conn);
            conn = null;
        }

    }

    public void restoreTask(String struct) throws Exception {
        Connection conn = null;

        try {
            String e = super.baseTool.getCurrentUser();
            long var18 = (long) PublicLib.getFieldCount(struct, ";");
            String whereClause = "";

            String baseConnectionString;
            for (int sqlClause = 0; (long) sqlClause < var18; ++sqlClause) {
                baseConnectionString = PublicLib.getField(struct, ";", (long) sqlClause);
                String flowInid = PublicLib.getField(baseConnectionString, ",", 0L);
                String stepInid = PublicLib.getField(baseConnectionString, ",", 1L);
                String dealMan = PublicLib.getField(baseConnectionString, ",", 2L);
                if (!whereClause.equals("")) {
                    whereClause = String.valueOf(String.valueOf(whereClause)).concat(" or ");
                }

                whereClause = String.valueOf(String.valueOf(whereClause)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" (flow_inid=\'")).append(flowInid).append("\'").append(" and deal_man=\'").append(dealMan).append("\') "))))));
            }

            String var19 = "update deal_info set del_flag=0 where".concat(String.valueOf(String.valueOf(whereClause)));
            baseConnectionString = super.baseTool.getBaseConnectionString();
            conn = super.baseTool.getConnectionFrom(baseConnectionString);
            conn.setAutoCommit(false);
            super.baseTool.executeSQL(conn, var19);
            conn.commit();
        } catch (Exception var16) {
            if (conn != null) {
                conn.rollback();
            }

            String infomation = String.valueOf(String.valueOf((new StringBuffer("恢复公文结构=")).append(struct).append(",登陆用户=").append(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "restoreTask", infomation, var16);
            throw new Exception(var16.getMessage());
        } finally {
            PublicLib.closeConnection(conn);
            conn = null;
        }

    }

    public void callbackAgentTask(String aAgentMan) throws Exception {
        Connection conn = null;
        PreparedStatement pstm = null;

        try {
            String e = super.baseTool.getCurrentUser();
            String sqlClause = "update deal_info set deal_man=?,agen_men=?,agen_flag=0  where (fini_flag=0 or fini_flag=-1 or fini_flag=3 ) and agen_men=?";
            String baseConnectionString = super.baseTool.getBaseConnectionString();
            conn = super.baseTool.getConnectionFrom(baseConnectionString);
            conn.setAutoCommit(false);
            pstm = conn.prepareStatement(sqlClause);
            pstm.setString(1, e);
            pstm.setString(2, "");
            pstm.setString(3, e);
            pstm.executeUpdate();
            conn.commit();
        } catch (Exception var12) {
            conn.rollback();
            String infomation = "登陆用户=".concat(String.valueOf(String.valueOf(super.baseTool.getCurrentUser())));
            PublicLib.log(true, "callbackAgentTask", infomation, var12);
            throw new Exception(var12.getMessage());
        } finally {
            PublicLib.closePreparedStatement(pstm);
            pstm = null;
            PublicLib.closeConnection(conn);
            conn = null;
        }

    }

    public CachedRowSet getAllographTaskCount(Map parameters) throws Exception {
        CachedRowSet crsRet = null;
        String condition = "";
        SQLPara para = new SQLPara();
        String sql;
        if (parameters != null) {
            sql = (String) parameters.get("flow_inst_name");
            String docSeq = (String) parameters.get("doc_seq");
            String receFrom = (String) parameters.get("rece_from");
            String receTo = (String) parameters.get("rece_to");
            if (sql != null && sql.trim().length() > 0) {
                condition = " f.fins_name like ?";
                para.add(String.valueOf(String.valueOf((new StringBuffer("%")).append(sql).append("%"))));
            }

            if (docSeq != null && docSeq.trim().length() > 0) {
                if (condition.trim().length() > 0) {
                    condition = String.valueOf(String.valueOf(condition)).concat(" and ");
                }

                condition = String.valueOf(String.valueOf(condition)).concat(" f.doc_seq like ?");
                para.add(String.valueOf(String.valueOf((new StringBuffer("%")).append(docSeq).append("%"))));
            }

            if ((receFrom != null || receTo != null) && (receFrom.trim().length() > 0 || receTo.trim().length() > 0)) {
                String receStr = "";
                if (receFrom != null && receFrom.trim().length() > 0) {
                    receStr = String.valueOf(String.valueOf(receStr)).concat("d.rece_date>?");
                    para.add(PublicLib.getTimestamp(String.valueOf(String.valueOf(receFrom)).concat(" 0:0:0")), 2);
                }

                if (receTo != null && receTo.trim().length() > 0) {
                    if (receStr.trim().length() > 0) {
                        receStr = String.valueOf(String.valueOf(receStr)).concat(" and ");
                    }

                    receStr = String.valueOf(String.valueOf(receStr)).concat("d.rece_date<?");
                    para.add(PublicLib.getTimestamp(String.valueOf(String.valueOf(receTo)).concat(" 23:59:59")), 2);
                }

                if (receStr.trim().length() > 0) {
                    if (condition.trim().length() > 0) {
                        condition = String.valueOf(String.valueOf(condition)).concat(" and ");
                    }

                    condition = String.valueOf(String.valueOf(condition)).concat("(");
                    condition = String.valueOf(String.valueOf(condition)).concat(String.valueOf(String.valueOf(receStr)));
                    condition = String.valueOf(String.valueOf(condition)).concat(")");
                }
            }
        }

        sql = String.valueOf(String.valueOf((new StringBuffer("select d.flow_inid,d.step_inco,d.deal_indx from deal_info d ,flow_inst f where f.flow_inid=d.flow_inid ")).append(condition.trim().length() > 0 ? " and ".concat(String.valueOf(String.valueOf(condition))) : "").append(" and (d.fini_flag=0 or (d.fini_flag=3 and d.read_flag=0))").append(" and exists(select * from deal_info x where d.flow_inid=x.flow_inid and x.step_type in(1,2) and x.deal_man=?)").append(" and d.step_type in (1,2) and d.deal_man in (select allograph_user_code from wf_allograph_def").append(" where user_code=? and user_type=0) order by d.rece_date desc")));
        para.add(this.baseTool.getCurrentUser());
        para.add(this.baseTool.getCurrentUser());
        crsRet = this.baseTool.getRowSet(sql, "", para);
        return crsRet;
    }

    public CachedRowSet getAllographTask(Map parameters) throws Exception {
        SQLStatementFormer former = this.baseTool.getSQLStatementFormer();
        String totalDelay = former.isNull(String.valueOf(String.valueOf((new StringBuffer("A.total_term-(")).append(former.isNull("C.fini_date", former.getSysTimestamp())).append("-C.rece_date)*24*60"))), "0");
        String dealDelay = former.isNull(String.valueOf(String.valueOf((new StringBuffer("C.deal_term-(")).append(former.isNull("C.fini_date", former.getSysTimestamp())).append("-C.rece_date)*24*60"))), "0");
        StringBuffer sb = new StringBuffer("");
        sb.append("select A.flow_inid,A.fins_name,A.doc_seq,A.worklist_itemex1,A.worklist_itemex2,A.worklist_itemex3,");
        sb.append(" A.worklist_itemex4,A.worklist_itemex5,T.item_name as flow_name,A.awoke_flag,A.sys_urge,B2.flow_id,");
        sb.append("B2.step_code,B2.crea_date,B2.step_name,C.peri_man,C.fini_date,C.deal_man,C.rece_date,C.step_inco,");
        sb.append(String.valueOf(String.valueOf((new StringBuffer("C.step_type,C.deal_indx,")).append(totalDelay).append(" as total_delay,").append(dealDelay).append(" as deal_delay,A.total_term,C.deal_term,"))));
        sb.append("D.full_name as deal_man_name");
        sb.append(" from flow_inst A ");
        sb.append(" left join step_inst B2 on A.curr_step=B2.step_inco ");
        sb.append(" left join tree_def T on T.item_seq=A.flow_id,");
        sb.append(" deal_info C left join base_user D on C.deal_man=D.user_code");
        sb.append(" where A.flow_inid=C.flow_inid");
        String condition = (String) parameters.get("para_condition");
        if (condition == null) {
            condition = "";
        }

        if (condition.trim().length() > 0) {
            sb.append(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(condition).append(")"))));
        }

        sb.append(" order by C.rece_date desc");
        String sqlClause = sb.substring(0);
        this.baseTool.debug("getAllographTask.SQL=".concat(String.valueOf(String.valueOf(sqlClause))));
        CachedRowSet crsRet = null;
        crsRet = this.baseTool.getRowSet(sqlClause, "", new SQLPara());
        return crsRet;
    }
}

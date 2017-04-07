//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package flowengineforjava.operator;

import com.minstone.util.DateTimeUtils;
import com.minstone.util.SQLPara;
import com.minstone.util.StringSplitStru;
import com.minstone.util.cache.SimpleCache;
import flowengineforjava.operator.FormControlHtmlDataImpl;
import flowengineforjava.script.formscript.FormExpressionScript;
import flowengineforjava.structure.CondStruct;
import flowengineforjava.structure.ControlDataStruct;
import flowengineforjava.structure.FileSafeStruct;
import flowengineforjava.structure.FlowInstInfo;
import flowengineforjava.structure.FlowMiscSet;
import flowengineforjava.structure.FormControlDataStruct;
import flowengineforjava.structure.FormControlHtmlData;
import flowengineforjava.structure.FormInstData;
import flowengineforjava.structure.FormStruct;
import flowengineforjava.structure.FormTabPage;
import flowengineforjava.structure.KeyItem;
import flowengineforjava.structure.MtblItem;
import flowengineforjava.structure.MtblStruct;
import flowengineforjava.structure.MyAttach;
import flowengineforjava.structure.MyButton;
import flowengineforjava.structure.MyCheckBox;
import flowengineforjava.structure.MyCombo;
import flowengineforjava.structure.MyControl;
import flowengineforjava.structure.MyControlData;
import flowengineforjava.structure.MyHyperLink;
import flowengineforjava.structure.MyListItem;
import flowengineforjava.structure.MyListStruct;
import flowengineforjava.structure.MyMap;
import flowengineforjava.structure.MyPen;
import flowengineforjava.structure.MyPrgForm;
import flowengineforjava.structure.MyPrintInfo;
import flowengineforjava.structure.MyRadio;
import flowengineforjava.structure.MySign;
import flowengineforjava.structure.MyStamp;
import flowengineforjava.structure.MySubForm;
import flowengineforjava.structure.MyText;
import flowengineforjava.structure.MyTextArea;
import flowengineforjava.structure.NADSelectRange;
import flowengineforjava.structure.NADSelectRangeParser;
import flowengineforjava.structure.RelationValidStruct;
import flowengineforjava.structure.SignDataStruct;
import flowengineforjava.structure.SignItem;
import flowengineforjava.structure.StepMiscStruct;
import flowengineforjava.structure.UserDefineSignFormat;
import flowengineforjava.structure.ValidListItem;
import flowengineforjava.structure.ValidStruct;
import flowengineforjava.util.IBaseTool;
import flowengineforjava.util.PublicLib;
import flowengineforjava.util.RecordSet;

import java.io.InputStream;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Timestamp;
import java.util.Date;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;

import sun.jdbc.rowset.CachedRowSet;

public class FormLib {
    protected IBaseTool baseTool;
    private static Hashtable _$13453 = new Hashtable();
    private static long minFlowInid = 371208;//313837;  //最小

    public FormLib() {
    }

    public void restoreSomeInfo(FlowInstInfo fii, boolean aReadOnly, CachedRowSet aDealInfoRs, CachedRowSet aFlowRs, Hashtable aReturnStruct, int startMode) throws Exception {
        CachedRowSet attachRs = null;
        RecordSet someInfoRs = new RecordSet();
        Object mapAttachRight = new Hashtable();
        CachedRowSet flowDefRs = null;
        CachedRowSet tempRs = null;
        long totalTerm = fii.getTotalTerm();
        String sFlowInid = null;

        try {
            String e = "";
            String currentUser = this.baseTool.getCurrentUser();
            String flowId = aFlowRs.getString("flow_id");
            boolean hasSignData = false;
            if (!fii.isNewInst()) {
                sFlowInid = aFlowRs.getString("flow_inid");
            }

            String sql = "select misc_set,total_term from flow_def where flow_id=?";
            SQLPara para = new SQLPara();
            para.add(flowId);
            flowDefRs = this.baseTool.getRowSet(sql, "", para);
            flowDefRs.beforeFirst();
            flowDefRs.next();
            para.clear();
            para = null;
            FlowMiscSet fs = new FlowMiscSet();
            fs.restore(flowDefRs.getString("misc_set"));
            someInfoRs.insertRow();
            someInfoRs.putObject("__flOw_Choice___tiTle__", fs.getChoiceTitle());
            someInfoRs.putObject("__fLow_appEnd_tiT_le__", fs.getAppendTitle());
            someInfoRs.putObject("__flow_notice____Title__", fs.getNoticeTitle());
            someInfoRs.putObject("_Flo_w____tot_alt_erm_enabl_ed___", new Boolean(fs.isTotalTermEnabled()));
            someInfoRs.putObject("++F___LO___WDe__f_____MISc________", fs);
            if (fs.isTotalTermEnabled()) {
                if (fii.isNewInst()) {
                    totalTerm = flowDefRs.getLong("total_term");
                }

                someInfoRs.putObject("_Flo_w____tot_alt_er___m___", new Long(totalTerm));
            }

            String[][] miscStructXml;
            String adviseDoc;
            String noteUsers;
            String userName;
            if (PublicLib.getRecordCount(aFlowRs) > 0) {
                miscStructXml = new String[][]{{"send_titl", "__flow_SeNd____Title__", "VARCHAR"}, {"step_desc", "step_desc", "VARCHAR"}, {"step_name", "step_name", "VARCHAR"}, {"step_type", "step_type", "INTEGER"}, {"step_man", "step_man", "VARCHAR"}, {"time_set", "time_set", "VARCHAR"}, {"if_mark", "if_mark", "INTEGER"}, {"mark_path", "mark_path", "VARCHAR"}, {"if_doc", "doc_flag", "INTEGER"}, {"doc_name", "doc_name", "VARCHAR"}, {"form_code", "form_code", "VARCHAR"}, {"step_code", "step_code", "INTEGER"}, {"flow_id", "flow_id", "VARCHAR"}};
                PublicLib.CopyRowSetToRecordSet(aFlowRs, someInfoRs, miscStructXml);
                adviseDoc = aFlowRs.getString("step_choi");
                noteUsers = "";
                if (adviseDoc != null && !adviseDoc.equals("")) {
                    int popedom = PublicLib.getFieldCount(adviseDoc, ";");

                    for (int i = 0; i < popedom; ++i) {
                        userName = PublicLib.getField(adviseDoc, ";", (long) i);
                        noteUsers = String.valueOf(String.valueOf(noteUsers)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<OPTION value=\"")).append(i).append("\""))))));
                        noteUsers = String.valueOf(String.valueOf(noteUsers)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(">")).append(userName).append("</OPTION>"))))));
                    }

                    switch (startMode) {
                        case 0:
                            someInfoRs.putObject("step_choi", adviseDoc);
                            break;
                        case 1:
                            someInfoRs.putObject("step_choi", "0");
                            break;
                        default:
                            someInfoRs.putObject("step_choi", "0");
                    }
                }

                someInfoRs.putObject("choi_list", noteUsers);
            }

            String var36;
            long var40;
            if (!fii.isNewInst()) {
                if (PublicLib.getRecordCount(aDealInfoRs) > 0) {
                    miscStructXml = new String[][]{{"deal_comm", "deal_comm", "VARCHAR"}, {"time_set", "time_set", "VARCHAR"}, {"deal_indx", "deal_indx", "INTEGER"}, {"curr_man", "curr_man", "VARCHAR"}, {"note_man", "note_man", "VARCHAR"}};
                    PublicLib.CopyRowSetToRecordSet(aDealInfoRs, someInfoRs, miscStructXml);
                }

                someInfoRs.putObject("_fLOW__S_tAT_e_", String.valueOf(aFlowRs.getInt("if_efec")));
                var36 = "select count(*) as cnt from deal_info where ca_sign_state=2 and flow_inid=?";
                SQLPara var38 = new SQLPara();
                var38.add(sFlowInid);
                if (Long.parseLong(sFlowInid) < minFlowInid) {
                    var36 = var36.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                            .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                            .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                }
                tempRs = this.baseTool.getRowSet(var36, "", var38);
                var40 = 0L;
                if (tempRs.next()) {
                    var40 = tempRs.getLong(1);
                }

                hasSignData = var40 > (long) 0;
            } else {
                someInfoRs.putObject("deal_indx", "0");
            }

            if (aFlowRs.getLong("if_doc") == (long) 1) {
                e = " select * from SYS_FILE2";
                SQLPara var37 = new SQLPara();
                if (!fii.isNewInst()) {
                    e = String.valueOf(String.valueOf(e)).concat(" where flow_inid=?");
                    var37.add(String.valueOf(fii.getFlowInid()));
                } else {
                    e = String.valueOf(String.valueOf(e)).concat(" where 1=2");
                }

                e = String.valueOf(String.valueOf(e)).concat(" order by FILE_INID");
                attachRs = this.baseTool.getRowSet(e, "SYS_FILE2", var37);
                mapAttachRight = this.baseTool.getFlowLib().readAttachRightSet(fii.getFlowInid());
                adviseDoc = aFlowRs.getString("man_stru");
                FileSafeStruct var42 = new FileSafeStruct();
                var42.restore(adviseDoc);
                long var41 = (long) var42.getStepRight();
                someInfoRs.putObject("popedom", String.valueOf(var41));
                userName = this.baseTool.getUserName(currentUser);
                someInfoRs.putObject("user_name", userName);
                String docName = aFlowRs.getString("doc_name");
                if (docName == null || docName.equals("")) {
                    String clearStatus = this.baseTool.getTagValue("WF/TEXT");
                    if (clearStatus == null || clearStatus.equals("")) {
                        System.out.println("WF/TEXT tag not exist");
                    }

                    someInfoRs.putObject("doc_name", clearStatus);
                }

                if (!fii.isNewInst()) {
                    int var43 = aFlowRs.getInt("clear_status");
                    someInfoRs.putObject("clear_status", String.valueOf(var43));
                }
            }

            if (aFlowRs.getLong("if_mark") == (long) 1) {
                long var39 = aFlowRs.getLong("step_code");
                var40 = aFlowRs.getLong("mark_step");
                if (var39 != var40) {
                    someInfoRs.putObject("if_mark", "0");
                }
            }

            if (aReadOnly) {
                someInfoRs.putObject("readOnly", "1");
            } else {
                someInfoRs.putObject("readOnly", "0");
            }

            var36 = aFlowRs.getString("misc_set");
            if (var36 == null) {
                var36 = "";
            }

            someInfoRs.putObject("misc_set", var36);
            if (aFlowRs.getInt("step_type") == 3) {
                adviseDoc = this.baseTool.getTagValue("WF/ADVISEDOC");
                if (adviseDoc != null && adviseDoc.trim().equals("1")) {
                    someInfoRs.putObject("advise", "1");
                } else {
                    noteUsers = this.baseTool.getFlowLib().getNoteUsers(fii.getFlowInid());
                    if (!noteUsers.equals("")) {
                        someInfoRs.putObject("advise", "1");
                    }
                }
            }

            someInfoRs.putObject("__签名sign_ca_HAs_Si__GN_T__aG____", new Boolean(hasSignData));
            aReturnStruct.put("someInfoRs", someInfoRs);
            if (attachRs != null) {
                aReturnStruct.put("attachRs", attachRs);
                someInfoRs.putObject("__At__tac__hR__ight__Rs", mapAttachRight);
            }
        } catch (Exception var34) {
            PublicLib.closeCachedRowSet(flowDefRs);
            flowDefRs = null;
            PublicLib.closeCachedRowSet(attachRs);
            attachRs = null;
            this.baseTool.error("restoreSomeInfo", var34);
            throw new Exception(var34.getMessage());
        } finally {
        }

    }

    public String restoreHtml(boolean aNewFlag, boolean aReadOnly, CachedRowSet aFlowRs, CachedRowSet aFormRs, CachedRowSet aDealInfoRs, Hashtable aReturnStruct, CachedRowSet aMainRs) throws Exception {
        CachedRowSet mainRs = aMainRs;
        StringBuffer formHtml = null;

        try {
            String e = "";
            String formCode = aFlowRs.getString("form_code");
            if (formCode == null || formCode.equals("")) {
                e = "步骤设置没指定表单,请检查流程设计。";
                throw new Exception(e);
            }

            String serverName = aFlowRs.getString("svr_name");
            serverName = PublicLib.getField(serverName, "<", 0L);
            String criteria = "form_code=".concat(String.valueOf(String.valueOf(formCode)));
            boolean rsFinded = PublicLib.find(aFormRs, criteria, true);
            if (!rsFinded) {
                throw new Exception("系统找不到指定的表单");
            }

            StringBuffer rawHtml = new StringBuffer(PublicLib.getStringFromClob(aFormRs, "form_cont"));
            rawHtml = PublicLib.findReplace(rawHtml, "&lt;", "<", false);
            rawHtml = PublicLib.findReplace(rawHtml, "&gt;", ">", false);
            rawHtml = PublicLib.findReplace(rawHtml, "&amp;", "&", false);
            StringBuffer formStructXml = new StringBuffer(PublicLib.getStringFromClob(aFormRs, "form_stru"));
            formStructXml = PublicLib.findReplace(formStructXml, "&lt;", "@lt;", false);
            formStructXml = PublicLib.findReplace(formStructXml, "&gt;", "@gt;", false);
            String fsetId = aFormRs.getString("fset_id");
            FormStruct formStruct = new FormStruct();
            formStruct.restore(formStructXml.toString());
            String tableName = formStruct.getSysInfo().getBindTable();
            tableName = PublicLib.getField(tableName, "<", 0L);
            String validStructXml = PublicLib.getStringFromClob(aFlowRs, "vali_set");
            ValidStruct validStruct = new ValidStruct();
            validStruct.restore(validStructXml);
            long flowInid = -1L;
            long stepInid = -1L;
            String stepName = "";
            if (!aNewFlag) {
                flowInid = aFlowRs.getLong("flow_inid");
                stepInid = aFlowRs.getLong("step_inco");
                stepName = aFlowRs.getString("step_name");
            } else {
                stepName = aFlowRs.getString("step_name");
            }

            String stepTag = aFlowRs.getString("step_tag");
            if (stepTag == null) {
                stepTag = "";
            }

            if (stepTag.trim().length() == 0) {
                stepTag = stepName;
            }

            mainRs.first();
            if (!aReadOnly) {
                this.baseTool.getFlowLib().stepEventSet((Connection) null, "ente_set", mainRs, aDealInfoRs, aFlowRs, "", "");
            }

            int stepType = aFlowRs.getInt("step_type");
            long totalTerm = aFlowRs.getLong("total_term");
            FlowInstInfo fii = new FlowInstInfo(flowInid, stepInid, -1L, aNewFlag);
            fii.setStepType(stepType);
            fii.setTotalTerm(totalTerm);
            String miscStructXml = aFlowRs.getString("misc_set");
            StepMiscStruct miscSet = new StepMiscStruct();
            miscSet.restore(miscStructXml);
            fii.setStepMiscStruct(miscSet);
            FormInstData formInstData = new FormInstData();
            formInstData.setFormStruct(formStruct);
            formHtml = this.restoreControlHtml(fii, aReadOnly, false, fsetId, stepTag, rawHtml, serverName, formInstData, validStruct, mainRs, aReturnStruct);
            formHtml = this.restoreSubForm(aReadOnly, formHtml, formStruct, aFormRs, mainRs, serverName, validStruct, aReturnStruct);
            RecordSet fieldMapNameRs = (RecordSet) aReturnStruct.remove("fieldMapNameRs");
            this.restoreRelationForm(fii, aReadOnly, formStruct, aFormRs, mainRs, serverName, validStruct, aReturnStruct);
            aReturnStruct.put("fieldMapNameRs", fieldMapNameRs);
            aReturnStruct.put("___0-3__48_f_uct_FormStruct___", formStruct);
            aReturnStruct.put("0_9UAad_form_iNst_Data__", formInstData);
            aReturnStruct.put("formHtml", formHtml.substring(0));
            aReturnStruct.put("mainRs", mainRs);
        } catch (Exception var40) {
            PublicLib.closeCachedRowSet(mainRs);
            mainRs = null;
            this.baseTool.error("restoreHtml", var40);
            throw new Exception(var40.getMessage());
        } finally {
            ;
        }

        return formHtml.toString();
    }

    public String restoreHtml(boolean aNewFlag, boolean aReadOnly, CachedRowSet aFlowRs, CachedRowSet aFormRs, CachedRowSet aDealInfoRs, Hashtable aReturnStruct) throws Exception {
        long flowInid = aFlowRs.getLong("flow_inid");
        long stepInid = aFlowRs.getLong("step_inco");
        int stepType = aFlowRs.getInt("step_type");
        long totalTerm = aFlowRs.getLong("total_term");
        FlowInstInfo fii = new FlowInstInfo(flowInid, stepInid, -1L, aNewFlag);
        fii.setStepType(stepType);
        fii.setTotalTerm(totalTerm);
        String miscStructXml = aFlowRs.getString("misc_set");
        StepMiscStruct miscSet = new StepMiscStruct();
        miscSet.restore(miscStructXml);
        fii.setStepMiscStruct(miscSet);
        return this.restoreHtml(fii, aReadOnly, aFlowRs, aFormRs, aDealInfoRs, aReturnStruct, (Map) null);
    }

    public String restoreHtml(FlowInstInfo fii, boolean aReadOnly, CachedRowSet aFlowRs, CachedRowSet aFormRs, CachedRowSet aDealInfoRs, Hashtable aReturnStruct, Map initData) throws Exception {
        CachedRowSet mainRs = null;
        StringBuffer formHtml = null;

        try {
            String e = "";
            aFlowRs.first();
            String formCode = aFlowRs.getString("form_code");
            if (formCode == null || formCode.equals("")) {
                e = "步骤设置没指定表单,请检查流程设计。";
                throw new Exception(e);
            }

            String serverName = aFlowRs.getString("svr_name");
            serverName = PublicLib.getField(serverName, "<", 0L);
            String criteria = "form_code=".concat(String.valueOf(String.valueOf(formCode)));
            boolean rsFinded = PublicLib.find(aFormRs, criteria, true);
            if (!rsFinded) {
                throw new Exception("系统找不到指定的表单");
            }

            StringBuffer rawHtml = new StringBuffer(PublicLib.getStringFromClob(aFormRs, "form_cont"));
            rawHtml = PublicLib.findReplace(rawHtml, "&lt;", "<", false);
            rawHtml = PublicLib.findReplace(rawHtml, "&gt;", ">", false);
            rawHtml = PublicLib.findReplace(rawHtml, "&amp;", "&", false);
            StringBuffer formStructXml = new StringBuffer(PublicLib.getStringFromClob(aFormRs, "form_stru"));
            formStructXml = PublicLib.findReplace(formStructXml, "&lt;", "@lt;", false);
            formStructXml = PublicLib.findReplace(formStructXml, "&gt;", "@gt;", false);
            String fsetId = aFormRs.getString("fset_id");
            FormStruct formStruct = new FormStruct();
            formStruct.restore(formStructXml.toString());
            String tableName = formStruct.getSysInfo().getBindTable();
            tableName = PublicLib.getField(tableName, "<", 0L);
            String validStructXml = PublicLib.getStringFromClob(aFlowRs, "vali_set");
            ValidStruct validStruct = new ValidStruct();
            validStruct.restore(validStructXml);
            String flowInid = "";
            String stepInid = "";
            String stepName = "";
            String sqlClause = String.valueOf(String.valueOf((new StringBuffer("select * from ")).append(tableName).append(" where ")));
            SQLPara sqlPara = new SQLPara();
            String stepTag;
            if (!fii.isNewInst()) {
                flowInid = aFlowRs.getString("flow_inid");
                stepInid = aFlowRs.getString("step_inco");
                stepName = aFlowRs.getString("step_name");
                stepTag = aFlowRs.getString("mtbl_stru");
                MtblStruct formInstData = new MtblStruct();
                formInstData.restore(stepTag);
                Hashtable fieldMapNameRs = formInstData.getKeyColl();

                for (int i = 0; i < fieldMapNameRs.size(); ++i) {
                    if (i > 0) {
                        sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and ");
                    }

                    MtblItem mtblItem = (MtblItem) fieldMapNameRs.get(String.valueOf(i));
                    sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(mtblItem.getFieldName())).concat("=?"))));
                    if (mtblItem.getFieldType() >= (long) 1 && mtblItem.getFieldType() <= (long) 8) {
                        sqlPara.add(mtblItem.getFieldValue(), 1);
                    } else {
                        sqlPara.add(mtblItem.getFieldValue());
                    }
                }
            } else {
                stepName = aFlowRs.getString("step_name");
                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" 1=2 ");
            }

            stepTag = aFlowRs.getString("step_tag");
            if (stepTag == null) {
                stepTag = "";
            }

            if (stepTag.trim().length() == 0) {
                stepTag = stepName;
            }

            this.baseTool.info("sqlClause=".concat(String.valueOf(String.valueOf(sqlClause))));
            mainRs = this.baseTool.getServerRowSet(serverName, sqlClause, tableName.toUpperCase(), sqlPara);
            if (fii.isNewInst()) {
                boolean var40 = false;
                if (aFlowRs.getString("key_fld").equals("0")) {
                    var40 = true;
                }

                this.baseTool.genMainKey(var40, serverName, tableName, mainRs);
            }

            mainRs.first();
            if (initData != null && initData.size() > 0) {
                try {
                    com.minstone.util.PublicLib.updateCachedRowSet(mainRs, initData);
                } catch (Exception var37) {
                    var37.printStackTrace();
                }
            }

            if (!aReadOnly) {
                this.baseTool.getFlowLib().stepEventSet((Connection) null, "ente_set", mainRs, aDealInfoRs, aFlowRs, "", "");
            }

            FormInstData var41 = new FormInstData();
            var41.setFormStruct(formStruct);
            fii.setFormInitData(initData);
            formHtml = this.restoreControlHtml(fii, aReadOnly, false, fsetId, stepTag, rawHtml, serverName, var41, validStruct, mainRs, aReturnStruct);
            formHtml = this.restoreSubForm(aReadOnly, formHtml, formStruct, aFormRs, mainRs, serverName, validStruct, aReturnStruct);
            RecordSet var42 = (RecordSet) aReturnStruct.remove("fieldMapNameRs");
            this.restoreRelationForm(fii, aReadOnly, formStruct, aFormRs, mainRs, serverName, validStruct, aReturnStruct);
            aReturnStruct.put("fieldMapNameRs", var42);
            aReturnStruct.put("___0-3__48_f_uct_FormStruct___", formStruct);
            aReturnStruct.put("0_9UAad_form_iNst_Data__", var41);
            aReturnStruct.put("formHtml", formHtml.substring(0));
            aReturnStruct.put("mainRs", mainRs);
        } catch (Exception var38) {
            PublicLib.closeCachedRowSet(mainRs);
            mainRs = null;
            this.baseTool.error("restoreHtml", var38);
            throw var38;
        } finally {
            ;
        }

        return formHtml.toString();
    }

    public StringBuffer restoreControlHtml(FlowInstInfo fii, boolean aReadOnly, boolean isPrint, String aFormSetId, String aStepTag, StringBuffer aRawHtml, String aServerName, FormInstData aFormInstData, ValidStruct aValidStruct, CachedRowSet aMainRs, Hashtable aReturnStruct) throws Exception {
        StringBuffer formHtml = null;
        boolean isNewInst = fii.isNewInst();
        String flowInid = fii.getFlowInid() == (long) -1 ? "" : String.valueOf(fii.getFlowInid());
        String stepInid = fii.getStepInid() == (long) -1 ? "" : String.valueOf(fii.getStepInid());
        long dealIndx = fii.getDealIndx();

        try {
            formHtml = new StringBuffer('썐');
            String e = "";
            formHtml.append(aRawHtml);
            RecordSet fieldMapNameRs = new RecordSet();
            RecordSet someInfoRs = (RecordSet) aReturnStruct.get("someInfoRs");
            String tableName = aFormInstData.getFormStruct().getSysInfo().getBindTable();
            tableName = PublicLib.getField(tableName, "<", 0L);
            RecordSet fieldRs = this.baseTool.fieldList(aServerName, tableName);
            Hashtable controlColl = aFormInstData.getFormStruct().getControlColl();
            Enumeration keys = controlColl.keys();
            int dbType = this.baseTool.getDBServerType();
            String primayKey = "";
            if (aMainRs != null && aMainRs.size() > 0) {
                MtblStruct nameList = this.baseTool.getFlowLib().genMtblStruct(aServerName, tableName, aMainRs);
                Hashtable control = nameList.getKeyColl();
                if (control.size() > 0) {
                    MtblItem fieldName = (MtblItem) control.get("0");
                    primayKey = fieldName.getFieldValue();
                }
            }

            while (keys.hasMoreElements()) {
                String nameList1 = (String) keys.nextElement();
                MyControl control1 = (MyControl) controlColl.get(nameList1);
                String fieldName1 = PublicLib.getField(control1.getFieldName(), "<", 0L);
                String fieldDesc = "";
                boolean allowNull = false;
                String disabled = "";
                String fieldValue = "";
                boolean isFormula = false;
                boolean forceUpdate = false;
                long fieldType = control1.getFieldType();
                long controlType = control1.getControlType();
                boolean isDate = fieldType == (long) 4 || fieldType == (long) 5 || fieldType == (long) 6;
                String valid;
                String tag;
                if (fieldName1 != null && fieldName1.trim().length() != 0) {
                    valid = "fld_name=".concat(String.valueOf(String.valueOf(fieldName1)));
                    boolean replace = fieldRs.find(valid);
                    if (replace) {
                        fieldDesc = (String) fieldRs.getObject("fld_desc");
                    }

                    tag = "";
                    if (controlType != (long) 10 && controlType != (long) 13) {
                        tag = control1.getControlName();
                    } else {
                        tag = String.valueOf(String.valueOf(control1.getControlName())).concat("_hidden");
                    }

                    if (!aValidStruct.isEnabled(fieldName1)) {
                        disabled = " disabled ";
                    }

                    aValidStruct.isAllowNull(fieldName1);
                    isFormula = aValidStruct.isFormula(fieldName1);
                    forceUpdate = aValidStruct.isForceUpdate(fieldName1);
                    fieldMapNameRs.insertRow();
                    fieldMapNameRs.putObject("ctl_name", tag);
                    fieldMapNameRs.putObject("fld_name", fieldName1);
                    fieldMapNameRs.putObject("fld_type", String.valueOf(fieldType));
                    fieldMapNameRs.putObject("ctl_type", String.valueOf(controlType));
                    fieldMapNameRs.putObject("db_type", String.valueOf(dbType));
                    if (controlType == (long) 3) {
                        fieldMapNameRs.putObject("fld_flag", "0");
                    } else if ((disabled.equals("") || isFormula) && !aReadOnly) {
                        fieldMapNameRs.putObject("fld_flag", "1");
                        if (!e.equals("")) {
                            e = String.valueOf(String.valueOf(e)).concat("#@#");
                        }

                        e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(fieldName1)))).append(":").append(controlType).append(":").append(control1.getControlName()))))));
                    } else {
                        fieldMapNameRs.putObject("fld_flag", "0");
                    }

                    try {
                        if (PublicLib.getRecordCount(aMainRs) > 0 && aMainRs.getObject(fieldName1) != null) {
                            fieldValue = aMainRs.getString(fieldName1);
                            if (isDate) {
                                fieldValue = PublicLib.getField(fieldValue, ".", 0L);
                            } else if (fieldType == (long) 3) {
                                fieldValue = PublicLib.getNonZeroValue(fieldValue);
                            }
                        }
                    } catch (Exception var65) {
                        System.err.println(String.valueOf(String.valueOf((new StringBuffer("invalid field ")).append(fieldName1).append("\n").append(var65.getMessage()))));
                        var65.printStackTrace();
                        throw var65;
                    }
                }

                if (aReadOnly) {
                    isFormula = false;
                    disabled = " disabled ";
                }

                if (disabled.trim().length() != 0) {
                    allowNull = true;
                }

                valid = "";
                String replace1 = "";
                tag = "";
                long formCode;
                switch ((int) controlType) {
                    case 0:
                    case 8:
                    case 14:
                    default:
                        break;
                    case 1:
                        if (disabled.trim().length() == 0) {
                            valid = aValidStruct.getValidFromControl(fieldName1, fieldDesc, false);
                        } else {
                            disabled = " readonly class=\'form_disable_text\' ";
                        }

                        Map hTextReplace = this._$13517(isNewInst, isFormula, forceUpdate, (MyText) control1, disabled, fieldValue, valid, aServerName, aFormInstData, aReadOnly);
                        replace1 = (String) hTextReplace.get("replace");
                        String hiddenReplace = (String) hTextReplace.get("hiddenReplace");
                        long iDateType = ((MyText) control1).getDateType();
                        if (isDate && iDateType != (long) -1) {
                            tag = String.valueOf(String.valueOf((new StringBuffer("F_l_D__WF__C_t_L=\"")).append(iDateType).append(";").append(control1.getFieldName()).append(";").append(tableName).append("\"")));
                        } else {
                            tag = String.valueOf(String.valueOf((new StringBuffer("F_l_D__WF__C_t_L=\"")).append(control1.getFieldName()).append(";").append(tableName).append("\"")));
                        }

                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, false);
                        if (hiddenReplace.trim().length() > 0) {
                            tag = String.valueOf(String.valueOf((new StringBuffer("D_s_B__WF__C_t_L=\"")).append(control1.getLogicName()).append("\"")));
                            formHtml = PublicLib.findReplace(formHtml, tag, hiddenReplace, false);
                        }
                        break;
                    case 2:
                        tag = String.valueOf(String.valueOf((new StringBuffer("D_s_B__WF__C_t_L=\"")).append(control1.getLogicName()).append("\"")));
                        if (disabled.trim().length() == 0 && fieldType != (long) 4 && fieldType != (long) 5 && fieldType != (long) 6) {
                            replace1 = aValidStruct.getValidFromControl(fieldName1, fieldDesc, false);
                        } else {
                            replace1 = " readonly class=\'form_disable_text\' ";
                        }

                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        tag = String.valueOf(String.valueOf((new StringBuffer(">F_l_D__WF__C_t_L=\"")).append(control1.getFieldName()).append(";").append(tableName).append("\" ")));
                        Hashtable hTextAreaReplace = this._$13524(isNewInst, isFormula, forceUpdate, (MyTextArea) control1, disabled, fieldValue, aReadOnly, aFormInstData);
                        replace1 = (String) hTextAreaReplace.get("replace");
                        String hiddenAreaReplace = (String) hTextAreaReplace.get("hiddenReplace");
                        if (hiddenAreaReplace.trim().length() > 0) {
                            tag = String.valueOf(String.valueOf(tag)).concat("</TEXTAREA>");
                            replace1 = String.valueOf(String.valueOf(replace1)).concat(String.valueOf(String.valueOf("</TEXTAREA>".concat(String.valueOf(String.valueOf(hiddenAreaReplace))))));
                        }

                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, false);
                        break;
                    case 3:
                        tag = String.valueOf(String.valueOf((new StringBuffer("B_t_N__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        if (!aValidStruct.isEnabled(control1.getLogicName())) {
                            disabled = " disabled ";
                        }

                        formCode = aFormInstData.getFormStruct().getSysInfo().getFormCode();
                        replace1 = String.valueOf(String.valueOf(this._$13527(isNewInst, (MyButton) control1, aFormSetId, formCode, aStepTag, controlColl, disabled))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 4:
                        tag = String.valueOf(String.valueOf((new StringBuffer("C_h_K__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        if (disabled.equals("")) {
                            valid = aValidStruct.getValidFromControl(fieldName1, fieldDesc, false);
                        }

                        replace1 = String.valueOf(String.valueOf(this._$13529(isNewInst, (MyCheckBox) control1, disabled, fieldValue, fieldType, valid, false, aServerName))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 5:
                        tag = String.valueOf(String.valueOf((new StringBuffer("R_d_O__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        if (disabled.equals("")) {
                            valid = aValidStruct.getValidFromControl(fieldName1, fieldDesc, false);
                        }

                        replace1 = String.valueOf(String.valueOf(this._$13529(isNewInst, (MyRadio) control1, disabled, fieldValue, fieldType, valid, true, aServerName))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 6:
                    case 7:
                        tag = String.valueOf(String.valueOf((new StringBuffer("D_s_B__WF__C_t_L=\"")).append(control1.getLogicName()).append("\"")));
                        formHtml = PublicLib.findReplace(formHtml, tag, disabled, true);
                        tag = "C_b_O__WF__C_t_L=".concat(String.valueOf(String.valueOf(control1.getControlName())));
                        replace1 = this._$13533(isNewInst, (MyCombo) control1, disabled, fieldValue, fieldType, aServerName, false);
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 9:
                        if (!aStepTag.equalsIgnoreCase(((MySign) control1).getTag()) || aReadOnly) {
                            disabled = " disabled ";
                        }

                        tag = String.valueOf(String.valueOf((new StringBuffer("S_i_g_N__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        replace1 = String.valueOf(String.valueOf(this._$13537(isNewInst, (MySign) control1, disabled, flowInid, stepInid, dealIndx, someInfoRs, true))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 10:
                        tag = String.valueOf(String.valueOf((new StringBuffer("M_a_P__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        formCode = aFormInstData.getFormStruct().getSysInfo().getFormCode();
                        StringBuffer realValue = new StringBuffer();
                        MyMap mapCtrl = (MyMap) control1;
                        replace1 = String.valueOf(String.valueOf(this._$13542(fii, aServerName, isNewInst, isFormula, forceUpdate, mapCtrl, disabled, fieldValue, aFormSetId, formCode, realValue, aReadOnly))).concat(" ");
                        String value = this.restoreMapDisplay((MyMap) control1, aServerName, realValue);
                        if (mapCtrl.isMultiline()) {
                            replace1 = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(value)))).append("</TEXTAREA>").append(replace1)));
                            int fromPos1 = formHtml.indexOf(tag);
                            int inputPos = formHtml.lastIndexOf("<INPUT", fromPos1);
                            formHtml.replace(inputPos, inputPos + 6, "<TEXTAREA");
                            value = "";
                        }

                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        tag = String.valueOf(String.valueOf((new StringBuffer("F_l_D__WF__C_t_L=\"")).append(control1.getFieldName()).append(";").append(tableName).append("\"")));
                        if (disabled.equals("")) {
                            valid = aValidStruct.getValidFromControl(fieldName1, fieldDesc, true);
                            value = String.valueOf(String.valueOf(value)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" ")).append(valid).append(" "))))));
                        } else {
                            value = String.valueOf(String.valueOf(value)).concat(" class=\'form_disable_text\' ");
                        }

                        realValue = null;
                        formHtml = PublicLib.findReplace(formHtml, tag, value, false);
                        break;
                    case 11:
                        if (!aStepTag.equalsIgnoreCase(((MyPen) control1).getTag()) || aReadOnly) {
                            disabled = " disabled ";
                        }

                        tag = String.valueOf(String.valueOf((new StringBuffer("P_e_N__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        replace1 = String.valueOf(String.valueOf(this._$13539(isNewInst, (MyPen) control1, disabled, flowInid, stepInid))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 12:
                        if (!aValidStruct.isEnabled(control1.getLogicName())) {
                            disabled = "disabled";
                        }

                        tag = String.valueOf(String.valueOf((new StringBuffer("A_t_T__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        long fromPos = 0L;
                        if (someInfoRs != null && someInfoRs.getObject("doc_flag") != null) {
                            fromPos = (long) Integer.parseInt(someInfoRs.getString("doc_flag"));
                        }

                        long popedom = 17L;
                        if (someInfoRs != null && someInfoRs.getObject("popedom") != null) {
                            popedom = (long) Integer.parseInt(someInfoRs.getString("popedom"));
                        }

                        replace1 = String.valueOf(String.valueOf(this._$13548(isNewInst, aReadOnly, (MyAttach) control1, flowInid, aMainRs, aStepTag, disabled, fromPos, popedom, fii))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 13:
                        tag = String.valueOf(String.valueOf((new StringBuffer("S_m_p__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        replace1 = this._$13549((MyStamp) control1, disabled, fieldValue, someInfoRs);
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 15:
                        tag = String.valueOf(String.valueOf((new StringBuffer("F_o_r_M__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        boolean prgReadOnly = false;
                        if (aReadOnly || !aValidStruct.isEnabled(control1.getLogicName())) {
                            prgReadOnly = true;
                        }

                        replace1 = String.valueOf(String.valueOf(this._$13551(isNewInst, (MyPrgForm) control1, disabled, flowInid, stepInid, primayKey, prgReadOnly, tableName))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 16:
                        if (!aValidStruct.isEnabled(control1.getLogicName())) {
                            disabled = " disabled ";
                        }

                        tag = String.valueOf(String.valueOf((new StringBuffer("L_n_K__WF__C_t_L=")).append(control1.getControlName()).append(" ")));
                        replace1 = String.valueOf(String.valueOf(this._$13553((MyHyperLink) control1, flowInid, stepInid, isPrint))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                }
            }

            if (someInfoRs != null) {
                if (aFormInstData.getFormStruct().getSysInfo().getFormType() == (long) 0) {
                    someInfoRs.putObject("modifyDocStructXml", e);
                    someInfoRs.putObject("serverName", aServerName);
                    someInfoRs.putObject("tableName", tableName);
                    someInfoRs.putObject("primaryKey", primayKey);
                    String[] nameList2 = this.getPrgFormList(aFormInstData.getFormStruct());
                    someInfoRs.putObject("prgFormNameList", nameList2);
                }

                someInfoRs.putObject("formStyle", aFormInstData.getFormStruct().getSysInfo().getFormStyle());
            }

            aReturnStruct.put("fieldMapNameRs", fieldMapNameRs);
            return formHtml;
        } catch (Exception var66) {
            formHtml = null;
            this.baseTool.error("restoreControlHtml", var66);
            throw new Exception(var66.getMessage());
        } finally {
            ;
        }
    }

    public StringBuffer restoreSubForm(boolean aReadOnly, StringBuffer aFormHtml, FormStruct aFormStruct, CachedRowSet aFormRs, CachedRowSet aMainRs, String aServerName, ValidStruct aValidStruct, Hashtable aReturnStruct) throws Exception {
        RecordSet hypoMapRs = new RecordSet();
        Hashtable hypoRsColl = new Hashtable();

        try {
            Hashtable e = aValidStruct.getListValidColl();
            Enumeration subFormColl = aFormStruct.getSubFormColl().elements();
            int index = 0;

            while (subFormColl.hasMoreElements()) {
                CachedRowSet hypoRs = null;

                try {
                    MySubForm ex = (MySubForm) subFormColl.nextElement();
                    long left = ex.getLeft();
                    long top = ex.getTop();
                    long width = ex.getWidth();
                    long height = ex.getHeight();
                    String tag = "S_u_b__WF__C_t_L=".concat(String.valueOf(String.valueOf(ex.getSubFormCode())));
                    String criteria = "form_code=".concat(String.valueOf(String.valueOf(ex.getSubFormCode())));
                    boolean rsFinded = PublicLib.find(aFormRs, criteria, true);
                    if (rsFinded) {
                        String replace = String.valueOf(String.valueOf((new StringBuffer("<DIV style=\"left:")).append(left).append("px;").append("top:").append(top).append("px;").append("width:").append(width).append("px;").append("height:").append(height).append("px;overflow:auto;")));
                        if (ex.isAbsolute()) {
                            replace = String.valueOf(String.valueOf(replace)).concat("position:absolute;");
                        }

                        replace = String.valueOf(String.valueOf(replace)).concat("Z-INDEX:100\">");
                        String subFormStructXml = PublicLib.getStringFromClob(aFormRs, "form_stru");
                        subFormStructXml = PublicLib.findReplace(subFormStructXml, "&lt;", "@lt;");
                        subFormStructXml = PublicLib.findReplace(subFormStructXml, "&gt;", "@gt;");
                        FormStruct subFormStruct = new FormStruct();
                        subFormStruct.restore(subFormStructXml);
                        String tableName = subFormStruct.getSysInfo().getBindTable();
                        tableName = PublicLib.getField(tableName, "<", 0L);
                        String mainTable = aFormStruct.getSysInfo().getBindTable();
                        String keyXml = subFormStruct.getKeyXml();
                        Hashtable keyColl = subFormStruct.getSysInfo().getCondStruct().getKeyColl();
                        String where = "";
                        SQLPara sqlPara = new SQLPara();
                        String fromTable;
                        String listFormHtml;
                        String fsetId;
                        if (PublicLib.getRecordCount(aMainRs) != 0) {
                            for (int condStruct = 0; condStruct < keyColl.size(); ++condStruct) {
                                fromTable = String.valueOf(condStruct);
                                KeyItem sqlClause = (KeyItem) keyColl.get(fromTable);
                                listFormHtml = sqlClause.getMainField();
                                listFormHtml = PublicLib.getField(listFormHtml, "<", 0L);
                                String validListItem = sqlClause.getHypoField();
                                validListItem = PublicLib.getField(validListItem, "<", 0L);
                                if (sqlClause.getKeyValue().equals("2")) {
                                    fsetId = aMainRs.getString(listFormHtml);
                                    if (!where.equals("")) {
                                        where = String.valueOf(String.valueOf(where)).concat(" and ");
                                    }

                                    int needFlag = (int) sqlClause.getFieldType();
                                    where = String.valueOf(String.valueOf(where)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(tableName)))).append(".").append(validListItem).append("=?"))))));
                                    if (needFlag != 1 && needFlag != 2) {
                                        sqlPara.add(fsetId, 1);
                                    } else {
                                        sqlPara.add(fsetId);
                                    }

                                    where = String.valueOf(String.valueOf(where)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" and ")).append(mainTable).append(".").append(listFormHtml).append("=").append(tableName).append(".").append(validListItem))))));
                                    where = String.valueOf(String.valueOf((new StringBuffer(" (")).append(where).append(") ")));
                                }
                            }
                        } else if (keyColl.size() != 0) {
                            where = "1=2";
                        }

                        CondStruct var61 = subFormStruct.getSysInfo().getCondStruct();
                        if (where.equals("")) {
                            where = var61.getFilterCondition(tableName, mainTable);
                        } else {
                            where = String.valueOf(String.valueOf(where)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(var61.getFilterCondition(tableName, mainTable)).append(")"))))));
                        }

                        fromTable = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(tableName)))).append(",").append(mainTable)));
                        String var62 = String.valueOf(String.valueOf((new StringBuffer("select distinct ")).append(tableName).append(".* from ").append(fromTable)));
                        if (!where.equals("")) {
                            var62 = String.valueOf(String.valueOf(var62)).concat(String.valueOf(String.valueOf(" where ".concat(String.valueOf(String.valueOf(where))))));
                        }

                        hypoRs = this.baseTool.getServerRowSet(aServerName, var62, tableName.toUpperCase(), sqlPara);
                        listFormHtml = "";
                        ValidListItem var63 = (ValidListItem) e.get(String.valueOf(ex.getSubFormCode()));
                        listFormHtml = this.restoreListFormHtml(aReadOnly, index, subFormStruct, hypoRs, aServerName, height, var63);
                        hypoMapRs.insertRow();
                        hypoMapRs.putObject("tbl_name", tableName);
                        fsetId = aFormRs.getString("fset_id");
                        hypoMapRs.putObject("fset_id", fsetId);
                        hypoMapRs.putObject("form_code", String.valueOf(ex.getSubFormCode()));
                        hypoMapRs.putObject("rela_fld", keyXml);
                        long var60 = 0L;
                        if (ex.isHaveRecord()) {
                            var60 = 1L;
                        }

                        hypoMapRs.putObject("need_flag", String.valueOf(var60));
                        replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(listFormHtml)));
                        replace = String.valueOf(String.valueOf(replace)).concat("</DIV>");
                        aFormHtml = PublicLib.findReplace(aFormHtml, tag, replace, true);
                        hypoRsColl.put(String.valueOf(index), hypoRs);
                        ++index;
                    }
                } catch (Exception var56) {
                    PublicLib.closeCachedRowSet(hypoRs);
                    hypoRs = null;
                    throw new Exception(var56.getMessage());
                } finally {
                    ;
                }
            }

            aReturnStruct.put("hypoMapRs", hypoMapRs);
            aReturnStruct.put("hypoRsColl", hypoRsColl);
        } catch (Exception var58) {
            this.baseTool.error("restoreSubForm", var58);
            throw new Exception(var58.getMessage());
        } finally {
            ;
        }

        return aFormHtml;
    }

    public void restoreRelationForm(FlowInstInfo fii, boolean aReadOnly, FormStruct aFormStruct, CachedRowSet aFormRs, CachedRowSet aMainRs, String aServerName, ValidStruct aValidStruct, Hashtable aReturnStruct) throws Exception {
        Hashtable relaMapRsColl = new Hashtable();
        Hashtable relaRsColl = new Hashtable();
        Hashtable relaHtmlColl = new Hashtable();

        try {
            aFormStruct.setHideMainForm(aValidStruct.isHideMainForm());
            Iterator e = aFormStruct.getTabPagesList().iterator();

            while (e.hasNext()) {
                CachedRowSet relaRs = null;

                try {
                    FormTabPage ex = (FormTabPage) e.next();
                    RelationValidStruct relaValidStruct = aValidStruct.getRelationTableValid(ex.getTabIndex());
                    if (relaValidStruct == null) {
                        relaValidStruct = new RelationValidStruct();
                    }

                    ex.setVisible(relaValidStruct.isVisible());
                    if (ex.getTabType() == (long) 1) {
                        String criteria = "form_code=".concat(String.valueOf(String.valueOf(ex.getTabRefStruct())));
                        boolean rsFinded = PublicLib.find(aFormRs, criteria, true);
                        if (rsFinded) {
                            StringBuffer rawHtml = new StringBuffer(PublicLib.getStringFromClob(aFormRs, "form_cont"));
                            rawHtml = PublicLib.findReplace(rawHtml, "&lt;", "<", false);
                            rawHtml = PublicLib.findReplace(rawHtml, "&gt;", ">", false);
                            rawHtml = PublicLib.findReplace(rawHtml, "&amp;", "&", false);
                            String relaFormStructXml = PublicLib.getStringFromClob(aFormRs, "form_stru");
                            relaFormStructXml = PublicLib.findReplace(relaFormStructXml, "&lt;", "@lt;");
                            relaFormStructXml = PublicLib.findReplace(relaFormStructXml, "&gt;", "@gt;");
                            FormStruct relaFormStruct = new FormStruct();
                            relaFormStruct.restore(relaFormStructXml);
                            String bindTableName = relaFormStruct.getSysInfo().getBindTable();
                            bindTableName = PublicLib.getField(bindTableName, "<", 0L);
                            String mainTable = aFormStruct.getSysInfo().getBindTable();
                            String keyXml = relaFormStruct.getKeyXml();
                            Hashtable keyColl = relaFormStruct.getSysInfo().getCondStruct().getKeyColl();
                            String where = "";
                            SQLPara sqlPara = new SQLPara();
                            String fromTable;
                            if (PublicLib.getRecordCount(aMainRs) != 0) {
                                for (int condStruct = 0; condStruct < keyColl.size(); ++condStruct) {
                                    fromTable = String.valueOf(condStruct);
                                    KeyItem sqlClause = (KeyItem) keyColl.get(fromTable);
                                    String fsetId = sqlClause.getMainField();
                                    fsetId = PublicLib.getField(fsetId, "<", 0L);
                                    String relaField = sqlClause.getHypoField();
                                    relaField = PublicLib.getField(relaField, "<", 0L);
                                    if (sqlClause.getKeyValue().equals("2")) {
                                        String formInstData = aMainRs.getString(fsetId);
                                        if (!where.equals("")) {
                                            where = String.valueOf(String.valueOf(where)).concat(" and ");
                                        }

                                        int relaHtml = (int) sqlClause.getFieldType();
                                        where = String.valueOf(String.valueOf(where)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(bindTableName)))).append(".").append(relaField).append("=?"))))));
                                        if (relaHtml != 1 && relaHtml != 2) {
                                            sqlPara.add(formInstData, 1);
                                        } else {
                                            sqlPara.add(formInstData);
                                        }

                                        where = String.valueOf(String.valueOf(where)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" and ")).append(mainTable).append(".").append(fsetId).append("=").append(bindTableName).append(".").append(relaField))))));
                                        where = String.valueOf(String.valueOf((new StringBuffer(" (")).append(where).append(") ")));
                                    }
                                }
                            } else if (keyColl.size() != 0) {
                                where = "1=2";
                            }

                            CondStruct var43 = relaFormStruct.getSysInfo().getCondStruct();
                            if (where.equals("")) {
                                where = var43.getFilterCondition(bindTableName, mainTable, aMainRs, sqlPara);
                            } else {
                                where = String.valueOf(String.valueOf(where)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" and (")).append(var43.getFilterCondition(bindTableName, mainTable)).append(")"))))));
                            }

                            fromTable = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(bindTableName)))).append(",").append(mainTable)));
                            String var44 = String.valueOf(String.valueOf((new StringBuffer("select distinct ")).append(bindTableName).append(".* from ").append(fromTable)));
                            if (!where.equals("")) {
                                var44 = String.valueOf(String.valueOf(var44)).concat(String.valueOf(String.valueOf(" where ".concat(String.valueOf(String.valueOf(where))))));
                            }

                            long var45 = aFormRs.getLong("fset_id");
                            relaRs = this.baseTool.getServerRowSet(aServerName, var44, bindTableName.toUpperCase(), sqlPara);
                            if (fii.isNewInst() || relaRs.size() == 0) {
                                this.baseTool.getFlowLib().genRelationKey(var45, aServerName, bindTableName.toUpperCase(), aMainRs, relaRs);
                            }

                            relaRs.first();
                            relaRsColl.put(String.valueOf(ex.getTabIndex()), relaRs);
                            FormInstData var46 = new FormInstData();
                            var46.setFormStruct(relaFormStruct);
                            StringBuffer var47 = this.restoreControlHtml(fii, aReadOnly, false, String.valueOf(var45), "", rawHtml, aServerName, var46, relaValidStruct, relaRs, aReturnStruct);
                            RecordSet relaMapRs = (RecordSet) aReturnStruct.remove("fieldMapNameRs");
                            relaMapRsColl.put(String.valueOf(ex.getTabIndex()), relaMapRs);
                            relaHtmlColl.put(String.valueOf(ex.getTabIndex()), var47.substring(0));
                        }
                    }
                } catch (Exception var40) {
                    PublicLib.closeCachedRowSet(relaRs);
                    relaRs = null;
                    throw var40;
                } finally {
                    ;
                }
            }

            aReturnStruct.put("__RE__laMa__pRs", relaMapRsColl);
            aReturnStruct.put("Re_la_Rs_CO_lL_", relaRsColl);
            aReturnStruct.put("Re_la_Html_CO_lL_", relaHtmlColl);
        } catch (Exception var42) {
            this.baseTool.error("restoreRelationForm", var42);
            throw var42;
        }
    }

    public String restoreListFormHtml(boolean aReadOnly, int aIndex, FormStruct aFormStruct, CachedRowSet aHypoRs, String aServerName, long aHeight, ValidListItem aValidListItem) throws Exception {
        String listFormHtml = "";
        Hashtable mapRsColl = new Hashtable();

        try {
            String e = "";
            String listContent = "";
            String listButton = "";
            if (aFormStruct.getSysInfo().getFormType() == (long) 1) {
                String listFormId = String.valueOf(aFormStruct.getSysInfo().getFormCode());
                e = "<td class=\"head_firstTd\" width=\"30\">&nbsp;</td>";
                MyListStruct listStruct = aFormStruct.getSysInfo().getListStruct();
                long addFormCode = listStruct.getAddFormCode();
                long updateFormCode = listStruct.getUpdateFormCode();
                if (updateFormCode <= (long) 0) {
                    updateFormCode = addFormCode;
                }

                Hashtable fieldColl = listStruct.getFieldColl();

                int value;
                String deleteButtonHtml;
                String viewButtonHtml;
                String hiddenHtml;
                String fieldValue;
                for (value = 0; value < fieldColl.size(); ++value) {
                    MyListItem addButtonHtml = (MyListItem) fieldColl.get(String.valueOf(value));
                    float updateButtonHtml = (float) (addButtonHtml.getWidth() * (long) 100 / listStruct.getTotalWidth());
                    e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<td class=\"head_Td\" width=")).append(updateButtonHtml).append("% title=\'").append(addButtonHtml.getColCaption()).append("\'>"))))));
                    e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(addButtonHtml.getColCaption())).concat("</td>"))));
                    if ((long) 1 != addButtonHtml.getMapType()) {
                        deleteButtonHtml = PublicLib.getField(addButtonHtml.getMapString(), "=", 0L);
                        viewButtonHtml = PublicLib.getField(addButtonHtml.getMapString(), "=", 1L);
                        hiddenHtml = PublicLib.getField(addButtonHtml.getMapString(), ".", 0L);
                        fieldValue = String.valueOf(String.valueOf((new StringBuffer("select ")).append(viewButtonHtml).append(",").append(deleteButtonHtml).append(" from ").append(hiddenHtml)));
                        CachedRowSet fieldType = this.baseTool.getServerRowSet(aServerName, fieldValue, "", new SQLPara());
                        mapRsColl.put(addButtonHtml.getFieldName(), fieldType);
                    }
                }

                e = String.valueOf(String.valueOf((new StringBuffer("<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"listTable\"><tr class=\"head_Tr\">")).append(e).append("</tr></table>")));
                value = 0;
                aHypoRs.beforeFirst();

                String var43;
                while (aHypoRs.next()) {
                    var43 = String.valueOf(String.valueOf((new StringBuffer("<td class=\"hiddenCheckboxTd\" width=\"30\"><input type=\"radio\" name=\"sys_subform_recordindex_")).append(listFormId).append("\" value=").append(value).append("></td>")));
                    fieldColl = listStruct.getFieldColl();

                    for (int var44 = 0; var44 < fieldColl.size(); ++var44) {
                        MyListItem var46 = (MyListItem) fieldColl.get(String.valueOf(var44));
                        float var47 = (float) (var46.getWidth() * (long) 100 / listStruct.getTotalWidth());
                        hiddenHtml = PublicLib.getField(var46.getFieldName(), "<", 0L);
                        fieldValue = "";
                        long var48 = var46.getFieldType();
                        if (aHypoRs.getString(hiddenHtml) != null) {
                            fieldValue = aHypoRs.getString(hiddenHtml);
                        }

                        if (var48 == (long) 3) {
                            fieldValue = PublicLib.getNonZeroValue(fieldValue);
                        } else if (var48 == (long) 4 || var48 == (long) 5 || var48 == (long) 6) {
                            fieldValue = PublicLib.getField(fieldValue, " ", 0L);
                        }

                        String mapString;
                        String oldValue;
                        String newValue;
                        if ((long) 1 != var46.getMapType()) {
                            if (!fieldValue.equals("")) {
                                CachedRowSet var49 = (CachedRowSet) mapRsColl.get(var46.getFieldName());
                                if (PublicLib.getRecordCount(var49) != 0) {
                                    mapString = PublicLib.getField(var46.getMapString(), "=", 0L);
                                    mapString = PublicLib.getField(mapString, ".", 1L);
                                    oldValue = PublicLib.getField(var46.getMapString(), "=", 1L);
                                    oldValue = PublicLib.getField(oldValue, ".", 1L);
                                    newValue = "";
                                    if (var46.getFieldType() != (long) 1 && var46.getFieldType() != (long) 2) {
                                        newValue = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(mapString)))).append("=").append(fieldValue)));
                                    } else {
                                        newValue = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(mapString)))).append("=\'").append(fieldValue).append("\'")));
                                    }

                                    boolean rsFinded = PublicLib.find(var49, newValue, true);
                                    if (rsFinded) {
                                        fieldValue = var49.getString(oldValue);
                                    }
                                }
                            }
                        } else {
                            StringTokenizer st = new StringTokenizer(var46.getMapString(), "||");

                            while (st.hasMoreTokens()) {
                                mapString = st.nextToken();
                                oldValue = mapString.substring(0, mapString.indexOf("="));
                                newValue = mapString.substring(mapString.indexOf("=") + 1, mapString.length());
                                if (fieldValue.equals(oldValue)) {
                                    fieldValue = newValue;
                                }
                            }
                        }

                        if (fieldValue == null) {
                            fieldValue = "";
                        }

                        if (var44 == fieldColl.size() - 1) {
                            var43 = String.valueOf(String.valueOf(var43)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<td class=\"data_lastTd_char\" width=")).append(var47).append("% title=\'").append(fieldValue).append("\'>").append(fieldValue).append("&nbsp;</td>"))))));
                        } else {
                            var43 = String.valueOf(String.valueOf(var43)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<td class=\"data_Td_char\" width=")).append(var47).append("% title=\'").append(fieldValue).append("\'>").append(fieldValue).append("&nbsp;</td>"))))));
                        }
                    }

                    var43 = String.valueOf(String.valueOf((new StringBuffer("<tr>")).append(var43).append("</tr>")));
                    listContent = String.valueOf(String.valueOf(listContent)).concat(String.valueOf(String.valueOf(var43)));
                    ++value;
                }

                listContent = String.valueOf(String.valueOf((new StringBuffer("<div style=\"width:100%;height:")).append(aHeight - (long) 50).append("px;overflow:auto;\" class=\"freeTableDiv\"><table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"listTable\">").append(listContent).append("</table></div>")));
                var43 = "";
                String var45 = "";
                deleteButtonHtml = "";
                viewButtonHtml = "";
                if (aValidListItem != null && aValidListItem.isAllowAdd()) {
                    var43 = String.valueOf(String.valueOf((new StringBuffer("<input type=\"button\" value=\"")).append(listStruct.getAddCaption()).append("\" class=\"standardBtn\" onclick=\"javascript:addRecord(").append(listFormId).append(");\">")));
                }

                if (aValidListItem != null && aValidListItem.isAllowUpdate()) {
                    var45 = String.valueOf(String.valueOf((new StringBuffer("<input type=\"button\" value=\"")).append(listStruct.getUpdateCaption()).append("\" class=\"standardBtn\" onclick=\"javascript:updateRecord(").append(listFormId).append(");\">")));
                }

                if (aValidListItem != null && aValidListItem.isAllowDelete()) {
                    deleteButtonHtml = String.valueOf(String.valueOf((new StringBuffer("<input type=\"button\" value=\"")).append(listStruct.getDeleteCaption()).append("\" class=\"standardBtn\" onclick=\"javascript:deleteRecord(").append(listFormId).append(");\">")));
                }

                if (aValidListItem != null && aValidListItem.isAllowView()) {
                    viewButtonHtml = String.valueOf(String.valueOf((new StringBuffer("<input type=\"button\" value=\"")).append(listStruct.getViewCaption()).append("\" class=\"standardBtn\" onclick=\"javascript:viewRecord(").append(listFormId).append(");\">")));
                }

                listButton = String.valueOf(String.valueOf((new StringBuffer("<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"listTable\"><tr class=\"tail_Tr\"><td class=\"tail_Td\">")).append(var43).append(var45).append(deleteButtonHtml).append(viewButtonHtml).append("&nbsp</td></tr></table>")));
                hiddenHtml = String.valueOf(String.valueOf((new StringBuffer("<input name=\"sys_subform_formindex_")).append(listFormId).append("\" ").append("type=\"hidden\" value=\"").append(aIndex).append("\">").append("<input name=\"sys_subform_freeformid_for_update_").append(listFormId).append("\" ").append("type=\"hidden\" value=\"").append(updateFormCode).append("\">").append("<input name=\"sys_subform_freeformid_for_add_").append(listFormId).append("\" ").append("type=\"hidden\" value=\"").append(addFormCode).append("\">")));
                listFormHtml = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(hiddenHtml)))).append(e).append(listContent).append(listButton)));
            }
        } catch (Exception var41) {
            this.baseTool.error("restoreListFormHtml", var41);
            throw new Exception(var41.getMessage());
        } finally {
            Enumeration mapRsEnum = mapRsColl.elements();

            while (mapRsEnum.hasMoreElements()) {
                CachedRowSet mapRs = (CachedRowSet) mapRsEnum.nextElement();
                PublicLib.closeCachedRowSet(mapRs);
            }

            mapRsColl.clear();
        }

        return listFormHtml;
    }

    private Map _$13517(boolean aNewFlag, boolean aIsFormula, boolean aForceUpdate, MyText aControl, String aDisabled, String aFieldValue, String aValid, String aServerName, FormInstData aFormInstData, boolean aReadOnly) throws Exception {
        Hashtable hRet = new Hashtable();
        String replace = "";
        String hiddenReplace = "";
        String commonWord = "";
        String dateTypeStr = "";
        String dateDispValue = "";
        String dateHiddenValue = "";
        boolean isDate = false;

        try {
            String e = "";
            int fieldType = (int) aControl.getFieldType();
            boolean showTime = false;
            dateTypeStr = aControl.getDateTypeStr();
            isDate = fieldType == 4 || fieldType == 5 || fieldType == 6;
            if (aControl.getDateType() == (long) 8) {
                showTime = true;
            }

            Timestamp t1;
            if (aReadOnly) {
                e = aFieldValue;
            } else if ((!aIsFormula || aForceUpdate || aFieldValue != null && !aFieldValue.equals("")) && (!aIsFormula || !aForceUpdate)) {
                e = aFieldValue;
            } else {
                t1 = PublicLib.getCurrentTime();
                label273:
                switch ((int) aControl.getDefaultType()) {
                    case 0:
                        e = aControl.getDefaultValue();
                        break;
                    case 1:
                        switch (fieldType) {
                            case 1:
                            case 2:
                            case 3:
                            case 7:
                                if (aControl.getDefaultValue().equals("2")) {
                                    e = this.baseTool.getCurrentUser();
                                } else if (aControl.getDefaultValue().equals("0")) {
                                    e = DateTimeUtils.dateToString(t1, "yyyy-MM-dd");
                                } else if (aControl.getDefaultValue().equals("1")) {
                                    e = DateTimeUtils.dateToString(t1, "HH:mm:ss");
                                } else if (aControl.getDefaultValue().equals("3")) {
                                    e = this.baseTool.getDeptSeq(this.baseTool.getCurrentUser());
                                }
                                break label273;
                            case 4:
                            case 5:
                            case 6:
                                if (aControl.getDefaultValue().equals("0") || aControl.getDefaultValue().equals("1")) {
                                    dateDispValue = DateTimeUtils.dateToString(t1, dateTypeStr);
                                    dateHiddenValue = e = DateTimeUtils.dateToString(t1, "yyyy-MM-dd HH:mm:ss");
                                }
                            default:
                                break label273;
                        }
                    case 2:
                        String valueScript = aControl.getValueExpression();
                        FormExpressionScript pes = new FormExpressionScript();
                        valueScript = pes.translateFormExpressionToJavaScript(valueScript);
                        StringBuffer setValScript = new StringBuffer();
                        setValScript.append("try{");
                        setValScript.append(String.valueOf(String.valueOf((new StringBuffer("document.all.item(\"")).append(aControl.getControlName()).append("\").value="))));
                        setValScript.append(valueScript);
                        setValScript.append(";");
                        if (isDate) {
                            setValScript.append(String.valueOf(String.valueOf((new StringBuffer("document.all.item(\"")).append(aControl.getControlName()).append("___hidden___value\").value="))));
                            setValScript.append(valueScript);
                            setValScript.append(";");
                        }

                        setValScript.append("}catch(e){}");
                        aFormInstData.addExpressionScript(setValScript.substring(0));
                }
            }

            if (e == null) {
                e = "";
            }

            if (isDate && e.trim().length() > 0) {
                t1 = DateTimeUtils.str2Timestamp(e, (Timestamp) null);
                dateDispValue = DateTimeUtils.dateToString(t1, dateTypeStr);
                dateHiddenValue = e;
            }

            if (e.trim().length() > 0) {
                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" value=\"")).append(isDate ? dateDispValue : e).append("\" "))))));
            } else {
                replace = String.valueOf(String.valueOf(replace)).concat(" value ");
            }

            if (isDate) {
                hiddenReplace = String.valueOf(String.valueOf(hiddenReplace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("><input type=hidden name=")).append(aControl.getControlName()).append("___hidden___value value=\'").append(dateHiddenValue).append("\'"))))));
            }

            if (aDisabled.trim().length() == 0 && isDate) {
                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" onclick=\"javascript:showCalendar(this,")).append(aControl.getControlName()).append("___hidden___value,\'").append(dateTypeStr).append("\');\" "))))));
                aDisabled = " readonly ";
            }

            if (aDisabled.trim().length() == 0 && !isDate && aControl.isEnableCommonWord()) {
                replace = String.valueOf(String.valueOf(replace)).concat(" ONSELECT=\'this.pos=document.selection.createRange();\'");
                replace = String.valueOf(String.valueOf(replace)).concat(" ONKEYUP=\'this.pos=document.selection.createRange();\'");
                replace = String.valueOf(String.valueOf(replace)).concat(" onclick=\'javascript:this.pos=document.selection.createRange();\' ");
                commonWord = String.valueOf(String.valueOf((new StringBuffer("><input type=\'button\' value=\'常用语\' class=\'standardBtn\' style=\'Z-INDEX: 110;POSITION: absolute; TOP: ")).append(aControl.getTop()).append("px;LEFT:").append(aControl.getLeft() + aControl.getWidth() + (long) 4).append("px;\' ").append("onclick=\'javascritp:showCommonWordPicker(document.all.item(\"").append(aControl.getControlName()).append("\"));\'")));
            }

            if (!isDate && aControl.isEnableValueRepeatCheck()) {
                aValid = String.valueOf(String.valueOf(aValid)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" originalValue=\"")).append(e).append("\" checkRepeat=\"1\" checkRepeatPara=\"").append(aServerName).append(",").append(aFormInstData.getFormStruct().getSysInfo().getBindTable()).append(",").append(PublicLib.getField(aControl.getFieldName(), "<", 0L)).append(",").append(aNewFlag ? "1" : "0").append("\" "))))));
            }

            replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(aDisabled)).concat(String.valueOf(String.valueOf(aValid))))));
        } catch (Exception var30) {
            this.baseTool.error("restoreText", var30);
            throw var30;
        } finally {
            ;
        }

        hRet.put("replace", replace);
        if (isDate) {
            hRet.put("hiddenReplace", hiddenReplace);
        } else {
            hRet.put("hiddenReplace", commonWord);
        }

        return hRet;
    }

    private Hashtable _$13524(boolean aNewFlag, boolean aIsFormula, boolean aForceUpdate, MyTextArea aControl, String aDisabled, String aFieldValue, boolean aReadOnly, FormInstData aFormInstData) throws Exception {
        String replace = "";
        String commonWord = "";
        boolean isDate = false;
        Hashtable hRet = new Hashtable();

        try {
            String e = "";
            int fieldType = (int) aControl.getFieldType();
            isDate = fieldType == 4 || fieldType == 5 || fieldType == 6;
            byte showTime = 0;
            if (aReadOnly) {
                e = aFieldValue;
            } else if (aIsFormula && !aForceUpdate && (aFieldValue == null || aFieldValue.equals("")) || aIsFormula && aForceUpdate) {
                Timestamp t = PublicLib.getCurrentTime();
                label206:
                switch ((int) aControl.getDefaultType()) {
                    case 0:
                        e = aControl.getDefaultValue();
                        break;
                    case 1:
                        switch (fieldType) {
                            case 1:
                            case 2:
                            case 3:
                            case 7:
                                if (aControl.getDefaultValue().equals("2")) {
                                    e = this.baseTool.getCurrentUser();
                                } else if (aControl.getDefaultValue().equals("0")) {
                                    e = DateTimeUtils.dateToString(t, "yyyy-MM-dd");
                                } else if (aControl.getDefaultValue().equals("1")) {
                                    e = DateTimeUtils.dateToString(t, "HH:mm:ss");
                                } else if (aControl.getDefaultValue().equals("3")) {
                                    e = this.baseTool.getDeptSeq(this.baseTool.getCurrentUser());
                                }
                                break label206;
                            case 4:
                            case 5:
                            case 6:
                                if (aControl.getDefaultValue().equals("0") || aControl.getDefaultValue().equals("1")) {
                                    e = PublicLib.getCurrentTimeStr();
                                    aDisabled = " readonly ";
                                }
                            default:
                                break label206;
                        }
                    case 2:
                        String valueScript = aControl.getValueExpression();
                        FormExpressionScript pes = new FormExpressionScript();
                        valueScript = pes.translateFormExpressionToJavaScript(valueScript);
                        StringBuffer setValScript = new StringBuffer();
                        setValScript.append("try{");
                        setValScript.append(String.valueOf(String.valueOf((new StringBuffer("document.all.item(\"")).append(aControl.getControlName()).append("\").value="))));
                        setValScript.append(valueScript);
                        setValScript.append(";");
                        if (isDate) {
                            setValScript.append(String.valueOf(String.valueOf((new StringBuffer("document.all.item(\"")).append(aControl.getControlName()).append("___hidden___value\").value="))));
                            setValScript.append(valueScript);
                            setValScript.append(";");
                        }

                        setValScript.append("}catch(e){}");
                        aFormInstData.addExpressionScript(setValScript.substring(0));
                }
            } else {
                e = aFieldValue;
            }

            if (isDate && showTime == 0) {
                e = PublicLib.getField(e, " ", 0L);
            }

            if (aDisabled.equals("") && isDate) {
                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" onclick=\"javascript:calendar_ex(this,")).append(showTime).append(");\" "))))));
            }

            if (aDisabled.trim().length() == 0 && !isDate && aControl.isEnableCommonWord()) {
                replace = String.valueOf(String.valueOf(replace)).concat(" ONSELECT=\'this.pos=document.selection.createRange();\'");
                replace = String.valueOf(String.valueOf(replace)).concat(" ONKEYUP=\'this.pos=document.selection.createRange();\'");
                replace = String.valueOf(String.valueOf(replace)).concat(" onclick=\'javascript:this.pos=document.selection.createRange();\' ");
                commonWord = String.valueOf(String.valueOf((new StringBuffer("<input type=\'button\' value=\'常用语\' class=\'standardBtn\' style=\'Z-INDEX: 110;POSITION: absolute; TOP: ")).append(aControl.getTop()).append("px;LEFT:").append(aControl.getLeft() + aControl.getWidth() + (long) 4).append("px;\' ").append("onclick=\'javascritp:showCommonWordPicker(document.all.item(\"").append(aControl.getControlName()).append("\"));\'>")));
            }

            replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(">".concat(String.valueOf(String.valueOf(e))))));
        } catch (Exception var24) {
            this.baseTool.error("restoreTextArea", var24);
            throw new Exception(var24.getMessage());
        } finally {
            ;
        }

        hRet.put("replace", replace);
        if (!isDate) {
            hRet.put("hiddenReplace", commonWord);
        }

        return hRet;
    }

    private String _$13527(boolean aNewFlag, MyButton aControl, String aFormSetId, long aFormCode, String aStepName, Hashtable aControlColl, String aDisabled) throws Exception {
        String replace = "";

        try {
            String e = "";
            if (aControl.getHide() == 1) {
                e = ";VISIBILITY: hidden;";
            }

            if (aDisabled.equals("")) {
                String controlList = aControl.getControlList();
                String urlPara;
                if (!controlList.equals("")) {
                    controlList = String.valueOf(String.valueOf((new StringBuffer(";")).append(controlList).append(";")));
                    Enumeration style = aControlColl.keys();

                    while (style.hasMoreElements()) {
                        urlPara = (String) style.nextElement();
                        MyControl url = (MyControl) aControlColl.get(urlPara);
                        if (url.getControlType() == (long) 9 && aStepName.equalsIgnoreCase(((MySign) url).getTag())) {
                            String controlName = url.getControlName();
                            controlName = String.valueOf(String.valueOf((new StringBuffer(";")).append(controlName).append(";")));
                            controlList = PublicLib.findReplace(controlList, controlName, ";sys_flow_comm;");
                        }
                    }

                    controlList = controlList.substring(1, controlList.length() - 1);
                }

                String style1 = String.valueOf(String.valueOf((new StringBuffer("<DIV style=\"")).append(aControl.getFont()).append(";left:").append(String.valueOf(aControl.getLeft())).append(";top:").append(String.valueOf(aControl.getTop())).append(";height:").append(String.valueOf(aControl.getHeight())).append(";width:").append(String.valueOf(aControl.getWidth())).append(e).append("Z-INDEX: 100\">")));
                urlPara = String.valueOf(String.valueOf((new StringBuffer("fsetId=")).append(String.valueOf(aFormSetId)).append("&formCode=").append(String.valueOf(aFormCode)).append("&logicName=").append(aControl.getLogicName())));
                String url1 = String.valueOf(String.valueOf((new StringBuffer("<input class=\'standardBtn\' type=button value=\"")).append(aControl.getCaption()).append("\"").append(" onclick=\"javascript:selectButton(\'").append(urlPara).append("\'").append(",\'").append(controlList).append("\');\" ")));
                if (!aDisabled.equals("")) {
                    url1 = String.valueOf(String.valueOf(url1)).concat(" disabled ");
                }

                url1 = String.valueOf(String.valueOf(url1)).concat(" >");
                replace = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(style1)))).append(url1).append("</DIV>")));
            }
        } catch (Exception var20) {
            this.baseTool.error("restoreButton2", var20);
            throw new Exception(var20.getMessage());
        } finally {
            ;
        }

        return replace;
    }

    private String _$13529(boolean aNewFlag, MyRadio aControl, String aDisabled, String aFieldValue, long aFieldType, String aValid, boolean aIsRadio, String aServerName) throws Exception {
        String replace = "";
        CachedRowSet crs = null;

        try {
            String e = "";
            if (aControl.getHide() == 1) {
                e = ";VISIBILITY: hidden;";
            }

            long left = aControl.getLeft();
            long top = aControl.getTop();
            long width = aControl.getWidth();
            long height = aControl.getHeight();
            if (height <= (long) 0) {
                height = aControl.getFontSize();
            }

            if (height > width) {
                height = width;
            }

            String valueList = "";
            String displayList = "";
            String style;
            if (aControl.getFrom() == (long) 0) {
                valueList = aControl.getValueList();
                displayList = aControl.getDisplayList();
            } else {
                String fieldCount = aControl.getServerName();
                if (fieldCount == null || fieldCount.equals("")) {
                    fieldCount = aServerName;
                }

                String maxSize = aControl.getDisplayField();
                String valueField = aControl.getValueField();
                String i = aControl.getTable();
                maxSize = PublicLib.getField(maxSize, "<", 0L);
                valueField = PublicLib.getField(valueField, "<", 0L);
                i = PublicLib.getField(i, "<", 0L);
                style = String.valueOf(String.valueOf((new StringBuffer("select ")).append(maxSize).append(",").append(valueField).append(" from ").append(i).append(" where ").append(valueField).append(" is not null")));
                crs = this.baseTool.getServerRowSet(fieldCount, style, "", new SQLPara());

                while (crs.next()) {
                    if (!valueList.equals("") && !displayList.equals("")) {
                        valueList = String.valueOf(String.valueOf(valueList)).concat(";");
                        displayList = String.valueOf(String.valueOf(displayList)).concat(";");
                    }

                    if (aFieldType == (long) 3) {
                        valueList = String.valueOf(String.valueOf(valueList)).concat(String.valueOf(String.valueOf(PublicLib.getNonZeroValue(crs.getString(valueField)))));
                    } else {
                        valueList = String.valueOf(String.valueOf(valueList)).concat(String.valueOf(String.valueOf(crs.getString(valueField))));
                    }

                    if (crs.getString(maxSize) != null) {
                        displayList = String.valueOf(String.valueOf(displayList)).concat(String.valueOf(String.valueOf(crs.getString(maxSize))));
                    }
                }
            }

            int var41 = PublicLib.getFieldCount(valueList, ";");
            long var42 = this.getMaxSize(displayList);

            for (int var43 = 0; var43 < var41; ++var43) {
                style = "";
                String checked = "";
                String value = PublicLib.getField(valueList, ";", (long) var43);
                String display = PublicLib.getField(displayList, ";", (long) var43);
                if (aControl.isAbsolute()) {
                    style = String.valueOf(String.valueOf((new StringBuffer("<DIV style=\"")).append(aControl.getFont()).append("left:").append(String.valueOf(left)).append("px").append(";top:").append(String.valueOf(top)).append("px").append(";height:").append(String.valueOf(height)).append("px").append(";width:").append(String.valueOf(width + this.getMaxSize(display) * aControl.getFontSize())).append("px").append(e).append(";text-align:left;Z-INDEX: 100\">")));
                }

                for (int controlType = 0; controlType < PublicLib.getFieldCount(aFieldValue, ";"); ++controlType) {
                    if (PublicLib.getField(aFieldValue, ";", (long) controlType).equalsIgnoreCase(value)) {
                        checked = " CHECKED ";
                        break;
                    }
                }

                String var44 = "";
                if (aIsRadio) {
                    var44 = "radio";
                } else {
                    var44 = "checkbox";
                }

                String controlHtml = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(style)))).append("<INPUT style=\"height:").append(String.valueOf(height)).append("px;width:").append(String.valueOf(width)).append("px \"").append(" name=\"").append(aControl.getControlName()).append("\" id=\"").append(aControl.getControlName()).append("\"").append(" type=\"").append(var44).append("\"").append(" value=\"").append(value).append("\" tempvalue=\"").append(display).append("\" ").append(aDisabled).append(checked).append(aValid).append(">")));
                if (aDisabled.equals("")) {
                    if (aIsRadio) {
                        controlHtml = String.valueOf(String.valueOf(controlHtml)).concat("<span style=\"cursor:hand\" onclick=\"previousSibling.checked = true;\">");
                    } else {
                        controlHtml = String.valueOf(String.valueOf(controlHtml)).concat("<span style=\"cursor:hand\" onclick=\"previousSibling.checked=!previousSibling.checked;\">");
                    }

                    controlHtml = String.valueOf(String.valueOf(controlHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(display)).concat("</span>"))));
                } else {
                    controlHtml = String.valueOf(String.valueOf(controlHtml)).concat(String.valueOf(String.valueOf(display)));
                }

                if (aControl.isAbsolute()) {
                    controlHtml = String.valueOf(String.valueOf(controlHtml)).concat("</DIV>");
                }

                long interval = 0L;
                if (aControl.getDirect() == (long) 0) {
                    if (aControl.getColumn() > (long) 0 && var43 > 0 && (long) (var43 + 1) % aControl.getColumn() == (long) 0) {
                        top = (long) ((double) top + 1.5D * (double) aControl.getFontSize());
                        left = aControl.getLeft();
                    } else {
                        left += aControl.getWidth() + var42 * aControl.getFontSize() / (long) 2 + interval;
                    }
                } else if (aControl.getColumn() > (long) 0 && var43 > 0 && (long) (var43 + 1) % aControl.getColumn() == (long) 0) {
                    top = aControl.getTop();
                    left += aControl.getWidth() + var42 * aControl.getFontSize() / (long) 2 + interval;
                } else {
                    top = (long) ((double) top + 1.5D * (double) aControl.getFontSize());
                }

                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(controlHtml)));
            }
        } catch (Exception var39) {
            this.baseTool.error("restoreRadio", var39);
            throw new Exception(var39.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
        }

        return replace;
    }

    private String _$13533(boolean aNewFlag, MyCombo aControl, String aDisabled, String aFieldValue, long aFieldType, String aServerName, boolean aBatchState) throws Exception {
        String replace = "";
        CachedRowSet crs = null;

        try {
            String e = "";
            String displayList = "";
            String style;
            String value;
            String display;
            if (aControl.getFrom() == (long) 0) {
                e = aControl.getValueList();
                displayList = aControl.getDisplayList();
            } else {
                String fieldCount = aControl.getServerName();
                if (fieldCount == null || fieldCount.equals("")) {
                    fieldCount = aServerName;
                }

                String i = aControl.getDisplayField();
                style = aControl.getValueField();
                value = aControl.getTable();
                i = PublicLib.getField(i, "<", 0L);
                style = PublicLib.getField(style, "<", 0L);
                value = PublicLib.getField(value, "<", 0L);
                display = String.valueOf(String.valueOf((new StringBuffer("select ")).append(i).append(",").append(style).append(" from ").append(value).append(" where ").append(style).append(" is not null")));
                crs = this.baseTool.getServerRowSet(fieldCount, display, "", new SQLPara());

                while (crs.next()) {
                    if (!e.equals("") && !displayList.equals("")) {
                        e = String.valueOf(String.valueOf(e)).concat(";");
                        displayList = String.valueOf(String.valueOf(displayList)).concat(";");
                    }

                    if (aFieldType == (long) 3) {
                        e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(PublicLib.getNonZeroValue(crs.getString(style)))));
                    } else {
                        e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(crs.getString(style))));
                    }

                    if (crs.getString(i) != null) {
                        displayList = String.valueOf(String.valueOf(displayList)).concat(String.valueOf(String.valueOf(crs.getString(i))));
                    }
                }
            }

            int var24 = PublicLib.getFieldCount(e, ";");
            if (aBatchState) {
                replace = String.valueOf(String.valueOf(replace)).concat("<OPTION value=\"\" selected ></OPTION>");
            }

            for (int var25 = 0; var25 < var24; ++var25) {
                style = "";
                value = PublicLib.getField(e, ";", (long) var25);
                display = PublicLib.getField(displayList, ";", (long) var25);
                if (value.equalsIgnoreCase(aFieldValue)) {
                    replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<OPTION value=\"")).append(value).append("\" selected >"))))));
                } else {
                    replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<OPTION value=\"")).append(value).append("\" >"))))));
                }

                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(display)).concat("</OPTION>"))));
            }
        } catch (Exception var22) {
            this.baseTool.error("restoreCombo", var22);
            throw new Exception(var22.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
        }

        return replace;
    }

    private String _$13682(CachedRowSet dealInfo, StringBuffer table, MySign aControl) throws Exception {
        String sCurrentUserDealComm = "";
        dealInfo.beforeFirst();
        HashMap hRef = new HashMap();
        HashMap hRowIndex = new HashMap();
        Vector vecCate = new Vector();
        String preDealIdx = null;
        String appendPath = null;
        String rootNode = null;
        String dealIndx = null;
        Object frameRs = null;
        boolean oldData = false;

        try {
            String dealComm0;
            String dealComm;
            String url;
            String css;
            SignItem sItem1;
            Vector vecRow;
            for (; dealInfo.next(); vecRow.add(sItem1)) {
                long ex = dealInfo.getLong("fini_flag");
                dealComm0 = dealInfo.getString("deal_man");
                dealComm = dealInfo.getString("agen_men");
                String i = dealInfo.getString("deal_dept");
                url = dealInfo.getString("deal_comm");
                int dispDept = dealInfo.getInt("agen_flag");
                if (i == null) {
                    i = "";
                }

                if (url == null) {
                    url = "";
                }

                String isRoot = "";
                if (dealInfo.getTimestamp("rece_date") != null) {
                    isRoot = dealInfo.getTimestamp("rece_date").toString();
                }

                String iSize = "";
                String j = "";
                if (dealComm == null) {
                    dealComm = "";
                }

                if (dealComm.trim().length() > 0) {
                    if (dispDept == 3) {
                        iSize = this.baseTool.getUserName(dealComm);
                    } else {
                        j = this.baseTool.getUserName(dealComm);
                        if (j != null && j.trim().length() > 0) {
                            j = String.valueOf(String.valueOf((new StringBuffer("<font color=red>(代理")).append(j).append(")</font>")));
                        }
                    }
                }

                if (iSize == null || iSize.trim().length() == 0) {
                    if (dealComm0.indexOf(":") == -1) {
                        iSize = this.baseTool.getUserName(dealComm0);
                    } else {
                        iSize = this.baseTool.getFrameName(PublicLib.getField(dealComm0, ":", 0L));
                    }
                }

                css = dealComm != null && dealComm.trim().length() > 0 ? dealComm : dealComm0;
                String sItem = i.trim().length() > 0 ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(css)))).append("@").append(i))) : css;
                String hasSign = this.baseTool.getDeptNameOfUser(isRoot, sItem);
                Timestamp t = dealInfo.getTimestamp("save_date");
                String signDate = t == null ? "" : t.toString();
                appendPath = dealInfo.getString("append_path");
                dealIndx = dealInfo.getString("deal_indx");
                sItem1 = new SignItem();
                sItem1.setAgenManName(j);
                sItem1.setDealComm(url);
                sItem1.setDepartName(hasSign);
                sItem1.setFiniFlag(ex);
                sItem1.setSignDate(signDate);
                sItem1.setSignMan(dealComm0);
                sItem1.setUserName(iSize);
                if (appendPath == null) {
                    appendPath = "";
                }

                if (!appendPath.trim().equals("")) {
                    rootNode = PublicLib.getField(appendPath, ";", 0L);
                } else {
                    if (!oldData) {
                        oldData = true;
                    }

                    preDealIdx = dealInfo.getString("pre_deal_idx");
                    if (preDealIdx == null) {
                        preDealIdx = "";
                    }

                    if (preDealIdx.trim().equals("")) {
                        preDealIdx = "-1";
                    }

                    if (preDealIdx.trim().equals("-1")) {
                        rootNode = dealIndx;
                    } else {
                        rootNode = (String) hRef.get(preDealIdx);
                    }
                }

                hRef.put(dealIndx, rootNode);
                Integer intRowIndex = (Integer) hRowIndex.get(rootNode);
                vecRow = null;
                if (intRowIndex == null) {
                    vecRow = new Vector();
                    hRowIndex.put(rootNode, new Integer(vecCate.size()));
                    vecCate.add(vecRow);
                } else {
                    vecRow = (Vector) vecCate.get(intRowIndex.intValue());
                }
            }

            boolean var39 = true;
            String font = "x".concat(String.valueOf(String.valueOf(aControl.getFont())));
            dealComm0 = "";
            dealComm = "";

            for (int var40 = 0; var40 < vecCate.size(); ++var40) {
                url = "";
                boolean var41 = aControl.getStyle() == 3;
                Vector rows = (Vector) vecCate.get(var40);
                boolean var42 = true;
                int var43 = rows.size();

                for (int var44 = 0; var44 < rows.size(); ++var44) {
                    css = "signTd";
                    if (var39) {
                        css = "signFirstTd";
                        var39 = false;
                    }

                    SignItem var45 = (SignItem) rows.get(var44);
                    dealComm0 = var45.getDealComm();
                    dealComm = dealComm0;
                    if (dealComm0.equals("")) {
                        dealComm = "&nbsp";
                    }

                    dealComm = PublicLib.findReplace(dealComm, "\n", "<br>");
                    dealComm = PublicLib.findReplace(dealComm, " ", "&nbsp;  ");
                    boolean var46 = false;
                    if (var45.getFiniFlag() == (long) 1 && var45.getSignMan() != null && var45.getSignMan().trim().length() > 0 && (var46 = this._$13719(var45.getSignMan()))) {
                        url = String.valueOf(String.valueOf((new StringBuffer("<img border=\'0\' src=\'ImgLoader?type=1&id=")).append(var45.getSignMan()).append("\'>")));
                    }

                    if (oldData) {
                        if (var41) {
                            table.append(String.valueOf(String.valueOf((new StringBuffer("<td rowspan=\'2\' width=\'20%\' style=\'padding-left:5px;")).append(this.genSignGridStyle(String.valueOf(String.valueOf(css)).concat("1_1"), font)).append(font).append("\'>"))));
                            table.append(String.valueOf(String.valueOf(var45.getDepartName())).concat("</td>"));
                        }

                        table.append(String.valueOf(String.valueOf((new StringBuffer("<td  width=\'")).append(var41 ? "80" : "100").append("%\' style=\'padding-left:5px;").append(this.genSignGridStyle(String.valueOf(String.valueOf(css)).concat("2_1"), font)).append(font).append("\'>"))));
                        table.append(String.valueOf(String.valueOf(dealComm)).concat("</td>"));
                        table.append("</tr>");
                        table.append("<tr>");
                        table.append(String.valueOf(String.valueOf((new StringBuffer("<td valign=\'middle\' width=\'")).append(var41 ? "80" : "100").append("%\' style=\'padding-left:5px;").append(this.genSignGridStyle(String.valueOf(String.valueOf(css)).concat("3_1"), font)).append(font).append("\'>"))));
                        table.append(String.valueOf(String.valueOf((new StringBuffer("<b></b>")).append(var46 ? url : var45.getUserName()).append(var45.getAgenManName()).append("&nbsp;&nbsp;"))));
                        table.append(String.valueOf(String.valueOf(PublicLib.getField(var45.getSignDate(), " ", 0L))).concat("&nbsp;</td>"));
                        table.append("</tr>");
                    } else {
                        if (var42) {
                            var42 = false;
                            sCurrentUserDealComm = dealComm0;
                            table.append("<tr>");
                            if (var41) {
                                table.append(String.valueOf(String.valueOf((new StringBuffer("<td rowspan=\'2\' width=\'20%\' style=\'padding-left:5px;")).append(this.genSignGridStyle(String.valueOf(String.valueOf(css)).concat("1_1"), font)).append(font).append("\'>"))));
                                table.append(String.valueOf(String.valueOf(var45.getDepartName())).concat("</td>"));
                            }

                            table.append(String.valueOf(String.valueOf((new StringBuffer("<td  width=\'")).append(var41 ? "80" : "100").append("%\'"))));
                            table.append(String.valueOf(String.valueOf((new StringBuffer(" style=\'padding-left:5px;")).append(this.genSignGridStyle(String.valueOf(String.valueOf(css)).concat("2_1"), font)).append(font).append("\'>"))));
                            table.append(String.valueOf(String.valueOf(dealComm)).concat("</td>"));
                            table.append("</tr>");
                            table.append("<tr>");
                            table.append(String.valueOf(String.valueOf((new StringBuffer("<td valign=\'middle\' width=\'")).append(var41 ? "80" : "100").append("%\' style=\'padding-left:5px;").append(this.genSignGridStyle(String.valueOf(String.valueOf(css)).concat("3_1"), font)).append(font).append("\'>"))));
                        }

                        table.append(String.valueOf(String.valueOf((new StringBuffer("<b></b>")).append(var46 ? url : var45.getUserName()).append(var45.getAgenManName()).append("&nbsp;&nbsp;"))));
                        table.append(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(PublicLib.getField(var45.getSignDate(), " ", 0L))))).append("&nbsp;").append(var44 == var43 - 1 ? "" : "|").append("&nbsp;"))));
                    }
                }

                if (!oldData) {
                    table.append("</td>");
                    table.append("</tr>");
                }
            }
        } catch (Exception var37) {
            throw var37;
        } finally {
            PublicLib.clearObject(hRef);
            PublicLib.clearObject(hRowIndex);
            PublicLib.clearObject(vecCate);
            hRef = null;
            hRowIndex = null;
            vecCate = null;
        }

        return sCurrentUserDealComm;
    }

    public String genSignGridStyle(String tag, String fontStyle) {
        String style = (String) _$13453.get(tag);
        if (style != null && fontStyle != null) {
            int iStartPos = fontStyle.indexOf("BORDERCOLOR:");
            if (iStartPos == -1) {
                style = PublicLib.findReplace(style, "#333366", "#000000");
            } else {
                int iEndPos = 0;
                iStartPos += 12;
                if (iStartPos < fontStyle.length()) {
                    iEndPos = fontStyle.indexOf(";", iStartPos);
                }

                String borderColor = "";
                if (iStartPos <= iEndPos) {
                    borderColor = fontStyle.substring(iStartPos, iEndPos);
                }

                borderColor = borderColor.trim();
                if (borderColor.length() > 0) {
                    style = PublicLib.findReplace(style, "#333366", borderColor);
                }
            }
        } else {
            style = "";
        }

        return style;
    }

    protected String genSignOrderByField(int orderByType) {
        String sRet = "rece_date";
        switch (orderByType) {
            case 1:
                sRet = "rece_date";
                break;
            case 2:
                sRet = "fini_date";
                break;
            case 3:
                sRet = "rece_date desc";
                break;
            case 4:
                sRet = "fini_date desc";
        }

        return sRet;
    }

    protected boolean isStepFinish(String aFlowInid, String stepTag) {
        CachedRowSet crs = null;
        boolean stepFinish = false;

        label46:
        try {
            try {
                String ex = "select count(*) as CNT from deal_info d left join step_inst si on d.step_inco=si.step_inco left join step_def sd on si.step_code=sd.step_code  where si.send_flag=0 and d.flow_inid=? and (sd.step_tag=? or sd.step_name=?)";
                SQLPara para = new SQLPara();
                para.add(aFlowInid);
                para.add(stepTag);
                para.add(stepTag);
                if (Long.parseLong(aFlowInid) < minFlowInid) {
                    ex = ex.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                            .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                            .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                }
                crs = this.baseTool.getRowSet(ex, "", para);
                if (crs.next()) {
                    stepFinish = crs.getLong("CNT") == (long) 0;
                }
            } catch (Exception var11) {
                stepFinish = false;
                var11.printStackTrace();
            }
        } finally {
            break label46;
        }

        PublicLib.closeCachedRowSet(crs);
        crs = null;
        return stepFinish;
    }

    private String _$13537(boolean aNewFlag, MySign aControl, String aDisabled, String aFlowInid, String aStepInid, long dealIndx, RecordSet aSomeInfoRs, boolean includeDiv) throws Exception {
        StringBuffer replace = new StringBuffer("");
        CachedRowSet dealInfoRs = null;
        Object frameRs = null;
        boolean integration = false;
        boolean isAllograph = false;

        try {
            if (aStepInid == null) {
                aStepInid = "-1";
            }

            if (aStepInid.trim().length() == 0) {
                aStepInid = "-1";
            }

            long e = Long.parseLong(aStepInid);
            String keyPressEvent = "";
            StringBuffer opinion = new StringBuffer("");
            String style = "";
            StringBuffer table = new StringBuffer("");
            String controlHidden = "";
            if (aControl.getHide() == 1) {
                controlHidden = ";VISIBILITY: hidden;";
            }

            long left = aControl.getLeft();
            long top = aControl.getTop();
            long width = aControl.getWidth();
            long height = aControl.getHeight();
            String font = "x".concat(String.valueOf(String.valueOf(aControl.getFont())));
            long fontSize = aControl.getFontSize();
            boolean blacked = false;
            String currentUserDealComm = "";
            String currentUser = this.baseTool.getCurrentUser();
            String border = "border:0px solid #000000;";
            style = String.valueOf(String.valueOf((new StringBuffer("width=")).append(aControl.isDispCommonWord() ? width - (long) 80 : width).append("px;")));
            Object hRowTab = null;
            Object hRows = null;
            boolean seeSelfOnly = false;
            if (aControl.isCanSeeWhenStepFinish()) {
                seeSelfOnly = !this.isStepFinish(aFlowInid, aControl.getTag());
            }

            integration = aControl.isAppendIntergration() && (aControl.getStyle() == 3 || aControl.getStyle() == 4);
            table.append(String.valueOf(String.valueOf((new StringBuffer("<Table class=signTable ")).append(aControl.isSplitGrid() ? "height=\"100%\"" : "").append(" border=0 cellspacing=\"0\" cellpadding=\"0\" width=\"100%\">"))));
            String sqlClause = "";
            Timestamp currentAllographDate = null;
            String defaultComm;
            if (!aNewFlag) {
                sqlClause = String.valueOf(String.valueOf((new StringBuffer(" select D.step_inco,D.append_path,D.deal_indx,D.pre_deal_idx,D.agen_men,D.agen_flag,D.fini_flag,D.deal_man,D.deal_dept,D.deal_comm,D.fini_date,D.save_date,D.rece_date,allograph, D.allograph_date from deal_info D,step_inst S,step_def f where D.step_inco=S.step_inco  and S.step_code=f.step_code and S.flow_id=f.flow_id and (D.fini_flag=0 or D.fini_flag=1) and (f.step_tag=? or f.step_name=?) and D.flow_inid=?")).append(seeSelfOnly ? " and D.deal_man=? " : "").append(" order by D.").append(integration ? "deal_indx" : this.genSignOrderByField(aControl.getOrderByType()))));
                SQLPara flowCommCtrlName = new SQLPara();
                flowCommCtrlName.add(aControl.getTag());
                flowCommCtrlName.add(aControl.getTag());
                flowCommCtrlName.add(String.valueOf(aFlowInid));
                if (seeSelfOnly) {
                    flowCommCtrlName.add(this.baseTool.getCurrentUser());
                }

                if (Long.parseLong(aFlowInid) < minFlowInid) {
                    sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                            .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                            .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                }
                dealInfoRs = this.baseTool.getRowSet(sqlClause, "", flowCommCtrlName);
                dealInfoRs.beforeFirst();
                if (integration) {
                    currentUserDealComm = this._$13682(dealInfoRs, table, aControl);
                } else {
                    boolean tempAllographDate = true;

                    label853:
                    while (true) {
                        long supportCaSign;
                        String iRowCount;
                        String dealComm;
                        String dealDept;
                        int agentFlag;
                        Timestamp allographDate;
                        do {
                            do {
                                if (!dealInfoRs.next()) {
                                    break label853;
                                }

                                supportCaSign = dealInfoRs.getLong("fini_flag");
                                iRowCount = dealInfoRs.getString("deal_man");
                                defaultComm = dealInfoRs.getString("agen_men");
                                dealComm = dealInfoRs.getString("deal_comm");
                                String allograph = dealInfoRs.getString("allograph");
                                long tempDealIndx = dealInfoRs.getLong("deal_indx");
                                long tempStepInco = dealInfoRs.getLong("step_inco");
                                dealDept = dealInfoRs.getString("deal_dept");
                                agentFlag = dealInfoRs.getInt("agen_flag");
                                allographDate = dealInfoRs.getTimestamp("allograph_date");
                                if (allograph == null) {
                                    allograph = "";
                                }

                                if (dealDept == null) {
                                    dealDept = "";
                                }

                                if (dealComm == null) {
                                    dealComm = "";
                                }

                                boolean isCurrentDealInfo = dealIndx == tempDealIndx && e == tempStepInco;
                                if (isCurrentDealInfo) {
                                    List receDate = this.baseTool.getFlowLib().getUserAllograph(currentUser);
                                    isAllograph = receDate.contains(iRowCount);
                                    currentAllographDate = allographDate;
                                }

                                if ((iRowCount.equalsIgnoreCase(currentUser) || isAllograph) && isCurrentDealInfo) {
                                    currentUserDealComm = dealComm;
                                }
                            } while (dealComm.trim().length() == 0 && !aControl.isDispNullRec());
                        } while (aControl.isDispOnlyHasSign() && supportCaSign != (long) 1);

                        String receDate1 = "";
                        if (dealInfoRs.getTimestamp("rece_date") != null) {
                            receDate1 = dealInfoRs.getTimestamp("rece_date").toString();
                        }

                        String userName = "";
                        String agenManName = "";
                        if (defaultComm == null) {
                            defaultComm = "";
                        }

                        if (defaultComm.trim().length() > 0) {
                            if (agentFlag == 3) {
                                userName = this.baseTool.getUserName(defaultComm);
                            } else {
                                agenManName = this.baseTool.getUserName(defaultComm);
                                if (agenManName != null && agenManName.trim().length() > 0) {
                                    agenManName = String.valueOf(String.valueOf((new StringBuffer("<font color=red>(代理")).append(agenManName).append(")</font>")));
                                }
                            }
                        }

                        if (userName == null || userName.trim().length() == 0) {
                            if (iRowCount.indexOf(":") == -1) {
                                userName = this.baseTool.getUserName(iRowCount);
                            } else {
                                userName = this.baseTool.getFrameName(PublicLib.getField(iRowCount, ":", 0L));
                            }
                        }

                        String signUser = defaultComm != null && defaultComm.trim().length() > 0 ? defaultComm : iRowCount;
                        String signUserDept = dealDept.trim().length() > 0 ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(signUser)))).append("@").append(dealDept))) : signUser;
                        String signDate;
                        String url;
                        if (aControl.getStyle() != 0 && aControl.getStyle() != 1) {
                            if (aControl.getStyle() == 2 || aControl.getStyle() == 3 || aControl.getStyle() == 4) {
                                signDate = "";
                                if (supportCaSign != (long) 1) {
                                    userName = "&nbsp";
                                } else if (allographDate != null) {
                                    signDate = DateTimeUtils.dateToString(allographDate, "yyyy-MM-dd HH:mm");
                                } else {
                                    signDate = DateTimeUtils.dateToString(dealInfoRs.getTimestamp(supportCaSign == (long) 1 ? "fini_date" : "save_date"), "yyyy-MM-dd HH:mm");
                                }

                                if (dealComm.equals("")) {
                                    dealComm = "&nbsp";
                                }

                                dealComm = PublicLib.findReplace(dealComm, "\n", "<br>");
                                dealComm = PublicLib.findReplace(dealComm, " ", "&nbsp;");
                                String css1 = "signTd";
                                if (tempAllographDate) {
                                    css1 = "signFirstTd";
                                }

                                String departName1 = this.baseTool.getDeptNameOfUser(receDate1, signUserDept);
                                int iGridStyle1 = aControl.getStyle();
                                switch (iGridStyle1) {
                                    case 2:
                                        table.append(String.valueOf(String.valueOf((new StringBuffer("<tr><td width=\'20%\' style=\'padding-left:5px;")).append(this.genSignGridStyle(String.valueOf(String.valueOf(css1)).concat("1"), font)).append(font).append("\'>"))));
                                        table.append(String.valueOf(String.valueOf(departName1)).concat("</td>"));
                                        table.append(String.valueOf(String.valueOf((new StringBuffer("<td width=\'55%\' style=\'padding-left:5px;")).append(this.genSignGridStyle(String.valueOf(String.valueOf(css1)).concat("2"), font)).append(font).append("\'>"))));
                                        table.append(String.valueOf(String.valueOf(dealComm)).concat("</td>"));
                                        table.append(String.valueOf(String.valueOf((new StringBuffer("<td width=\'25%\' style=\'padding-left:5px;")).append(this.genSignGridStyle(String.valueOf(String.valueOf(css1)).concat("3"), font)).append(font).append("\'>"))));
                                        table.append(String.valueOf(String.valueOf(userName)).concat(String.valueOf(String.valueOf(signDate))));
                                        table.append(String.valueOf(String.valueOf((new StringBuffer("<br>")).append(agenManName).append("</td></tr>"))));
                                        break;
                                    case 3:
                                    case 4:
                                        url = "";
                                        boolean dispDept1 = iGridStyle1 == 3;
                                        boolean hasSign = false;
                                        if (supportCaSign == (long) 1 && iRowCount != null && iRowCount.trim().length() > 0 && (hasSign = this._$13719(iRowCount))) {
                                            url = String.valueOf(String.valueOf((new StringBuffer("<img border=\'0\' src=\'ImgLoader?type=1&id=")).append(iRowCount).append("\'>")));
                                        }

                                        table.append("<tr>");
                                        if (dispDept1) {
                                            table.append(String.valueOf(String.valueOf((new StringBuffer("<td rowspan=\'2\' width=\'20%\' style=\'padding-left:5px;")).append(this.genSignGridStyle(String.valueOf(String.valueOf(css1)).concat("1_1"), font)).append(font).append("\'>"))));
                                            table.append(String.valueOf(String.valueOf(departName1)).concat("</td>"));
                                        }

                                        table.append(String.valueOf(String.valueOf((new StringBuffer("<td  width=\'")).append(dispDept1 ? "80" : "100").append("%\' style=\'padding-left:5px;").append(this.genSignGridStyle(String.valueOf(String.valueOf(css1)).concat("2_1"), font)).append(font).append("\'>"))));
                                        table.append(String.valueOf(String.valueOf(dealComm)).concat("</td>"));
                                        table.append("</tr>");
                                        table.append("<tr>");
                                        table.append(String.valueOf(String.valueOf((new StringBuffer("<td valign=\'middle\' width=\'")).append(dispDept1 ? "80" : "100").append("%\' style=\'padding-left:5px;").append(this.genSignGridStyle(String.valueOf(String.valueOf(css1)).concat("3_1"), font)).append(font).append("\'>"))));
                                        table.append(String.valueOf(String.valueOf((new StringBuffer("<b></b>")).append(hasSign ? url : userName).append(agenManName).append("&nbsp;&nbsp;"))));
                                        table.append(String.valueOf(String.valueOf(signDate)).concat("&nbsp;</td>"));
                                        table.append("</tr>");
                                }
                            }
                        } else {
                            signDate = "";
                            boolean css = false;
                            Timestamp departName = null;
                            if (allographDate != null) {
                                departName = allographDate;
                                signDate = DateTimeUtils.dateToString(allographDate, "yyyy-MM-dd HH:mm");
                                css = true;
                            } else {
                                departName = dealInfoRs.getTimestamp(supportCaSign == (long) 1 ? "fini_date" : "save_date");
                                if (departName != null) {
                                    signDate = DateTimeUtils.dateToString(departName, "yyyy-MM-dd HH:mm");
                                    css = true;
                                }
                            }

                            if (css) {
                                if (dealComm.equals("")) {
                                    dealComm = "&nbsp";
                                }

                                dealComm = PublicLib.findReplace(dealComm, "\n", "<br>");
                                dealComm = PublicLib.findReplace(dealComm, " ", "&nbsp;");
                                String iGridStyle = "";
                                if (aControl.getStyle() == 1) {
                                    iGridStyle = this.baseTool.getDeptLevelNameOfUser(receDate1, signUserDept, -100);
                                } else {
                                    iGridStyle = this.baseTool.getDeptNameOfUser(receDate1, signUserDept);
                                }

                                switch (aControl.getStyle()) {
                                    case 0:
                                        table.append(String.valueOf(String.valueOf((new StringBuffer("<tr><td width=\"100%\" style=\"")).append(font).append("\">"))));
                                        table.append("<B>".concat(String.valueOf(String.valueOf(iGridStyle))));
                                        table.append(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(userName)))).append(agenManName).append(signDate).append("："))));
                                        table.append(String.valueOf(String.valueOf(dealComm)).concat("</B>"));
                                        table.append("</td></tr>");
                                        break;
                                    case 1:
                                        String tempSignDate = this.baseTool.getPosiNameOfUser(receDate1, defaultComm != null && defaultComm.trim().length() > 0 ? defaultComm : iRowCount);
                                        url = aControl.getFooterDef();
                                        if (url == null) {
                                            url = "";
                                        }

                                        if (url.trim().length() > 0) {
                                            UserDefineSignFormat dispDept = new UserDefineSignFormat(url);
                                            table.append(dispDept.format(dealComm, iGridStyle, String.valueOf(String.valueOf(userName)).concat(String.valueOf(String.valueOf(agenManName))), tempSignDate, font, departName));
                                        } else {
                                            if (blacked) {
                                                table.append("<tr style=\"height=30px,width=100%\"><td>&nbsp;</td></tr>");
                                            }

                                            table.append(String.valueOf(String.valueOf((new StringBuffer("<tr><td><textarea readonly style=\"border-width:2;height=37px;width:")).append(width).append(";"))));
                                            table.append(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(font)))).append("\">").append(dealComm))));
                                            table.append("</textarea></td></tr>");
                                            table.append("<tr style=\"".concat(String.valueOf(String.valueOf(font))));
                                            table.append(String.valueOf(String.valueOf((new StringBuffer("\"><td nowrap=\"true\" style=\"width:")).append(width).append(";"))));
                                            table.append(String.valueOf(String.valueOf(font)).concat("\">"));
                                            table.append(String.valueOf(String.valueOf((new StringBuffer("<B>")).append(iGridStyle).append("</B>").append(userName).append(agenManName))));
                                            table.append("<B>&nbsp;&nbsp;职位:</B>".concat(String.valueOf(String.valueOf(tempSignDate))));
                                            table.append("<B>&nbsp;&nbsp;时间:</B>".concat(String.valueOf(String.valueOf(signDate))));
                                            table.append("</td></tr>");
                                            blacked = true;
                                        }
                                }
                            }
                        }

                        tempAllographDate = false;
                    }
                }
            }

            table.append("</table>");
            String flowCommCtrlName1 = "sys_flow_comm";
            String tempAllographDate1 = "";
            if (currentAllographDate != null) {
                tempAllographDate1 = DateTimeUtils.dateToString(currentAllographDate, "yyyy-MM-dd HH:mm:ss");
            }

            if (aDisabled.trim().length() == 0) {
                boolean supportCaSign1 = PublicLib.str2Integer(this.baseTool.getTagValue("WF/SUPPORT_CASIGN")) == 1;
                boolean doCaSign = supportCaSign1 && !integration && aControl.isDoCaSign();
                aSomeInfoRs.putObject("___sign_ca__require__哈那_____", new Boolean(doCaSign));
                if (doCaSign) {
                    aSomeInfoRs.putObject("—__sign_ca_form--__Data__Fiel__d___", "_200050428_");
                }

                if (currentUserDealComm == null) {
                    currentUserDealComm = "";
                }

                int iRowCount1 = PublicLib.countSubString(currentUserDealComm, "\n");
                if (iRowCount1 < aControl.getCommInitRows()) {
                    iRowCount1 = aControl.getCommInitRows();
                }

                if (iRowCount1 > aControl.getCommMaxRows()) {
                    iRowCount1 = aControl.getCommMaxRows();
                }

                keyPressEvent = String.valueOf(String.valueOf((new StringBuffer("<script language=\"javascript\">function ")).append(flowCommCtrlName1).append("_onkeypress() {if(event.keyCode==13){changeRows(event.srcElement,").append(aControl.getCommMaxRows()).append(");}}</script>")));
                switch (aControl.getStyle()) {
                    case 0:
                    case 2:
                        opinion.append(String.valueOf(String.valueOf((new StringBuffer("<textarea rows=\"")).append(iRowCount1).append("\" name=\"").append(flowCommCtrlName1).append("\" style=\"border-width=2;").append(style).append("\""))));
                        if (aControl.isDispCommonWord()) {
                            opinion.append(" ONSELECT=\'this.pos=document.selection.createRange();\'");
                            opinion.append(" ONKEYUP=\'this.pos=document.selection.createRange();\'");
                            opinion.append(" onclick=\'javascript:this.pos=document.selection.createRange();\' ");
                        }

                        opinion.append(String.valueOf(String.valueOf((new StringBuffer(" onkeypress=\"return ")).append(flowCommCtrlName1).append("_onkeypress();\" propname=\"签署意见\" autocheck=\"1\" cannull=\"").append(aControl.isAllowNull() ? "1" : "0").append("\">").append(currentUserDealComm))));
                        opinion.append("</textarea>");
                        if (aControl.isDispCommonWord()) {
                            opinion.append("<input type=\'button\' value=\'常用语\' class=\'standardBtn\'");
                            opinion.append(String.valueOf(String.valueOf((new StringBuffer("onclick=\'javascritp:showCommonWordPicker(document.all.item(\"")).append(flowCommCtrlName1).append("\"));\'>"))));
                        }

                        if (isAllograph) {
                            opinion.append(String.valueOf(String.valueOf((new StringBuffer("<br>代录签字日期 &nbsp;<input readonly type=\"text\" name=\"")).append(flowCommCtrlName1).append("_date\" value=\"").append(tempAllographDate1).append("\""))));
                            opinion.append(" onclick=\"javascript:showCalendar(this,this,\'yyyy-MM-dd HH:mm\');\"");
                            opinion.append(">");
                        }
                        break;
                    case 1:
                        opinion.append("<Table border=0 cellspacing=\"0\" cellpadding=\"0\" width=\"100%\">");
                        opinion.append(String.valueOf(String.valueOf((new StringBuffer("<tr><td valign=\'middle\'><textarea rows=\"")).append(iRowCount1).append("\" name=\"").append(flowCommCtrlName1).append("\" style=\"border-width=2;").append(style).append("\""))));
                        if (aControl.isDispCommonWord()) {
                            opinion.append(" ONSELECT=\'this.pos=document.selection.createRange();\'");
                            opinion.append(" ONKEYUP=\'this.pos=document.selection.createRange();\'");
                            opinion.append(" onclick=\'javascript:this.pos=document.selection.createRange();\' ");
                        }

                        opinion.append(String.valueOf(String.valueOf((new StringBuffer(" onkeypress=\"return ")).append(flowCommCtrlName1).append("_onkeypress();\" propname=\"签署意见\" autocheck=\"1\" cannull=\"").append(aControl.isAllowNull() ? "1" : "0").append("\">").append(currentUserDealComm))));
                        opinion.append("</textarea>");
                        opinion.append(aControl.isDispCommonWord() ? String.valueOf(String.valueOf((new StringBuffer("<input type=\'button\' value=\'常用语\' class=\'standardBtn\'onclick=\'javascritp:showCommonWordPicker(document.all.item(\"")).append(flowCommCtrlName1).append("\"));\'>"))) : "");
                        if (isAllograph) {
                            opinion.append(String.valueOf(String.valueOf((new StringBuffer("<br>代录签字日期 &nbsp;<input readonly type=\"text\" name=\"")).append(flowCommCtrlName1).append("_date\" value=\"").append(tempAllographDate1).append("\""))));
                            opinion.append(" onclick=\"javascript:showCalendar(this,this,\'yyyy-MM-dd HH:mm:ss\');\"");
                            opinion.append(">");
                        }

                        opinion.append("</td></tr>");
                        if (blacked) {
                            opinion.append("<tr style=\"height=30px,width=100%\"><td>&nbsp;</td></tr>");
                        }

                        opinion.append("</table>");
                        break;
                    case 3:
                    case 4:
                        opinion.append(String.valueOf(String.valueOf((new StringBuffer("<textarea rows=\"")).append(iRowCount1).append("\" name=\"").append(flowCommCtrlName1).append("\" style=\"border-width=2;").append(style).append("\""))));
                        if (aControl.isDispCommonWord()) {
                            opinion.append(" ONSELECT=\'this.pos=document.selection.createRange();\'");
                            opinion.append(" ONKEYUP=\'this.pos=document.selection.createRange();\'");
                            opinion.append(" onclick=\'javascript:this.pos=document.selection.createRange();\' ");
                        }

                        opinion.append(String.valueOf(String.valueOf((new StringBuffer(" onkeypress=\"return ")).append(flowCommCtrlName1).append("_onkeypress();\" propname=\"签署意见\" autocheck=\"1\" cannull=\"").append(aControl.isAllowNull() ? "1" : "0").append("\">").append(currentUserDealComm))));
                        opinion.append("</textarea>");
                        opinion.append(String.valueOf(String.valueOf((new StringBuffer("<input type=\'hidden\' name=\'sys_flow_comm__integration__\' value=\'")).append(integration ? 1 : 0).append("\'>"))));
                        if (aControl.isDispCommonWord()) {
                            opinion.append("<input type=\'button\' value=\'常用语\' class=\'standardBtn\'");
                            opinion.append(String.valueOf(String.valueOf((new StringBuffer("onclick=\'javascritp:showCommonWordPicker(document.all.item(\"")).append(flowCommCtrlName1).append("\"));\'>"))));
                        }

                        if (isAllograph) {
                            opinion.append(String.valueOf(String.valueOf((new StringBuffer("<br>代录签字日期 &nbsp;<input readonly type=\"text\" name=\"")).append(flowCommCtrlName1).append("_date\" value=\"").append(tempAllographDate1).append("\""))));
                            opinion.append(" onclick=\"javascript:showCalendar(this,this,\'yyyy-MM-dd HH:mm:ss\');\"");
                            opinion.append(">");
                        }
                }

                defaultComm = aControl.getDefaultValue();
                if (aSomeInfoRs != null) {
                    aSomeInfoRs.putObject("default_comm", defaultComm);
                }
            }

            if (includeDiv) {
                replace.append("<DIV style=\"overflow-x:no;overflow-y:auto;");
                replace.append(border);
                replace.append(String.valueOf(String.valueOf((new StringBuffer("left:")).append(String.valueOf(left)).append("px;"))));
                replace.append(String.valueOf(String.valueOf((new StringBuffer("top:")).append(String.valueOf(top)).append("px;"))));
                replace.append(String.valueOf(String.valueOf((new StringBuffer("height:")).append(String.valueOf(height)).append("px;"))));
                replace.append(String.valueOf(String.valueOf((new StringBuffer("width:")).append(String.valueOf(width)).append("px;"))));
                replace.append(controlHidden);
                replace.append(aControl.getFont());
                replace.append(String.valueOf(String.valueOf((new StringBuffer("Z-INDEX:100\">")).append(opinion).append(table).append("</DIV>"))));
                replace.append(keyPressEvent);
            } else {
                replace.append(table);
            }
        } catch (Exception var76) {
            this.baseTool.error("restoreSign", var76);
            throw var76;
        } finally {
            PublicLib.closeCachedRowSet(dealInfoRs);
            dealInfoRs = null;
            PublicLib.closeCachedRowSet((CachedRowSet) frameRs);
            frameRs = null;
        }

        return replace.substring(0);
    }

    private String _$13773(boolean aNewFlag, MySign aControl, String aFlowInid, String aStepInid, long dealIndx, RecordSet aSomeInfoRs, int dataFormat) throws Exception {
        StringBuffer replace = new StringBuffer("");
        CachedRowSet dealInfoRs = null;
        Object frameRs = null;
        Vector vec = new Vector();
        boolean integration = false;
        boolean isAllograph = false;

        try {
            if (aStepInid == null) {
                aStepInid = "-1";
            }

            if (aStepInid.trim().length() == 0) {
                aStepInid = "-1";
            }

            long e = Long.parseLong(aStepInid);
            StringBuffer signData = new StringBuffer("");
            String currentUser = this.baseTool.getCurrentUser();
            boolean seeSelfOnly = false;
            if (aControl.isCanSeeWhenStepFinish()) {
                seeSelfOnly = !this.isStepFinish(aFlowInid, aControl.getTag());
            }

            if (!aNewFlag) {
                String sdss = String.valueOf(String.valueOf((new StringBuffer(" select D.step_inco,D.append_path,D.deal_indx,D.pre_deal_idx,D.agen_men,D.agen_flag,D.fini_flag,D.deal_man,D.deal_comm,D.fini_date,D.save_date,D.rece_date,allograph, D.allograph_date from deal_info D,step_inst S,step_def f where D.step_inco=S.step_inco  and S.step_code=f.step_code and S.flow_id=f.flow_id and (D.fini_flag=0 or D.fini_flag=1) and (f.step_tag=? or f.step_name=?) and D.flow_inid=?")).append(seeSelfOnly ? " and D.deal_man=? " : "").append(" order by D.").append(integration ? "deal_indx" : this.genSignOrderByField(aControl.getOrderByType()))));
                SQLPara sqlPara = new SQLPara();
                sqlPara.add(aControl.getTag());
                sqlPara.add(aControl.getTag());
                sqlPara.add(String.valueOf(aFlowInid));
                if (seeSelfOnly) {
                    sqlPara.add(this.baseTool.getCurrentUser());
                }
                if (Long.parseLong(aFlowInid) < minFlowInid) {
                    sdss = sdss.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                            .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                            .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                }
                dealInfoRs = this.baseTool.getRowSet(sdss, "", sqlPara);
                dealInfoRs.beforeFirst();

                label339:
                while (true) {
                    long finiFlag;
                    String dealMan;
                    String agenMan;
                    String dealComm;
                    String signUser;
                    String signUserDept;
                    Timestamp allographDate;
                    do {
                        do {
                            if (!dealInfoRs.next()) {
                                break label339;
                            }

                            finiFlag = dealInfoRs.getLong("fini_flag");
                            dealMan = dealInfoRs.getString("deal_man");
                            agenMan = dealInfoRs.getString("agen_men");
                            dealComm = dealInfoRs.getString("deal_comm");
                            String dealDept = dealInfoRs.getString("deal_dept");
                            String allograph = dealInfoRs.getString("allograph");
                            long tempDealIndx = dealInfoRs.getLong("deal_indx");
                            long tempStepInco = dealInfoRs.getLong("step_inco");
                            if (dealDept == null) {
                                dealDept = "";
                            }

                            signUser = agenMan != null && agenMan.trim().length() > 0 ? agenMan : dealMan;
                            signUserDept = dealDept.trim().length() > 0 ? String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(signUser)))).append("@").append(dealDept))) : signUser;
                            allographDate = dealInfoRs.getTimestamp("allograph_date");
                            if (allograph == null) {
                                allograph = "";
                            }

                            if (dealComm == null) {
                                dealComm = "";
                            }

                            boolean isCurrentDealInfo = dealIndx == tempDealIndx && e == tempStepInco;
                            if (isCurrentDealInfo) {
                                List receDate = this.baseTool.getFlowLib().getUserAllograph(currentUser);
                                receDate.contains(dealMan);
                            }
                        } while (dealComm.trim().length() == 0 && !aControl.isDispNullRec());
                    } while (aControl.isDispOnlyHasSign() && finiFlag != (long) 1);

                    String receDate1 = "";
                    if (dealInfoRs.getTimestamp("rece_date") != null) {
                        receDate1 = dealInfoRs.getTimestamp("rece_date").toString();
                    }

                    String userName = "";
                    String agenManName = "";
                    if (agenMan != null && agenMan.trim().length() > 0) {
                        agenManName = this.baseTool.getUserName(agenMan);
                        if (agenManName != null && agenManName.trim().length() > 0) {
                            agenManName = String.valueOf(String.valueOf((new StringBuffer("(代理")).append(agenManName).append(")")));
                        }
                    }

                    if (dealMan.indexOf(":") == -1) {
                        userName = this.baseTool.getUserName(dealMan);
                    } else {
                        userName = this.baseTool.getFrameName(PublicLib.getField(dealMan, ":", 0L));
                    }

                    String finishDate = "";
                    boolean hasSave = false;
                    Timestamp tmpFinishDate = null;
                    if (allographDate != null) {
                        tmpFinishDate = allographDate;
                        finishDate = DateTimeUtils.dateToString(allographDate, "yyyy-MM-dd HH:mm");
                        hasSave = true;
                    } else {
                        tmpFinishDate = dealInfoRs.getTimestamp(finiFlag == (long) 1 ? "fini_date" : "save_date");
                        if (tmpFinishDate != null) {
                            finishDate = DateTimeUtils.dateToString(tmpFinishDate, "yyyy-MM-dd HH:mm");
                            hasSave = true;
                        }
                    }

                    if (hasSave) {
                        if (dealComm.equals("")) {
                            dealComm = "";
                        }

                        String departName = "";
                        if (aControl.getStyle() == 1) {
                            departName = this.baseTool.getDeptLevelNameOfUser(receDate1, signUserDept, -100);
                        } else {
                            departName = this.baseTool.getDeptNameOfUser(receDate1, signUserDept);
                        }

                        switch (dataFormat) {
                            case 0:
                                signData.append(String.valueOf(String.valueOf(dealComm)).concat("\n"));
                                signData.append(String.valueOf(String.valueOf(departName)).concat("\t"));
                                signData.append(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(userName)))).append(agenManName).append("\t").append(finishDate))));
                                signData.append("\n");
                                break;
                            case 1:
                                SignDataStruct sds = new SignDataStruct();
                                sds.setSignUser(signUser);
                                sds.setSignUserName(String.valueOf(String.valueOf(userName)).concat(String.valueOf(String.valueOf(agenManName))));
                                sds.setSignComm(dealComm);
                                sds.setSignDate(tmpFinishDate);
                                sds.setSignDateDisp(finishDate);
                                sds.setSignUserDeptName(departName);
                                vec.add(sds);
                                signData.append(sds.toXml(""));
                        }
                    }
                }
            }

            replace.append(signData);
            if (vec.size() > 0) {
                SignDataStruct[] sdss1 = new SignDataStruct[vec.size()];
                vec.toArray(sdss1);
                aSomeInfoRs.putObject("_SN_REE_L_RN_TAG__", sdss1);
            }
        } catch (Exception var49) {
            this.baseTool.error("restoreSignData", var49);
            throw var49;
        } finally {
            PublicLib.closeCachedRowSet(dealInfoRs);
            dealInfoRs = null;
            PublicLib.closeCachedRowSet((CachedRowSet) frameRs);
            frameRs = null;
            PublicLib.clearObject(vec);
            vec = null;
        }

        return replace.substring(0);
    }

    private String _$13539(boolean aNewFlag, MyPen aControl, String aDisabled, String aFlowInId, String aStepInId) throws Exception {
        String replace = "";
        ResultSet dealInfoRs = null;

        try {
            String e = "";
            String appletValue = "";
            String appletHtml = "";
            String tips = "";
            String sign = "";
            long color = 0L;
            String currentUser = "";
            String controlHidden = "";
            if (aControl.getHide() == 1) {
                controlHidden = ";VISIBILITY: hidden;";
            }

            long left = aControl.getLeft();
            long top = aControl.getTop();
            long width = aControl.getWidth();
            long height = aControl.getHeight();
            String handSignName = "";
            e = String.valueOf(String.valueOf((new StringBuffer("<DIV style=\"border-left:1px solid #0;border-top:1px solid #0;border-right:1px solid #0;border-bottom:1px solid #0;left:")).append(String.valueOf(left)).append("px;").append("top:").append(String.valueOf(top)).append("px;").append("height:").append(String.valueOf(height)).append("px;").append("width:").append(String.valueOf(width)).append("px;").append(controlHidden).append(aControl.getFont()).append(";").append("Z-INDEX: 100\">")));
            String penSizeType;
            if (aDisabled.equals("")) {
                currentUser = this.baseTool.getCurrentUser();
                handSignName = " name=\"HandSign\"";
                penSizeType = this.baseTool.getUserName(currentUser);
                tips = String.valueOf(String.valueOf(tips)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(currentUser)))).append(":").append(penSizeType))))));
                appletHtml = "<input name=\"sys_flow_sign\" type=\"hidden\" value=\"\">";
            }

            if (!aNewFlag) {
                penSizeType = "";
                penSizeType = " select D.deal_man,D.save_date,I.deal_sign,U.full_name  from deal_info D left join base_user U on D.deal_man=U.user_code,step_inst S,sign_info I,step_def f where D.step_inco=S.step_inco  and D.step_inco=I.step_inco and D.deal_indx=I.deal_indx and S.step_code=f.step_code and S.flow_id=f.flow_id and D.flow_inid=? and (f.step_tag=? or f.step_name=?)";
                SQLPara penWidth = new SQLPara();
                penWidth.add(String.valueOf(aFlowInId));
                penWidth.add(aControl.getTag());
                penWidth.add(aControl.getTag());
                if (Long.parseLong(aFlowInId) < minFlowInid) {
                    penSizeType = penSizeType.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                            .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                            .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                }
                dealInfoRs = this.baseTool.getResultSet(penSizeType, penWidth);
                InputStream in = null;

                while (dealInfoRs.next()) {
                    try {
                        in = dealInfoRs.getBinaryStream("deal_sign");
                        if (in != null) {
                            byte[] ex = PublicLib.unzip(in);
                            sign = String.valueOf(String.valueOf(sign)).concat(String.valueOf(String.valueOf(new String(ex))));
                            String dealMan = dealInfoRs.getString("deal_man");
                            String saveDate = dealInfoRs.getTimestamp("save_date").toString();
                            String fullName = dealInfoRs.getString("full_name");
                            if (!tips.equals("")) {
                                tips = String.valueOf(String.valueOf(tips)).concat(";");
                            }

                            tips = String.valueOf(String.valueOf(tips)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(dealMan)))).append(":").append(fullName))))));
                        }
                    } catch (Exception var45) {
                        throw new Exception(var45.getMessage());
                    } finally {
                        PublicLib.closeStream(in);
                        in = null;
                    }
                }
            }

            int penSizeType1 = aControl.getPenSizeType();
            float penWidth1 = 0.4F;
            switch (penSizeType1) {
                case 0:
                    penWidth1 = 1.0F;
                    break;
                case 1:
                    penWidth1 = 0.5F;
                    break;
                case 2:
                    penWidth1 = 0.1F;
                    break;
                case 3:
                    penWidth1 = 0.01F;
            }

            appletHtml = String.valueOf(String.valueOf(appletHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<applet codebase=\".\" code=\"signcontroller.DynaDraw.class\"")).append(handSignName).append(" archive=\"HandSign.jar\"").append(" width=").append(width).append(" height=").append(height).append(" hspace=\"0\" vspace=\"0\" align=\"middle\" MAYSCRIPT>"))))));
            appletHtml = String.valueOf(String.valueOf(appletHtml)).concat("<param name=\"defaultColor\" value=\"0,0,0\">");
            appletHtml = String.valueOf(String.valueOf(appletHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<param name=\"currentUser\" value=\"")).append(currentUser).append("\">"))))));
            appletHtml = String.valueOf(String.valueOf(appletHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<param name=\"tipInfo\" value=\"")).append(tips).append("\">"))))));
            appletHtml = String.valueOf(String.valueOf(appletHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<param name=\"signInfo\" value=\"")).append(sign).append("\">"))))));
            appletHtml = String.valueOf(String.valueOf(appletHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<param name=\"penWidth\" value=\"")).append(penWidth1).append("\">"))))));
            appletHtml = String.valueOf(String.valueOf(appletHtml)).concat("</applet>");
            replace = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(e)))).append(appletHtml).append("</DIV>")));
        } catch (Exception var47) {
            this.baseTool.error("restorePen", var47);
            throw new Exception(var47.getMessage());
        } finally {
            PublicLib.closeRsToConn(dealInfoRs);
            dealInfoRs = null;
        }

        return replace;
    }

    private String _$13551(boolean aNewFlag, MyPrgForm aControl, String aDisabled, String aFlowInId, String aStepInId, String aPrimayKey, boolean aReadOnly, String tableName) throws Exception {
        String replace = "";
        StringBuffer frameUrl = null;
        StringBuffer frameHtml = null;

        try {
            StringBuffer e = new StringBuffer("");
            String appletValue = "";
            String appletHtml = "";
            String tips = "";
            String sign = "";
            long color = 0L;
            String currentUser = "";
            String controlHidden = "";
            if (aControl.getHide() == 1) {
                controlHidden = ";VISIBILITY: hidden;";
            }

            long left = aControl.getLeft();
            long top = aControl.getTop();
            long width = aControl.getWidth();
            long height = aControl.getHeight();
            String sFont = aControl.getFont();
            String handSignName = "";
            if (sFont == null) {
                sFont = "";
            }

            String borderColor = "#0";
            if (sFont.trim().length() > 0) {
                sFont = sFont.toUpperCase();
                int iTagPos = sFont.indexOf("BORDERCOLOR:", 0);
                int iTagLen = "BORDERCOLOR:".length();
                if (iTagPos != -1) {
                    int iEndPos = sFont.indexOf(";", iTagPos + iTagLen);
                    if (iEndPos != -1) {
                        borderColor = sFont.substring(iTagPos + iTagLen, iEndPos);
                    }
                }
            }

            e.append("style=\"border-left:1px solid ".concat(String.valueOf(String.valueOf(borderColor))));
            e.append(String.valueOf(String.valueOf((new StringBuffer(";border-top:1px solid ")).append(borderColor).append(";"))));
            e.append("border-right:1px solid ".concat(String.valueOf(String.valueOf(borderColor))));
            e.append(String.valueOf(String.valueOf((new StringBuffer(";border-bottom:1px solid ")).append(borderColor).append(";"))));
            e.append(String.valueOf(String.valueOf((new StringBuffer("left:")).append(String.valueOf(left)).append("px;"))));
            e.append(String.valueOf(String.valueOf((new StringBuffer("top:")).append(String.valueOf(top)).append("px;"))));
            e.append(String.valueOf(String.valueOf((new StringBuffer("height:")).append(String.valueOf(height)).append("px;"))));
            e.append(String.valueOf(String.valueOf((new StringBuffer("width:")).append(String.valueOf(width)).append("px;"))));
            e.append(controlHidden);
            e.append(String.valueOf(String.valueOf(aControl.getFont())).concat(";"));
            e.append("Z-INDEX: 100\" class=\"PrgForm\" ");
            frameUrl = new StringBuffer(aControl.getUrl());
            if (!aControl.getUrl().trim().equals("")) {
                if (aControl.getUrl().indexOf("?") == -1) {
                    frameUrl.append("?");
                } else {
                    frameUrl.append("&");
                }

                frameUrl.append("flowInstId=".concat(String.valueOf(String.valueOf(aFlowInId))));
                frameUrl.append("&stepInstId=".concat(String.valueOf(String.valueOf(aStepInId))));
                frameUrl.append("&readOnly=".concat(String.valueOf(String.valueOf(aReadOnly))));
                frameUrl.append("&primaryKey=".concat(String.valueOf(String.valueOf(aPrimayKey))));
                frameUrl.append("&tableName=".concat(String.valueOf(String.valueOf(tableName))));
                if (aDisabled != null && !aDisabled.equals("")) {
                    frameUrl.append("&formdisabled=true");
                }
            }

            frameHtml = new StringBuffer("<iframe ");
            frameHtml.append(e);
            frameHtml.append(String.valueOf(String.valueOf((new StringBuffer(" name=\"")).append(aControl.getControlName()).append("\" "))));
            frameHtml.append(" frameborder=\"0\" ");
            frameHtml.append(" src=\"".concat(String.valueOf(String.valueOf(frameUrl))));
            frameHtml.append("\" ></iframe>");
            replace = frameHtml.toString();
        } catch (Exception var39) {
            this.baseTool.error("restorePrgForm", var39);
            throw new Exception(var39.getMessage());
        } finally {
            frameUrl = null;
            frameHtml = null;
        }

        return replace;
    }

    private String _$13542(FlowInstInfo fii, String aServerName, boolean aNewFlag, boolean aIsFormula, boolean aForceUpdate, MyMap aControl, String aDisabled, String aFieldValue, String aFormSetId, long aFormCode, StringBuffer aRealValue, boolean aReadOnly) throws Exception {
        String replace = "";

        try {
            String e = this.baseTool.getCurrentUser();
            String currentDept = "";
            if (fii != null && fii.getFormInitData() != null) {
                currentDept = (String) fii.getFormInitData().get("currentUserDeptCode");
            }

            String fieldValue = "";
            boolean iDefaultValue = true;
            String controlHidden = "";
            if (aControl.getHide() == 1) {
                controlHidden = ";VISIBILITY: hidden;";
            }

            int fieldType = (int) aControl.getFieldType();
            if (aReadOnly) {
                fieldValue = aFieldValue;
            } else if ((!aIsFormula || aForceUpdate || aFieldValue != null && !aFieldValue.equals("")) && (!aIsFormula || !aForceUpdate)) {
                fieldValue = aFieldValue;
            } else if (aControl.getDefaultType() == (long) 1) {
                int iDefaultValue1;
                try {
                    iDefaultValue1 = Integer.parseInt(aControl.getDefaultValue());
                } catch (Exception var40) {
                    iDefaultValue1 = -1;
                }

                label286:
                switch (fieldType) {
                    case 1:
                    case 2:
                    case 3:
                    case 7:
                        switch (iDefaultValue1) {
                            case 2:
                                fieldValue = e;
                                break label286;
                            case 3:
                                if (currentDept != null && currentDept.trim().length() != 0) {
                                    fieldValue = currentDept;
                                } else {
                                    fieldValue = this.baseTool.getDeptSeq(e);
                                }
                            default:
                                break label286;
                        }
                    case 4:
                    case 5:
                    case 6:
                        if (iDefaultValue1 == 0 || iDefaultValue1 == 1) {
                            fieldValue = PublicLib.getCurrentTimeStr();
                        }
                }
            }

            if (fieldType == 4 || fieldType == 5 || fieldType == 6) {
                fieldValue = PublicLib.getField(fieldValue, " ", 0L);
            }

            aRealValue.append(fieldValue);
            String controlName = aControl.getControlName();
            String valueHtml;
            if (aDisabled.equals("")) {
                valueHtml = String.valueOf(String.valueOf((new StringBuffer("<DIV id=Main_Map_Href style=\"")).append(aControl.getFont()).append(";left:").append(String.valueOf(aControl.getLeft() + aControl.getWidth())).append(";top:").append(String.valueOf(aControl.getTop())).append(controlHidden).append(";Z-INDEX: 100\">")));
                String urlPara = String.valueOf(String.valueOf((new StringBuffer("fsetId=")).append(String.valueOf(aFormSetId)).append("&formCode=").append(String.valueOf(aFormCode)).append("&logicName=").append(aControl.getLogicName())));
                String url = "";
                String function = "";
                switch ((int) aControl.getFrom()) {
                    case 0:
                    case 1:
                        String otherPara = "fromdsn=0";
                        boolean bFromdsn = false;
                        if (aControl.getFrom() == (long) 1) {
                            otherPara = "fromdsn=1";
                            bFromdsn = true;
                        }

                        if (aControl.getSearchField() != null) {
                            otherPara = String.valueOf(String.valueOf(otherPara)).concat(String.valueOf(String.valueOf("&searchfield=".concat(String.valueOf(String.valueOf(aControl.getSearchField()))))));
                        } else {
                            otherPara = String.valueOf(String.valueOf(otherPara)).concat("&searchfield=");
                        }

                        function = String.valueOf(String.valueOf((new StringBuffer("javascript:selectList(\'")).append(urlPara).append("&").append(otherPara).append("\',\'").append(controlName).append("\',\'").append(this.getExtraName(controlName, "hidden")).append("\',").append(bFromdsn).append(");")));
                        break;
                    case 2:
                        long iServerType = aControl.getServerType();
                        String sServerName = "";
                        if (iServerType == (long) 0) {
                            sServerName = aServerName;
                        } else if (iServerType == (long) 2) {
                            sServerName = aControl.getServerName();
                        }

                        long selFlag = 4L;
                        if (aControl.getSelectType() == (long) 0) {
                            selFlag = 3L;
                        }

                        String framList = "";
                        String groupList = "";
                        if (aControl.getFrameFlag() == (long) 3) {
                            framList = aControl.getFrameList();
                        }

                        if (aControl.getFrameFlag() == (long) 4) {
                            groupList = aControl.getGroupList();
                        }

                        function = String.valueOf(String.valueOf((new StringBuffer("javascript:openFrameList(\'")).append(controlName).append("\',\'").append(this.getExtraName(controlName, "hidden")).append("\',").append(selFlag).append(",").append(aControl.getFrameFlag()).append(",").append(aControl.getFrameLevel()).append(",").append(aControl.getFrameLevelTo()).append(",\'").append(framList).append("\',").append(aControl.isDisplayGroupNode() ? "1" : "0").append(",").append(aControl.isDisplayPersGroupNode() ? "1" : "0").append(",").append(aControl.isDisplayAllUserNode() ? "1" : "0").append(",\'").append(aControl.getGroupNodeDesc()).append("\',\'").append(aControl.getPersGroupNodeDesc()).append("\',\'").append(groupList).append("\',\'").append(sServerName).append("\',").append(aControl.getModifyMode()).append(")")));
                }

                if (aControl.getButtonStyle() == (long) 0) {
                    url = String.valueOf(String.valueOf((new StringBuffer("<span style=\"cursor:hand;\" onclick=\"")).append(function).append(";\"><B>[选择]</B></span>")));
                } else if (aControl.getButtonStyle() == (long) 1) {
                    url = String.valueOf(String.valueOf((new StringBuffer("&nbsp;<span class=\"standardBtn\" style=\"width:40px;\" onclick=\"")).append(function).append("\" onmouseover=\"javascript:selectBtnOver();\" onmouseout=\"javascript:selectBtnOut();\">选择</span>")));
                }

                replace = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(valueHtml)))).append(url).append("</DIV>")));
            }

            valueHtml = String.valueOf(String.valueOf((new StringBuffer("<input type=hidden name=\'")).append(this.getExtraName(controlName, "hidden")).append("\'").append(" value=\'").append(fieldValue).append("\' ").append(">")));
            replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(valueHtml)));
        } catch (Exception var41) {
            this.baseTool.error("restoreMap", var41);
            throw new Exception(var41.getMessage());
        } finally {
            ;
        }

        return replace;
    }

    protected String getExtraName(String controlName, String suffix) {
        return String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(controlName)))).append("_").append(suffix)));
    }

    public String restoreMapDisplay(MyMap aControl, String aServerName, StringBuffer aRealValue) throws Exception {
        String replace = "";
        CachedRowSet crs = null;
        byte refreshInterval = 60;
        SimpleCache datasourceCache = null;

        try {
            String e = aRealValue.toString();
            e = e.trim();
            String displayField;
            if (e.equals("")) {
                displayField = aControl.isMultiline() ? "" : "value=\"\"";
                return displayField;
            }

            displayField = "";
            String valueField = "";
            String table = "";
            long from = aControl.getFrom();
            String dataServerName;
            if (aControl.getFormat() == (long) 1) {
                if (aControl.isMultiline()) {
                    replace = e;
                } else {
                    replace = String.valueOf(String.valueOf((new StringBuffer("value=\"")).append(e).append("\"")));
                }

                dataServerName = replace;
                return dataServerName;
            }

            dataServerName = null;
            StringSplitStru sssFieldValueList = new StringSplitStru(e, ";");
            int fieldCount = sssFieldValueList.getFieldCount();
            String value;
            String type;
            if (from == (long) 1) {
                displayField = aControl.getDisplayField();
                valueField = aControl.getValueField();
                table = aControl.getTable();
                displayField = PublicLib.getField(displayField, "<", 0L);
                valueField = PublicLib.getField(valueField, "<", 0L);
                table = PublicLib.getField(table, "<", 0L);
                String var30 = null;
                switch ((int) aControl.getServerType()) {
                    case 0:
                        dataServerName = aServerName;
                        var30 = aServerName;
                        break;
                    case 1:
                        var30 = "WORKFLOW_".concat(String.valueOf(String.valueOf(this.baseTool.getCurrentRegion())));
                        dataServerName = null;
                        break;
                    case 2:
                        dataServerName = aControl.getServerName();
                        var30 = dataServerName;
                }

                String i = this.baseTool.getCurrentRegion();
                value = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(i)))).append("_").append(dataServerName == null ? "_" : dataServerName).append("_").append(var30).append(".").append(table).append(".").append(displayField).append("_").append(valueField)));
                datasourceCache = (SimpleCache) this.baseTool.getCacheHolder().get(value);
                if (datasourceCache == null) {
                    datasourceCache = new SimpleCache(2147483647, (long) refreshInterval);
                    this.baseTool.getCacheHolder().put(value, datasourceCache);
                }

                datasourceCache.tochCache();
                if (datasourceCache.size() == 0) {
                    SQLPara display = new SQLPara();
                    type = String.valueOf(String.valueOf((new StringBuffer("select ")).append(displayField).append(",").append(valueField).append(" from ").append(table)));
                    if (dataServerName == null) {
                        crs = this.baseTool.getRowSet(type, "", display);
                    } else {
                        crs = this.baseTool.getServerRowSet(dataServerName, type, "", display);
                    }

                    String iType = null;
                    String value1 = null;

                    while (crs.next()) {
                        iType = crs.getString(displayField);
                        value1 = crs.getString(valueField);
                        if (iType != null && value1 != null) {
                            datasourceCache.put(value1, iType);
                        }
                    }
                }
            } else if (from == (long) 2) {
                long isFromServer = aControl.getServerType();
                if (isFromServer == (long) 0) {
                    dataServerName = aServerName;
                } else if (isFromServer == (long) 2) {
                    dataServerName = aControl.getServerName();
                } else {
                    dataServerName = null;
                }
            }

            boolean var32 = dataServerName != null;
            if (datasourceCache != null) {
                datasourceCache.lock();
            }

            for (int var31 = 0; var31 < fieldCount; ++var31) {
                value = PublicLib.getField(sssFieldValueList.getField(var31), "|", 0L);
                String var33 = "";
                if (value != null && value.trim().length() != 0) {
                    if (from == (long) 0) {
                        for (int var34 = 0; var34 < PublicLib.getFieldCount(aControl.getValueList(), ";"); ++var34) {
                            if (PublicLib.getField(aControl.getValueList(), ";", (long) var34).equalsIgnoreCase(value)) {
                                var33 = PublicLib.getField(aControl.getDisplayList(), ";", (long) var34);
                                break;
                            }
                        }
                    } else if (from == (long) 1) {
                        var33 = (String) datasourceCache.get(value);
                    } else if (from == (long) 2) {
                        type = PublicLib.getField(value, ":", 1L);
                        if (type == null || type.trim().length() == 0) {
                            type = "0";
                        }

                        int var35 = Integer.parseInt(type);
                        value = PublicLib.getField(value, ":", 0L);
                        if (value != null) {
                            switch (var35) {
                                case 0:
                                    var33 = var32 ? this.baseTool.getServerUserName(dataServerName, value) : this.baseTool.getUserName(value);
                                    break;
                                case 3:
                                    var33 = var32 ? this.baseTool.getServerFrameName(dataServerName, value) : this.baseTool.getFrameName(value);
                                    break;
                                case 8:
                                    var33 = var32 ? this.baseTool.getServerPrivateGroupName(dataServerName, value) : this.baseTool.getPrivateGroupName(value);
                            }
                        }

                        if (var33 == null || var33.equals("")) {
                            var33 = value;
                        }
                    }

                    if (var33 == null) {
                        var33 = "";
                    }

                    if (!replace.equals("")) {
                        replace = String.valueOf(String.valueOf(replace)).concat("、");
                    }

                    switch ((int) aControl.getFormat()) {
                        case 0:
                            replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(var33)));
                        case 1:
                        default:
                            break;
                        case 2:
                            replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(var33)))).append("[").append(value).append("]"))))));
                            break;
                        case 3:
                            replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(value)))).append("[").append(var33).append("]"))))));
                    }
                }
            }

            if (datasourceCache != null) {
                datasourceCache.unlock();
            }

            if (!aControl.isMultiline()) {
                replace = String.valueOf(String.valueOf((new StringBuffer("value=\"")).append(replace).append("\"")));
            }
        } catch (Exception var28) {
            this.baseTool.error("restoreMapDisplay", var28);
            throw new Exception(var28.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
        }

        return replace;
    }

    public String restoreMapDisplay_backup(MyMap aControl, String aServerName, StringBuffer aRealValue) throws Exception {
        String replace = "";
        CachedRowSet crs = null;
        CachedRowSet privateGroups = null;
        Hashtable hPrivateGroups = new Hashtable();

        String sssFieldValueList;
        try {
            String e = aRealValue.toString();
            e = e.trim();
            String displayField;
            if (e.equals("")) {
                displayField = "value=\"\"";
                return displayField;
            }

            displayField = "";
            String valueField = "";
            String table = "";
            long from = aControl.getFrom();
            if (aControl.getFormat() != (long) 1) {
                StringSplitStru var32 = new StringSplitStru(e, ";");
                int fieldCount = var32.getFieldCount();
                String i;
                String value;
                SQLPara display;
                int iType;
                String criteria;
                int var35;
                String var37;
                if (from == (long) 1) {
                    displayField = aControl.getDisplayField();
                    valueField = aControl.getValueField();
                    table = aControl.getTable();
                    displayField = PublicLib.getField(displayField, "<", 0L);
                    valueField = PublicLib.getField(valueField, "<", 0L);
                    table = PublicLib.getField(table, "<", 0L);
                    i = "";
                    int var33 = 1;
                    switch ((int) aControl.getServerType()) {
                        case 0:
                            var33 = this.baseTool.getFieldType(aServerName, table, valueField);
                            break;
                        case 1:
                            var33 = this.baseTool.getFieldType(table, valueField);
                            break;
                        case 2:
                            var33 = this.baseTool.getFieldType(aControl.getServerName(), table, valueField);
                    }

                    display = new SQLPara();
                    var35 = 0;

                    label470:
                    while (true) {
                        if (var35 >= fieldCount) {
                            var37 = String.valueOf(String.valueOf((new StringBuffer("select ")).append(displayField).append(",").append(valueField).append(" from ").append(table).append(" where ")));
                            if (fieldCount > 1) {
                                var37 = String.valueOf(String.valueOf(var37)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(valueField)))).append(" in (").append(i).append(")"))))));
                            } else {
                                var37 = String.valueOf(String.valueOf(var37)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(valueField)).concat("=?"))));
                            }

                            switch ((int) aControl.getServerType()) {
                                case 0:
                                    crs = this.baseTool.getServerRowSet(aServerName, var37, "", display);
                                    break label470;
                                case 1:
                                    crs = this.baseTool.getRowSet(var37, "", display);
                                    break label470;
                                case 2:
                                    crs = this.baseTool.getServerRowSet(aControl.getServerName(), var37, "", display);
                                default:
                                    break label470;
                            }
                        }

                        if (!i.equals("")) {
                            i = String.valueOf(String.valueOf(i)).concat(",");
                        }

                        i = String.valueOf(String.valueOf(i)).concat("?");
                        String var39 = PublicLib.getField(var32.getField(var35), "|", 0L);
                        if (var33 == 3) {
                            display.add(var39, 1);
                        } else {
                            display.add(var39);
                        }

                        ++var35;
                    }
                } else if (from == (long) 2) {
                    i = "";
                    value = "";
                    display = new SQLPara();
                    SQLPara type = new SQLPara();

                    String rsFinded;
                    String groupCode;
                    for (iType = 0; iType < fieldCount; ++iType) {
                        criteria = PublicLib.getField(var32.getField(iType), "|", 0L);
                        rsFinded = PublicLib.getField(criteria, ":", 1L);
                        groupCode = PublicLib.getField(criteria, ":", 0L);
                        if (rsFinded == null) {
                            rsFinded = "0";
                        }

                        if (rsFinded.trim().length() == 0) {
                            rsFinded = "0";
                        }

                        int groupName = Integer.parseInt(rsFinded);
                        switch (groupName) {
                            case 0:
                                if (!i.equals("")) {
                                    i = String.valueOf(String.valueOf(i)).concat(" or ");
                                }

                                type.add(groupCode);
                                i = String.valueOf(String.valueOf(i)).concat("p.empl_id=?");
                                break;
                            case 3:
                                if (!i.equals("")) {
                                    i = String.valueOf(String.valueOf(i)).concat(" or ");
                                }

                                type.add(groupCode);
                                i = String.valueOf(String.valueOf(i)).concat("f2.fram_seq=?");
                                break;
                            case 8:
                                if (value.trim().length() > 0) {
                                    value = String.valueOf(String.valueOf(value)).concat(" or ");
                                }

                                display.add(groupCode);
                                value = String.valueOf(String.valueOf(value)).concat("pw.grop_code=?");
                        }
                    }

                    long var38 = aControl.getServerType();
                    if (i.trim().length() > 0) {
                        i = String.valueOf(String.valueOf((new StringBuffer(" where (")).append(i).append(") ")));
                        rsFinded = "select distinct f2.fram_seq,f2.fram_name,p.empl_id,u.full_name from framework f2 left join position p on f2.fram_seq=p.posi_seq left join base_user u on p.empl_id=u.user_code".concat(String.valueOf(String.valueOf(i)));
                        if (var38 == (long) 0) {
                            crs = this.baseTool.getServerRowSet(aServerName, rsFinded, "", type);
                        } else if (var38 == (long) 2) {
                            crs = this.baseTool.getServerRowSet(aControl.getServerName(), rsFinded, "", type);
                        } else {
                            crs = this.baseTool.getRowSet(rsFinded, "", type);
                        }
                    }

                    if (value.trim().length() > 0) {
                        rsFinded = "select pw.*,f.user_name from wf_private_workgroup pw left join frameuser f on pw.owner_user=f.user_code where ".concat(String.valueOf(String.valueOf(value)));
                        if (var38 == (long) 0) {
                            privateGroups = this.baseTool.getServerRowSet(aServerName, rsFinded, "", display);
                        } else if (var38 == (long) 2) {
                            privateGroups = this.baseTool.getServerRowSet(aControl.getServerName(), rsFinded, "", display);
                        } else {
                            privateGroups = this.baseTool.getRowSet(rsFinded, "", display);
                        }

                        groupCode = "";
                        String var41 = "";

                        for (String userName = ""; privateGroups.next(); hPrivateGroups.put(groupCode, var41)) {
                            groupCode = privateGroups.getString("GROP_CODE");
                            var41 = privateGroups.getString("GROP_NAME");
                            userName = privateGroups.getString("USER_NAME");
                            if (groupCode == null) {
                                groupCode = "";
                            }

                            if (var41 == null) {
                                var41 = "";
                            }

                            if (userName == null) {
                                userName = "";
                            }
                        }
                    }
                }

                for (int var36 = 0; var36 < fieldCount; ++var36) {
                    value = PublicLib.getField(var32.getField(var36), "|", 0L);
                    String var34 = "";
                    if (!value.equals("")) {
                        if (from == (long) 0) {
                            for (var35 = 0; var35 < PublicLib.getFieldCount(aControl.getValueList(), ";"); ++var35) {
                                if (PublicLib.getField(aControl.getValueList(), ";", (long) var35).equalsIgnoreCase(value)) {
                                    var34 = PublicLib.getField(aControl.getDisplayList(), ";", (long) var35);
                                    break;
                                }
                            }
                        } else if (from == (long) 1) {
                            var37 = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(valueField)))).append("=").append(value).append("")));
                            boolean var40 = PublicLib.find(crs, var37, true);
                            if (var40) {
                                var34 = crs.getString(displayField);
                            }
                        } else if (from == (long) 2) {
                            var37 = PublicLib.getField(value, ":", 1L);
                            if (var37 == null) {
                                var37 = "0";
                            }

                            if (var37.trim().length() == 0) {
                                var37 = "0";
                            }

                            iType = Integer.parseInt(var37);
                            value = PublicLib.getField(value, ":", 0L);
                            criteria = "";
                            boolean var42 = false;
                            switch (iType) {
                                case 0:
                                    criteria = String.valueOf(String.valueOf((new StringBuffer("empl_id=\'")).append(value).append("\'")));
                                    var42 = PublicLib.find(crs, criteria, true);
                                    if (var42) {
                                        var34 = crs.getString("full_name");
                                    }
                                    break;
                                case 3:
                                    criteria = String.valueOf(String.valueOf((new StringBuffer("fram_seq=\'")).append(value).append("\'")));
                                    var42 = PublicLib.find(crs, criteria, true);
                                    if (var42) {
                                        var34 = crs.getString("fram_name");
                                    }
                                    break;
                                case 8:
                                    if (value != null && value.trim().length() > 0) {
                                        var34 = (String) hPrivateGroups.get(value);
                                    }
                            }

                            if (var34 == null || var34.equals("")) {
                                var34 = value;
                            }
                        }

                        if (var34 == null) {
                            var34 = "";
                        }

                        if (!replace.equals("")) {
                            replace = String.valueOf(String.valueOf(replace)).concat("、");
                        }

                        switch ((int) aControl.getFormat()) {
                            case 0:
                                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(var34)));
                            case 1:
                            default:
                                break;
                            case 2:
                                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(var34)))).append("[").append(value).append("]"))))));
                                break;
                            case 3:
                                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(value)))).append("[").append(var34).append("]"))))));
                        }
                    }
                }

                replace = String.valueOf(String.valueOf((new StringBuffer("value=\"")).append(replace).append("\"")));
                return replace;
            }

            replace = String.valueOf(String.valueOf((new StringBuffer("value=\"")).append(e).append("\"")));
            sssFieldValueList = replace;
        } catch (Exception var30) {
            this.baseTool.error("restoreMapDisplay", var30);
            throw new Exception(var30.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(crs);
            crs = null;
            PublicLib.closeCachedRowSet(privateGroups);
            privateGroups = null;
            PublicLib.clearObject(hPrivateGroups);
            hPrivateGroups = null;
        }

        return sssFieldValueList;
    }

    private String _$13548(boolean aNewFlag, boolean aReadOnly, MyAttach aControl, String aFlowInid, CachedRowSet aMainRs, String aStepName, String aDisabled, long aIfDoc, long aPopedom, FlowInstInfo fii) throws Exception {
        String replace = "";
        CachedRowSet attachRs = null;

        try {
            String e = "";
            if (aControl.getHide() == 1) {
                e = ";VISIBILITY: hidden;";
            }

            String style = String.valueOf(String.valueOf((new StringBuffer("left:")).append(String.valueOf(aControl.getLeft())).append("px;").append("top:").append(String.valueOf(aControl.getTop())).append("px;").append("height:").append(String.valueOf(aControl.getHeight())).append("px;").append("width:").append(String.valueOf(aControl.getWidth())).append("px;").append(e).append(aControl.getFont()).append(";")));
            if (aControl.getType() == 1) {
                String flag = "";
                if (aIfDoc != (long) 0 && (aPopedom & (long) 16) == (long) 16) {
                    flag = " style=\"width:100px;cursor:hand;\" onclick=\"javascript:buttonWord(false,\'\',true);\" ";
                } else {
                    flag = " disabled ";
                }

                String editDisabled = "";
                if (aIfDoc != (long) 0 && !aReadOnly && (aPopedom & (long) 32) == (long) 32) {
                    editDisabled = " style=\"width:100px;cursor:hand;\" onclick=\"javascript:buttonWord(false,\'\',false);\" ";
                } else {
                    editDisabled = " disabled ";
                }

                replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<DIV style=\"")).append(style).append("\">").append("<span").append(flag).append("><img src=\"Images/icon_read.gif\" border=\"0\">&nbsp;阅读正文</span>").append("<span").append(editDisabled).append("><img src=\"Images/icon_write.gif\" border=\"0\">&nbsp;编辑正文</span>").append("</div>"))))));
            } else {
                long var41 = (long) aControl.getFlag();
                long iStyle = (long) aControl.getStyle();
                String owner = aControl.getOwner();
                String tag = aControl.getTag();
                int displayStyle = aControl.getDisplayStyle();
                String controlName = aControl.getControlName();
                switch (displayStyle) {
                    case 0:
                        style = String.valueOf(String.valueOf(style)).concat("BORDER-RIGHT:thin inset;BORDER-TOP:thin inset;BORDER-LEFT:thin inset;BORDER-BOTTOM:thin inset;");
                        break;
                    case 1:
                        style = String.valueOf(String.valueOf(style)).concat("BORDER-RIGHT:1px solid;BORDER-TOP:1px solid;BORDER-LEFT:1px solid; BORDER-BOTTOM:1px solid;");
                        break;
                    case 2:
                        style = String.valueOf(String.valueOf(style)).concat("BORDER-RIGHT:medium none;BORDER-TOP:medium none; BORDER-LEFT:medium none;BORDER-BOTTOM:1px solid;");
                        break;
                    case 3:
                        style = String.valueOf(String.valueOf(style)).concat("BORDER-RIGHT:1px; BORDER-TOP:1px;BORDER-LEFT:1px;BORDER-BOTTOM:1px;");
                }

                String value = "";
                if (!aNewFlag) {
                    String readOnly = "";
                    if (owner != null && !owner.equals("")) {
                        owner = PublicLib.getField(owner, "<", 0L);
                        if (aMainRs != null && aMainRs.size() > 0) {
                            try {
                                readOnly = aMainRs.getString(owner);
                            } catch (Exception var38) {
                                System.err.println("invalid field ".concat(String.valueOf(String.valueOf(owner))));
                                throw new Exception(var38.getMessage());
                            }
                        }
                    }

                    String sqlClause = "";
                    SQLPara sqlPara = new SQLPara();
                    if (tag != null && !tag.equals("")) {
                        sqlClause = "select f.file_name from sys_file2 f,step_inst s,flow_inst t,step_def e where f.flow_inid=s.flow_inid and f.step_inco=s.step_inco and s.step_code=e.step_code and s.flow_id=e.flow_id and e.step_name=? and f.step_inco is not null and ";
                        sqlPara.add(tag);
                    } else {
                        sqlClause = "select f.file_name from sys_file2 f,flow_inst t where ";
                    }

                    sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" f.flow_inid=t.flow_inid and f.file_name<>t.doc_name and f.flow_inid=?");
                    sqlPara.add(String.valueOf(aFlowInid));
                    StepMiscStruct stepMiscSet = fii.getStepMiscStruct();
                    if (stepMiscSet != null && stepMiscSet.isOnlyCreatorCanRead()) {
                        readOnly = this.baseTool.getCurrentUser();
                    }

                    if (readOnly != null && !readOnly.equals("")) {
                        sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and f.modi_user=?");
                        sqlPara.add(readOnly);
                    }

                    sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" order by f.FILE_INID");

                    if (Long.parseLong(aFlowInid) < minFlowInid) {
                        sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                        sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                    }

                    attachRs = this.baseTool.getRowSet(sqlClause, "", sqlPara);
                    int i = 0;

                    while (attachRs.next()) {
                        String fileName = attachRs.getString("file_name");
                        int pos = fileName.lastIndexOf(".");
                        if (pos == -1) {
                            pos = fileName.length();
                        }

                        if (iStyle == (long) 0) {
                            value = String.valueOf(String.valueOf(value)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<OPTION value=\"")).append(fileName).append("\">《").append(fileName.substring(0, pos)).append("》</OPTION>"))))));
                        } else if (iStyle == (long) 1) {
                            if (!value.equals("")) {
                                value = String.valueOf(String.valueOf(value)).concat(";");
                            }

                            value = String.valueOf(String.valueOf(value)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("《")).append(fileName.substring(0, pos)).append("》"))))));
                        } else if (iStyle == (long) 2) {
                            String var10000 = String.valueOf(String.valueOf(value));
                            StringBuffer var10001 = new StringBuffer("<tr><td width=\'10%\' align=\'center\' class=\"attach_cont_1\">");
                            ++i;
                            value = var10000.concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(var10001.append(i).append("</td>").append("<td width=\'90%\' align=\'left\' style=\'cursor:hand;padding-left:5px;\' class=\"attach_cont_2\" onclick=\"javascript:openAttach(\'").append(fileName).append("\');\">").append(fileName).append("</td></tr>"))))));
                        }
                    }
                }

                if (iStyle == (long) 0) {
                    replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<select style=\"")).append(style).append("\" name=\"").append(controlName).append("\" onchange=\"javascript:openFile(\'").append(controlName).append("\');\">").append("<option value=\"\">请选择附件</option>").append(value).append("</select>"))))));
                } else if (iStyle == (long) 1) {
                    replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<textarea readonly style=\"")).append(style).append("\" name=\"").append(controlName).append("\">").append(value).append("</textarea>"))))));
                } else if (iStyle == (long) 2) {
                    replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<div style=\"")).append(style).append("\" class=\"attach_div\">").append("<table width=\'100%\' cellspacing=\'0\' cellpadding=\'0\' borderColorDark=\'white\' rules=rows borderColorLight=\'black\'>").append("<tr><td align=\'center\' width=\'10%\' class=\'attach_title\'>序号</td>").append("<td align=\'center\' width=\'90%\' class=\'attach_title\'>文件名</td>").append("</tr></table>").append("<table width=\'100%\' cellspacing=\'0\' cellpadding=\'0\' borderColorDark=\'white\' rules=rows borderColorLight=\'black\' class=\"attach_table\">").append(value).append("</table></div>"))))));
                }

                if (var41 == (long) 1) {
                    long var42 = 0L;
                    if (aReadOnly) {
                        var42 = 1L;
                    }

                    if (aDisabled.equals("")) {
                        replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<DIV style=\"")).append(aControl.getFont()).append(";left:").append(String.valueOf(aControl.getLeft() + aControl.getWidth())).append(";top:").append(String.valueOf(aControl.getTop())).append(e).append(";Z-INDEX: 100\">").append("<span style=\"width:100px;cursor:hand;\"  onclick=\"javascript:buttonAttach(").append(var42).append(");\"><img src=\"Images/icon_attach.gif\" border=\"0\">&nbsp;附件编辑</span>").append("</div>"))))));
                    }
                }
            }
        } catch (Exception var39) {
            this.baseTool.error("restoreAttach", var39);
            throw new Exception(var39.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(attachRs);
        }

        return replace;
    }

    private boolean _$13719(String userCode) {
        boolean bRet = false;
        ResultSet rs = null;

        label40:
        try {
            try {
                String ex = " select user_sign from usersign where user_code=?";
                SQLPara sqlPara = new SQLPara();
                sqlPara.add(userCode);
                rs = this.baseTool.getResultSet(ex, sqlPara);
                if (rs.next()) {
                    InputStream in = rs.getBinaryStream("user_sign");
                    if (in != null) {
                        bRet = true;
                    }
                }
            } catch (Exception var11) {
                bRet = false;
                var11.printStackTrace();
            }
        } finally {
            break label40;
        }

        PublicLib.closeRsToConn(rs);
        rs = null;
        return bRet;
    }

    private String _$13549(MyStamp aControl, String aDisabled, String aFieldValue, RecordSet someInfoRs) throws Exception {
        String replace = "";

        try {
            String e = "";
            String controlHidden = "";
            if (aControl.getHide() == 1) {
                controlHidden = ";VISIBILITY: hidden;";
            }

            long width = aControl.getWidth();
            long height = aControl.getHeight();
            long type = aControl.getType();
            String controlName = aControl.getControlName();
            String fieldValue = aFieldValue;
            if (aFieldValue == null) {
                fieldValue = "";
            }

            String style = String.valueOf(String.valueOf((new StringBuffer("<DIV id=\'")).append(this.getExtraName(controlName, "div")).append("\' name=\'").append(this.getExtraName(controlName, "div")).append("\' ").append("style=\"").append(aControl.getFont()).append(";left:").append(String.valueOf(aControl.getLeft())).append(";top:").append(String.valueOf(aControl.getTop())).append(";width:").append(String.valueOf(width)).append(";height:").append(String.valueOf(height)).append(controlHidden).append(";text-align:center").append(";Z-INDEX: 100\">")));
            boolean requireCa;
            if (!fieldValue.equals("")) {
                e = String.valueOf(String.valueOf((new StringBuffer("<img id=\'")).append(controlName).append("\' name=\'").append(controlName).append("\'").append(" border=\'0\' src=\'ImgLoader?type=").append(type).append("&id=").append(fieldValue).append("\'").append(" style=\'width:").append(width).append("px;height:").append(height).append("px;\'>")));
                replace = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(style)))).append(e).append("</DIV>")));
            } else if (aDisabled.trim().length() == 0) {
                String supportCaSign = "";
                requireCa = false;
                if (type != (long) 0) {
                    supportCaSign = "签名";
                    String currentUser = this.baseTool.getCurrentUser();
                    requireCa = this._$13719(currentUser);
                } else {
                    supportCaSign = "盖章";
                    requireCa = true;
                }

                if (requireCa) {
                    e = String.valueOf(String.valueOf((new StringBuffer("<input  class=\'standardBtn\' type=button value=\'")).append(supportCaSign).append("\'").append(" onclick=\"javascript:buttonImage(").append(type).append(",\'").append(controlName).append("\',").append(aControl.isUseCA()).append(");\">")));
                    replace = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(style)))).append(e).append("</DIV>")));
                }
            }

            if (aDisabled.trim().length() == 0) {
                boolean supportCaSign1 = PublicLib.str2Integer(this.baseTool.getTagValue("WF/SUPPORT_CASIGN")) == 1;
                requireCa = supportCaSign1 && aControl.isUseCA();
                someInfoRs.putObject("___sign_ca__require__哈那_____", new Boolean(requireCa));
                if (requireCa) {
                    someInfoRs.putObject("—__sign_ca_form--__Data__Fiel__d___", aControl.getCaDataFields());
                }
            }

            replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<input type=\'hidden\' name=\'")).append(this.getExtraName(controlName, "hidden")).append("\' value=\'").append(fieldValue).append("\'>"))))));
        } catch (Exception var24) {
            this.baseTool.error("restoreStamp", var24);
            throw new Exception(var24.getMessage());
        } finally {
            ;
        }

        return replace;
    }

    private String _$13553(MyHyperLink aControl, String flowInid, String stepInid, boolean isPrint) {
        if (isPrint) {
            return "";
        } else {
            String replace = "";
            String url = aControl.genURL();
            String title = aControl.getTitle();
            String para = "";
            String sqlClause = "";
            String clickFunction = aControl.getClickFunction();
            String errMsg = "";
            para = aControl.genURLPara();
            CachedRowSet tmpRs = null;
            if (aControl.isUseSysLink()) {
                switch (aControl.getSysLink()) {
                    case 100001:
                        url = "syslink/ViewParentForm.jsp";
                        sqlClause = "select para_inid,para_inco,if_sub from flow_inst where flow_inid=?";

                        try {
                            try {
                                SQLPara left = new SQLPara();
                                left.add(flowInid);
                                if (Long.parseLong(flowInid) < minFlowInid) {
                                    sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                                    sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                                }
                                tmpRs = this.baseTool.getRowSet(sqlClause, "", left);
                                boolean isSubflow = false;
                                String top = "-1";
                                String paraStepInco = "-1";
                                if (tmpRs.next()) {
                                    isSubflow = tmpRs.getInt("if_sub") == 1;
                                    if (isSubflow) {
                                        top = tmpRs.getString("para_inid");
                                        paraStepInco = tmpRs.getString("para_inco");
                                    } else {
                                        errMsg = "当前表单不是子流程的表单，只有子流程的表单才有父表单！";
                                    }
                                }

                                para = String.valueOf(String.valueOf((new StringBuffer("flowInid=")).append(top).append("&stepInid=").append(paraStepInco)));
                            } catch (Exception var33) {
                                var33.printStackTrace();
                            }
                            break;
                        } finally {
                            PublicLib.closeCachedRowSet(tmpRs);
                            tmpRs = null;
                        }
                    case 100002:
                        sqlClause = "select count(*) as cnt from deal_info where step_type=5 and flow_inid=?";

                        try {
                            SQLPara ex = new SQLPara();
                            ex.add(flowInid);
                            if (Long.parseLong(flowInid) < minFlowInid) {
                                sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                                        .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                                        .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                            }
                            tmpRs = this.baseTool.getRowSet(sqlClause, "", ex);
                            boolean hasSubflow = false;
                            if (tmpRs.next()) {
                                hasSubflow = tmpRs.getInt("cnt") > 0;
                                if (!hasSubflow) {
                                    errMsg = "当前表单没有启动过子流程，无法查看！";
                                }
                            }

                            para = String.valueOf(String.valueOf((new StringBuffer("flowInid=")).append(flowInid).append("&stepInid=").append(stepInid)));
                        } catch (Exception var31) {
                            var31.printStackTrace();
                        } finally {
                            PublicLib.closeCachedRowSet(tmpRs);
                            tmpRs = null;
                        }

                        url = "syslink/ViewSubflowForm.jsp";
                }
            }

            long left1 = aControl.getLeft();
            long top1 = aControl.getTop();
            switch (aControl.getDisplayStyle()) {
                case 0:
                    String style = String.valueOf(String.valueOf((new StringBuffer("<DIV style=\"")).append(aControl.getFont()).append(";left:").append(String.valueOf(aControl.getLeft())).append(";top:").append(String.valueOf(aControl.getTop())).append(";height:").append(String.valueOf(aControl.getHeight())).append(";width:").append(String.valueOf(aControl.getWidth())).append("Z-INDEX: 100\">")));
                    String btn = String.valueOf(String.valueOf((new StringBuffer("<input class=\'standardBtn\' type=button value=\"")).append(title).append("\"").append(" onclick=\"javascript:").append(clickFunction).append("(\'").append(url).append("\'").append(",\'").append(para).append("\',\'").append(errMsg).append("\');\" ")));
                    btn = String.valueOf(String.valueOf(btn)).concat(" >");
                    replace = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(style)))).append(btn).append("</DIV>")));
                    break;
                case 1:
                    replace = String.valueOf(String.valueOf((new StringBuffer("<span class=\"hypercss\" style=\"cursor:hand;POSITION:absolute;left:")).append(left1).append("px;top:").append(top1).append("px;\"").append(" onclick=\"").append(clickFunction).append("(\'").append(url).append("\',\'").append(para).append("\',\'").append(errMsg).append("\');\">").append(title).append("</span>")));
            }

            return replace;
        }
    }

    public String getFieldDisplay(String aValueList, String aDisplayList, String aFieldValue) {
        String fieldDisplay = "";
        int fieldCount = PublicLib.getFieldCount(aValueList, ";");

        for (int i = 0; i < fieldCount; ++i) {
            String value = PublicLib.getField(aValueList, ";", (long) i);
            String display = PublicLib.getField(aDisplayList, ";", (long) i);
            long lCount = (long) PublicLib.getFieldCount(aFieldValue, ";");

            for (int j = 0; (long) j < lCount; ++j) {
                if (PublicLib.getField(aFieldValue, ";", (long) j).equalsIgnoreCase(value)) {
                    if (!fieldDisplay.equals("")) {
                        fieldDisplay = String.valueOf(String.valueOf(fieldDisplay)).concat(";");
                    }

                    fieldDisplay = String.valueOf(String.valueOf(fieldDisplay)).concat(String.valueOf(String.valueOf(display)));
                    break;
                }
            }
        }

        return fieldDisplay;
    }

    public void restoreBatchSomeInfo(CachedRowSet aFlowRs, Hashtable aReturnStruct) throws Exception {
        CachedRowSet attachRs = null;
        CachedRowSet flowDefRs = null;
        RecordSet someInfoRs = new RecordSet();

        try {
            String e = aFlowRs.getString("flow_id");
            long totalTerm = aFlowRs.getLong("total_term");
            String sql = "select misc_set from flow_def where flow_id=?";
            SQLPara para = new SQLPara();
            para.add(e);
            flowDefRs = this.baseTool.getRowSet(sql, "", para);
            flowDefRs.beforeFirst();
            flowDefRs.next();
            para.clear();
            para = null;
            FlowMiscSet fs = new FlowMiscSet();
            fs.restore(flowDefRs.getString("misc_set"));
            someInfoRs.insertRow();
            someInfoRs.putObject("__flOw_Choice___tiTle__", fs.getChoiceTitle());
            someInfoRs.putObject("__fLow_appEnd_tiT_le__", fs.getAppendTitle());
            someInfoRs.putObject("__flow_notice____Title__", fs.getNoticeTitle());
            someInfoRs.putObject("++F___LO___WDe__f_____MISc________", fs);
            if (fs.isTotalTermEnabled()) {
                someInfoRs.putObject("_Flo_w____tot_alt_erm_enabl_ed___", new Boolean(fs.isTotalTermEnabled()));
                someInfoRs.putObject("_Flo_w____tot_alt_er___m___", new Long(totalTerm));
            }

            String[][] fieldInfo = new String[][]{{"send_titl", "__flow_SeNd____Title__", "VARCHAR"}, {"step_desc", "step_desc", "VARCHAR"}, {"step_name", "step_name", "VARCHAR"}, {"step_type", "step_type", "INTEGER"}, {"step_man", "step_man", "VARCHAR"}, {"time_set", "time_set", "VARCHAR"}, {"if_doc", "doc_flag", "INTEGER"}, {"doc_name", "doc_name", "VARCHAR"}};
            PublicLib.CopyRowSetToRecordSet(aFlowRs, someInfoRs, fieldInfo);
            String choiceList = aFlowRs.getString("step_choi");
            String choiceHtml = "";
            String instName;
            if (choiceList != null && !choiceList.equals("")) {
                int miscStructXml = PublicLib.getFieldCount(choiceList, ";");

                for (int flowInid = 0; flowInid < miscStructXml; ++flowInid) {
                    instName = PublicLib.getField(choiceList, ";", (long) flowInid);
                    choiceHtml = String.valueOf(String.valueOf(choiceHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("<OPTION value=\"")).append(flowInid).append("\""))))));
                    choiceHtml = String.valueOf(String.valueOf(choiceHtml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(">")).append(instName).append("</OPTION>"))))));
                }
            }

            someInfoRs.putObject("choi_list", choiceHtml);
            String var28;
            String var29;
            if (aFlowRs.getLong("if_doc") == (long) 1) {
                var28 = " select * from SYS_FILE2 where 1=2";
                attachRs = this.baseTool.getRowSet(var28, "SYS_FILE2", new SQLPara());
                var29 = aFlowRs.getString("man_stru");
                FileSafeStruct var30 = new FileSafeStruct();
                var30.restore(var29);
                long popedom = (long) var30.getStepRight();
                someInfoRs.putObject("popedom", String.valueOf(popedom));
                String docName = aFlowRs.getString("doc_name");
                if (docName == null || docName.equals("")) {
                    String text = this.baseTool.getTagValue("WF/TEXT");
                    if (text == null || text.equals("")) {
                        System.out.println("WF/TEXT tag not exist");
                    }

                    someInfoRs.putObject("doc_name", text);
                }
            }

            var28 = aFlowRs.getString("misc_set");
            if (var28 == null) {
                var28 = "";
            }

            someInfoRs.putObject("misc_set", var28);
            aFlowRs.beforeFirst();

            while (true) {
                if (!aFlowRs.next()) {
                    aReturnStruct.put("someInfoRs", someInfoRs);
                    if (attachRs != null) {
                        aReturnStruct.put("attachRs", attachRs);
                    }
                    break;
                }

                var29 = aFlowRs.getString("flow_inid");
                instName = aFlowRs.getString("fins_name");
                someInfoRs.putObject(var29, instName);
            }
        } catch (Exception var26) {
            PublicLib.closeCachedRowSet(flowDefRs);
            flowDefRs = null;
            PublicLib.closeCachedRowSet(attachRs);
            attachRs = null;
            this.baseTool.error("restoreBatchSomeInfo", var26);
            throw new Exception(var26.getMessage());
        } finally {
            ;
        }

    }

    public void restoreBatchHtml(CachedRowSet aFlowRs, CachedRowSet aFormRs, Hashtable aReturnStruct) throws Exception {
        StringBuffer formHtml = null;

        try {
            String e = "";
            aFlowRs.first();
            String formCode = aFlowRs.getString("form_code");
            if (formCode == null || formCode.equals("")) {
                e = "步骤设置没指定表单,请检查流程设计。";
                throw new Exception(e);
            }

            String serverName = aFlowRs.getString("svr_name");
            serverName = PublicLib.getField(serverName, "<", 0L);
            String criteria = "form_code=".concat(String.valueOf(String.valueOf(formCode)));
            boolean rsFinded = PublicLib.find(aFormRs, criteria, true);
            if (!rsFinded) {
                throw new Exception("系统找不到指定的表单");
            }

            String rawHtml = PublicLib.getStringFromClob(aFormRs, "form_cont");
            rawHtml = PublicLib.findReplace(rawHtml, "&lt;", "<");
            rawHtml = PublicLib.findReplace(rawHtml, "&gt;", ">");
            rawHtml = PublicLib.findReplace(rawHtml, "&amp;", "&");
            String formStructXml = PublicLib.getStringFromClob(aFormRs, "form_stru");
            formStructXml = PublicLib.findReplace(formStructXml, "&lt;", "@lt;");
            formStructXml = PublicLib.findReplace(formStructXml, "&gt;", "@gt;");
            String fsetId = aFormRs.getString("fset_id");
            FormStruct formStruct = new FormStruct();
            formStruct.restore(formStructXml);
            String tableName = formStruct.getSysInfo().getBindTable();
            tableName = PublicLib.getField(tableName, "<", 0L);
            String validStructXml = PublicLib.getStringFromClob(aFlowRs, "vali_set");
            ValidStruct validStruct = new ValidStruct();
            validStruct.restore(validStructXml);
            String stepName = aFlowRs.getString("step_name");
            String stepTag = aFlowRs.getString("step_tag");
            if (stepTag == null) {
                stepTag = "";
            }

            if (stepTag.trim().length() == 0) {
                stepTag = stepName;
            }

            String currentUser = this.baseTool.getCurrentUser();
            String belongIn = this.baseTool.buildInDeptString(currentUser);
            if (!belongIn.equals("")) {
                belongIn = String.valueOf(String.valueOf(belongIn)).concat(",");
            }

            belongIn = PublicLib.findReplace(belongIn, "\',", ":3\',");
            belongIn = String.valueOf(String.valueOf(belongIn)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(currentUser).append("\'"))))));
            aFlowRs.beforeFirst();
            Hashtable mainRsColl = new Hashtable();

            while (aFlowRs.next()) {
                CachedRowSet formInstData = null;
                CachedRowSet mainRs = null;

                try {
                    String ex = aFlowRs.getString("flow_inid");
                    long stepInid = aFlowRs.getLong("step_inco");
                    String mtblStructXml = aFlowRs.getString("mtbl_stru");
                    MtblStruct mtblStruct = new MtblStruct();
                    mtblStruct.restore(mtblStructXml);
                    String sqlClause = String.valueOf(String.valueOf((new StringBuffer("select * from ")).append(tableName).append(" where ")));
                    Hashtable keyColl = mtblStruct.getKeyColl();
                    SQLPara sqlPara = new SQLPara();

                    for (int tmpBelongIn = 0; tmpBelongIn < keyColl.size(); ++tmpBelongIn) {
                        if (tmpBelongIn > 0) {
                            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and ");
                        }

                        MtblItem stationList = (MtblItem) keyColl.get(String.valueOf(tmpBelongIn));
                        sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(stationList.getFieldName())).concat("=?"))));
                        if (stationList.getFieldType() >= (long) 1 && stationList.getFieldType() <= (long) 8) {
                            sqlPara.add(stationList.getFieldValue(), 1);
                        } else {
                            sqlPara.add(stationList.getFieldValue());
                        }
                    }

                    mainRs = this.baseTool.getServerRowSet(serverName, sqlClause, tableName.toUpperCase(), sqlPara);
                    mainRs.first();
                    String var52 = PublicLib.findReplace(belongIn, "\'", "");
                    String var53 = "";
                    sqlPara = new SQLPara();

                    for (int k = 0; k < PublicLib.getFieldCount(var52, ","); ++k) {
                        if (!var53.equals("")) {
                            var53 = String.valueOf(String.valueOf(var53)).concat(",");
                        }

                        var53 = String.valueOf(String.valueOf(var53)).concat("?");
                        sqlPara.add(PublicLib.getField(var52, ",", (long) k));
                    }

                    sqlPara.add(String.valueOf(ex));
                    sqlPara.add(String.valueOf(stepInid), 1);
                    sqlClause = String.valueOf(String.valueOf((new StringBuffer("select flow_inid,step_inco,deal_indx,fini_flag,read_flag,deal_comm,curr_man,step_choi,time_set,note_man from deal_info where deal_man in (")).append(var53).append(")").append(" and flow_inid=? and step_inco=?")));
                    if (Long.parseLong(ex) < minFlowInid) {
                        sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                                .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                                .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                    }
                    formInstData = this.baseTool.getRowSet(sqlClause, "DEAL_INFO", sqlPara);
                    this.baseTool.getFlowLib().stepEventSet((Connection) null, "ente_set", mainRs, formInstData, aFlowRs, "", "");
                    mainRsColl.put(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(ex)))).append("_").append(stepInid))), mainRs);
                } catch (Exception var47) {
                    PublicLib.closeCachedRowSet(mainRs);
                    mainRs = null;
                } finally {
                    PublicLib.closeCachedRowSet(formInstData);
                    formInstData = null;
                }
            }

            FormInstData var51 = new FormInstData();
            var51.setFormStruct(formStruct);
            formHtml = this.restoreBatchControlHtml(fsetId, stepTag, rawHtml, serverName, var51, validStruct, aReturnStruct);
            formHtml = this.restoreBatchSubForm(formHtml, formStruct);
            aReturnStruct.put("___0-3__48_f_uct_FormStruct___", formStruct);
            aReturnStruct.put("0_9UAad_form_iNst_Data__", var51);
            aReturnStruct.put("formHtml", formHtml.substring(0));
            aReturnStruct.put("mainRsColl", mainRsColl);
        } catch (Exception var49) {
            this.baseTool.error("restoreBatchHtml", var49);
            throw new Exception(var49.getMessage());
        } finally {
            ;
        }

    }

    public StringBuffer restoreBatchControlHtml(String aFormSetId, String aStepTag, String aRawHtml, String aServerName, FormInstData aFormInstData, ValidStruct aValidStruct, Hashtable aReturnStruct) throws Exception {
        StringBuffer formHtml = null;
        RecordSet fieldRs = null;
        RecordSet fieldMapNameRs = null;

        try {
            String e = "";
            formHtml = new StringBuffer('썐');
            formHtml.append(aRawHtml);
            fieldMapNameRs = new RecordSet();
            RecordSet someInfoRs = (RecordSet) aReturnStruct.get("someInfoRs");
            String tableName = aFormInstData.getFormStruct().getSysInfo().getBindTable();
            tableName = PublicLib.getField(tableName, "<", 0L);
            fieldRs = this.baseTool.fieldList(aServerName, tableName);
            Hashtable controlColl = aFormInstData.getFormStruct().getControlColl();
            Enumeration keys = controlColl.keys();
            int dbType = this.baseTool.getDBServerType();

            while (keys.hasMoreElements()) {
                String logicName = (String) keys.nextElement();
                MyControl control = (MyControl) controlColl.get(logicName);
                String fieldName = PublicLib.getField(control.getFieldName(), "<", 0L);
                String fieldDesc = "";
                boolean allowNull = false;
                String disabled = "";
                String fieldValue = "";
                boolean isFormula = false;
                boolean forceUpdate = false;
                long fieldType = control.getFieldType();
                long controlType = control.getControlType();
                String valid;
                String tag;
                if (fieldName != null && !fieldName.equals("")) {
                    valid = "fld_name=".concat(String.valueOf(String.valueOf(fieldName)));
                    boolean replace = fieldRs.find(valid);
                    if (replace) {
                        fieldDesc = (String) fieldRs.getObject("fld_desc");
                    }

                    tag = "";
                    if (controlType != (long) 10 && controlType != (long) 13) {
                        tag = control.getControlName();
                    } else {
                        tag = String.valueOf(String.valueOf(control.getControlName())).concat("_hidden");
                    }

                    if (!aValidStruct.isEnabled(fieldName)) {
                        disabled = " disabled ";
                    }

                    aValidStruct.isAllowNull(fieldName);
                    isFormula = aValidStruct.isFormula(fieldName);
                    forceUpdate = aValidStruct.isForceUpdate(fieldName);
                    fieldMapNameRs.insertRow();
                    fieldMapNameRs.putObject("ctl_name", tag);
                    fieldMapNameRs.putObject("fld_name", fieldName);
                    fieldMapNameRs.putObject("fld_type", String.valueOf(fieldType));
                    fieldMapNameRs.putObject("ctl_type", String.valueOf(controlType));
                    fieldMapNameRs.putObject("db_type", String.valueOf(dbType));
                    if (controlType == (long) 3) {
                        fieldMapNameRs.putObject("fld_flag", "0");
                    } else if (!disabled.equals("") && !isFormula) {
                        fieldMapNameRs.putObject("fld_flag", "0");
                    } else {
                        fieldMapNameRs.putObject("fld_flag", "1");
                        if (!e.equals("")) {
                            e = String.valueOf(String.valueOf(e)).concat("#@#");
                        }

                        e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(fieldName)))).append(":").append(controlType).append(":").append(control.getControlName()))))));
                    }
                }

                valid = "";
                String replace1 = "";
                tag = "";
                boolean isDate = fieldType == (long) 4 || fieldType == (long) 5 || fieldType == (long) 6;
                long formCode;
                switch ((int) controlType) {
                    case 0:
                    case 8:
                    case 14:
                    default:
                        break;
                    case 1:
                        if (disabled.equals("")) {
                            valid = aValidStruct.getValidFromControlEx(fieldName, fieldDesc, false);
                        } else {
                            disabled = " readonly class=\'form_disable_text\' ";
                        }

                        Map hTextReplace = this._$13517(false, isFormula, forceUpdate, (MyText) control, disabled, fieldValue, valid, aServerName, aFormInstData, false);
                        replace1 = (String) hTextReplace.get("replace");
                        String hiddenReplace = (String) hTextReplace.get("hiddenReplace");
                        long iDateType = ((MyText) control).getDateType();
                        if (isDate && iDateType != (long) -1) {
                            tag = String.valueOf(String.valueOf((new StringBuffer("F_l_D__WF__C_t_L=\"")).append(iDateType).append(";").append(control.getFieldName()).append(";").append(tableName).append("\"")));
                        }

                        tag = String.valueOf(String.valueOf((new StringBuffer("F_l_D__WF__C_t_L=\"")).append(control.getFieldName()).append(";").append(tableName).append("\"")));
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, false);
                        if (hiddenReplace.trim().length() > 0) {
                            tag = String.valueOf(String.valueOf((new StringBuffer("D_s_B__WF__C_t_L=\"")).append(control.getLogicName()).append("\"")));
                            formHtml = PublicLib.findReplace(formHtml, tag, hiddenReplace, false);
                        }
                        break;
                    case 2:
                        tag = String.valueOf(String.valueOf((new StringBuffer("D_s_B__WF__C_t_L=\"")).append(control.getLogicName()).append("\"")));
                        if (disabled.equals("") && fieldType != (long) 4 && fieldType != (long) 5 && fieldType != (long) 6) {
                            replace1 = aValidStruct.getValidFromControlEx(fieldName, fieldDesc, false);
                        } else {
                            replace1 = " readonly class=\'form_disable_text\' ";
                        }

                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        tag = String.valueOf(String.valueOf((new StringBuffer(">F_l_D__WF__C_t_L=\"")).append(control.getFieldName()).append(";").append(tableName).append("\" ")));
                        Hashtable hTextAreaReplace = this._$13524(false, isFormula, forceUpdate, (MyTextArea) control, disabled, fieldValue, false, aFormInstData);
                        replace1 = (String) hTextAreaReplace.get("replace");
                        String hiddenAreaReplace = (String) hTextAreaReplace.get("hiddenReplace");
                        if (hiddenAreaReplace.trim().length() > 0) {
                            tag = String.valueOf(String.valueOf(tag)).concat("</TEXTAREA>");
                            replace1 = String.valueOf(String.valueOf(replace1)).concat(String.valueOf(String.valueOf("</TEXTAREA>".concat(String.valueOf(String.valueOf(hiddenAreaReplace))))));
                        }

                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, false);
                        break;
                    case 3:
                        tag = String.valueOf(String.valueOf((new StringBuffer("B_t_N__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        if (!aValidStruct.isEnabled(control.getLogicName())) {
                            disabled = "disabled";
                        }

                        formCode = aFormInstData.getFormStruct().getSysInfo().getFormCode();
                        replace1 = String.valueOf(String.valueOf(this._$13527(false, (MyButton) control, aFormSetId, formCode, aStepTag, controlColl, disabled))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 4:
                        tag = String.valueOf(String.valueOf((new StringBuffer("C_h_K__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        if (disabled.equals("")) {
                            valid = aValidStruct.getValidFromControlEx(fieldName, fieldDesc, false);
                        }

                        replace1 = String.valueOf(String.valueOf(this._$13529(false, (MyCheckBox) control, disabled, fieldValue, fieldType, valid, false, aServerName))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 5:
                        tag = String.valueOf(String.valueOf((new StringBuffer("R_d_O__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        if (disabled.equals("")) {
                            valid = aValidStruct.getValidFromControlEx(fieldName, fieldDesc, false);
                        }

                        replace1 = String.valueOf(String.valueOf(this._$13529(false, (MyRadio) control, disabled, fieldValue, fieldType, valid, true, aServerName))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 6:
                    case 7:
                        tag = String.valueOf(String.valueOf((new StringBuffer("D_s_B__WF__C_t_L=\"")).append(control.getLogicName()).append("\"")));
                        formHtml = PublicLib.findReplace(formHtml, tag, disabled, true);
                        tag = "C_b_O__WF__C_t_L=".concat(String.valueOf(String.valueOf(control.getControlName())));
                        replace1 = this._$13533(false, (MyCombo) control, disabled, fieldValue, fieldType, aServerName, true);
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 9:
                        if (!aStepTag.equalsIgnoreCase(((MySign) control).getTag())) {
                            disabled = " disabled ";
                        }

                        tag = String.valueOf(String.valueOf((new StringBuffer("S_i_g_N__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        replace1 = String.valueOf(String.valueOf(this._$13537(false, (MySign) control, disabled, "-1", "-1", -1L, someInfoRs, true))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 10:
                        tag = String.valueOf(String.valueOf((new StringBuffer("M_a_P__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        formCode = aFormInstData.getFormStruct().getSysInfo().getFormCode();
                        StringBuffer realValue = new StringBuffer();
                        replace1 = String.valueOf(String.valueOf(this._$13542((FlowInstInfo) null, aServerName, false, isFormula, forceUpdate, (MyMap) control, disabled, fieldValue, aFormSetId, formCode, realValue, false))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        tag = String.valueOf(String.valueOf((new StringBuffer("F_l_D__WF__C_t_L=\"")).append(control.getFieldName()).append(";").append(tableName).append("\"")));
                        replace1 = this.restoreMapDisplay((MyMap) control, aServerName, realValue);
                        if (disabled.equals("")) {
                            valid = aValidStruct.getValidFromControlEx(fieldName, fieldDesc, true);
                            replace1 = String.valueOf(String.valueOf(replace1)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer(" ")).append(valid).append(" "))))));
                        } else {
                            replace1 = String.valueOf(String.valueOf(replace1)).concat(" class=\'form_disable_text\' ");
                        }

                        realValue = null;
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, false);
                        break;
                    case 11:
                        if (!aStepTag.equalsIgnoreCase(((MyPen) control).getTag())) {
                            disabled = " disabled ";
                        }

                        tag = String.valueOf(String.valueOf((new StringBuffer("P_e_N__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        replace1 = String.valueOf(String.valueOf(this._$13539(false, (MyPen) control, disabled, "-1", "-1"))).concat(" ");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 12:
                        tag = String.valueOf(String.valueOf((new StringBuffer("A_t_T__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        replace1 = " ";
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 13:
                        tag = String.valueOf(String.valueOf((new StringBuffer("S_m_p__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        replace1 = this._$13549((MyStamp) control, disabled, fieldValue, someInfoRs);
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 15:
                        tag = String.valueOf(String.valueOf((new StringBuffer("F_o_r_M__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        replace1 = " ";
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                        break;
                    case 16:
                        if (!aValidStruct.isEnabled(control.getLogicName())) {
                            disabled = " disabled ";
                        }

                        tag = String.valueOf(String.valueOf((new StringBuffer("L_n_K__WF__C_t_L=")).append(control.getControlName()).append(" ")));
                        replace1 = " ";
                        formHtml = PublicLib.findReplace(formHtml, tag, replace1, true);
                }
            }

            aReturnStruct.put("fieldMapNameRs", fieldMapNameRs);
            someInfoRs.putObject("modifyDocStructXml", e);
            return formHtml;
        } catch (Exception var47) {
            formHtml = null;
            this.baseTool.error("restoreBatchControlHtml", var47);
            throw new Exception(var47.getMessage());
        } finally {
            ;
        }
    }

    public StringBuffer restoreBatchSubForm(StringBuffer aFormHtml, FormStruct aFormStruct) throws Exception {
        try {
            String tag;
            String replace;
            try {
                for (Enumeration e = aFormStruct.getSubFormColl().elements(); e.hasMoreElements(); aFormHtml = PublicLib.findReplace(aFormHtml, tag, replace, true)) {
                    MySubForm subForm = (MySubForm) e.nextElement();
                    tag = "S_u_b__WF__C_t_L=".concat(String.valueOf(String.valueOf(subForm.getSubFormCode())));
                    replace = "<div></div>";
                }
            } catch (Exception var11) {
                this.baseTool.error("restoreBatchSubForm", var11);
                throw new Exception(var11.getMessage());
            }
        } finally {
            ;
        }

        return aFormHtml;
    }

    public long getMaxSize(String aDisplayList) {
        long maxSize = 0L;
        long fieldCount = (long) PublicLib.getFieldCount(aDisplayList, ";");

        for (long i = 0L; i < fieldCount; ++i) {
            String display = PublicLib.getField(aDisplayList, ";", i);
            if ((long) display.getBytes().length > maxSize) {
                maxSize = (long) display.getBytes().length;
            }
        }

        return maxSize;
    }

    public void updateOpenDateAndRegisterStatus(StepMiscStruct miscSet, CachedRowSet aDealInfoRs, Hashtable returnStruct) throws Exception {
        Connection conn = null;

        try {
            if (aDealInfoRs.size() > 0 && aDealInfoRs.getTimestamp("open_date") == null) {
                Timestamp e = new Timestamp((new Date()).getTime());
                aDealInfoRs.updateTimestamp("open_date", e);
                returnStruct.put("__IS_forM__First_OPen___", e);
                if (miscSet != null && miscSet.isRegisterOnOpen()) {
                    aDealInfoRs.updateLong("regi_flag", 1L);
                }

                aDealInfoRs.updateRow();
                conn = this.baseTool.getConnection();
                aDealInfoRs.acceptChanges(conn);
            }
        } catch (Exception var10) {
            throw new Exception(var10.getMessage());
        } finally {
            PublicLib.closeConnection(conn);
            conn = null;
        }

    }

    public String[] getPrgFormList(FormStruct aFormStruct) {
        String[] nameList = null;
        Hashtable controlColl = aFormStruct.getControlColl();
        Enumeration keys = controlColl.keys();
        Vector list = new Vector();

        while (keys.hasMoreElements()) {
            String i = (String) keys.nextElement();
            MyControl control = (MyControl) controlColl.get(i);
            if ((long) 15 == control.getControlType()) {
                list.add(control.getControlName());
            }
        }

        nameList = new String[list.size()];

        for (int var8 = 0; var8 < list.size(); ++var8) {
            nameList[var8] = (String) list.get(var8);
        }

        return nameList;
    }

    public IBaseTool getBaseTool() {
        return this.baseTool;
    }

    public void setBaseTool(IBaseTool baseTool) {
        this.baseTool = baseTool;
    }

    public Hashtable getStartStepHtml(long aFlowId, Map initData) throws Exception {
        return this.getStartStepHtml(aFlowId, 0, initData);
    }

    public Hashtable getStartStepHtml(long aFlowId, CachedRowSet aMainRs, int startMode) throws Exception {
        CachedRowSet flowRs = null;
        CachedRowSet formRs = null;
        Hashtable returnStruct = new Hashtable();

        try {
            String e = " select A.step_tag,A.step_name,A.step_desc,A.step_code,A.step_man,A.flow_id,A.vali_set,A.form_id as form_code,A.step_choi,A.step_type,A.send_titl,A.man_stru,A.ente_set,A.Misc_Set,A.time_set, B.if_doc,B.doc_name,B.if_mark,B.mark_path,B.mark_step, C.fset_id,C.svr_name,C.key_fld from step_def A,flow_def B,formset C  where A.flow_id=B.flow_id and A.step_code=B.star_step and B.fset_id=C.fset_id and A.flow_id=?";
            SQLPara infomation1 = new SQLPara();
            infomation1.add(String.valueOf(aFlowId));
            flowRs = this.baseTool.getRowSet(e, "", infomation1);
            flowRs.first();
            String fsetId = flowRs.getString("fset_id");
            int stepType = flowRs.getInt("step_type");
            if (fsetId == null || fsetId.equals("")) {
                throw new Exception("流程没有设置表单集。");
            }

            e = " select fset_id,form_code,form_cont,form_stru from form where fset_id=?";
            infomation1 = new SQLPara();
            infomation1.add(String.valueOf(fsetId));
            formRs = this.baseTool.getRowSet(e, "", infomation1);
            FlowInstInfo fii = new FlowInstInfo(-1L, -1L, -1L, true);
            fii.setStepType(stepType);
            fii.setTotalTerm(-1L);
            this.restoreSomeInfo(fii, false, (CachedRowSet) null, flowRs, returnStruct, startMode);
            this.restoreHtml(true, false, flowRs, formRs, (CachedRowSet) null, returnStruct, aMainRs);
        } catch (Exception var17) {
            String infomation = String.valueOf(String.valueOf((new StringBuffer("流程id=")).append(aFlowId).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
            this.baseTool.error("getStartStepHtml:".concat(String.valueOf(String.valueOf(infomation))), var17);
            this.unloadData(returnStruct);
            returnStruct = null;
            throw new Exception(var17.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(flowRs);
            flowRs = null;
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
        }

        return returnStruct;
    }

    public Hashtable getStartStepHtml(long aFlowId, int startMode, Map initData) throws Exception {
        CachedRowSet flowRs = null;
        CachedRowSet formRs = null;
        Hashtable returnStruct = new Hashtable();

        try {
            String e = " select A.step_tag,A.step_name,A.step_desc,A.step_code,A.step_man,A.flow_id,A.vali_set,A.form_id as form_code,A.step_choi,A.step_type,A.send_titl,A.man_stru,A.ente_set,A.Misc_Set,A.time_set,A.ext_attr, B.if_doc,B.doc_name,B.if_mark,B.mark_path,B.mark_step, C.fset_id,C.svr_name,C.key_fld from step_def A,flow_def B,formset C  where A.flow_id=B.flow_id and A.step_code=B.star_step and B.fset_id=C.fset_id and A.flow_id=?";
            SQLPara infomation1 = new SQLPara();
            infomation1.add(String.valueOf(aFlowId));
            flowRs = this.baseTool.getRowSet(e, "", infomation1);
            flowRs.first();
            String fsetId = flowRs.getString("fset_id");
            int stepType = flowRs.getInt("step_type");
            String creator = this.baseTool.getCurrentUser();
            String sender = "";
            String stepExtAttributes = PublicLib.getStringFromClob(flowRs, "ext_attr");
            if (fsetId == null || fsetId.equals("")) {
                throw new Exception("流程没有设置表单集。");
            }

            e = " select fset_id,form_code,form_cont,form_stru from form where fset_id=?";
            infomation1 = new SQLPara();
            infomation1.add(String.valueOf(fsetId));
            formRs = this.baseTool.getRowSet(e, "", infomation1);
            FlowInstInfo fii = new FlowInstInfo(-1L, -1L, -1L, true);
            fii.setStepType(stepType);
            fii.setTotalTerm(-1L);
            String miscStructXml = flowRs.getString("misc_set");
            StepMiscStruct miscSet = new StepMiscStruct();
            miscSet.restore(miscStructXml);
            fii.setStepMiscStruct(miscSet);
            this.restoreSomeInfo(fii, false, (CachedRowSet) null, flowRs, returnStruct, startMode);
            this.restoreHtml(fii, false, flowRs, formRs, (CachedRowSet) null, returnStruct, initData);
            RecordSet someInfoRs = (RecordSet) returnStruct.get("someInfoRs");
            CachedRowSet mainRs = (CachedRowSet) returnStruct.get("mainRs");
            this.parseExtAttribute(creator, sender, stepExtAttributes, someInfoRs, mainRs);
        } catch (Exception var24) {
            String infomation = String.valueOf(String.valueOf((new StringBuffer("流程id=")).append(aFlowId).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
            this.baseTool.error("getStartStepHtml:".concat(String.valueOf(String.valueOf(infomation))), var24);
            this.unloadData(returnStruct);
            returnStruct = null;
            throw new Exception(var24.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(flowRs);
            flowRs = null;
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
        }

        return returnStruct;
    }

    public Hashtable getStepInsHtml(long aFlowInid, long aStepInid, long aDealIndx, int startMode, Map initData) throws Exception {
        CachedRowSet flowRs = null;
        CachedRowSet formRs = null;
        CachedRowSet dealInfoRs = null;
        CachedRowSet crs = null;
        Hashtable returnStruct = new Hashtable();

        try {
            String infomation;
            try {
                String e;
                String fsetId;
                String creator;
                String sender;
                long totalTerm;
                String stepExtAttributes;
                long stepType;
                long stepState;
                int someInfoRs;
                SQLPara var42;
                label252:
                {
                    e = " select B.peri_inco,C.step_tag,C.step_name,B.step_code,B.step_inco,B.step_stat,C.form_id as form_code,B.step_choi,B.step_type,C.send_titl,C.time_set,B.step_man,B.peri_man, A.mark_step,A.if_mark,A.mark_path,A.flow_inid,A.flow_id,A.inst_crea,A.fset_id,A.svr_name, A.mtbl_stru,A.if_doc,A.doc_name,A.clear_status,A.if_efec,A.total_term, C.vali_set,C.man_stru,C.step_desc,C.ente_set,C.Misc_Set,C.ext_attr from flow_inst A,step_inst B,step_def C  where A.flow_inid=B.flow_inid and B.flow_id=C.flow_id and B.step_code=C.step_code and A.flow_inid=? and B.step_inco=?";
                    var42 = new SQLPara();
                    var42.add(String.valueOf(aFlowInid));
                    var42.add(String.valueOf(aStepInid), 1);
                    if (aFlowInid < minFlowInid) {
                        e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                        e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                        System.out.println(e);
                        System.out.println(var42);
                    }
                    flowRs = this.baseTool.getRowSet(e, "", var42);
                    flowRs.first();
                    fsetId = flowRs.getString("fset_id");
                    String formCode = flowRs.getString("form_code");
                    creator = flowRs.getString("inst_crea");
                    sender = flowRs.getString("peri_man");
                    totalTerm = flowRs.getLong("total_term");
                    stepExtAttributes = PublicLib.getStringFromClob(flowRs, "ext_attr");
                    stepType = flowRs.getLong("step_type");
                    stepState = flowRs.getLong("step_stat");
                    if (fsetId != null && !fsetId.equals("")) {
                        long formReadOnly;
                        if (flowRs.getInt("step_type") != 5) {
                            if (formCode == null || formCode.trim().length() == 0) {
                                e = "select form_code from step_inst where step_type=1 and flow_inid=?";
                                var42.clear();
                                var42 = new SQLPara();
                                var42.add(String.valueOf(aFlowInid));
                                if (aFlowInid < minFlowInid) {
                                    e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                                    e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                                }
                                crs = this.baseTool.getRowSet(e, "", var42);
                                crs.next();
                                formReadOnly = crs.getLong("form_code");
                                flowRs.updateString("form_code", String.valueOf(String.valueOf(formReadOnly)).concat(""));
                                flowRs.updateRow();
                            }
                            break label252;
                        }

                        formReadOnly = flowRs.getLong("peri_inco");
                        e = "select step_inco,step_type,form_code,peri_inco from step_inst where flow_inid=?";
                        var42.clear();
                        var42 = new SQLPara();
                        var42.add(String.valueOf(aFlowInid));
                        if (aFlowInid < minFlowInid) {
                            e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                            e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                            System.out.println(e);
                            System.out.println(var42);
                        }
                        crs = this.baseTool.getRowSet(e, "", var42);
                        boolean currentUser = true;
                        long fii = 0L;
                        boolean miscSet = PublicLib.find(crs, "step_inco=".concat(String.valueOf(String.valueOf(formReadOnly))), true);
                        someInfoRs = crs.size();
                        int mainRs = 0;

                        while (true) {
                            if (miscSet && mainRs < someInfoRs) {
                                int var44 = crs.getInt("step_type");
                                if (var44 != 2 && var44 != 3 && var44 != 1) {
                                    formReadOnly = crs.getLong("peri_inco");
                                    miscSet = PublicLib.find(crs, "step_inco=".concat(String.valueOf(String.valueOf(formReadOnly))), true);
                                    ++mainRs;
                                    continue;
                                }

                                fii = crs.getLong("form_code");
                            }

                            flowRs.updateLong("form_code", fii);
                            flowRs.updateRow();
                            break label252;
                        }
                    }

                    throw new Exception("该流程没有设置表单集");
                }

                var42.clear();
                infomation = null;
                e = "";
                e = " select fset_id,form_code,form_cont,form_stru from form where fset_id=?";
                var42 = new SQLPara();
                var42.add(String.valueOf(fsetId));
                if (aFlowInid < minFlowInid) {
                    e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                    e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                }
                formRs = this.baseTool.getRowSet(e, "", var42);
                boolean var43 = false;
                boolean flowReadOnly = false;
                if (stepState == (long) 2) {
                    flowReadOnly = true;
                }

                String var45 = this.baseTool.getCurrentUser();
                String miscStructXml;
                if (aDealIndx == (long) -1) {
                    String var46 = this.baseTool.getTagValue("WF/NOSTATION");
                    miscStructXml = "";
                    var42 = new SQLPara();
                    var42.add(String.valueOf(aFlowInid));
                    var42.add(String.valueOf(aStepInid), 1);
                    if ((var46.equals("") || var46.equals("0")) && !flowReadOnly && stepType != (long) 3) {
                        miscStructXml = this.baseTool.buildInDeptString(var45);
                        if (!miscStructXml.equals("")) {
                            miscStructXml = String.valueOf(String.valueOf(miscStructXml)).concat(",");
                            miscStructXml = PublicLib.findReplace(miscStructXml, "\',", ":3\',");
                        }

                        miscStructXml = String.valueOf(String.valueOf(miscStructXml)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("\'")).append(var45).append("\'"))))));
                        miscStructXml = PublicLib.findReplace(miscStructXml, "\'", "");
                        String var49 = "";

                        for (someInfoRs = 0; someInfoRs < PublicLib.getFieldCount(miscStructXml, ","); ++someInfoRs) {
                            if (!var49.equals("")) {
                                var49 = String.valueOf(String.valueOf(var49)).concat(",");
                            }

                            var49 = String.valueOf(String.valueOf(var49)).concat("?");
                            var42.add(PublicLib.getField(miscStructXml, ",", (long) someInfoRs));
                        }

                        miscStructXml = String.valueOf(String.valueOf((new StringBuffer(" in (")).append(var49).append(")")));
                    } else {
                        miscStructXml = "=?";
                        var42.add(var45);
                    }

                    e = "select flow_inid,step_inco,deal_indx,fini_flag,read_flag,regi_flag,deal_comm,curr_man,step_choi,time_set,note_man,open_date,deal_man from deal_info where flow_inid=? and step_inco=? and deal_man ".concat(String.valueOf(String.valueOf(miscStructXml)));
                    if (aFlowInid < minFlowInid) {
                        e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                                .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                                .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                        System.out.println(e);
                        System.out.println(var42);
                    }
                    dealInfoRs = this.baseTool.getRowSet(e, "DEAL_INFO", var42);
                    if (dealInfoRs.next() && dealInfoRs.getInt("fini_flag") == 0) {
                        aDealIndx = (long) dealInfoRs.getInt("deal_indx");
                    }
                }

                if (aDealIndx != (long) -1) {
                    if (dealInfoRs != null) {
                        dealInfoRs.close();
                    }

                    dealInfoRs = null;
                    e = "select flow_inid,step_inco,deal_indx,fini_flag,read_flag,regi_flag,deal_comm,curr_man,step_choi,time_set,note_man,open_date,deal_man from deal_info where flow_inid=? and step_inco=? and deal_indx=?";
                    var42 = new SQLPara();
                    var42.add(String.valueOf(aFlowInid));
                    var42.add(String.valueOf(aStepInid), 1);
                    var42.add(String.valueOf(aDealIndx), 1);
                    if (aFlowInid < minFlowInid) {
                        e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
                                .replaceAll("\\bstep_inst\\b", "step_inst_bak")
                                .replaceAll("\\bdeal_info\\b", "deal_info_bak");
                        System.out.println(e);
                        System.out.println(var42);
                    }
                    dealInfoRs = this.baseTool.getRowSet(e, "DEAL_INFO", var42);
                } else {
                    dealInfoRs.beforeFirst();
                }

                if (dealInfoRs.next()) {
                    List var47 = this.baseTool.getFlowLib().getAllograph();
                    long var52 = (long) dealInfoRs.getInt("fini_flag");
                    long var50 = (long) dealInfoRs.getInt("read_flag");
                    aDealIndx = (long) dealInfoRs.getInt("deal_indx");
                    String dealMan = dealInfoRs.getString("deal_man");
                    flowReadOnly = false;
                    if (stepType == (long) 5 || var52 == (long) 1 || !var45.equals(dealMan) && !var47.contains(dealMan) || stepType == (long) 3 && var50 == (long) 1) {
                        flowReadOnly = true;
                    }
                } else {
                    flowReadOnly = true;
                }

                var43 = stepType == (long) 5 || flowReadOnly || stepType == (long) 3;
                FlowInstInfo var48 = new FlowInstInfo(aFlowInid, aStepInid, aDealIndx, false);
                var48.setStepType((int) stepType);
                var48.setTotalTerm(totalTerm);
                miscStructXml = flowRs.getString("misc_set");
                StepMiscStruct var51 = new StepMiscStruct();
                var51.restore(miscStructXml);
                var48.setStepMiscStruct(var51);
                this.restoreSomeInfo(var48, flowReadOnly, dealInfoRs, flowRs, returnStruct, startMode);
                this.restoreHtml(var48, var43, flowRs, formRs, dealInfoRs, returnStruct, initData);
                this.updateOpenDateAndRegisterStatus(var48.getStepMiscStruct(), dealInfoRs, returnStruct);
                RecordSet var53 = (RecordSet) returnStruct.get("someInfoRs");
                CachedRowSet var54 = (CachedRowSet) returnStruct.get("mainRs");
                this.parseExtAttribute(creator, sender, stepExtAttributes, var53, var54);
            } catch (Exception var40) {
                infomation = String.valueOf(String.valueOf((new StringBuffer("流程实例id=")).append(aFlowInid).append(",步骤实例id=").append(aStepInid).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
                this.baseTool.error("getStepInsHtml:".concat(String.valueOf(String.valueOf(infomation))), var40);
                var40.printStackTrace();
                this.unloadData(returnStruct);
                returnStruct = null;
                throw new Exception(var40.getMessage());
            }
        } finally {
            PublicLib.closeCachedRowSet(dealInfoRs);
            dealInfoRs = null;
            PublicLib.closeCachedRowSet(flowRs);
            flowRs = null;
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
            PublicLib.closeCachedRowSet(crs);
            crs = null;
        }

        return returnStruct;
    }

    protected void parseExtAttribute(String aCreator, String aSender, String stepExtAttributes, RecordSet someInfoRs, CachedRowSet mainRs) {
        NADSelectRangeParser selRangeParser = new NADSelectRangeParser();
        selRangeParser.restore(stepExtAttributes);
        NADSelectRange noticeSelectRange = selRangeParser.getNoticeSelectRange();
        NADSelectRange appendSelectRange = selRangeParser.getAppendSelectRange();
        NADSelectRange deliverToSelectRange = selRangeParser.getDeliverToSelectRange();
        String deliverToRangeDef = deliverToSelectRange.getRangeValue();
        String sTemp = null;
        long selFlag = 4L;
        String framList = "";
        String groupList = "";
        String function;
        if (noticeSelectRange.isFromFramework()) {
            if (noticeSelectRange.getFrameFlag() == (long) 3) {
                framList = noticeSelectRange.getFrameList();
            }

            if (noticeSelectRange.getFrameFlag() == (long) 4) {
                groupList = noticeSelectRange.getGroupList();
            }

            function = String.valueOf(String.valueOf((new StringBuffer("javascript:openFrameList(\'sys_step_notice_show\',\'sys_step_notice_hidden\',")).append(selFlag).append(",").append(noticeSelectRange.getFrameFlag()).append(",").append(noticeSelectRange.getFrameLevel()).append(",").append(noticeSelectRange.getFrameLevelTo()).append(",\'").append(framList).append("\',").append(noticeSelectRange.isDisplayGroupNode() ? "1" : "0").append(",").append(noticeSelectRange.isDisplayPersGroupNode() ? "1" : "0").append(",").append(noticeSelectRange.isDisplayAllUserNode() ? "1" : "0").append(",\'").append(noticeSelectRange.getGroupNodeDesc()).append("\',\'").append(noticeSelectRange.getPersGroupNodeDesc()).append("\',\'").append(groupList).append("\',\'\',1)")));
            someInfoRs.putObject("__nOTi_ce__RANge______", function);
        } else {
            function = noticeSelectRange.getRangeValue();
            if (function != null && function.trim().length() > 0) {
                try {
                    sTemp = this.getBaseTool().getFlowLib().computeUserSet(function, aCreator, aSender, mainRs, 0);
                    if (sTemp != null && sTemp.trim().length() > 0) {
                        someInfoRs.putObject("__nOTi_ce__RANge______", sTemp.trim());
                    }
                } catch (Exception var20) {
                    ;
                }
            } else {
                someInfoRs.putObject("__nOTi_ce__RANge______", "__ranGe__not___DEfinE__");
            }
        }

        if (appendSelectRange.isFromFramework()) {
            if (appendSelectRange.getFrameFlag() == (long) 3) {
                framList = appendSelectRange.getFrameList();
            }

            if (appendSelectRange.getFrameFlag() == (long) 4) {
                groupList = appendSelectRange.getGroupList();
            }

            function = String.valueOf(String.valueOf((new StringBuffer("javascript:openFrameList(\'sys_step_append_show\',\'sys_step_append_hidden\',")).append(selFlag).append(",").append(appendSelectRange.getFrameFlag()).append(",").append(appendSelectRange.getFrameLevel()).append(",").append(appendSelectRange.getFrameLevelTo()).append(",\'").append(framList).append("\',").append(appendSelectRange.isDisplayGroupNode() ? "1" : "0").append(",").append(appendSelectRange.isDisplayPersGroupNode() ? "1" : "0").append(",").append(appendSelectRange.isDisplayAllUserNode() ? "1" : "0").append(",\'").append(appendSelectRange.getGroupNodeDesc()).append("\',\'").append(appendSelectRange.getPersGroupNodeDesc()).append("\',\'").append(groupList).append("\',\'\',1)")));
            someInfoRs.putObject("____ApPenD_RA_nge_", function);
        } else {
            function = appendSelectRange.getRangeValue();
            if (function != null && function.trim().length() > 0) {
                try {
                    sTemp = this.getBaseTool().getFlowLib().computeUserSet(function, aCreator, aSender, mainRs, 0);
                    if (sTemp != null && sTemp.trim().length() > 0) {
                        someInfoRs.putObject("____ApPenD_RA_nge_", sTemp.trim());
                    }
                } catch (Exception var19) {
                    ;
                }
            } else {
                someInfoRs.putObject("____ApPenD_RA_nge_", "__ranGe__not___DEfinE__");
            }
        }

        if (deliverToSelectRange.isFromFramework()) {
            if (deliverToSelectRange.getFrameFlag() == (long) 3) {
                framList = deliverToSelectRange.getFrameList();
            }

            if (deliverToSelectRange.getFrameFlag() == (long) 4) {
                groupList = deliverToSelectRange.getGroupList();
            }

            function = String.valueOf(String.valueOf((new StringBuffer("javascript:openFrameList(\'sys_step_deliverto_show\',\'sys_step_deliverto_hidden\',")).append(selFlag).append(",").append(deliverToSelectRange.getFrameFlag()).append(",").append(deliverToSelectRange.getFrameLevel()).append(",").append(deliverToSelectRange.getFrameLevelTo()).append(",\'").append(framList).append("\',").append(deliverToSelectRange.isDisplayGroupNode() ? "1" : "0").append(",").append(deliverToSelectRange.isDisplayPersGroupNode() ? "1" : "0").append(",").append(deliverToSelectRange.isDisplayAllUserNode() ? "1" : "0").append(",\'").append(deliverToSelectRange.getGroupNodeDesc()).append("\',\'").append(deliverToSelectRange.getPersGroupNodeDesc()).append("\',\'").append(groupList).append("\',\'\',1)")));
            someInfoRs.putObject("__delIvEr__TO____Range____", function);
        } else if (deliverToRangeDef != null && deliverToRangeDef.trim().length() > 0) {
            try {
                sTemp = this.getBaseTool().getFlowLib().computeUserSet(deliverToRangeDef, aCreator, aSender, mainRs, 0);
                if (sTemp != null && sTemp.trim().length() > 0) {
                    someInfoRs.putObject("__delIvEr__TO____Range____", sTemp.trim());
                }
            } catch (Exception var18) {
                ;
            }
        } else {
            someInfoRs.putObject("__delIvEr__TO____Range____", "__ranGe__not___DEfinE__");
        }

    }

    public Hashtable getBatchStepInsHtml(String structs) throws Exception {
        CachedRowSet flowRs = null;
        CachedRowSet formRs = null;
        Object dealInfoRs = null;
        Hashtable returnStruct = new Hashtable();

        try {
            String e = "";
            SQLPara var22 = new SQLPara();
            long fieldCount = (long) PublicLib.getFieldCount(structs, ";");

            for (long sqlClause = 0L; sqlClause < fieldCount; ++sqlClause) {
                String tmpStruct = PublicLib.getField(structs, ";", sqlClause);
                String tmpFlowInid = PublicLib.getField(tmpStruct, ",", 0L);
                String tmpStepInid = PublicLib.getField(tmpStruct, ",", 1L);
                if (!e.equals("")) {
                    e = String.valueOf(String.valueOf(e)).concat(" or ");
                }

                e = String.valueOf(String.valueOf(e)).concat(" (A.flow_inid=? and B.step_inco=?) ");
                var22.add(String.valueOf(tmpFlowInid));
                var22.add(String.valueOf(tmpStepInid), 1);
            }

            String var21 = String.valueOf(String.valueOf((new StringBuffer(" select C.step_name,B.step_code,B.step_inco,B.step_stat,C.form_id as form_code,B.step_choi,B.step_type,C.send_titl,C.time_set,B.step_man, A.fins_name,A.flow_inid,A.flow_id,A.inst_crea,A.fset_id,A.svr_name, A.mtbl_stru,A.if_doc,A.doc_name,A.total_term, C.vali_set,C.man_stru,C.step_desc,C.step_tag,C.ente_set,C.Misc_Set from flow_inst A,step_inst B,step_def C  where A.flow_inid=B.flow_inid and B.flow_id=C.flow_id and B.step_code=C.step_code and (")).append(e).append(")")));
            flowRs = this.baseTool.getRowSet(var21, "", var22);
            flowRs.first();
            String fsetId = flowRs.getString("fset_id");
            if (fsetId == null || fsetId.equals("")) {
                throw new Exception("该流程没有设置表单集");
            }

            var21 = " select fset_id,form_code,form_cont,form_stru from form where fset_id=?";
            var22 = new SQLPara();
            var22.add(String.valueOf(fsetId));
            formRs = this.baseTool.getRowSet(var21, "", var22);
            this.restoreBatchSomeInfo(flowRs, returnStruct);
            this.restoreBatchHtml(flowRs, formRs, returnStruct);
        } catch (Exception var19) {
            String infomation = String.valueOf(String.valueOf((new StringBuffer("流程实例id列表=")).append(structs).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
            this.baseTool.error("getBatchStepInsHtml:".concat(String.valueOf(String.valueOf(infomation))), var19);
            this.unloadData(returnStruct);
            returnStruct = null;
            throw new Exception(var19.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(flowRs);
            flowRs = null;
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
        }

        return returnStruct;
    }

    public Hashtable getFreeFormHtml(long aFlowInid, long aStepInid, boolean aReadOnly, boolean aNewFlag, long aFSetId, long aListFormId, long aFreeFormId, int aRecordIndex, CachedRowSet aMainRs, CachedRowSet aHypoRs) throws Exception {
        Hashtable returnStruct = new Hashtable();
        CachedRowSet formRs = null;
        CachedRowSet flowRs = null;
        StringBuffer formHtml = null;
        String sqlClause = "";
        CachedRowSet crs = null;
        long stepInid = aStepInid;

        try {
            String infomation;
            try {
                SQLPara e = new SQLPara();
                if (aStepInid < (long) 0) {
                    sqlClause = " select C.vali_set from flow_def A,step_def C where A.flow_id=C.flow_id and C.flow_id=? and C.step_code=A.star_step";
                    e.add(String.valueOf(aFlowInid));
                } else {
                    if (aStepInid == (long) 0) {
                        SQLPara infomation1 = new SQLPara();
                        infomation1.add(String.valueOf(aFlowInid));
                        sqlClause = "select curr_step from flow_inst where flow_inid=?";
                        if (aFlowInid < minFlowInid) {
                            sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                            sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                        }
                        crs = this.baseTool.getRowSet(sqlClause, "", infomation1);
                        crs.next();
                        stepInid = crs.getLong("curr_step");
                    }

                    sqlClause = "select B.vali_set from flow_def A,step_def B,step_inst C where A.flow_id=C.flow_id and B.step_code=C.step_code and C.flow_id=B.flow_id and C.flow_inid=? and C.step_inco=?";
                    e.add(String.valueOf(aFlowInid));
                    e.add(String.valueOf(stepInid), 1);
                }
                if (aFlowInid < minFlowInid) {
                    sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                    sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                }
                flowRs = this.baseTool.getRowSet(sqlClause, "", e);
                flowRs.next();
                infomation = PublicLib.getStringFromClob(flowRs, "vali_set");
                ValidStruct validStruct = new ValidStruct();
                validStruct.restore(infomation);
                ValidListItem validListItem = (ValidListItem) validStruct.getListValidColl().get(String.valueOf(aListFormId));
                sqlClause = " select A.fset_id,A.form_code,A.form_cont,A.form_stru,B.svr_name  from form A, formset B  where A.fset_id=B.fset_id  and A.fset_id=? and A.form_code=?";
                e = new SQLPara();
                e.add(String.valueOf(aFSetId));
                e.add(String.valueOf(aFreeFormId), 1);
                if (aFlowInid < minFlowInid) {
                    sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                    sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                }
                formRs = this.baseTool.getRowSet(sqlClause, "", e);
                if (PublicLib.getRecordCount(formRs) == 0) {
                    throw new Exception("生成自由表单失败，是否已绑定了自由表单。");
                }

                formRs.first();
                StringBuffer rawHtml = new StringBuffer(PublicLib.getStringFromClob(formRs, "form_cont"));
                rawHtml = PublicLib.findReplace(rawHtml, "&lt;", "<", false);
                rawHtml = PublicLib.findReplace(rawHtml, "&gt;", ">", false);
                rawHtml = PublicLib.findReplace(rawHtml, "&amp;", "&", false);
                StringBuffer formStructXml = new StringBuffer(PublicLib.getStringFromClob(formRs, "form_stru"));
                formStructXml = PublicLib.findReplace(formStructXml, "&lt;", "@lt;", false);
                formStructXml = PublicLib.findReplace(formStructXml, "&gt;", "@gt;", false);
                FormStruct formStruct = new FormStruct();
                formStruct.restore(formStructXml.toString());
                String serverName = formRs.getString("svr_name");
                serverName = PublicLib.getField(serverName, "<", 0L);
                String tableName = formStruct.getSysInfo().getBindTable();
                tableName = PublicLib.getField(tableName, "<", 0L);
                if (aNewFlag) {
                    this.baseTool.getFlowLib().genHypoKey(aFSetId, serverName, tableName, aMainRs, aHypoRs);
                } else {
                    aHypoRs.absolute(aRecordIndex + 1);
                }

                FlowInstInfo fii = new FlowInstInfo(aFlowInid, aStepInid, -1L, false);
                fii.setStepMiscStruct(new StepMiscStruct());
                FormInstData formInstData = new FormInstData();
                formInstData.setFormStruct(formStruct);
                formHtml = this.restoreControlHtml(fii, aReadOnly, false, String.valueOf(aFSetId), "", rawHtml, serverName, formInstData, validListItem, aHypoRs, returnStruct);
                returnStruct.put("___hypoRs___", aHypoRs);
                returnStruct.put("___0-3__48_f_uct_FormStruct___", formStruct);
                returnStruct.put("0_9UAad_form_iNst_Data__", formInstData);
                returnStruct.put("formHtml", formHtml.substring(0));
            } catch (Exception var39) {
                infomation = String.valueOf(String.valueOf((new StringBuffer("流程实例id=")).append(aFlowInid).append(",步骤实例id=").append(aStepInid).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
                this.baseTool.error("getFreeFormHtml:".concat(String.valueOf(String.valueOf(infomation))), var39);
                this.unloadData(returnStruct);
                returnStruct = null;
                throw new Exception(var39.getMessage());
            }
        } finally {
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
            formHtml = null;
        }

        return returnStruct;
    }

    public void unloadData(Hashtable aReturnStruct) {
        CachedRowSet hypoRsColl;
        try {
            hypoRsColl = (CachedRowSet) aReturnStruct.get("mainRs");
            PublicLib.closeCachedRowSet(hypoRsColl);
        } catch (Exception var12) {
            var12.printStackTrace();
        }

        try {
            hypoRsColl = (CachedRowSet) aReturnStruct.get("attachRs");
            PublicLib.closeCachedRowSet(hypoRsColl);
        } catch (Exception var11) {
            var11.printStackTrace();
        }

        Hashtable hypoRsColl1 = (Hashtable) aReturnStruct.get("hypoRsColl");
        if (hypoRsColl1 != null) {
            Enumeration mainRsColl = hypoRsColl1.elements();

            while (mainRsColl.hasMoreElements()) {
                try {
                    CachedRowSet fieldMapNameRs = (CachedRowSet) mainRsColl.nextElement();
                    PublicLib.closeCachedRowSet(fieldMapNameRs);
                } catch (Exception var10) {
                    var10.printStackTrace();
                }
            }

            hypoRsColl1.clear();
        }

        Hashtable mainRsColl1 = (Hashtable) aReturnStruct.get("mainRsColl");
        if (mainRsColl1 != null) {
            Enumeration fieldMapNameRs1 = mainRsColl1.elements();

            while (fieldMapNameRs1.hasMoreElements()) {
                try {
                    CachedRowSet ex = (CachedRowSet) fieldMapNameRs1.nextElement();
                    PublicLib.closeCachedRowSet(ex);
                } catch (Exception var9) {
                    var9.printStackTrace();
                }
            }

            mainRsColl1.clear();
        }

        RecordSet fieldMapNameRs2 = (RecordSet) aReturnStruct.get("fieldMapNameRs");

        try {
            if (fieldMapNameRs2 != null) {
                fieldMapNameRs2.deleteAllRecord();
            }
        } catch (Exception var8) {
            var8.printStackTrace();
        }

        RecordSet ex1;
        try {
            ex1 = (RecordSet) aReturnStruct.get("someInfoRs");
            if (ex1 != null) {
                ex1.deleteAllRecord();
            }
        } catch (Exception var7) {
            var7.printStackTrace();
        }

        try {
            ex1 = (RecordSet) aReturnStruct.get("hypoMapRs");
            if (ex1 != null) {
                ex1.deleteAllRecord();
            }
        } catch (Exception var6) {
            var6.printStackTrace();
        }

        aReturnStruct.clear();
    }

    public String getPrintFormHtml(long aFlowInid, long aStepInid) throws Exception {
        CachedRowSet flowRs = null;
        CachedRowSet formRs = null;
        CachedRowSet crs = null;
        CachedRowSet mainRs = null;
        Hashtable returnStruct = new Hashtable();
        StringBuffer formHtml = null;

        try {
            String infomation;
            try {
                String e = "";
                infomation = null;
                SQLPara var48;
                if (aStepInid <= (long) 0) {
                    e = " select B.peri_inco,C.step_name,B.step_code,B.step_inco,B.step_stat,C.form_id as form_code,B.step_choi,B.step_type,C.send_titl,C.time_set,B.step_man, A.mark_step,A.if_mark,A.mark_path,A.flow_inid,A.flow_id,A.inst_crea,A.fset_id,A.svr_name, A.mtbl_stru,A.if_doc,A.doc_name,A.clear_status,A.total_term, C.vali_set,C.man_stru,C.step_desc,C.step_tag,C.ente_set,C.Misc_Set from flow_inst A,step_inst B,step_def C  where A.flow_inid=B.flow_inid and B.flow_id=C.flow_id and B.step_code=C.step_code and A.curr_step=B.step_inco and A.flow_inid=?";
                    var48 = new SQLPara();
                    var48.add(String.valueOf(aFlowInid));
                    if (aFlowInid < minFlowInid) {
                        e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                        e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                    }
                    flowRs = this.baseTool.getRowSet(e, "", var48);
                } else {
                    e = " select B.peri_inco,C.step_name,B.step_code,B.step_inco,B.step_stat,C.form_id as form_code,B.step_choi,B.step_type,C.send_titl,C.time_set,B.step_man, A.mark_step,A.if_mark,A.mark_path,A.flow_inid,A.flow_id,A.inst_crea,A.fset_id,A.svr_name, A.mtbl_stru,A.if_doc,A.doc_name,A.clear_status,A.total_term, C.vali_set,C.man_stru,C.step_desc,C.step_tag,C.ente_set,C.Misc_Set from flow_inst A,step_inst B,step_def C  where A.flow_inid=B.flow_inid and B.flow_id=C.flow_id and B.step_code=C.step_code and A.flow_inid=? and B.step_inco=?";
                    var48 = new SQLPara();
                    var48.add(String.valueOf(aFlowInid));
                    var48.add(String.valueOf(aStepInid), 1);
                    if (aFlowInid < minFlowInid) {
                        e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                        e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                    }
                    flowRs = this.baseTool.getRowSet(e, "", var48);
                }

                flowRs.beforeFirst();
                flowRs.next();
                String fsetId = flowRs.getString("fset_id");
                String formCode = flowRs.getString("form_code");
                if (fsetId != null && !fsetId.equals("")) {
                    long errorMessage;
                    if (flowRs.getInt("step_type") == 5) {
                        errorMessage = flowRs.getLong("peri_inco");
                        e = "select step_inco,step_type,form_code,peri_inco from step_inst where flow_inid=?";
                        var48.clear();
                        var48 = new SQLPara();
                        var48.add(String.valueOf(aFlowInid));
                        if (aFlowInid < minFlowInid) {
                            e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                            e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                        }
                        crs = this.baseTool.getRowSet(e, "", var48);
                        boolean criteria = true;
                        long rsFinded = 0L;
                        boolean formStructXml = PublicLib.find(crs, "step_inco=".concat(String.valueOf(String.valueOf(errorMessage))), true);
                        int formStruct = crs.size();

                        for (int tableName = 0; formStructXml && tableName < formStruct; ++tableName) {
                            int var50 = crs.getInt("step_type");
                            if (var50 == 2 || var50 == 3 || var50 == 1) {
                                rsFinded = crs.getLong("form_code");
                                break;
                            }

                            errorMessage = crs.getLong("peri_inco");
                            formStructXml = PublicLib.find(crs, "step_inco=".concat(String.valueOf(String.valueOf(errorMessage))), true);
                        }

                        flowRs.updateLong("form_code", rsFinded);
                        flowRs.updateRow();
                    } else if (formCode == null || formCode.trim().length() == 0) {
                        e = "select form_code from step_inst where step_type=1 and flow_inid=?";
                        var48.clear();
                        var48 = new SQLPara();
                        var48.add(String.valueOf(aFlowInid));
                        if (aFlowInid < minFlowInid) {
                            e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                            e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                        }
                        crs = this.baseTool.getRowSet(e, "", var48);
                        crs.next();
                        errorMessage = crs.getLong("form_code");
                        flowRs.updateString("form_code", String.valueOf(String.valueOf(errorMessage)).concat(""));
                        flowRs.updateRow();
                    }

                    var48.clear();
                    infomation = null;
                    e = "";
                    e = " select fset_id,form_code,form_cont,form_stru from form where fset_id=?";
                    var48 = new SQLPara();
                    var48.add(String.valueOf(fsetId));
                    if (aFlowInid < minFlowInid) {
                        e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                        e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                    }
                    formRs = this.baseTool.getRowSet(e, "", var48);
                    formCode = flowRs.getString("form_code");
                    String var49 = "";
                    if (formCode != null && !formCode.equals("")) {
                        String serverName = flowRs.getString("svr_name");
                        serverName = PublicLib.getField(serverName, "<", 0L);
                        String var51 = "form_code=".concat(String.valueOf(String.valueOf(formCode)));
                        boolean var52 = PublicLib.find(formRs, var51, true);
                        if (!var52) {
                            throw new Exception("系统找不到指定的表单");
                        } else {
                            StringBuffer rawHtml = new StringBuffer(PublicLib.getStringFromClob(formRs, "form_cont"));
                            rawHtml = PublicLib.findReplace(rawHtml, "&lt;", "<", false);
                            rawHtml = PublicLib.findReplace(rawHtml, "&gt;", ">", false);
                            rawHtml = PublicLib.findReplace(rawHtml, "&amp;", "&", false);
                            StringBuffer var53 = new StringBuffer(PublicLib.getStringFromClob(formRs, "form_stru"));
                            var53 = PublicLib.findReplace(var53, "&lt;", "@lt;", false);
                            var53 = PublicLib.findReplace(var53, "&gt;", "@gt;", false);
                            FormStruct var54 = new FormStruct();
                            var54.restore(var53.toString());
                            String var55 = var54.getSysInfo().getBindTable();
                            var55 = PublicLib.getField(var55, "<", 0L);
                            String validStructXml = PublicLib.getStringFromClob(flowRs, "vali_set");
                            ValidStruct validStruct = new ValidStruct();
                            validStruct.restore(validStructXml);
                            String stepName = "";
                            e = String.valueOf(String.valueOf((new StringBuffer("select * from ")).append(var55).append(" where ")));
                            var48 = new SQLPara();
                            stepName = flowRs.getString("step_name");
                            String stepTag = flowRs.getString("step_tag");
                            if (stepTag == null) {
                                stepTag = "";
                            }

                            if (stepTag.trim().length() == 0) {
                                stepTag = stepName;
                            }

                            String mtblStructXml = flowRs.getString("mtbl_stru");
                            MtblStruct mtblStruct = new MtblStruct();
                            mtblStruct.restore(mtblStructXml);
                            Hashtable keyColl = mtblStruct.getKeyColl();

                            for (int readOnly = 0; readOnly < keyColl.size(); ++readOnly) {
                                if (readOnly > 0) {
                                    e = String.valueOf(String.valueOf(e)).concat(" and ");
                                }

                                MtblItem someInfoRs = (MtblItem) keyColl.get(String.valueOf(readOnly));
                                e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(someInfoRs.getFieldName())).concat("=?"))));
                                if (someInfoRs.getFieldType() >= (long) 1 && someInfoRs.getFieldType() <= (long) 8) {
                                    var48.add(someInfoRs.getFieldValue(), 1);
                                } else {
                                    var48.add(someInfoRs.getFieldValue());
                                }
                            }

                            this.baseTool.info("sqlClause=".concat(String.valueOf(String.valueOf(e))));
                            mainRs = this.baseTool.getServerRowSet(serverName, e, var55.toUpperCase(), var48);
                            mainRs.first();
                            boolean var56 = true;
                            RecordSet var57 = new RecordSet();
                            long docFlag = (long) flowRs.getInt("if_doc");
                            var57.insertRow();
                            var57.putObject("doc_flag", String.valueOf(docFlag));
                            returnStruct.put("someInfoRs", var57);
                            int stepType = flowRs.getInt("step_type");
                            long totalTerm = flowRs.getLong("total_term");
                            FlowInstInfo fii = new FlowInstInfo(aFlowInid, aStepInid, -1L, false);
                            fii.setStepType(stepType);
                            fii.setTotalTerm(totalTerm);
                            String miscStructXml = flowRs.getString("misc_set");
                            StepMiscStruct miscSet = new StepMiscStruct();
                            miscSet.restore(miscStructXml);
                            fii.setStepMiscStruct(miscSet);
                            FormInstData formInstData = new FormInstData();
                            formInstData.setFormStruct(var54);
                            formHtml = this.restoreControlHtml(fii, var56, true, fsetId, stepTag, rawHtml, serverName, formInstData, validStruct, mainRs, returnStruct);
                            formHtml = this.restoreSubForm(var56, formHtml, var54, formRs, mainRs, serverName, validStruct, returnStruct);
                            RecordSet fieldMapNameRs = (RecordSet) returnStruct.remove("fieldMapNameRs");
                            this.restoreRelationForm(fii, var56, var54, formRs, mainRs, serverName, validStruct, returnStruct);
                            returnStruct.put("fieldMapNameRs", fieldMapNameRs);
                            return formHtml.substring(0);
                        }
                    } else {
                        var49 = "步骤设置没指定表单,请检查流程设计。";
                        throw new Exception(var49);
                    }
                } else {
                    throw new Exception("该流程没有设置表单集");
                }
            } catch (Exception var46) {
                infomation = String.valueOf(String.valueOf((new StringBuffer("生成表单。流程实例id=")).append(aFlowInid).append(",步骤实例id=").append(aStepInid).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
                this.baseTool.error("getPrintFormHtml:".concat(String.valueOf(String.valueOf(infomation))), var46);
                throw new Exception(var46.getMessage());
            }
        } finally {
            PublicLib.closeCachedRowSet(flowRs);
            flowRs = null;
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
            PublicLib.closeCachedRowSet(crs);
            crs = null;
            PublicLib.closeCachedRowSet(mainRs);
            mainRs = null;
            this.unloadData(returnStruct);
        }
    }

    public String getPrintFormHtml(long aFlowInid, long aStepInid, RecordSet aSomeInfoRs, CachedRowSet aMainRs, Hashtable aHypoRsColl, boolean aReadOnly) throws Exception {
        CachedRowSet tempRs = null;
        CachedRowSet flowRs = null;
        CachedRowSet formRs = null;
        StringBuffer formHtml = null;
        String tmpFormHtml = null;
        Hashtable returnStruct = new Hashtable();
        if (aSomeInfoRs == null) {
            aSomeInfoRs = new RecordSet();
            aSomeInfoRs.insertRow();
        }

        try {
            String e = "";
            SQLPara var71 = new SQLPara();
            if (aStepInid < (long) 0) {
                e = " select vali_set,form_id as form_code,A.step_tag,C.fset_id,svr_name,step_choi,step_name,step_type,man_stru,B.total_term,A.misc_set from step_def A,flow_def B,formset C where A.flow_id=B.flow_id and step_code=B.star_step and B.fset_id=C.fset_id and A.flow_id=?";
                var71.add(String.valueOf(aFlowInid));
            } else {
                if (aStepInid == (long) 0) {
                    e = "select curr_step from flow_inst where flow_inid=?";
                    SQLPara fsetId = new SQLPara();
                    fsetId.add(String.valueOf(aFlowInid));
                    if (aFlowInid < minFlowInid) {
                        e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                        e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                    }
                    tempRs = this.baseTool.getRowSet(e, "", fsetId);
                    tempRs.next();
                    aStepInid = tempRs.getLong("curr_step");
                }

                e = " select C.step_tag,C.step_name,B.step_code,B.step_inco,C.form_id as form_code,B.step_choi,B.step_type, A.flow_inid,A.flow_id,A.fset_id,A.svr_name, A.mtbl_stru,A.total_term, C.vali_set,C.man_stru,C.misc_set from flow_inst A,step_inst B,step_def C where A.flow_inid=B.flow_inid and B.flow_id=C.flow_id and B.step_code=C.step_code and A.flow_inid=? and B.step_inco=?";
                var71.add(String.valueOf(aFlowInid));
                var71.add(String.valueOf(aStepInid), 1);
            }

            if (aFlowInid < minFlowInid) {
                e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
            }
            flowRs = this.baseTool.getRowSet(e, "", var71);
            flowRs.next();
            String var72 = flowRs.getString("fset_id");
            e = " select fset_id,form_code,form_cont,form_stru from form where fset_id=?";
            var71 = new SQLPara();
            var71.add(String.valueOf(var72));
            if (aFlowInid < minFlowInid) {
                e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
            }
            formRs = this.baseTool.getRowSet(e, "", var71);
            String formCode = flowRs.getString("form_code");
            String serverName = flowRs.getString("svr_name");
            serverName = PublicLib.getField(serverName, "<", 0L);
            String criteria = "form_code=".concat(String.valueOf(String.valueOf(formCode)));
            PublicLib.find(formRs, criteria, true);
            StringBuffer rawHtml = new StringBuffer(PublicLib.getStringFromClob(formRs, "form_cont"));
            rawHtml = PublicLib.findReplace(rawHtml, "&lt;", "<", false);
            rawHtml = PublicLib.findReplace(rawHtml, "&gt;", ">", false);
            rawHtml = PublicLib.findReplace(rawHtml, "&amp;", "&", false);
            StringBuffer formStructXml = new StringBuffer(PublicLib.getStringFromClob(formRs, "form_stru"));
            formStructXml = PublicLib.findReplace(formStructXml, "&lt;", "@lt;", false);
            formStructXml = PublicLib.findReplace(formStructXml, "&gt;", "@gt;", false);
            FormStruct formStruct = new FormStruct();
            formStruct.restore(formStructXml.toString());
            String validStructXml = PublicLib.getStringFromClob(flowRs, "vali_set");
            ValidStruct validStruct = new ValidStruct();
            validStruct.restore(validStructXml);
            String stepName = flowRs.getString("step_name");
            String stepTag = flowRs.getString("step_tag");
            if (stepTag == null) {
                stepTag = "";
            }

            if (stepTag.trim().length() == 0) {
                stepTag = stepName;
            }

            returnStruct.put("someInfoRs", aSomeInfoRs);
            int stepType = flowRs.getInt("step_type");
            long totalTerm = flowRs.getLong("total_term");
            FlowInstInfo fii = new FlowInstInfo(aFlowInid, aStepInid, -1L, false);
            fii.setStepType(stepType);
            fii.setTotalTerm(totalTerm);
            String miscStructXml = flowRs.getString("misc_set");
            StepMiscStruct miscSet = new StepMiscStruct();
            miscSet.restore(miscStructXml);
            fii.setStepMiscStruct(miscSet);
            FormInstData formInstData = new FormInstData();
            formInstData.setFormStruct(formStruct);
            formHtml = this.restoreControlHtml(fii, aReadOnly, true, var72, stepTag, rawHtml, serverName, formInstData, validStruct, aMainRs, returnStruct);
            Enumeration subFormColl = formStruct.getSubFormColl().elements();
            Hashtable listValidColl = validStruct.getListValidColl();

            for (int index = 0; subFormColl.hasMoreElements(); ++index) {
                CachedRowSet hypoRs = null;

                try {
                    MySubForm ex = (MySubForm) subFormColl.nextElement();
                    long left = ex.getLeft();
                    long top = ex.getTop();
                    long width = ex.getWidth();
                    long height = ex.getHeight();
                    String tag = "S_u_b__WF__C_t_L=".concat(String.valueOf(String.valueOf(ex.getSubFormCode())));
                    criteria = "form_code=".concat(String.valueOf(String.valueOf(ex.getSubFormCode())));
                    boolean rsFinded = PublicLib.find(formRs, criteria, true);
                    if (rsFinded) {
                        String replace = String.valueOf(String.valueOf((new StringBuffer("<DIV style=\"left:")).append(left).append("px;").append("top:").append(top).append("px;").append("width:").append(width).append("px;").append("height:").append(height).append("px;")));
                        if (ex.isAbsolute()) {
                            replace = String.valueOf(String.valueOf(replace)).concat("position:absolute;overflow:auto;");
                        }

                        replace = String.valueOf(String.valueOf(replace)).concat("Z-INDEX: 100\">");
                        String subFormStructXml = PublicLib.getStringFromClob(formRs, "form_stru");
                        subFormStructXml = PublicLib.findReplace(subFormStructXml, "&lt;", "@lt;");
                        subFormStructXml = PublicLib.findReplace(subFormStructXml, "&gt;", "@gt;");
                        FormStruct subFormStruct = new FormStruct();
                        subFormStruct.restore(subFormStructXml);
                        hypoRs = (CachedRowSet) aHypoRsColl.get(String.valueOf(index));
                        ValidListItem validListItem = (ValidListItem) listValidColl.get(String.valueOf(ex.getSubFormCode()));
                        String listFormHtml = this.restoreListFormHtml(aReadOnly, index, subFormStruct, hypoRs, serverName, height, validListItem);
                        replace = String.valueOf(String.valueOf(replace)).concat(String.valueOf(String.valueOf(listFormHtml)));
                        replace = String.valueOf(String.valueOf(replace)).concat("</DIV>");
                        formHtml = PublicLib.findReplace(formHtml, tag, replace, true);
                    }
                } catch (Exception var67) {
                    throw new Exception(var67.getMessage());
                } finally {
                    ;
                }
            }

            tmpFormHtml = formHtml.substring(0);
        } catch (Exception var69) {
            var69.printStackTrace();
            String infomation = String.valueOf(String.valueOf((new StringBuffer("流程实例id=")).append(aFlowInid).append(",步骤实例id=").append(aStepInid).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
            this.baseTool.error("reGetFormHtml:".concat(String.valueOf(String.valueOf(infomation))), var69);
            this.unloadData(returnStruct);
            returnStruct = null;
            throw new Exception(var69.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(tempRs);
            tempRs = null;
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
            PublicLib.closeCachedRowSet(flowRs);
            flowRs = null;
            formHtml = null;
        }

        return tmpFormHtml;
    }

    public FormControlHtmlData getFormControlHtmlData(long aFlowInid, long aStepInid) throws Exception {
        CachedRowSet tempRs = null;
        CachedRowSet flowRs = null;
        CachedRowSet formRs = null;
        CachedRowSet formTempletRs = null;
        CachedRowSet mainRs = null;
        CachedRowSet mapRs = null;
        Object dealInfoRs = null;
        Object frameRs = null;
        CachedRowSet attachRs = null;
        FormControlHtmlDataImpl fchd = new FormControlHtmlDataImpl();
        long dealIndx = -1L;

        try {
            RecordSet e = new RecordSet();
            RecordSet var61 = new RecordSet();
            String sqlClause;
            SQLPara sqlPara;
            if (aStepInid <= (long) 0) {
                sqlClause = "select curr_step from flow_inst where flow_inid=?";
                sqlPara = new SQLPara();
                sqlPara.add(String.valueOf(aFlowInid));
                if (aFlowInid < minFlowInid) {
                    sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                    sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                }
                tempRs = this.baseTool.getRowSet(sqlClause, "", sqlPara);
                tempRs.next();
                aStepInid = tempRs.getLong("curr_step");
            }

            sqlClause = " select C.form_id as form_code,A.fset_id,A.svr_name,A.mtbl_stru from flow_inst A,step_inst B,step_def C  where A.flow_inid=B.flow_inid and B.flow_id=C.flow_id and B.step_code=C.step_code and A.flow_inid=? and B.step_inco=?";
            sqlPara = new SQLPara();
            sqlPara.add(String.valueOf(aFlowInid));
            sqlPara.add(String.valueOf(aStepInid), 1);
            if (aFlowInid < minFlowInid) {
                sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
            }
            flowRs = this.baseTool.getRowSet(sqlClause, "", sqlPara);
            if (!flowRs.next()) {
                throw new Exception("在打印之前请先做保存。");
            } else {
                String fsetId = flowRs.getString("fset_id");
                String formCode = flowRs.getString("form_code");
                String serverName = flowRs.getString("svr_name");
                serverName = PublicLib.getField(serverName, "<", 0L);
                String mtblStructXml = flowRs.getString("mtbl_stru");
                sqlClause = " select form_stru from form where fset_id=? and form_code=?";
                sqlPara = new SQLPara();
                sqlPara.add(String.valueOf(fsetId));
                sqlPara.add(String.valueOf(formCode), 1);
                formRs = this.baseTool.getRowSet(sqlClause, "", sqlPara);
                sqlPara.clear();
                sqlPara = null;
                formRs.first();
                String formStructXml = PublicLib.getStringFromClob(formRs, "form_stru");
                formStructXml = PublicLib.findReplace(formStructXml, "&lt;", "@lt;");
                formStructXml = PublicLib.findReplace(formStructXml, "&gt;", "@gt;");
                FormStruct formStruct = new FormStruct();
                formStruct.restore(formStructXml);
                String tableName = formStruct.getSysInfo().getBindTable();
                tableName = PublicLib.getField(tableName, "<", 0L);
                MtblStruct mtblStruct = new MtblStruct();
                mtblStruct.restore(mtblStructXml);
                sqlClause = String.valueOf(String.valueOf((new StringBuffer("select * from ")).append(tableName).append(" where ")));
                Hashtable keyColl = mtblStruct.getKeyColl();
                sqlPara = new SQLPara();

                for (int controlColl = 0; controlColl < keyColl.size(); ++controlColl) {
                    if (controlColl > 0) {
                        sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and ");
                    }

                    MtblItem fieldRs = (MtblItem) keyColl.get(String.valueOf(controlColl));
                    sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(fieldRs.getFieldName())).concat("=?"))));
                    if (fieldRs.getFieldType() >= (long) 1 && fieldRs.getFieldType() <= (long) 8) {
                        sqlPara.add(fieldRs.getFieldValue(), 1);
                    } else {
                        sqlPara.add(fieldRs.getFieldValue());
                    }
                }

                mainRs = this.baseTool.getServerRowSet(serverName, sqlClause, "", sqlPara);
                mainRs.next();
                Hashtable var62 = formStruct.getControlColl();
                RecordSet var63 = this.baseTool.fieldList(serverName, tableName);
                Enumeration keys = var62.keys();

                String title;
                String formula;
                String templetId;
                MyControl templetContents;
                String templetName;
                while (keys.hasMoreElements()) {
                    String printInfo = "";
                    title = "";
                    formula = "";
                    templetId = (String) keys.nextElement();
                    templetContents = (MyControl) var62.get(templetId);
                    templetName = PublicLib.getField(templetContents.getFieldName(), "<", 0L);
                    int isDefaultTemplet = (int) templetContents.getControlType();
                    if (templetName != null && !templetName.equals("")) {
                        if (mainRs.getObject(templetName) != null) {
                            printInfo = mainRs.getString(templetName);
                            formula = printInfo;
                        }
                    } else if (isDefaultTemplet != 9 && isDefaultTemplet != 12) {
                        continue;
                    }

                    long fieldType = templetContents.getFieldType();
                    if (fieldType == (long) 3) {
                        printInfo = PublicLib.getNonZeroValue(printInfo);
                        formula = printInfo;
                    }

                    long tabOrder = templetContents.getTabOrder();
                    String fieldDesc = "";
                    String critera = "fld_name=".concat(String.valueOf(String.valueOf(templetName)));
                    boolean rsFinded = var63.find(critera);
                    if (rsFinded) {
                        fieldDesc = (String) var63.getObject("fld_desc");
                    }

                    if (fieldDesc == null || fieldDesc.equals("")) {
                        fieldDesc = templetName;
                    }

                    String table;
                    String display;
                    String user;
                    switch (isDefaultTemplet) {
                        case 1:
                        case 2:
                            if (printInfo == null) {
                                printInfo = "";
                                formula = printInfo;
                            }

                            if (fieldType != (long) 4 && fieldType != (long) 5 && fieldType != (long) 6) {
                                printInfo = PublicLib.findReplace(printInfo, "\n", "<br>");
                                printInfo = PublicLib.findReplace(printInfo, " ", "&nbsp;");
                            } else if ((templetContents.getDefaultType() != (long) 1 || !templetContents.getDefaultValue().equals("0")) && templetContents.getDefaultType() == (long) 1) {
                                printInfo = PublicLib.getField(printInfo, ".", 0L);
                                formula = printInfo;
                            } else {
                                printInfo = PublicLib.getField(printInfo, " ", 0L);
                                formula = printInfo;
                            }
                        case 3:
                        case 8:
                        case 11:
                        default:
                            break;
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                            String valueList = "";
                            String displayList = "";
                            if (((MyControlData) templetContents).getFrom() == (long) 0) {
                                valueList = ((MyControlData) templetContents).getValueList();
                                displayList = ((MyControlData) templetContents).getDisplayList();
                            } else if (((MyControlData) templetContents).getFrom() == (long) 1) {
                                String var67 = ((MyControlData) templetContents).getDisplayField();
                                String var68 = ((MyControlData) templetContents).getValueField();
                                table = ((MyControlData) templetContents).getTable();
                                var67 = PublicLib.getField(var67, "<", 0L);
                                var68 = PublicLib.getField(var68, "<", 0L);
                                table = PublicLib.getField(table, "<", 0L);
                                sqlClause = String.valueOf(String.valueOf((new StringBuffer("select ")).append(var67).append(",").append(var68).append(" from ").append(table).append(" where ").append(var68).append(" is not null")));

                                for (mapRs = this.baseTool.getServerRowSet(serverName, sqlClause, "", new SQLPara()); mapRs.next(); valueList = String.valueOf(String.valueOf(valueList)).concat(String.valueOf(String.valueOf(user)))) {
                                    display = "";
                                    user = "";
                                    if (mapRs.getString(var67) != null) {
                                        display = String.valueOf(String.valueOf(display)).concat(String.valueOf(String.valueOf(mapRs.getString(var67))));
                                    }

                                    if (fieldType == (long) 3) {
                                        user = PublicLib.getNonZeroValue(mapRs.getString(var68));
                                    } else {
                                        user = mapRs.getString(var68);
                                    }

                                    if (!valueList.equals("")) {
                                        displayList = String.valueOf(String.valueOf(displayList)).concat(";");
                                        valueList = String.valueOf(String.valueOf(valueList)).concat(";");
                                    }

                                    displayList = String.valueOf(String.valueOf(displayList)).concat(String.valueOf(String.valueOf(display)));
                                }
                            }

                            printInfo = this.getFieldDisplay(valueList, displayList, printInfo);
                            formula = printInfo;
                            break;
                        case 9:
                            RecordSet valueField = new RecordSet();
                            valueField.insertRow();
                            templetName = ((MySign) templetContents).getTag();
                            fieldDesc = templetName;
                            printInfo = this._$13537(false, (MySign) templetContents, "disabled", String.valueOf(String.valueOf(aFlowInid)).concat(""), String.valueOf(String.valueOf(aStepInid)).concat(""), dealIndx, valueField, false);
                            formula = this._$13773(false, (MySign) templetContents, String.valueOf(String.valueOf(aFlowInid)).concat(""), String.valueOf(String.valueOf(aStepInid)).concat(""), dealIndx, valueField, 0);
                            break;
                        case 10:
                            StringBuffer displayField = new StringBuffer(printInfo);
                            printInfo = this.restoreMapDisplay((MyMap) templetContents, serverName, displayField);
                            displayField = null;
                            if (!((MyMap) templetContents).isMultiline()) {
                                printInfo = PublicLib.getField(printInfo, "\"", 1L);
                            }

                            formula = printInfo;
                            break;
                        case 12:
                            fieldDesc = "附件描述";
                            templetName = "__MCUBE_ATTACHMENT__";
                            title = "MCUBE_ATTACHMENT__";
                            table = ((MyAttach) templetContents).getOwner();
                            display = ((MyAttach) templetContents).getTag();
                            user = "";
                            if (table != null && !table.equals("")) {
                                table = PublicLib.getField(table, "<", 0L);
                                user = mainRs.getString(table);
                            }

                            sqlClause = "";
                            sqlPara = new SQLPara();
                            if (display != null && !display.equals("")) {
                                sqlClause = "select f.file_name from sys_file2 f,step_inst s,flow_inst t where f.flow_inid=s.flow_inid and f.step_inco=s.step_inco and s.step_name=? and f.step_inco is not null and ";
                                sqlPara.add(display);
                            } else {
                                sqlClause = "select f.file_name from sys_file2 f,flow_inst t where ";
                            }

                            sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" f.flow_inid=t.flow_inid and f.file_name<>t.doc_name and f.flow_inid=?");
                            sqlPara.add(String.valueOf(aFlowInid));
                            if (user != null && !user.equals("")) {
                                sqlClause = String.valueOf(String.valueOf(sqlClause)).concat(" and f.modi_user=?");
                                sqlPara.add(user);
                            }

                            String fileName;
                            if (aFlowInid < minFlowInid) {
                                sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                                sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                            }
                            for (attachRs = this.baseTool.getRowSet(sqlClause, "", sqlPara); attachRs.next(); printInfo = String.valueOf(String.valueOf(printInfo)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf((new StringBuffer("《")).append(fileName).append("》"))))))) {
                                fileName = attachRs.getString("file_name");
                                if (!printInfo.equals("")) {
                                    printInfo = String.valueOf(String.valueOf(printInfo)).concat(";");
                                }
                            }

                            formula = printInfo;
                    }

                    e.insertRow();
                    e.putObject("fieldName", templetName);
                    e.putObject("fieldDesc", fieldDesc);
                    e.putObject("fieldValue", printInfo);
                    e.putObject("tabOrder", String.valueOf(tabOrder));
                    var61.insertRow();
                    var61.putObject("fieldName", templetName);
                    var61.putObject("fieldDesc", fieldDesc);
                    var61.putObject("fieldValue", formula);
                    var61.putObject("tabOrder", String.valueOf(tabOrder));
                    if (title.trim().length() > 0) {
                        e.insertRow();
                        e.putObject("fieldName", title);
                        e.putObject("fieldDesc", fieldDesc);
                        e.putObject("fieldValue", printInfo);
                        e.putObject("tabOrder", String.valueOf(tabOrder));
                        var61.insertRow();
                        var61.putObject("fieldName", title);
                        var61.putObject("fieldDesc", fieldDesc);
                        var61.putObject("fieldValue", formula);
                        var61.putObject("tabOrder", String.valueOf(tabOrder));
                    }
                }

                e.beforeFirst();
                e.sort("tabOrder");
                MyPrintInfo var66 = formStruct.getPrintInfo();
                title = var66.getTitle();
                formula = var66.getFormula();
                formula = this.baseTool.getFlowLib().getInstName(formula, mainRs);
                sqlClause = "select templet_id,templet_contents,is_default,templet_name from wf_form_print_templet where fset_id=? and form_code=?";
                sqlPara = new SQLPara();
                sqlPara.add(String.valueOf(fsetId));
                sqlPara.add(String.valueOf(formCode), 1);
                if (aFlowInid < minFlowInid) {
                    sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                    sqlClause = sqlClause.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                }
                formTempletRs = this.baseTool.getRowSet(sqlClause, "", sqlPara);
                templetId = null;
                templetContents = null;
                templetName = null;
                boolean var65 = false;

                while (formTempletRs.next()) {
                    templetId = formTempletRs.getString("templet_id");
                    String var64 = PublicLib.getStringFromClob(formTempletRs, "templet_contents");
                    var65 = formTempletRs.getInt("is_default") == 1;
                    templetName = formTempletRs.getString("templet_name");
                    if (templetName == null) {
                        templetName = "";
                    }

                    templetId = String.valueOf(String.valueOf((new StringBuffer(String.valueOf(String.valueOf(fsetId)))).append("____").append(formCode).append("____").append(templetId)));
                    if (var64 != null) {
                        fchd.addTempletItem(templetId, templetName, new StringBuffer(var64));
                        if (var65) {
                            fchd.setDefaultTempletId(templetId);
                        }
                    }
                }

                fchd.setTitle(title);
                fchd.setFormula(formula);
                fchd.setControlHtmlData(e, var61);
                return fchd;
            }
        } catch (Exception var59) {
            String infomation = String.valueOf(String.valueOf((new StringBuffer("流程实例id=")).append(aFlowInid).append(",步骤实例id=").append(aStepInid).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
            this.baseTool.error("getFormControlHtmlData:".concat(String.valueOf(String.valueOf(infomation))), var59);
            throw new Exception(var59.getMessage());
        } finally {
            PublicLib.closeCachedRowSet(tempRs);
            tempRs = null;
            PublicLib.closeCachedRowSet(flowRs);
            flowRs = null;
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
            PublicLib.closeCachedRowSet(formTempletRs);
            formTempletRs = null;
            PublicLib.closeCachedRowSet(mainRs);
            mainRs = null;
            PublicLib.closeCachedRowSet(mapRs);
            mapRs = null;
            PublicLib.closeCachedRowSet((CachedRowSet) dealInfoRs);
            dealInfoRs = null;
            PublicLib.closeCachedRowSet((CachedRowSet) frameRs);
            frameRs = null;
            PublicLib.closeCachedRowSet(attachRs);
            attachRs = null;
        }
    }

    public FormControlDataStruct getFormControlDataXml(long aFlowInid) throws Exception {
        CachedRowSet tempRs = null;
        CachedRowSet flowRs = null;
        CachedRowSet formRs = null;
        Object formTempletRs = null;
        CachedRowSet mainRs = null;
        CachedRowSet mapRs = null;
        Object dealInfoRs = null;
        Object frameRs = null;
        Object attachRs = null;
        FormControlDataStruct fcds = new FormControlDataStruct();
        long dealIndx = -1L;
        long aStepInid = 0L;

        FormControlDataStruct fsetId;
        try {
            String infomation;
            try {
                String e = "select curr_step from flow_inst where flow_inid=?";
                SQLPara var56 = new SQLPara();
                var56.add(String.valueOf(aFlowInid));
                if (aFlowInid < minFlowInid) {
                    e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                    e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                }
                tempRs = this.baseTool.getRowSet(e, "", var56);
                tempRs.next();
                aStepInid = tempRs.getLong("curr_step");
                var56.clear();
                e = " select C.form_id as form_code,A.fset_id,A.svr_name,A.mtbl_stru from flow_inst A,step_inst B,step_def C  where A.flow_inid=B.flow_inid and B.flow_id=C.flow_id and B.step_code=C.step_code and A.flow_inid=? and B.step_inco=?";
                var56 = new SQLPara();
                var56.add(String.valueOf(aFlowInid));
                var56.add(String.valueOf(aStepInid), 1);
                if (aFlowInid < minFlowInid) {
                    e = e.replaceAll("\\bflow_inst\\b", "flow_inst_bak");
                    e = e.replaceAll("\\bstep_inst\\b", "step_inst_bak");
                }
                flowRs = this.baseTool.getRowSet(e, "", var56);
                if (flowRs.next()) {
                    String var57 = flowRs.getString("fset_id");
                    String formCode = flowRs.getString("form_code");
                    String serverName = flowRs.getString("svr_name");
                    serverName = PublicLib.getField(serverName, "<", 0L);
                    String mtblStructXml = flowRs.getString("mtbl_stru");
                    e = " select form_stru from form where fset_id=? and form_code=?";
                    var56 = new SQLPara();
                    var56.add(String.valueOf(var57));
                    var56.add(String.valueOf(formCode), 1);
                    formRs = this.baseTool.getRowSet(e, "", var56);
                    var56.clear();
                    infomation = null;
                    formRs.first();
                    String formStructXml = PublicLib.getStringFromClob(formRs, "form_stru");
                    formStructXml = PublicLib.findReplace(formStructXml, "&lt;", "@lt;");
                    formStructXml = PublicLib.findReplace(formStructXml, "&gt;", "@gt;");
                    FormStruct formStruct = new FormStruct();
                    formStruct.restore(formStructXml);
                    String tableName = formStruct.getSysInfo().getBindTable();
                    tableName = PublicLib.getField(tableName, "<", 0L);
                    MtblStruct mtblStruct = new MtblStruct();
                    mtblStruct.restore(mtblStructXml);
                    e = String.valueOf(String.valueOf((new StringBuffer("select * from ")).append(tableName).append(" where ")));
                    Hashtable keyColl = mtblStruct.getKeyColl();
                    var56 = new SQLPara();

                    for (int controlColl = 0; controlColl < keyColl.size(); ++controlColl) {
                        if (controlColl > 0) {
                            e = String.valueOf(String.valueOf(e)).concat(" and ");
                        }

                        MtblItem fieldRs = (MtblItem) keyColl.get(String.valueOf(controlColl));
                        e = String.valueOf(String.valueOf(e)).concat(String.valueOf(String.valueOf(String.valueOf(String.valueOf(fieldRs.getFieldName())).concat("=?"))));
                        if (fieldRs.getFieldType() >= (long) 1 && fieldRs.getFieldType() <= (long) 8) {
                            var56.add(fieldRs.getFieldValue(), 1);
                        } else {
                            var56.add(fieldRs.getFieldValue());
                        }
                    }

                    mainRs = this.baseTool.getServerRowSet(serverName, e, "", var56);
                    mainRs.next();
                    Hashtable var58 = formStruct.getControlColl();
                    RecordSet var59 = this.baseTool.fieldList(serverName, tableName);
                    Enumeration keys = var58.keys();

                    while (keys.hasMoreElements()) {
                        String fieldValue = "";
                        String fieldName2 = "";
                        String logicName = (String) keys.nextElement();
                        MyControl control = (MyControl) var58.get(logicName);
                        String fieldName = PublicLib.getField(control.getFieldName(), "<", 0L);
                        int controlType = (int) control.getControlType();
                        if (fieldName != null && !fieldName.equals("")) {
                            if (mainRs.getObject(fieldName) != null) {
                                fieldValue = mainRs.getString(fieldName);
                            }
                        } else if (controlType != 9 && controlType != 12) {
                            continue;
                        }

                        long fieldType = control.getFieldType();
                        if (fieldType == (long) 3) {
                            fieldValue = PublicLib.getNonZeroValue(fieldValue);
                        }

                        String fieldDesc = "";
                        String critera = "fld_name=".concat(String.valueOf(String.valueOf(fieldName)));
                        boolean rsFinded = var59.find(critera);
                        if (rsFinded) {
                            fieldDesc = (String) var59.getObject("fld_desc");
                        }

                        if (fieldDesc == null || fieldDesc.equals("")) {
                            ;
                        }

                        ControlDataStruct cds = null;
                        switch (controlType) {
                            case 1:
                            case 2:
                                if (fieldValue == null) {
                                    fieldValue = "";
                                }

                                if (fieldType == (long) 4 || fieldType == (long) 5 || fieldType == (long) 6) {
                                    if ((control.getDefaultType() != (long) 1 || !control.getDefaultValue().equals("0")) && control.getDefaultType() == (long) 1) {
                                        fieldValue = PublicLib.getField(fieldValue, ".", 0L);
                                    } else {
                                        fieldValue = PublicLib.getField(fieldValue, " ", 0L);
                                    }
                                }

                                cds = new ControlDataStruct();
                                cds.setCtrlName(control.getControlName());
                                cds.setCtrlType(99);
                                cds.setCtrlValue(fieldValue);
                                fcds.addControlData(cds);
                            case 3:
                            case 8:
                            case 11:
                            case 12:
                            default:
                                break;
                            case 4:
                            case 5:
                            case 6:
                            case 7:
                                String valueList = "";
                                String displayList = "";
                                String fieldDispValue;
                                if (((MyControlData) control).getFrom() == (long) 0) {
                                    valueList = ((MyControlData) control).getValueList();
                                    displayList = ((MyControlData) control).getDisplayList();
                                } else if (((MyControlData) control).getFrom() == (long) 1) {
                                    fieldDispValue = ((MyControlData) control).getDisplayField();
                                    String var60 = ((MyControlData) control).getValueField();
                                    String var61 = ((MyControlData) control).getTable();
                                    fieldDispValue = PublicLib.getField(fieldDispValue, "<", 0L);
                                    var60 = PublicLib.getField(var60, "<", 0L);
                                    var61 = PublicLib.getField(var61, "<", 0L);
                                    e = String.valueOf(String.valueOf((new StringBuffer("select ")).append(fieldDispValue).append(",").append(var60).append(" from ").append(var61).append(" where ").append(var60).append(" is not null")));

                                    String value;
                                    for (mapRs = this.baseTool.getServerRowSet(serverName, e, "", new SQLPara()); mapRs.next(); valueList = String.valueOf(String.valueOf(valueList)).concat(String.valueOf(String.valueOf(value)))) {
                                        String display = "";
                                        value = "";
                                        if (mapRs.getString(fieldDispValue) != null) {
                                            display = String.valueOf(String.valueOf(display)).concat(String.valueOf(String.valueOf(mapRs.getString(fieldDispValue))));
                                        }

                                        if (fieldType == (long) 3) {
                                            value = PublicLib.getNonZeroValue(mapRs.getString(var60));
                                        } else {
                                            value = mapRs.getString(var60);
                                        }

                                        if (!valueList.equals("")) {
                                            displayList = String.valueOf(String.valueOf(displayList)).concat(";");
                                            valueList = String.valueOf(String.valueOf(valueList)).concat(";");
                                        }

                                        displayList = String.valueOf(String.valueOf(displayList)).concat(String.valueOf(String.valueOf(display)));
                                    }
                                }

                                fieldDispValue = this.getFieldDisplay(valueList, displayList, fieldValue);
                                cds = new ControlDataStruct();
                                cds.setCtrlName(control.getControlName());
                                cds.setCtrlType(3);
                                cds.setCtrlValue(fieldValue);
                                cds.setCtrlDispValue(fieldDispValue);
                                fcds.addControlData(cds);
                                break;
                            case 9:
                                RecordSet someInfoRs = new RecordSet();
                                someInfoRs.insertRow();
                                fieldName = ((MySign) control).getTag();
                                this._$13773(false, (MySign) control, String.valueOf(String.valueOf(aFlowInid)).concat(""), String.valueOf(String.valueOf(aStepInid)).concat(""), dealIndx, someInfoRs, 1);
                                cds = new ControlDataStruct();
                                cds.setCtrlName(control.getControlName());
                                cds.setCtrlType(2);
                                cds.setCtrlValue("");
                                cds.setCtrlDispValue("");
                                cds.setSignList((SignDataStruct[]) someInfoRs.getObject("_SN_REE_L_RN_TAG__"));
                                fcds.addControlData(cds);
                                break;
                            case 10:
                                StringBuffer realValue = new StringBuffer(fieldValue);
                                fieldValue = this.restoreMapDisplay((MyMap) control, serverName, realValue);
                                fieldValue = PublicLib.getField(fieldValue, "\"", 1L);
                                cds = new ControlDataStruct();
                                cds.setCtrlName(control.getControlName());
                                cds.setCtrlType(1);
                                cds.setCtrlValue(realValue.substring(0));
                                cds.setCtrlDispValue(fieldValue);
                                fcds.addControlData(cds);
                        }
                    }

                    return fcds;
                }

                System.err.println("读取表单控件数据时失败。");
                fsetId = fcds;
            } catch (Exception var54) {
                infomation = String.valueOf(String.valueOf((new StringBuffer("流程实例id=")).append(aFlowInid).append(",步骤实例id=").append(aStepInid).append(",登陆用户=").append(this.baseTool.getCurrentUser())));
                this.baseTool.error("FormControlDataStruct:".concat(String.valueOf(String.valueOf(infomation))), var54);
                throw new Exception(var54.getMessage());
            }
        } finally {
            PublicLib.closeCachedRowSet(tempRs);
            tempRs = null;
            PublicLib.closeCachedRowSet(flowRs);
            flowRs = null;
            PublicLib.closeCachedRowSet(formRs);
            formRs = null;
            PublicLib.closeCachedRowSet((CachedRowSet) formTempletRs);
            formTempletRs = null;
            PublicLib.closeCachedRowSet(mainRs);
            mainRs = null;
            PublicLib.closeCachedRowSet(mapRs);
            mapRs = null;
            PublicLib.closeCachedRowSet((CachedRowSet) dealInfoRs);
            dealInfoRs = null;
            PublicLib.closeCachedRowSet((CachedRowSet) frameRs);
            frameRs = null;
            PublicLib.closeCachedRowSet((CachedRowSet) attachRs);
            attachRs = null;
        }

        return fsetId;
    }

    static {
        _$13453.put("signFirstTd1", "border-top:#333366 1px solid;border-left:#333366 1px solid;border-bottom:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signFirstTd2", "border-top:#333366 1px solid;border-left:#333366 1px solid;border-bottom:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signFirstTd3", "border-top:#333366 1px solid;border-left:#333366 1px solid;border-right:#333366 1px solid;border-bottom:#333366 1px solid;");
        _$13453.put("signFirstTd1_1", "border-top:#333366 1px solid;border-left:#333366 1px solid;border-bottom:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signFirstTd2_1", "border-top:#333366 1px solid;border-left:#333366 1px solid;border-right:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signFirstTd3_1", "border-left:#333366 1px solid;border-right:#333366 1px solid;border-bottom:#333366 1px solid;text-align:right;WORD-WRAP: break-word;");
        _$13453.put("signTd1", "border-left:#333366 1px solid;border-bottom:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signTd2", "border-left:#333366 1px solid;border-bottom:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signTd3", "border-left:#333366 1px solid;border-right:#333366 1px solid;border-bottom:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signTd1_1", "border-left:#333366 1px solid;border-bottom:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signTd2_1", "border-left:#333366 1px solid;border-right:#333366 1px solid;WORD-WRAP: break-word;");
        _$13453.put("signTd3_1", "border-left:#333366 1px solid;border-right:#333366 1px solid;border-bottom:#333366 1px solid;text-align:right;WORD-WRAP: break-word;");
    }
}


package com.riversql.actions;

import java.sql.SQLException;
import java.util.List;

import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.riversql.dbtree.*;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.riversql.IDManager;
import com.riversql.JSONAction;
import com.riversql.JSONDispatchAction;
import com.riversql.sql.SQLConnection;




public class DatabaseInfo extends JSONDispatchAction {

    String id=null;
    String catalogName;
    String schemaName;
    String tableName;

    public void setId(String id) {
        this.id = id;
    }

    public void setCatalogName(String catalogName) {
        this.catalogName = catalogName;
    }

    public void setTableName(String tableName) {
        this.tableName = tableName;
    }

    public void setSchemaName(String schemaName) {
        this.schemaName = schemaName;
    }

    public JSONObject getTables(HttpServletRequest request,
            HttpServletResponse response, EntityManager em, EntityTransaction et)
            throws Exception {
        SQLSession sqlsession=(SQLSession)IDManager.get().get(id);
        SQLConnection conn = sqlsession.getConn();

        TablesNode tablesNode = getTablesNode(conn);
        JSONArray tablesNameList = getTablesName(tablesNode);

        JSONObject ret=new JSONObject();
        ret.put("tables",tablesNameList);
        return ret;
    }

    public JSONObject getColumns(HttpServletRequest request,
            HttpServletResponse response, EntityManager em, EntityTransaction et)
            throws Exception {
        SQLSession sqlsession=(SQLSession)IDManager.get().get(id);
        SQLConnection conn = sqlsession.getConn();

        TablesNode tablesNode = getTablesNode(conn);
        TableNode tableNode = getTableNode(tablesNode);
        List<String> cols = tableNode.getColumnNames();
        JSONArray arr=new JSONArray();
        for(int i=0;i<cols.size();i++){
            //arr.put(cols.get(i));
            JSONObject obj=new JSONObject();
            obj.put("cname",cols.get(i));
            arr.put(obj);
        }
        JSONObject ret=new JSONObject();
        ret.put("columns",arr);
        return ret;
    }

    private TablesNode getTablesNode(SQLConnection conn) throws SQLException
    {
        DBNode dbNode = catalogName!=null?new CatalogNode( catalogName, conn, false):new SchemaNode(schemaName,conn);
        dbNode.nodeLoad();


        List<IStructureNode> tablesList = dbNode.getChildren();
        TablesNode tablesNode = null;

        for(int i=0;i<tablesList.size();i++)
        {
            //System.out.println("Tables:"+tablesList.get(i).getName()+tablesList.get(i).getQualifiedName()+tablesList.get(i).getType());
            if(tablesList.get(i).getName().equalsIgnoreCase("TABLE"))
            {
                tablesNode = (TablesNode) tablesList.get(i);
            }
        }
        return tablesNode;
    }

    private TableNode getTableNode(TablesNode tablesNode) throws SQLException
    {
        tablesNode.nodeLoad();
        List<IStructureNode> tableList = tablesNode.getChildren();
        TableNode tableNode = null;

        for(int i=0;i<tableList.size();i++)
        {
            if(tableList.get(i).getName().equalsIgnoreCase(tableName))
            {
                tableNode = (TableNode) tableList.get(i);
            }
        }
        return tableNode;
    }

    private JSONArray getTablesName(TablesNode tablesNode) throws SQLException, JSONException
    {
        tablesNode.nodeLoad();
        List<IStructureNode> tableList = tablesNode.getChildren();
        JSONArray tablesNameList = new JSONArray();

        for(int i=0;i<tableList.size();i++)
        {
            JSONObject tablename=new JSONObject();
            tablename.put("tablename", tableList.get(i).getName());
            tablename.put("type",catalogName==null?"schema":"catalog");
            tablesNameList.put(tablename);
        }
        return tablesNameList;
    }
}

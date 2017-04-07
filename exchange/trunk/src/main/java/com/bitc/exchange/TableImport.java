package com.bitc.exchange;

import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationAware;
import com.bitc.exchange.model.Cell;
import com.bitc.exchange.model.Row;
import com.bitc.exchange.model.Table;
import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.ResultSetHandler;
import org.apache.commons.dbutils.handlers.ScalarHandler;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.math.BigDecimal;
import java.sql.*;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class TableImport implements ConfigurationAware {
    private Configuration configuration;
    private File file;
    private Table table;
    private File directory;
    private QueryRunner queryRunner = new QueryRunner();

    public TableImport(Configuration configuration, File file, File directory) {
        this.configuration = configuration;
        this.file = file;
        this.directory = directory;
    }

    public void inport() throws Exception {
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file));
        try {
            Connection connection = configuration.getConnection();
            this.table = (Table) ois.readObject();
            Log.LOGGER.info("start import table " + this.table.getName());
            String insertSql = table.getInsertSql();
            String existsCheckSql = table.getExistsCheckSql();
            String deleteSql = table.getDeleteSql();
            PreparedStatement existsPs = connection.prepareStatement(existsCheckSql);
            PreparedStatement deletePs = connection.prepareStatement(deleteSql);
            PreparedStatement insertPs = connection.prepareStatement(insertSql);
            for (int i = 0; i < table.getRowCount(); i++) {
                Row row = (Row) ois.readObject();
                Log.LOGGER.info("import row:" + row.getPkValue() + " \t index:" + i);

                switch (configuration.getExistsStrategy()) {
                    case SKIP:
                        boolean exists = isExists(existsPs, row.getPkValue());
                        if (exists) {
                            Log.LOGGER.info("skip row");
                            continue;
                        }
                        break;
                    case REPLACE:
                        deleteRow(deletePs, row.getPkValue());
                        Log.LOGGER.info("delete row:" + row.getPkValue());
                        break;
                }

                int j = 1;
                for (Cell cell : row) {
                    insertPs.setObject(j, cell.getValue());
                    j++;
                }

                insertPs.execute();

                Log.LOGGER.info("insert row:" + row.getPkValue());
                if (table.getBlobs().size() > 0) {
                    for (String blobColumn : table.getBlobs()) {
                        handBlob(row, blobColumn);
                        Log.LOGGER.info("handle blob column:" + blobColumn);
                    }
                }
            }
            Log.LOGGER.info("import table end.");
        } catch (Exception e) {
            throw e;
        } finally {
            ois.close();
            Log.LOGGER.info("close fileinputstream.");
        }
    }


    private boolean isExists(PreparedStatement existsPs, Object pkValue) throws SQLException {
        existsPs.setObject(1, pkValue);
        ResultSet rs = existsPs.executeQuery();
        rs.next();
        int count = rs.getInt(1);
        rs.close();
        return count > 0;
    }

    private void deleteRow(PreparedStatement deletePs, Object pkValue) throws SQLException {
        deletePs.setObject(1, pkValue);
        deletePs.execute();
    }

    private void handBlob(Row row, String blobColumn) throws Exception {
        File file = new File(this.directory.getPath() + File.separator + Configuration.BlobsDirectory + File.separator + row.getBlobFiles().get(blobColumn));
        if (!file.exists()) {
            Log.LOGGER.warning("file not found");
            return;
        }
        StringBuilder sql = new StringBuilder();
        sql.append("update ").append(table.getName()).append(" set ").append(blobColumn).append("=? ").append(" where ").append(table.getPk()).append("=?");
        PreparedStatement st = configuration.getConnection().prepareStatement(sql.toString());

        FileInputStream fis = new FileInputStream(file);
        byte[] bytes = FileUtils.readFileToByteArray(file);
        st.setBytes(1, bytes);
        st.setObject(2, row.getPkValue());
        st.execute();
        st.close();
        fis.close();
    }

    @Override
    public Configuration getConfiguration() {
        return this.configuration;
    }

    @Override
    public void setConfiguration(Configuration config) {
        this.configuration = config;
    }
}

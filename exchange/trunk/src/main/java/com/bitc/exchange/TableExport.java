package com.bitc.exchange;

import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationAware;
import com.bitc.exchange.cfg.ConfigurationException;
import com.bitc.exchange.handler.RowListHandler;
import com.bitc.exchange.model.Cell;
import com.bitc.exchange.model.Row;
import com.bitc.exchange.model.Table;
import com.bitc.exchange.util.UUIDUtils;
import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.ResultSetHandler;
import org.apache.commons.dbutils.handlers.AbstractListHandler;
import org.apache.commons.dbutils.handlers.ScalarHandler;
import org.apache.commons.io.IOUtils;

import java.io.*;
import java.math.BigDecimal;
import java.sql.*;
import java.util.List;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class TableExport implements ConfigurationAware {
    private Configuration configuration;
    private Table table;
    private File directory;

    public TableExport(Configuration configuration, Table table, File file) {
        this.configuration = configuration;
        this.table = table;
        this.directory = file;
    }

    public Table getTable() {
        return table;
    }

    public void setTable(Table table) {
        this.table = table;
    }

    public void export() throws Exception {
//        QueryRunner queryRunner = new QueryRunner();
        File file = new File(this.directory.getPath() + File.separator + table.getName());
        file.createNewFile();
        Log.LOGGER.info("create file " + file.getName());
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(file));
        Connection connection = configuration.getConnection();
//        BigDecimal count = queryRunner.query(connection, table.getCountSql(), new ScalarHandler<BigDecimal>(), (Object[]) null);
        PreparedStatement st = connection.prepareStatement(table.getCountSql());
        ResultSet crs = st.executeQuery();
        if (crs.next()) {
            table.setRowCount(crs.getBigDecimal(1).intValue());
        }
        oos.writeObject(table);
        PreparedStatement stmt = null;
        ResultSet resultSet = null;
        try {
            stmt = connection.prepareStatement(table.getSelectSql());
            stmt.setFetchSize(1000);
            resultSet = stmt.executeQuery();
            int i = 1;
            Row row;
            while (resultSet.next()) {
                row = new Row();
                Cell pkCell = new Cell(resultSet.getObject(table.getPk()));
                row.setPkValue(pkCell.getValue());
                row.addCell(pkCell);
                for (String column : table.getColumns()) {
                    Cell cell = new Cell(resultSet.getObject(column));
                    row.addCell(cell);
                }
                if (table.getBlobs().size() > 0) {
                    for (String blobColumn : table.getBlobs()) {
                        String outputFileName = handBlob(row, blobColumn);
                        if (outputFileName != null) {
                            row.addBlobFile(blobColumn, outputFileName);
                        }
                    }
                }
                oos.writeObject(row);
                if (i++ % 1000 == 0) {
                    oos.reset();
                    oos.flush();
                    Log.LOGGER.info("read row " + pkCell.getValue() + " index :" + i);
                }
            }
        } catch (SQLException e) {
            throw e;
        } finally {
            oos.flush();
            oos.close();
        }
    }

    public String handBlob(Row row, String blobColumn) throws SQLException, ConfigurationException {
        final String fileName = UUIDUtils.nextCode();
        StringBuilder sql = new StringBuilder();
        Log.LOGGER.info("export blob file " + blobColumn + "->" + fileName);
        sql.append("select ").append(blobColumn).append(" from ").append(table.getName())
                .append(" where ").append(table.getPk()).append("=?");
        QueryRunner queryRunner = new QueryRunner();
        Boolean result = queryRunner.query(configuration.getConnection(), sql.toString(), new ResultSetHandler<Boolean>() {
            @Override
            public Boolean handle(ResultSet rs) throws SQLException {
                FileOutputStream fos = null;
                if (rs.next()) {
                    Blob blob = rs.getBlob(1);
                    if (blob == null) {
                        Log.LOGGER.warning("blob null!");
                        return false;
                    }
                    File dic = new File(directory.getPath() + File.separator + Configuration.BlobsDirectory);
                    if (!dic.exists()) {
                        dic.mkdir();
                    }
                    File blobFile = new File(directory.getPath() + File.separator + Configuration.BlobsDirectory + File.separator + fileName);
                    try {
                        blobFile.createNewFile();
                        fos = new FileOutputStream(blobFile);
                        InputStream is = blob.getBinaryStream();
                        IOUtils.copy(is, fos);
                        IOUtils.closeQuietly(is);
                    } catch (IOException e) {
                        throw new SQLException(e);
                    } finally {
                        try {
                            fos.flush();
                            fos.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
                return true;
            }
        }, row.getPkValue());
        Log.LOGGER.info("export blob file success");
        return result ? fileName : null;
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

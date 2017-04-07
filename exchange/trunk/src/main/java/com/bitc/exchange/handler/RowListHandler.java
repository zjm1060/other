package com.bitc.exchange.handler;

import com.bitc.exchange.Log;
import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.model.Cell;
import com.bitc.exchange.model.Row;
import com.bitc.exchange.model.Table;
import com.bitc.exchange.util.UUIDUtils;
import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.ResultSetHandler;
import org.apache.commons.dbutils.handlers.AbstractListHandler;
import org.apache.commons.io.IOUtils;

import java.io.*;
import java.sql.Blob;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * Created by BomWu on 2015/6/8 0008.
 */
public class RowListHandler extends AbstractListHandler<Row> {
    private Connection connection;
    private ObjectOutputStream oos;
    private Table table;
    private File directory;

    public RowListHandler(Connection connection, Table table, ObjectOutputStream oos, File directory) {
        this.connection = connection;
        this.table = table;
        this.oos = oos;
        this.directory = directory;
    }

    @Override
    public Row handleRow(ResultSet resultSet) throws SQLException {
        Row row = new Row();
        Cell pkCell = new Cell(resultSet.getObject(table.getPk()));
        row.setPkValue(pkCell.getValue());
        Log.LOGGER.info("read row " + pkCell.getValue());
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
        try {
            this.oos.writeObject(row);
            this.oos.flush();
//            System.out.println(row);
        } catch (Exception e) {
            throw new SQLException(e);
        }
        return row;
    }

    public String handBlob(Row row, String blobColumn) throws SQLException {
        final String fileName = UUIDUtils.nextCode();
        StringBuilder sql = new StringBuilder();
        Log.LOGGER.info("export blob file " + blobColumn + "->" + fileName);
        sql.append("select ").append(blobColumn).append(" from ").append(table.getName())
                .append(" where ").append(table.getPk()).append("=?");
        QueryRunner queryRunner = new QueryRunner();
        Boolean result = queryRunner.query(connection, sql.toString(), new ResultSetHandler<Boolean>() {
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
}

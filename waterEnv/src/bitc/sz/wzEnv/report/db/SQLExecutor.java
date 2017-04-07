package bitc.sz.wzEnv.report.db;

import org.apache.log4j.Logger;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Arrays;

public class SQLExecutor {

    private static final Logger logger = Logger.getLogger(SQLExecutor.class);
    private String dbname;

    private Connection connection;

    public SQLExecutor(String dbName) throws SQLException {
        this.dbname = dbName;
    }

    public ResultSet executeQuery(String sql) throws SQLException {
        return executeQuery(sql, null);
    }

    public ResultSet executeQuery(String sql, Object[] values)
            throws SQLException {
        if (logger.isDebugEnabled()) {
            logger.debug("execute sql:" + sql);
            logger.debug("params:" + Arrays.toString(values));
        }
        PreparedStatement ps = getConnection().prepareStatement(sql);
        if (values != null) {
            for (int i = 1; i <= values.length; i++) {
                ps.setObject(i, values[i - 1]);
            }
        }
        return ps.executeQuery();
    }

    public void execute(String sql, Object[] values) throws SQLException {
        if (logger.isDebugEnabled()) {
            logger.debug("execute sql:" + sql);
            logger.debug("params:" + Arrays.toString(values));
        }
        PreparedStatement ps = getConnection().prepareStatement(sql);
        if (values != null) {
            for (int i = 1; i <= values.length; i++) {
                ps.setObject(i, values[i - 1]);
            }
        }
        ps.execute();
        ps.close();
    }

    public <T> T execute(Executable<T> executable) throws SQLException {
        T object = null;
        Connection connection = getConnection();
        connection.setAutoCommit(false);
        try {
            object = executable.execute(connection);
            connection.commit();
            return object;
        } catch (SQLException e) {
            connection.rollback();
            throw e;
        } finally {
            this.close();
        }
    }

    public void commit() throws SQLException {
        getConnection().commit();
    }

    public void rollback() throws SQLException {
        getConnection().rollback();
    }

    public void close() {
        try {
            if (connection != null && !connection.isClosed()) {
                connection.close();
            }
            this.connection = null;
        } catch (SQLException e) {
        }
    }

    public String getDbname() {
        return dbname;
    }

    public void setDbname(String dbname) {
        this.dbname = dbname;
    }

    public Connection getConnection() throws SQLException {
        if (connection == null) {
            connection = DBConnector.getConnection(this.dbname);
        }
        return connection;
    }
}

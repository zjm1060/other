package bitc.sz.wzEnv.report.db;

import java.sql.Connection;
import java.sql.SQLException;

public interface Executable<T> {
    T execute(Connection connection) throws SQLException;
}

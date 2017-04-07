package bitc.sz.wzEnv.util;

import java.util.List;

/**
 * Created by zsz on 2015/11/19.
 */
public class ExtjsStoreResponse extends  ExtjsResponse {
    private List<?> rows;
    private long total;

    public ExtjsStoreResponse() {
        setSuccess(true);
    }

    public List<?> getRows() {
        return rows;
    }

    public void setRows(List<?> rows) {
        this.rows = rows;
    }

    public long getTotal() {
        return total;
    }

    public void setTotal(long total) {
        this.total = total;
    }
}

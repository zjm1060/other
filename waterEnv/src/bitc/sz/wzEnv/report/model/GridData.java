package bitc.sz.wzEnv.report.model;

import bitc.sz.wzEnv.util.ExtjsResponse;

import java.io.Serializable;
import java.util.List;
import java.util.Map;

public class GridData extends ExtjsResponse {
    private static final long serialVersionUID = 6901480102208128141L;
    private List<Map<String, String>> rows;
    private int total = 0;

    public GridData() {
        setSuccess(true);
    }

    public List<Map<String, String>> getRows() {
        return rows;
    }

    public void setRows(List<Map<String, String>> rows) {
        this.rows = rows;
    }
    public int getTotal() {
        return total;
    }

    public void setTotal(int total) {
        this.total = total;
    }
}

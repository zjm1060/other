package bitc.sz.wzEnv.util;

/**
 * Created by BomWu on 2015/11/23 0023.
 */
public class FormLoadResponse extends ExtjsResponse {
    private Object data;

    public FormLoadResponse(Object data) {
        super(true);
        this.data = data;
    }

    public Object getData() {
        return data;
    }

    public void setData(Object data) {
        this.data = data;
    }
}

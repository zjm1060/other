package bitc.sz.wzEnv.szpj.model;

/**
 * Created by Administrator on 2016/3/10.
 */
public class ZbValue {
    private String ITEM_ID;
    private String SYS_ID;
    private String ITEM_NAME;
    private String ITEM_VALUE;
    private String remark;

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public String getITEM_ID() {
        return ITEM_ID;
    }

    public void setITEM_ID(String ITEM_ID) {
        this.ITEM_ID = ITEM_ID;
    }

    public String getSYS_ID() {
        return SYS_ID;
    }

    public void setSYS_ID(String SYS_ID) {
        this.SYS_ID = SYS_ID;
    }

    public String getITEM_NAME() {
        return ITEM_NAME;
    }

    public void setITEM_NAME(String ITEM_NAME) {
        this.ITEM_NAME = ITEM_NAME;
    }

    public String getITEM_VALUE() {
        return ITEM_VALUE;
    }

    public void setITEM_VALUE(String ITEM_VALUE) {
        this.ITEM_VALUE = ITEM_VALUE;
    }
}

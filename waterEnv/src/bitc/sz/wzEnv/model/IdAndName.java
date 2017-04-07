package bitc.sz.wzEnv.model;

/**
 * Created by BomWu on 2015/12/9 0009.
 */
public class IdAndName {
    private String id;
    private String name;

    public IdAndName() {
    }

    public IdAndName(String id, String name) {
        this.id = id;
        this.name = name;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}

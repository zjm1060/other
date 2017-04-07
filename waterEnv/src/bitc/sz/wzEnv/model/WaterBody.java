package bitc.sz.wzEnv.model;

/**
 * Created by zsz on 2015/12/25.
 */
public class WaterBody {
    private String id;
    private String name;
    private String mapId;
    private String superObjId;
    private String icoFlag;
    private String typeIDs;

    public String getMapId() {
        return mapId;
    }

    public void setMapId(String mapId) {
        this.mapId = mapId;
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

    public String getSuperObjId() {
        return superObjId;
    }

    public void setSuperObjId(String superObjId) {
        this.superObjId = superObjId;
    }

    public String getIcoFlag() {
        return icoFlag;
    }

    public void setIcoFlag(String icoFlag) {
        this.icoFlag = icoFlag;
    }

    public String getTypeIDs() {
        return typeIDs;
    }

    public void setTypeIDs(String typeIDs) {
        this.typeIDs = typeIDs;
    }
}

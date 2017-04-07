package bitc.sz.wzEnv.model;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/1/19.
 */
public class ShuiZiYuanBody2 {
    private String id;
    private String name;
    private String location;
    private String mapId;
    private String superObjId;
    private String icoFlag;
    private String typeIDs;
    private List<Map<String,String>> attributes = new ArrayList<>();

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

    public List<Map<String, String>> getAttributes() {
        return attributes;
    }

    public void setAttributes(List<Map<String, String>> attributes) {
        this.attributes = attributes;
    }

    public void addAttributes(Map map){
        this.attributes.add(map);
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public String getMapId() {
        return mapId;
    }

    public void setMapId(String mapId) {
        this.mapId = mapId;
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

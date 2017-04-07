package com.bitc.exchange.model;

import com.bitc.exchange.util.JsonUtil;

import java.io.Serializable;
import java.util.*;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class Row implements Serializable, Iterable<Cell> {
    private static final long serialVersionUID = 1L;
    private Object pkValue;
    private List<Cell> cells;
    private Map<String, String> blobFiles;

    public Row() {
        this.cells = new ArrayList<Cell>();
        this.blobFiles = new HashMap<String, String>();
    }

    public void addCell(Cell cell) {
        this.cells.add(cell);
    }

    @Override
    public Iterator<Cell> iterator() {
        return cells.iterator();
    }

    public Object getPkValue() {
        return pkValue;
    }

    public void setPkValue(Object pkValue) {
        this.pkValue = pkValue;
    }

    public Map<String, String> getBlobFiles() {
        return blobFiles;
    }

    public void setBlobFiles(Map<String, String> blobFiles) {
        this.blobFiles = blobFiles;
    }

    public List<Cell> getCells() {
        return cells;
    }

    public void setCells(List<Cell> cells) {
        this.cells = cells;
    }

    @Override
    public String toString() {
        return JsonUtil.getJsonString(this);
    }

    public void addBlobFile(String columnName, String outputFileName) {
        this.blobFiles.put(columnName, outputFileName);
    }
}

package com.bitc.exchange.model;

import com.bitc.exchange.util.JsonUtil;

import java.io.Serializable;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class Cell implements Serializable {
    private static final long serialVersionUID = 1L;

    private Object value;

    public Cell(Object value) {
        this.value = value;
    }

    public Object getValue() {
        return value;
    }

    public void setValue(Object value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return JsonUtil.getJsonString(this);
    }
}

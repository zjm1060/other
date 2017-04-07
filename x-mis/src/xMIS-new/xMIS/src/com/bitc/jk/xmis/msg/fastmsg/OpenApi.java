/**
 * OpenApi.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Apr 22, 2006 (06:55:48 PDT) WSDL2Java emitter.
 */

package com.bitc.jk.xmis.msg.fastmsg;

public interface OpenApi extends javax.xml.rpc.Service {
    public java.lang.String getOpenApiPortAddress();

    public OpenApiPortType getOpenApiPort() throws javax.xml.rpc.ServiceException;

    public OpenApiPortType getOpenApiPort(java.net.URL portAddress) throws javax.xml.rpc.ServiceException;
}

/**
 * WoixService.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Apr 22, 2006 (06:55:48 PDT) WSDL2Java emitter.
 */

package com.bitc.woix.client;

public interface WoixService extends javax.xml.rpc.Service {
    public java.lang.String getWoixServiceSoapAddress();

    public com.bitc.woix.client.WoixServiceSoap getWoixServiceSoap() throws javax.xml.rpc.ServiceException;

    public com.bitc.woix.client.WoixServiceSoap getWoixServiceSoap(java.net.URL portAddress) throws javax.xml.rpc.ServiceException;
}

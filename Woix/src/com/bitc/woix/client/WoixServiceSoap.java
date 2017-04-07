/**
 * WoixServiceSoap.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Apr 22, 2006 (06:55:48 PDT) WSDL2Java emitter.
 */

package com.bitc.woix.client;

public interface WoixServiceSoap extends java.rmi.Remote {
    public java.lang.String getAlarmJson(java.lang.String start, java.lang.String end, int startInx, int endInx, java.lang.String searchStr, java.lang.String sort) throws java.rmi.RemoteException;
    public java.lang.String getQuickAlarmJson(int count) throws java.rmi.RemoteException;
    public double getCopyFileSpan(java.util.Calendar date) throws java.rmi.RemoteException;
    public java.lang.String getSetupXml() throws java.rmi.RemoteException;
    public java.lang.String getPointTableJson(int stnId, int grpId, int typeId, int start, int end) throws java.rmi.RemoteException;
    public java.lang.String getConfigValue(java.lang.String paramName) throws java.rmi.RemoteException;
    public java.lang.String helloWorld() throws java.rmi.RemoteException;
    public java.lang.String helloWorldDateTime(java.util.Calendar time) throws java.rmi.RemoteException;
    public java.lang.Object getPointResultByType(java.lang.String pointName, org.apache.axis.types.UnsignedByte type) throws java.rmi.RemoteException;
    public java.lang.String getPointResultByXML(java.lang.String requestXML) throws java.rmi.RemoteException;
    public java.lang.String loadSvg(java.lang.String displayName) throws java.rmi.RemoteException;
}

package com.bitc.woix.client;

public class WoixServiceSoapProxy implements com.bitc.woix.client.WoixServiceSoap {
  private String _endpoint = null;
  private com.bitc.woix.client.WoixServiceSoap woixServiceSoap = null;
  
  public WoixServiceSoapProxy() {
    _initWoixServiceSoapProxy();
  }
  
  public WoixServiceSoapProxy(String endpoint) {
    _endpoint = endpoint;
    _initWoixServiceSoapProxy();
  }
  
  private void _initWoixServiceSoapProxy() {
    try {
      woixServiceSoap = (new com.bitc.woix.client.WoixServiceLocator()).getWoixServiceSoap();
      if (woixServiceSoap != null) {
        if (_endpoint != null)
          ((javax.xml.rpc.Stub)woixServiceSoap)._setProperty("javax.xml.rpc.service.endpoint.address", _endpoint);
        else
          _endpoint = (String)((javax.xml.rpc.Stub)woixServiceSoap)._getProperty("javax.xml.rpc.service.endpoint.address");
      }
      
    }
    catch (javax.xml.rpc.ServiceException serviceException) {}
  }
  
  public String getEndpoint() {
    return _endpoint;
  }
  
  public void setEndpoint(String endpoint) {
    _endpoint = endpoint;
    if (woixServiceSoap != null)
      ((javax.xml.rpc.Stub)woixServiceSoap)._setProperty("javax.xml.rpc.service.endpoint.address", _endpoint);
    
  }
  
  public com.bitc.woix.client.WoixServiceSoap getWoixServiceSoap() {
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap;
  }
  
  public java.lang.String getAlarmJson(java.lang.String start, java.lang.String end, int startInx, int endInx, java.lang.String searchStr, java.lang.String sort) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.getAlarmJson(start, end, startInx, endInx, searchStr, sort);
  }
  
  public java.lang.String getQuickAlarmJson(int count) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.getQuickAlarmJson(count);
  }
  
  public double getCopyFileSpan(java.util.Calendar date) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.getCopyFileSpan(date);
  }
  
  public java.lang.String getSetupXml() throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.getSetupXml();
  }
  
  public java.lang.String getPointTableJson(int stnId, int grpId, int typeId, int start, int end) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.getPointTableJson(stnId, grpId, typeId, start, end);
  }
  
  public java.lang.String getConfigValue(java.lang.String paramName) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.getConfigValue(paramName);
  }
  
  public java.lang.String helloWorld() throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.helloWorld();
  }
  
  public java.lang.String helloWorldDateTime(java.util.Calendar time) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.helloWorldDateTime(time);
  }
  
  public java.lang.Object getPointResultByType(java.lang.String pointName, org.apache.axis.types.UnsignedByte type) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.getPointResultByType(pointName, type);
  }
  
  public java.lang.String getPointResultByXML(java.lang.String requestXML) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.getPointResultByXML(requestXML);
  }
  
  public java.lang.String loadSvg(java.lang.String displayName) throws java.rmi.RemoteException{
    if (woixServiceSoap == null)
      _initWoixServiceSoapProxy();
    return woixServiceSoap.loadSvg(displayName);
  }
  
  
}
/**
 * OpenApiLocator.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Apr 22, 2006 (06:55:48 PDT) WSDL2Java emitter.
 */

package com.bitc.jk.xmis.msg.fastmsg;

public class OpenApiLocator extends org.apache.axis.client.Service implements OpenApi {

    public OpenApiLocator() {
    }


    public OpenApiLocator(org.apache.axis.EngineConfiguration config) {
        super(config);
    }

    public OpenApiLocator(java.lang.String wsdlLoc, javax.xml.namespace.QName sName) throws javax.xml.rpc.ServiceException {
        super(wsdlLoc, sName);
    }

    // Use to get a proxy class for OpenApiPort
    private java.lang.String OpenApiPort_address = "http://xmis.bitc.net.cn:5880/openapi/openapi.php";

    public java.lang.String getOpenApiPortAddress() {
        return OpenApiPort_address;
    }

    // The WSDD service name defaults to the port name.
    private java.lang.String OpenApiPortWSDDServiceName = "OpenApiPort";

    public java.lang.String getOpenApiPortWSDDServiceName() {
        return OpenApiPortWSDDServiceName;
    }

    public void setOpenApiPortWSDDServiceName(java.lang.String name) {
        OpenApiPortWSDDServiceName = name;
    }

    public OpenApiPortType getOpenApiPort() throws javax.xml.rpc.ServiceException {
       java.net.URL endpoint;
        try {
            endpoint = new java.net.URL(OpenApiPort_address);
        }
        catch (java.net.MalformedURLException e) {
            throw new javax.xml.rpc.ServiceException(e);
        }
        return getOpenApiPort(endpoint);
    }

    public OpenApiPortType getOpenApiPort(java.net.URL portAddress) throws javax.xml.rpc.ServiceException {
        try {
            OpenApiBindingStub _stub = new OpenApiBindingStub(portAddress, this);
            _stub.setPortName(getOpenApiPortWSDDServiceName());
            return _stub;
        }
        catch (org.apache.axis.AxisFault e) {
            return null;
        }
    }

    public void setOpenApiPortEndpointAddress(java.lang.String address) {
        OpenApiPort_address = address;
    }

    /**
     * For the given interface, get the stub implementation.
     * If this service has no port for the given interface,
     * then ServiceException is thrown.
     */
    public java.rmi.Remote getPort(Class serviceEndpointInterface) throws javax.xml.rpc.ServiceException {
        try {
            if (OpenApiPortType.class.isAssignableFrom(serviceEndpointInterface)) {
                OpenApiBindingStub _stub = new OpenApiBindingStub(new java.net.URL(OpenApiPort_address), this);
                _stub.setPortName(getOpenApiPortWSDDServiceName());
                return _stub;
            }
        }
        catch (java.lang.Throwable t) {
            throw new javax.xml.rpc.ServiceException(t);
        }
        throw new javax.xml.rpc.ServiceException("There is no stub implementation for the interface:  " + (serviceEndpointInterface == null ? "null" : serviceEndpointInterface.getName()));
    }

    /**
     * For the given interface, get the stub implementation.
     * If this service has no port for the given interface,
     * then ServiceException is thrown.
     */
    public java.rmi.Remote getPort(javax.xml.namespace.QName portName, Class serviceEndpointInterface) throws javax.xml.rpc.ServiceException {
        if (portName == null) {
            return getPort(serviceEndpointInterface);
        }
        java.lang.String inputPortName = portName.getLocalPart();
        if ("OpenApiPort".equals(inputPortName)) {
            return getOpenApiPort();
        }
        else  {
            java.rmi.Remote _stub = getPort(serviceEndpointInterface);
            ((org.apache.axis.client.Stub) _stub).setPortName(portName);
            return _stub;
        }
    }

    public javax.xml.namespace.QName getServiceName() {
        return new javax.xml.namespace.QName("urn:OpenApi", "OpenApi");
    }

    private java.util.HashSet ports = null;

    public java.util.Iterator getPorts() {
        if (ports == null) {
            ports = new java.util.HashSet();
            ports.add(new javax.xml.namespace.QName("urn:OpenApi", "OpenApiPort"));
        }
        return ports.iterator();
    }

    /**
    * Set the endpoint address for the specified port name.
    */
    public void setEndpointAddress(java.lang.String portName, java.lang.String address) throws javax.xml.rpc.ServiceException {
        
if ("OpenApiPort".equals(portName)) {
            setOpenApiPortEndpointAddress(address);
        }
        else 
{ // Unknown Port Name
            throw new javax.xml.rpc.ServiceException(" Cannot set Endpoint Address for Unknown Port" + portName);
        }
    }

    /**
    * Set the endpoint address for the specified port name.
    */
    public void setEndpointAddress(javax.xml.namespace.QName portName, java.lang.String address) throws javax.xml.rpc.ServiceException {
        setEndpointAddress(portName.getLocalPart(), address);
    }

}

/**
 * OpenApiPortType.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Apr 22, 2006 (06:55:48 PDT) WSDL2Java emitter.
 */

package com.bitc.jk.xmis.msg.fastmsg;

public interface OpenApiPortType extends java.rmi.Remote {
    public int addDepartment(java.lang.String key, int departid, java.lang.String departname, int ordervalue, int parentdepartid) throws java.rmi.RemoteException;
    public int removeDepartment(java.lang.String key, int departid) throws java.rmi.RemoteException;
    public int modifyDepartment(java.lang.String key, int departid, java.lang.String departname) throws java.rmi.RemoteException;
    public int setDepartmentOrder(java.lang.String key, int departid, int ordervalue) throws java.rmi.RemoteException;
    public int addUser(java.lang.String key, int userid, java.lang.String username, int usertype, int canfindbypublicusers, java.lang.String nickname, java.lang.String actualname, int sex, int age, int birth_year, int birth_month, int birth_day, int province, int city, java.lang.String address, java.lang.String telephone, java.lang.String mobile, java.lang.String fax, java.lang.String qq, java.lang.String msn, java.lang.String email, java.lang.String homepage, int departid, java.lang.String departname, java.lang.String jobtitle, java.lang.String jobnumber, java.lang.String introduction, java.lang.String password, java.lang.String salt) throws java.rmi.RemoteException;
    public int removeUser(java.lang.String key, java.lang.String useridorname) throws java.rmi.RemoteException;
    public int modifyUser(java.lang.String key, java.lang.String useridorname, int canfindbypublicusers, java.lang.String nickname, java.lang.String actualname, int sex, int age, int birth_year, int birth_month, int birth_day, int province, int city, java.lang.String address, java.lang.String telephone, java.lang.String mobile, java.lang.String fax, java.lang.String qq, java.lang.String msn, java.lang.String email, java.lang.String homepage, int departid, java.lang.String departname, java.lang.String jobtitle, java.lang.String jobnumber, java.lang.String introduction) throws java.rmi.RemoteException;
    public UserInfo getUser(java.lang.String key, java.lang.String useridorname) throws java.rmi.RemoteException;
    public int changePassword(java.lang.String key, java.lang.String useridorname, java.lang.String password) throws java.rmi.RemoteException;
    public UserInfo validateToken(java.lang.String key, java.lang.String token) throws java.rmi.RemoteException;
    public int sendMessage(java.lang.String key, java.lang.String from, java.lang.String sendto, java.lang.String content) throws java.rmi.RemoteException;
    public int sendSysMsg(java.lang.String key, int forusertype, java.lang.String title, java.lang.String content, java.lang.String link, java.lang.String sendto, int online) throws java.rmi.RemoteException;
}

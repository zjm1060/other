/**
 * UserInfo.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Apr 22, 2006 (06:55:48 PDT) WSDL2Java emitter.
 */

package com.bitc.jk.xmis.msg.fastmsg;

public class UserInfo  implements java.io.Serializable {
    private int userid;

    private java.lang.String username;

    private int usertype;

    private int canfindbypublicusers;

    private java.lang.String nickname;

    private java.lang.String signature;

    private java.lang.String actualname;

    private java.lang.String sex;

    private java.lang.String age;

    private int birth_year;

    private int birth_month;

    private int birth_day;

    private java.lang.String country;

    private java.lang.String province;

    private java.lang.String city;

    private java.lang.String area;

    private java.lang.String address;

    private java.lang.String telephone;

    private java.lang.String mobile;

    private java.lang.String fax;

    private java.lang.String qq;

    private java.lang.String msn;

    private java.lang.String email;

    private java.lang.String homepage;

    private int departid;

    private java.lang.String departname;

    private java.lang.String jobtitle;

    private java.lang.String jobnumber;

    private java.lang.String introduction;

    private java.lang.String facefile;

    private java.lang.String photofile;

    private int loginstatus;

    private java.lang.String loginstatustext;

    private int hascamera;

    private int hasmic;

    private int vip;

    private int onlinelevel;

    private int integral;

    public UserInfo() {
    }

    public UserInfo(
           int userid,
           java.lang.String username,
           int usertype,
           int canfindbypublicusers,
           java.lang.String nickname,
           java.lang.String signature,
           java.lang.String actualname,
           java.lang.String sex,
           java.lang.String age,
           int birth_year,
           int birth_month,
           int birth_day,
           java.lang.String country,
           java.lang.String province,
           java.lang.String city,
           java.lang.String area,
           java.lang.String address,
           java.lang.String telephone,
           java.lang.String mobile,
           java.lang.String fax,
           java.lang.String qq,
           java.lang.String msn,
           java.lang.String email,
           java.lang.String homepage,
           int departid,
           java.lang.String departname,
           java.lang.String jobtitle,
           java.lang.String jobnumber,
           java.lang.String introduction,
           java.lang.String facefile,
           java.lang.String photofile,
           int loginstatus,
           java.lang.String loginstatustext,
           int hascamera,
           int hasmic,
           int vip,
           int onlinelevel,
           int integral) {
           this.userid = userid;
           this.username = username;
           this.usertype = usertype;
           this.canfindbypublicusers = canfindbypublicusers;
           this.nickname = nickname;
           this.signature = signature;
           this.actualname = actualname;
           this.sex = sex;
           this.age = age;
           this.birth_year = birth_year;
           this.birth_month = birth_month;
           this.birth_day = birth_day;
           this.country = country;
           this.province = province;
           this.city = city;
           this.area = area;
           this.address = address;
           this.telephone = telephone;
           this.mobile = mobile;
           this.fax = fax;
           this.qq = qq;
           this.msn = msn;
           this.email = email;
           this.homepage = homepage;
           this.departid = departid;
           this.departname = departname;
           this.jobtitle = jobtitle;
           this.jobnumber = jobnumber;
           this.introduction = introduction;
           this.facefile = facefile;
           this.photofile = photofile;
           this.loginstatus = loginstatus;
           this.loginstatustext = loginstatustext;
           this.hascamera = hascamera;
           this.hasmic = hasmic;
           this.vip = vip;
           this.onlinelevel = onlinelevel;
           this.integral = integral;
    }


    /**
     * Gets the userid value for this UserInfo.
     * 
     * @return userid
     */
    public int getUserid() {
        return userid;
    }


    /**
     * Sets the userid value for this UserInfo.
     * 
     * @param userid
     */
    public void setUserid(int userid) {
        this.userid = userid;
    }


    /**
     * Gets the username value for this UserInfo.
     * 
     * @return username
     */
    public java.lang.String getUsername() {
        return username;
    }


    /**
     * Sets the username value for this UserInfo.
     * 
     * @param username
     */
    public void setUsername(java.lang.String username) {
        this.username = username;
    }


    /**
     * Gets the usertype value for this UserInfo.
     * 
     * @return usertype
     */
    public int getUsertype() {
        return usertype;
    }


    /**
     * Sets the usertype value for this UserInfo.
     * 
     * @param usertype
     */
    public void setUsertype(int usertype) {
        this.usertype = usertype;
    }


    /**
     * Gets the canfindbypublicusers value for this UserInfo.
     * 
     * @return canfindbypublicusers
     */
    public int getCanfindbypublicusers() {
        return canfindbypublicusers;
    }


    /**
     * Sets the canfindbypublicusers value for this UserInfo.
     * 
     * @param canfindbypublicusers
     */
    public void setCanfindbypublicusers(int canfindbypublicusers) {
        this.canfindbypublicusers = canfindbypublicusers;
    }


    /**
     * Gets the nickname value for this UserInfo.
     * 
     * @return nickname
     */
    public java.lang.String getNickname() {
        return nickname;
    }


    /**
     * Sets the nickname value for this UserInfo.
     * 
     * @param nickname
     */
    public void setNickname(java.lang.String nickname) {
        this.nickname = nickname;
    }


    /**
     * Gets the signature value for this UserInfo.
     * 
     * @return signature
     */
    public java.lang.String getSignature() {
        return signature;
    }


    /**
     * Sets the signature value for this UserInfo.
     * 
     * @param signature
     */
    public void setSignature(java.lang.String signature) {
        this.signature = signature;
    }


    /**
     * Gets the actualname value for this UserInfo.
     * 
     * @return actualname
     */
    public java.lang.String getActualname() {
        return actualname;
    }


    /**
     * Sets the actualname value for this UserInfo.
     * 
     * @param actualname
     */
    public void setActualname(java.lang.String actualname) {
        this.actualname = actualname;
    }


    /**
     * Gets the sex value for this UserInfo.
     * 
     * @return sex
     */
    public java.lang.String getSex() {
        return sex;
    }


    /**
     * Sets the sex value for this UserInfo.
     * 
     * @param sex
     */
    public void setSex(java.lang.String sex) {
        this.sex = sex;
    }


    /**
     * Gets the age value for this UserInfo.
     * 
     * @return age
     */
    public java.lang.String getAge() {
        return age;
    }


    /**
     * Sets the age value for this UserInfo.
     * 
     * @param age
     */
    public void setAge(java.lang.String age) {
        this.age = age;
    }


    /**
     * Gets the birth_year value for this UserInfo.
     * 
     * @return birth_year
     */
    public int getBirth_year() {
        return birth_year;
    }


    /**
     * Sets the birth_year value for this UserInfo.
     * 
     * @param birth_year
     */
    public void setBirth_year(int birth_year) {
        this.birth_year = birth_year;
    }


    /**
     * Gets the birth_month value for this UserInfo.
     * 
     * @return birth_month
     */
    public int getBirth_month() {
        return birth_month;
    }


    /**
     * Sets the birth_month value for this UserInfo.
     * 
     * @param birth_month
     */
    public void setBirth_month(int birth_month) {
        this.birth_month = birth_month;
    }


    /**
     * Gets the birth_day value for this UserInfo.
     * 
     * @return birth_day
     */
    public int getBirth_day() {
        return birth_day;
    }


    /**
     * Sets the birth_day value for this UserInfo.
     * 
     * @param birth_day
     */
    public void setBirth_day(int birth_day) {
        this.birth_day = birth_day;
    }


    /**
     * Gets the country value for this UserInfo.
     * 
     * @return country
     */
    public java.lang.String getCountry() {
        return country;
    }


    /**
     * Sets the country value for this UserInfo.
     * 
     * @param country
     */
    public void setCountry(java.lang.String country) {
        this.country = country;
    }


    /**
     * Gets the province value for this UserInfo.
     * 
     * @return province
     */
    public java.lang.String getProvince() {
        return province;
    }


    /**
     * Sets the province value for this UserInfo.
     * 
     * @param province
     */
    public void setProvince(java.lang.String province) {
        this.province = province;
    }


    /**
     * Gets the city value for this UserInfo.
     * 
     * @return city
     */
    public java.lang.String getCity() {
        return city;
    }


    /**
     * Sets the city value for this UserInfo.
     * 
     * @param city
     */
    public void setCity(java.lang.String city) {
        this.city = city;
    }


    /**
     * Gets the area value for this UserInfo.
     * 
     * @return area
     */
    public java.lang.String getArea() {
        return area;
    }


    /**
     * Sets the area value for this UserInfo.
     * 
     * @param area
     */
    public void setArea(java.lang.String area) {
        this.area = area;
    }


    /**
     * Gets the address value for this UserInfo.
     * 
     * @return address
     */
    public java.lang.String getAddress() {
        return address;
    }


    /**
     * Sets the address value for this UserInfo.
     * 
     * @param address
     */
    public void setAddress(java.lang.String address) {
        this.address = address;
    }


    /**
     * Gets the telephone value for this UserInfo.
     * 
     * @return telephone
     */
    public java.lang.String getTelephone() {
        return telephone;
    }


    /**
     * Sets the telephone value for this UserInfo.
     * 
     * @param telephone
     */
    public void setTelephone(java.lang.String telephone) {
        this.telephone = telephone;
    }


    /**
     * Gets the mobile value for this UserInfo.
     * 
     * @return mobile
     */
    public java.lang.String getMobile() {
        return mobile;
    }


    /**
     * Sets the mobile value for this UserInfo.
     * 
     * @param mobile
     */
    public void setMobile(java.lang.String mobile) {
        this.mobile = mobile;
    }


    /**
     * Gets the fax value for this UserInfo.
     * 
     * @return fax
     */
    public java.lang.String getFax() {
        return fax;
    }


    /**
     * Sets the fax value for this UserInfo.
     * 
     * @param fax
     */
    public void setFax(java.lang.String fax) {
        this.fax = fax;
    }


    /**
     * Gets the qq value for this UserInfo.
     * 
     * @return qq
     */
    public java.lang.String getQq() {
        return qq;
    }


    /**
     * Sets the qq value for this UserInfo.
     * 
     * @param qq
     */
    public void setQq(java.lang.String qq) {
        this.qq = qq;
    }


    /**
     * Gets the msn value for this UserInfo.
     * 
     * @return msn
     */
    public java.lang.String getMsn() {
        return msn;
    }


    /**
     * Sets the msn value for this UserInfo.
     * 
     * @param msn
     */
    public void setMsn(java.lang.String msn) {
        this.msn = msn;
    }


    /**
     * Gets the email value for this UserInfo.
     * 
     * @return email
     */
    public java.lang.String getEmail() {
        return email;
    }


    /**
     * Sets the email value for this UserInfo.
     * 
     * @param email
     */
    public void setEmail(java.lang.String email) {
        this.email = email;
    }


    /**
     * Gets the homepage value for this UserInfo.
     * 
     * @return homepage
     */
    public java.lang.String getHomepage() {
        return homepage;
    }


    /**
     * Sets the homepage value for this UserInfo.
     * 
     * @param homepage
     */
    public void setHomepage(java.lang.String homepage) {
        this.homepage = homepage;
    }


    /**
     * Gets the departid value for this UserInfo.
     * 
     * @return departid
     */
    public int getDepartid() {
        return departid;
    }


    /**
     * Sets the departid value for this UserInfo.
     * 
     * @param departid
     */
    public void setDepartid(int departid) {
        this.departid = departid;
    }


    /**
     * Gets the departname value for this UserInfo.
     * 
     * @return departname
     */
    public java.lang.String getDepartname() {
        return departname;
    }


    /**
     * Sets the departname value for this UserInfo.
     * 
     * @param departname
     */
    public void setDepartname(java.lang.String departname) {
        this.departname = departname;
    }


    /**
     * Gets the jobtitle value for this UserInfo.
     * 
     * @return jobtitle
     */
    public java.lang.String getJobtitle() {
        return jobtitle;
    }


    /**
     * Sets the jobtitle value for this UserInfo.
     * 
     * @param jobtitle
     */
    public void setJobtitle(java.lang.String jobtitle) {
        this.jobtitle = jobtitle;
    }


    /**
     * Gets the jobnumber value for this UserInfo.
     * 
     * @return jobnumber
     */
    public java.lang.String getJobnumber() {
        return jobnumber;
    }


    /**
     * Sets the jobnumber value for this UserInfo.
     * 
     * @param jobnumber
     */
    public void setJobnumber(java.lang.String jobnumber) {
        this.jobnumber = jobnumber;
    }


    /**
     * Gets the introduction value for this UserInfo.
     * 
     * @return introduction
     */
    public java.lang.String getIntroduction() {
        return introduction;
    }


    /**
     * Sets the introduction value for this UserInfo.
     * 
     * @param introduction
     */
    public void setIntroduction(java.lang.String introduction) {
        this.introduction = introduction;
    }


    /**
     * Gets the facefile value for this UserInfo.
     * 
     * @return facefile
     */
    public java.lang.String getFacefile() {
        return facefile;
    }


    /**
     * Sets the facefile value for this UserInfo.
     * 
     * @param facefile
     */
    public void setFacefile(java.lang.String facefile) {
        this.facefile = facefile;
    }


    /**
     * Gets the photofile value for this UserInfo.
     * 
     * @return photofile
     */
    public java.lang.String getPhotofile() {
        return photofile;
    }


    /**
     * Sets the photofile value for this UserInfo.
     * 
     * @param photofile
     */
    public void setPhotofile(java.lang.String photofile) {
        this.photofile = photofile;
    }


    /**
     * Gets the loginstatus value for this UserInfo.
     * 
     * @return loginstatus
     */
    public int getLoginstatus() {
        return loginstatus;
    }


    /**
     * Sets the loginstatus value for this UserInfo.
     * 
     * @param loginstatus
     */
    public void setLoginstatus(int loginstatus) {
        this.loginstatus = loginstatus;
    }


    /**
     * Gets the loginstatustext value for this UserInfo.
     * 
     * @return loginstatustext
     */
    public java.lang.String getLoginstatustext() {
        return loginstatustext;
    }


    /**
     * Sets the loginstatustext value for this UserInfo.
     * 
     * @param loginstatustext
     */
    public void setLoginstatustext(java.lang.String loginstatustext) {
        this.loginstatustext = loginstatustext;
    }


    /**
     * Gets the hascamera value for this UserInfo.
     * 
     * @return hascamera
     */
    public int getHascamera() {
        return hascamera;
    }


    /**
     * Sets the hascamera value for this UserInfo.
     * 
     * @param hascamera
     */
    public void setHascamera(int hascamera) {
        this.hascamera = hascamera;
    }


    /**
     * Gets the hasmic value for this UserInfo.
     * 
     * @return hasmic
     */
    public int getHasmic() {
        return hasmic;
    }


    /**
     * Sets the hasmic value for this UserInfo.
     * 
     * @param hasmic
     */
    public void setHasmic(int hasmic) {
        this.hasmic = hasmic;
    }


    /**
     * Gets the vip value for this UserInfo.
     * 
     * @return vip
     */
    public int getVip() {
        return vip;
    }


    /**
     * Sets the vip value for this UserInfo.
     * 
     * @param vip
     */
    public void setVip(int vip) {
        this.vip = vip;
    }


    /**
     * Gets the onlinelevel value for this UserInfo.
     * 
     * @return onlinelevel
     */
    public int getOnlinelevel() {
        return onlinelevel;
    }


    /**
     * Sets the onlinelevel value for this UserInfo.
     * 
     * @param onlinelevel
     */
    public void setOnlinelevel(int onlinelevel) {
        this.onlinelevel = onlinelevel;
    }


    /**
     * Gets the integral value for this UserInfo.
     * 
     * @return integral
     */
    public int getIntegral() {
        return integral;
    }


    /**
     * Sets the integral value for this UserInfo.
     * 
     * @param integral
     */
    public void setIntegral(int integral) {
        this.integral = integral;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof UserInfo)) return false;
        UserInfo other = (UserInfo) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            this.userid == other.getUserid() &&
            ((this.username==null && other.getUsername()==null) || 
             (this.username!=null &&
              this.username.equals(other.getUsername()))) &&
            this.usertype == other.getUsertype() &&
            this.canfindbypublicusers == other.getCanfindbypublicusers() &&
            ((this.nickname==null && other.getNickname()==null) || 
             (this.nickname!=null &&
              this.nickname.equals(other.getNickname()))) &&
            ((this.signature==null && other.getSignature()==null) || 
             (this.signature!=null &&
              this.signature.equals(other.getSignature()))) &&
            ((this.actualname==null && other.getActualname()==null) || 
             (this.actualname!=null &&
              this.actualname.equals(other.getActualname()))) &&
            ((this.sex==null && other.getSex()==null) || 
             (this.sex!=null &&
              this.sex.equals(other.getSex()))) &&
            ((this.age==null && other.getAge()==null) || 
             (this.age!=null &&
              this.age.equals(other.getAge()))) &&
            this.birth_year == other.getBirth_year() &&
            this.birth_month == other.getBirth_month() &&
            this.birth_day == other.getBirth_day() &&
            ((this.country==null && other.getCountry()==null) || 
             (this.country!=null &&
              this.country.equals(other.getCountry()))) &&
            ((this.province==null && other.getProvince()==null) || 
             (this.province!=null &&
              this.province.equals(other.getProvince()))) &&
            ((this.city==null && other.getCity()==null) || 
             (this.city!=null &&
              this.city.equals(other.getCity()))) &&
            ((this.area==null && other.getArea()==null) || 
             (this.area!=null &&
              this.area.equals(other.getArea()))) &&
            ((this.address==null && other.getAddress()==null) || 
             (this.address!=null &&
              this.address.equals(other.getAddress()))) &&
            ((this.telephone==null && other.getTelephone()==null) || 
             (this.telephone!=null &&
              this.telephone.equals(other.getTelephone()))) &&
            ((this.mobile==null && other.getMobile()==null) || 
             (this.mobile!=null &&
              this.mobile.equals(other.getMobile()))) &&
            ((this.fax==null && other.getFax()==null) || 
             (this.fax!=null &&
              this.fax.equals(other.getFax()))) &&
            ((this.qq==null && other.getQq()==null) || 
             (this.qq!=null &&
              this.qq.equals(other.getQq()))) &&
            ((this.msn==null && other.getMsn()==null) || 
             (this.msn!=null &&
              this.msn.equals(other.getMsn()))) &&
            ((this.email==null && other.getEmail()==null) || 
             (this.email!=null &&
              this.email.equals(other.getEmail()))) &&
            ((this.homepage==null && other.getHomepage()==null) || 
             (this.homepage!=null &&
              this.homepage.equals(other.getHomepage()))) &&
            this.departid == other.getDepartid() &&
            ((this.departname==null && other.getDepartname()==null) || 
             (this.departname!=null &&
              this.departname.equals(other.getDepartname()))) &&
            ((this.jobtitle==null && other.getJobtitle()==null) || 
             (this.jobtitle!=null &&
              this.jobtitle.equals(other.getJobtitle()))) &&
            ((this.jobnumber==null && other.getJobnumber()==null) || 
             (this.jobnumber!=null &&
              this.jobnumber.equals(other.getJobnumber()))) &&
            ((this.introduction==null && other.getIntroduction()==null) || 
             (this.introduction!=null &&
              this.introduction.equals(other.getIntroduction()))) &&
            ((this.facefile==null && other.getFacefile()==null) || 
             (this.facefile!=null &&
              this.facefile.equals(other.getFacefile()))) &&
            ((this.photofile==null && other.getPhotofile()==null) || 
             (this.photofile!=null &&
              this.photofile.equals(other.getPhotofile()))) &&
            this.loginstatus == other.getLoginstatus() &&
            ((this.loginstatustext==null && other.getLoginstatustext()==null) || 
             (this.loginstatustext!=null &&
              this.loginstatustext.equals(other.getLoginstatustext()))) &&
            this.hascamera == other.getHascamera() &&
            this.hasmic == other.getHasmic() &&
            this.vip == other.getVip() &&
            this.onlinelevel == other.getOnlinelevel() &&
            this.integral == other.getIntegral();
        __equalsCalc = null;
        return _equals;
    }

    private boolean __hashCodeCalc = false;
    public synchronized int hashCode() {
        if (__hashCodeCalc) {
            return 0;
        }
        __hashCodeCalc = true;
        int _hashCode = 1;
        _hashCode += getUserid();
        if (getUsername() != null) {
            _hashCode += getUsername().hashCode();
        }
        _hashCode += getUsertype();
        _hashCode += getCanfindbypublicusers();
        if (getNickname() != null) {
            _hashCode += getNickname().hashCode();
        }
        if (getSignature() != null) {
            _hashCode += getSignature().hashCode();
        }
        if (getActualname() != null) {
            _hashCode += getActualname().hashCode();
        }
        if (getSex() != null) {
            _hashCode += getSex().hashCode();
        }
        if (getAge() != null) {
            _hashCode += getAge().hashCode();
        }
        _hashCode += getBirth_year();
        _hashCode += getBirth_month();
        _hashCode += getBirth_day();
        if (getCountry() != null) {
            _hashCode += getCountry().hashCode();
        }
        if (getProvince() != null) {
            _hashCode += getProvince().hashCode();
        }
        if (getCity() != null) {
            _hashCode += getCity().hashCode();
        }
        if (getArea() != null) {
            _hashCode += getArea().hashCode();
        }
        if (getAddress() != null) {
            _hashCode += getAddress().hashCode();
        }
        if (getTelephone() != null) {
            _hashCode += getTelephone().hashCode();
        }
        if (getMobile() != null) {
            _hashCode += getMobile().hashCode();
        }
        if (getFax() != null) {
            _hashCode += getFax().hashCode();
        }
        if (getQq() != null) {
            _hashCode += getQq().hashCode();
        }
        if (getMsn() != null) {
            _hashCode += getMsn().hashCode();
        }
        if (getEmail() != null) {
            _hashCode += getEmail().hashCode();
        }
        if (getHomepage() != null) {
            _hashCode += getHomepage().hashCode();
        }
        _hashCode += getDepartid();
        if (getDepartname() != null) {
            _hashCode += getDepartname().hashCode();
        }
        if (getJobtitle() != null) {
            _hashCode += getJobtitle().hashCode();
        }
        if (getJobnumber() != null) {
            _hashCode += getJobnumber().hashCode();
        }
        if (getIntroduction() != null) {
            _hashCode += getIntroduction().hashCode();
        }
        if (getFacefile() != null) {
            _hashCode += getFacefile().hashCode();
        }
        if (getPhotofile() != null) {
            _hashCode += getPhotofile().hashCode();
        }
        _hashCode += getLoginstatus();
        if (getLoginstatustext() != null) {
            _hashCode += getLoginstatustext().hashCode();
        }
        _hashCode += getHascamera();
        _hashCode += getHasmic();
        _hashCode += getVip();
        _hashCode += getOnlinelevel();
        _hashCode += getIntegral();
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(UserInfo.class, true);

    static {
        typeDesc.setXmlType(new javax.xml.namespace.QName("urn:OpenApi", "UserInfo"));
        org.apache.axis.description.ElementDesc elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("userid");
        elemField.setXmlName(new javax.xml.namespace.QName("", "userid"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("username");
        elemField.setXmlName(new javax.xml.namespace.QName("", "username"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("usertype");
        elemField.setXmlName(new javax.xml.namespace.QName("", "usertype"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("canfindbypublicusers");
        elemField.setXmlName(new javax.xml.namespace.QName("", "canfindbypublicusers"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("nickname");
        elemField.setXmlName(new javax.xml.namespace.QName("", "nickname"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("signature");
        elemField.setXmlName(new javax.xml.namespace.QName("", "signature"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("actualname");
        elemField.setXmlName(new javax.xml.namespace.QName("", "actualname"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("sex");
        elemField.setXmlName(new javax.xml.namespace.QName("", "sex"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("age");
        elemField.setXmlName(new javax.xml.namespace.QName("", "age"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("birth_year");
        elemField.setXmlName(new javax.xml.namespace.QName("", "birth_year"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("birth_month");
        elemField.setXmlName(new javax.xml.namespace.QName("", "birth_month"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("birth_day");
        elemField.setXmlName(new javax.xml.namespace.QName("", "birth_day"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("country");
        elemField.setXmlName(new javax.xml.namespace.QName("", "country"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("province");
        elemField.setXmlName(new javax.xml.namespace.QName("", "province"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("city");
        elemField.setXmlName(new javax.xml.namespace.QName("", "city"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("area");
        elemField.setXmlName(new javax.xml.namespace.QName("", "area"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("address");
        elemField.setXmlName(new javax.xml.namespace.QName("", "address"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("telephone");
        elemField.setXmlName(new javax.xml.namespace.QName("", "telephone"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("mobile");
        elemField.setXmlName(new javax.xml.namespace.QName("", "mobile"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("fax");
        elemField.setXmlName(new javax.xml.namespace.QName("", "fax"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("qq");
        elemField.setXmlName(new javax.xml.namespace.QName("", "qq"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("msn");
        elemField.setXmlName(new javax.xml.namespace.QName("", "msn"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("email");
        elemField.setXmlName(new javax.xml.namespace.QName("", "email"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("homepage");
        elemField.setXmlName(new javax.xml.namespace.QName("", "homepage"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("departid");
        elemField.setXmlName(new javax.xml.namespace.QName("", "departid"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("departname");
        elemField.setXmlName(new javax.xml.namespace.QName("", "departname"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("jobtitle");
        elemField.setXmlName(new javax.xml.namespace.QName("", "jobtitle"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("jobnumber");
        elemField.setXmlName(new javax.xml.namespace.QName("", "jobnumber"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("introduction");
        elemField.setXmlName(new javax.xml.namespace.QName("", "introduction"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("facefile");
        elemField.setXmlName(new javax.xml.namespace.QName("", "facefile"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("photofile");
        elemField.setXmlName(new javax.xml.namespace.QName("", "photofile"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("loginstatus");
        elemField.setXmlName(new javax.xml.namespace.QName("", "loginstatus"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("loginstatustext");
        elemField.setXmlName(new javax.xml.namespace.QName("", "loginstatustext"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("hascamera");
        elemField.setXmlName(new javax.xml.namespace.QName("", "hascamera"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("hasmic");
        elemField.setXmlName(new javax.xml.namespace.QName("", "hasmic"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("vip");
        elemField.setXmlName(new javax.xml.namespace.QName("", "vip"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("onlinelevel");
        elemField.setXmlName(new javax.xml.namespace.QName("", "onlinelevel"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("integral");
        elemField.setXmlName(new javax.xml.namespace.QName("", "integral"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
    }

    /**
     * Return type metadata object
     */
    public static org.apache.axis.description.TypeDesc getTypeDesc() {
        return typeDesc;
    }

    /**
     * Get Custom Serializer
     */
    public static org.apache.axis.encoding.Serializer getSerializer(
           java.lang.String mechType, 
           java.lang.Class _javaType,  
           javax.xml.namespace.QName _xmlType) {
        return 
          new  org.apache.axis.encoding.ser.BeanSerializer(
            _javaType, _xmlType, typeDesc);
    }

    /**
     * Get Custom Deserializer
     */
    public static org.apache.axis.encoding.Deserializer getDeserializer(
           java.lang.String mechType, 
           java.lang.Class _javaType,  
           javax.xml.namespace.QName _xmlType) {
        return 
          new  org.apache.axis.encoding.ser.BeanDeserializer(
            _javaType, _xmlType, typeDesc);
    }

}

//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\PaymentConditionObj.java

package com.bitc.jk.xmis.model;

import java.sql.Date;

public class PaymentConditionObj 
{
   private int contractID;
   private String contractNo;
   private String contractName;
   private int paymentConditionID;
   private String Description;
   private int projectStatusMapping;
   private String projectStatusName;
   private int delayDays;
   private Date paymentDate;
   private String manualFlag;
   private String manualReason;
   private int paymentPercent;
   private double pamymentAmt;
   
   /**
    * @roseuid 4C1AD5E5036B
    */
   public PaymentConditionObj() 
   {
    
   }
   
   /**
    * Access method for the contractID property.
    * 
    * @return   the current value of the contractID property
    */
   public int getContractID() 
   {
      return contractID;
   }
   
   /**
    * Sets the value of the contractID property.
    * 
    * @param aContractID the new value of the contractID property
    */
   public void setContractID(int aContractID) 
   {
      contractID = aContractID;
   }
   
   /**
    * Access method for the contractNo property.
    * 
    * @return   the current value of the contractNo property
    */
   public String getContractNo() 
   {
      return contractNo;
   }
   
   /**
    * Sets the value of the contractNo property.
    * 
    * @param aContractNo the new value of the contractNo property
    */
   public void setContractNo(String aContractNo) 
   {
      contractNo = aContractNo;
   }
   
   /**
    * Access method for the contractName property.
    * 
    * @return   the current value of the contractName property
    */
   public String getContractName() 
   {
      return contractName;
   }
   
   /**
    * Sets the value of the contractName property.
    * 
    * @param aContractName the new value of the contractName property
    */
   public void setContractName(String aContractName) 
   {
      contractName = aContractName;
   }
   
   /**
    * Access method for the paymentConditionID property.
    * 
    * @return   the current value of the paymentConditionID property
    */
   public int getPaymentConditionID() 
   {
      return paymentConditionID;
   }
   
   /**
    * Sets the value of the paymentConditionID property.
    * 
    * @param aPaymentConditionID the new value of the paymentConditionID property
    */
   public void setPaymentConditionID(int aPaymentConditionID) 
   {
      paymentConditionID = aPaymentConditionID;
   }
   
   /**
    * Access method for the Description property.
    * 
    * @return   the current value of the Description property
    */
   public String getDescription() 
   {
      return Description;
   }
   
   /**
    * Sets the value of the Description property.
    * 
    * @param aDescription the new value of the Description property
    */
   public void setDescription(String aDescription) 
   {
      Description = aDescription;
   }
   
   /**
    * Access method for the projectStatusMapping property.
    * 
    * @return   the current value of the projectStatusMapping property
    */
   public int getProjectStatusMapping() 
   {
      return projectStatusMapping;
   }
   
   /**
    * Sets the value of the projectStatusMapping property.
    * 
    * @param aProjectStatusMapping the new value of the projectStatusMapping property
    */
   public void setProjectStatusMapping(int aProjectStatusMapping) 
   {
      projectStatusMapping = aProjectStatusMapping;
   }
   
   /**
    * Access method for the projectStatusName property.
    * 
    * @return   the current value of the projectStatusName property
    */
   public String getProjectStatusName() 
   {
      return projectStatusName;
   }
   
   /**
    * Sets the value of the projectStatusName property.
    * 
    * @param aProjectStatusName the new value of the projectStatusName property
    */
   public void setProjectStatusName(String aProjectStatusName) 
   {
      projectStatusName = aProjectStatusName;
   }
   
   /**
    * Access method for the delayDays property.
    * 
    * @return   the current value of the delayDays property
    */
   public int getDelayDays() 
   {
      return delayDays;
   }
   
   /**
    * Sets the value of the delayDays property.
    * 
    * @param aDelayDays the new value of the delayDays property
    */
   public void setDelayDays(int aDelayDays) 
   {
      delayDays = aDelayDays;
   }
   
   /**
    * Access method for the paymentDate property.
    * 
    * @return   the current value of the paymentDate property
    */
   public Date getPaymentDate() 
   {
      return paymentDate;
   }
   
   /**
    * Sets the value of the paymentDate property.
    * 
    * @param aPaymentDate the new value of the paymentDate property
    */
   public void setPaymentDate(Date aPaymentDate) 
   {
      paymentDate = aPaymentDate;
   }
   
   /**
    * Access method for the paymentPercent property.
    * 
    * @return   the current value of the paymentPercent property
    */
   public int getPaymentPercent() 
   {
      return paymentPercent;
   }
   
   /**
    * Sets the value of the paymentPercent property.
    * 
    * @param aPaymentPercent the new value of the paymentPercent property
    */
   public void setPaymentPercent(int aPaymentPercent) 
   {
      paymentPercent = aPaymentPercent;
   }
   
   /**
    * Access method for the pamymentAmt property.
    * 
    * @return   the current value of the pamymentAmt property
    */
   public double getPamymentAmt() 
   {
      return pamymentAmt;
   }
   
   /**
    * Sets the value of the pamymentAmt property.
    * 
    * @param aPamymentAmt the new value of the pamymentAmt property
    */
   public void setPamymentAmt(double aPamymentAmt) 
   {
      pamymentAmt = aPamymentAmt;
   }

/**
 * @return the manualFlag
 */
public String getManualFlag() {
	return manualFlag;
}

/**
 * @param manualFlag the manualFlag to set
 */
public void setManualFlag(String manualFlag) {
	this.manualFlag = manualFlag;
}

/**
 * @return the manualReason
 */
public String getManualReason() {
	return manualReason;
}

/**
 * @param manualReason the manualReason to set
 */
public void setManualReason(String manualReason) {
	this.manualReason = manualReason;
}


}

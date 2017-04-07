package com.bitc.jk.xmis.util;

import java.beans.PropertyDescriptor;
import java.lang.reflect.InvocationTargetException;
import java.math.BigDecimal;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.apache.commons.beanutils.BeanUtilsBean;
import org.apache.commons.beanutils.DynaBean;
import org.apache.commons.beanutils.DynaProperty;
import org.apache.log4j.Logger;

import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.bid.BidPrice;
import com.bitc.jk.xmis.model.bid.Bidding;

public class BeanUtils {
	private static Logger log = Logger.getLogger(BeanUtils.class);

	/**
	 * 拷贝一个bean中的非空属性于另一个bean中
	 * 
	 * @param dest
	 * @param orig
	 * @throws IllegalAccessException
	 * @throws InvocationTargetException
	 */
	@SuppressWarnings("rawtypes")
	public static void copyNotNullProperties(Object dest, Object orig) throws IllegalAccessException,
			InvocationTargetException {
		BeanUtilsBean beanUtils = BeanUtilsBean.getInstance();
		// Validate existence of the specified beans
		if (dest == null) {
			throw new IllegalArgumentException("No destination bean specified");
		}
		if (orig == null) {
			throw new IllegalArgumentException("No origin bean specified");
		}
		if (log.isDebugEnabled()) {
			log.debug("BeanUtils.copyProperties(" + dest + ", " + orig + ")");
		}

		// Copy the properties, converting as necessary
		if (orig instanceof DynaBean) {
			DynaProperty[] origDescriptors = ((DynaBean) orig).getDynaClass().getDynaProperties();
			for (int i = 0; i < origDescriptors.length; i++) {
				String name = origDescriptors[i].getName();
				// Need to check isReadable() for WrapDynaBean
				// (see Jira issue# BEANUTILS-61)
				if (beanUtils.getPropertyUtils().isReadable(orig, name)
						&& beanUtils.getPropertyUtils().isWriteable(dest, name)) {
					Object value = ((DynaBean) orig).get(name);
					beanUtils.copyProperty(dest, name, value);
				}
			}
		} else if (orig instanceof Map) {
			Iterator entries = ((Map) orig).entrySet().iterator();
			while (entries.hasNext()) {
				Map.Entry entry = (Map.Entry) entries.next();
				String name = (String) entry.getKey();
				if (beanUtils.getPropertyUtils().isWriteable(dest, name)) {
					beanUtils.copyProperty(dest, name, entry.getValue());
				}
			}
		} else /* if (orig is a standard JavaBean) */{
			PropertyDescriptor[] origDescriptors = beanUtils.getPropertyUtils().getPropertyDescriptors(orig);
			for (int i = 0; i < origDescriptors.length; i++) {
				String name = origDescriptors[i].getName();
				if ("class".equals(name)) {
					continue; // No point in trying to set an object's class
				}
				if (beanUtils.getPropertyUtils().isReadable(orig, name)
						&& beanUtils.getPropertyUtils().isWriteable(dest, name)) {
					try {
						Object value = beanUtils.getPropertyUtils().getSimpleProperty(orig, name);
						if (value != null) {
							beanUtils.copyProperty(dest, name, value);
						}
					} catch (NoSuchMethodException e) {
						// Should not happen
					}
				}
			}
		}
	}

	@SuppressWarnings("unchecked")
	public static Map<String, Object> bean2Map(Object bean) {
		if (bean == null) {
			throw new IllegalArgumentException("No bean specified");
		}
		BeanUtilsBean beanUtils = BeanUtilsBean.getInstance();
		Map<String, Object> map = new HashMap<String, Object>();
		if (bean instanceof DynaBean) {
			DynaProperty[] origDescriptors = ((DynaBean) bean).getDynaClass().getDynaProperties();
			for (int i = 0; i < origDescriptors.length; i++) {
				String name = origDescriptors[i].getName();
				// Need to check isReadable() for WrapDynaBean
				// (see Jira issue# BEANUTILS-61)
				if (beanUtils.getPropertyUtils().isReadable(bean, name)) {
					Object value = ((DynaBean) bean).get(name);
					map.put(name, value);
				}
			}
		} else if (bean instanceof Map) {
			return (Map<String, Object>) bean;
		} else /* if (orig is a standard JavaBean) */{
			PropertyDescriptor[] origDescriptors = beanUtils.getPropertyUtils().getPropertyDescriptors(bean);
			for (int i = 0; i < origDescriptors.length; i++) {
				String name = origDescriptors[i].getName();
				if ("class".equals(name)) {
					continue; // No point in trying to set an object's class
				}
				if (beanUtils.getPropertyUtils().isReadable(bean, name)) {
					try {
						Object value = beanUtils.getPropertyUtils().getSimpleProperty(bean, name);
						map.put(name, value);
					} catch (NoSuchMethodException e) {
						// Should not happen
					} catch (IllegalAccessException e) {
					} catch (InvocationTargetException e) {
					}
				}
			}
		}
		return map;
	}

	public static void setProperty(Object o, String field, Object value) throws IllegalAccessException,
			InvocationTargetException, NoSuchMethodException {
		int index = field.indexOf(".");
		BeanUtilsBean beanUtils = BeanUtilsBean.getInstance();
		if (index > 0) {
			String name = field.substring(0, index);
			String field2 = field.substring(index + 1);
			Object object2 = beanUtils.getPropertyUtils().getSimpleProperty(o, name);
			setProperty(object2, field2, value);
		} else {
			beanUtils.getPropertyUtils().setProperty(o, field, value);
		}
	}

	public static Object getProperty(Object o, String field) throws Exception {
		int index = field.indexOf(".");
		BeanUtilsBean beanUtils = BeanUtilsBean.getInstance();
		if (index > 0) {
			String name = field.substring(0, index);
			String field2 = field.substring(index + 1);
			Object object2 = beanUtils.getPropertyUtils().getSimpleProperty(o, name);
			return getProperty(object2, field2);
		} else {
			return beanUtils.getPropertyUtils().getSimpleProperty(o, field);
		}
	}

	@SuppressWarnings("rawtypes")
	public static Class getPropertyClass(Object o, String field) throws Exception {
		BeanUtilsBean beanUtils = BeanUtilsBean.getInstance();
		return beanUtils.getPropertyUtils().getPropertyDescriptor(o, field).getPropertyType();
	}

	public static void main(String[] args) throws Throwable, InvocationTargetException, NoSuchMethodException {
		Bidding bidding = new Bidding();
		bidding.setName("sdfdf");
		System.out.println(bidding.getName());
		setProperty(bidding, "name", null);
		System.out.println(bidding.getName());
		CompanySimpleObj cso = new CompanySimpleObj();
		cso.setCompanyName("sdfdsfsdf");
		bidding.setBidCompany(cso);
		setProperty(bidding, "bidCompany.companyName", "sdf123123123");
		System.out.println(bidding.getBidCompany().getCompanyName());
		System.out.println(getPropertyClass(bidding, "winCompany"));
		BidPrice bd=new BidPrice();
		bd.setBidPrice(new BigDecimal(123));
		setProperty(bd, "bidPrice", null);
		System.out.println(bd.getBidPrice());
	}
}
package com.bitc.woix.client;

import java.net.MalformedURLException;
import java.net.URL;
import java.rmi.RemoteException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import org.apache.axis.AxisFault;

public class WSTester {

	/**
	 * @param args
	 * @throws MalformedURLException
	 * @throws RemoteException
	 * @throws ParseException
	 */
	public static void main(String[] args) throws MalformedURLException,
			RemoteException, ParseException {
		WoixServiceLocator service = new WoixServiceLocator();
		WoixServiceSoapStub ws = new WoixServiceSoapStub(new URL(
				"http://192.169.11.76/woix/WoixService.asmx"), service);
		WoixServiceSoapProxy proxy = new WoixServiceSoapProxy(
				"http://192.169.11.76/woix/WoixService.asmx");
		String startS = "2013-10-21";
//		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-mm-dd");
//		Calendar start = Calendar.getInstance();
//		
//		start.setTime(sdf.parse(startS));
//        clearHMS(start);
		
		String endS = "2013-10-31";
//		Calendar end = Calendar.getInstance();
//		end.setTime(sdf.parse(endS));
//		clearHMS(end);

		int startInx = Integer.valueOf("0");
		int endInx = Integer.valueOf(20);
		String searchStr = "DCOM2与梯调通讯机3的A通道";
		String sort = "NO ASC";
		String result;
		System.out.println(proxy.getAlarmJson(startS, endS, startInx, endInx, searchStr,
				sort));
////		
//		System.out.println(proxy.getQuickAlarmJson(2));
//		System.out.println(proxy.getPointTableJson(1, 1, 1, 1, 21));

	}
	
	public static void clearHMS(Calendar cal) {
		cal.set(Calendar.HOUR_OF_DAY, 0);
		cal.set(Calendar.MINUTE, 0);
		cal.set(Calendar.SECOND, 0);
		cal.set(Calendar.MILLISECOND, 0);
	}

}

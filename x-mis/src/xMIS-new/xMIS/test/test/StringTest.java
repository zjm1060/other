package test;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import org.apache.commons.lang.time.DateUtils;

enum AEnum {
	a, b
}

public class StringTest {

	public static void main(String[] args) throws ClassNotFoundException {
		String dateStr = "Wednesday,May 22,2013";
		try {
			SimpleDateFormat sdf=new SimpleDateFormat("E,MMM dd,yyyy", Locale.ENGLISH);
			System.out.println(sdf.format(new Date()));
			System.out.println(sdf.parse(dateStr));
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
}

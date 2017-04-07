package bitc.sz.wzEnv.util;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.TimeZone;

public class DateUtil {

	/**
	 * 将毫秒转换为年月日时分秒
	 * @param timeMillis
	 * @return
	 */
	public static String getYearMonthDayHourMinuteSecond(long timeMillis) {

		Calendar calendar = Calendar.getInstance(TimeZone.getTimeZone("GMT+8"));

		calendar.setTimeInMillis(timeMillis);

		int year = calendar.get(Calendar.YEAR);

		int month = calendar.get(Calendar.MONTH) + 1;

		String mToMonth = null;

		if (String.valueOf(month).length() == 1) {

			mToMonth = "0" + month;

		} else {

			mToMonth = String.valueOf(month);

		}

		int day = calendar.get(Calendar.DAY_OF_MONTH);

		String dToDay = null;

		if (String.valueOf(day).length() == 1) {

			dToDay = "0" + day;

		} else {

			dToDay = String.valueOf(day);

		}

		int hour = calendar.get(Calendar.HOUR_OF_DAY);
		String hToHour = null;

		if (String.valueOf(hour).length() == 1) {

			hToHour = "0" + hour;

		} else {

			hToHour = String.valueOf(hour);

		}

		int minute = calendar.get(Calendar.MINUTE);

		String mToMinute = null;

		if (String.valueOf(minute).length() == 1) {

			mToMinute = "0" + minute;

		} else {

			mToMinute = String.valueOf(minute);

		}

		int second = calendar.get(Calendar.SECOND);

		String sToSecond = null;

		if (String.valueOf(second).length() == 1) {

			sToSecond = "0" + second;

		} else {

			sToSecond = String.valueOf(second);

		}

		return year + "-" + mToMonth + "-" + dToDay + " " + hToHour + ":"
				+ mToMinute + ":" + sToSecond;

	}

	public static Date parseDate(String source, String format)
			throws ParseException {
		SimpleDateFormat sdf = new SimpleDateFormat(format);
		return sdf.parse(source);
	}

	public static Date parseDate(String source) throws ParseException {
		return parseDate(source, "yyyy-MM-dd");
	}

	public static String formatDate(Date date) {
		return formatDate(date, "yyyy-MM-dd");
	}

	public static String formatDate(Date date, String format) {
		if (date == null) {
			return "";
		}
		return new SimpleDateFormat(format).format(date);
	}

	public static Date getSystemDate() {
		return new Date(System.currentTimeMillis());
	}

	public static Date parseDateTime(String value) throws ParseException {
		return parseDate(value, "yyyy-MM-dd HH:mm:ss");
	}

	/**
	 * 对指定的日期字段进行加或者减操作
	 * 
	 * @param date
	 * @param field
	 *            see {@link Calendar}'s fields
	 * @param amount
	 *            要加减的数量
	 * @return 返回一个新的 {@link Date} 实例
	 */
	public static Date add(Date date, int field, int amount) {
		Calendar cal = Calendar.getInstance();
		cal.setTimeInMillis(date.getTime());
		cal.add(field, amount);
		return new Date(cal.getTimeInMillis());
	}

	/**
	 * 设置日期的时分秒及毫秒数均为0
	 * 
	 * @param cal
	 */
	public static void clearHMS(Calendar cal) {
		cal.set(Calendar.HOUR_OF_DAY, 0);
		cal.set(Calendar.MINUTE, 0);
		cal.set(Calendar.SECOND, 0);
		cal.set(Calendar.MILLISECOND, 0);
	}

	/**
	 * 设置日期的时分秒及毫秒数均为0
	 * 
	 * @param date
	 * @return 返回一个新的 {@link Date} 实例，此日期为一天中最小的时间
	 */
	public static Date clearHMS(Date date) {
		Calendar cal = Calendar.getInstance();
		cal.setTimeInMillis(date.getTime());
		clearHMS(cal);
		return new Date(cal.getTimeInMillis());
	}

	/**
	 * 设置日期的时为23，分为59，秒为29，毫秒数为999
	 * 
	 * @param date
	 * @return 返回一个新的 {@link Date} 实例，此日期为一天中最大的时间
	 */
	public static Date addFullHMS(Date date) {
		Calendar cal = Calendar.getInstance();
		cal.setTimeInMillis(date.getTime());
		cal.set(Calendar.HOUR_OF_DAY, 23);
		cal.set(Calendar.MINUTE, 59);
		cal.set(Calendar.SECOND, 59);
		cal.set(Calendar.MILLISECOND, 999);
		return new Date(cal.getTimeInMillis());
	}

	public static long betweenMonth(Date start, Date end) {
		return Math
				.abs(((end.getTime() - start.getTime()) / (1000L * 3600 * 24 * 30)));
	}

	/**
	 * 获取
	 * 
	 * @param start
	 * @return
	 */
	public static int getField(Date start, int field) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(start);
		return calendar.get(field);
	}

	public static Date getCurrentDateTime() {
		return new Date(System.currentTimeMillis());
	}

}

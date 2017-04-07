package test;

import java.math.BigDecimal;

public class BigDecimalTest {
	public static void main(String[] args) {
		BigDecimal d1=new BigDecimal("0");
		System.out.println(BigDecimal.ZERO==d1);
		System.out.println(BigDecimal.ZERO.compareTo(d1));
		System.out.println(BigDecimal.ZERO.equals(d1));
	}
}

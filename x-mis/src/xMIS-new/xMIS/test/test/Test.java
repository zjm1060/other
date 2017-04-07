package test;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Pattern pattern = Pattern.compile("^[A-Z]{1}.+\\.js$");
		Matcher m = pattern.matcher("Aaaaaab.js");
		boolean b = m.matches();
		System.out.println(b);
	}
}

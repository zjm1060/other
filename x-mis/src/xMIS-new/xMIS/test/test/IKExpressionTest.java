package test;

import java.util.ArrayList;
import java.util.List;

import org.wltea.expression.ExpressionEvaluator;
import org.wltea.expression.datameta.Variable;

public class IKExpressionTest {
	public static void main(String[] args) {
		List<Variable>  variables=new ArrayList<Variable>();
		variables.add(Variable.createVariable("projectCode", "BITC-JK-JS-2012"));
		String s=(String) ExpressionEvaluator.evaluate("\"项目编码：\"+projectCode", variables);
		System.out.println(s);
	}
}

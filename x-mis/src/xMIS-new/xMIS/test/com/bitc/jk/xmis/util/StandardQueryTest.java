package com.bitc.jk.xmis.util;

import org.junit.Test;

import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class StandardQueryTest {
	@Test
	public void test1() {
		StandardQuery param1 = SimpleParam.buildSimpleParam("a", 1,
				Type.INTEGER, RelationalOperator.EQ);
		StandardQuery param2 = SimpleParam.buildSimpleParam("b", "abc123",
				Type.STRING, RelationalOperator.EQ);
		CompositParam cparam = CompositParam.buildOrCompositParam();
		cparam.addStandardQuery(param1).addStandardQuery(param2);
		CompositParam cparam2 = CompositParam.buildAndCompositParam();
		cparam2.addStandardQuery(cparam);
		cparam2.addStandardQuery(SimpleParam.buildSimpleParam("c", 3,
				Type.INTEGER, RelationalOperator.EQ));
		// System.out.println(cparam2.toString());
		String sql = "select * into #result from table_a where id>100";
		SQLGenerator generator = new SQLGenerator(sql);
		cparam2.addToSQLGenerator(generator);
		SimpleParam param3 = SimpleParam.buildSimpleParam(
				RelationalOperator.JDBCSQL, "t=? or t=a and not exsit(select ...)", "d");
		param3.addToSQLGenerator(generator);
		System.out.println(generator.getSQL());
	}
}

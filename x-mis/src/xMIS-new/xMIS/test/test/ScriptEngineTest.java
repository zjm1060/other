package test;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

public class ScriptEngineTest {

	/**
	 * @param args
	 * @throws ScriptException
	 */
	public static void main(String[] args) throws ScriptException {
		ScriptEngineManager manager = new ScriptEngineManager();
		ScriptEngine engine = manager.getEngineByName("javascript");
		String script = "function a(value, metaData, record) {\r\n\t\t\tvalue = Ext.isEmpty(value) ? \'_\' : value;\r\n\t\t\treturn \'<a href=\"javascript:;\" onclick=\"parent.xMis.turn(\\\'project-info-\'\r\n\t\t\t\t\t+ record.get(ProjectID)\r\n\t\t\t\t\t+ \'\\\',\\\'项目信息-\'\r\n\t\t\t\t\t+ record.get(ProjectName)\r\n\t\t\t\t\t+ \'\\\',\\\'\'\r\n\t\t\t\t\t+ __cxtPath\r\n\t\t\t\t\t+ \'/projectMain.do?method=getProjectInfo&projectID=\'\r\n\t\t\t\t\t+ record.get(ProjectID)\r\n\t\t\t\t\t+ \'\\\',\\\'menu-project-info\\\')\">\'\r\n\t\t\t\t\t+ value + \'</a>\';\r\n\t\t}";
		System.out.println(engine.eval("a('中国^a.do')"));
	}

}

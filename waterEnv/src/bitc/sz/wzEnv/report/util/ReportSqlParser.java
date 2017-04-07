package bitc.sz.wzEnv.report.util;

import bitc.sz.wzEnv.report.model.Parameter;
import net.sf.jsqlparser.JSQLParserException;
import net.sf.jsqlparser.expression.*;
import net.sf.jsqlparser.parser.CCJSqlParserUtil;
import net.sf.jsqlparser.statement.Statement;
import net.sf.jsqlparser.statement.select.Join;
import net.sf.jsqlparser.statement.select.PlainSelect;
import net.sf.jsqlparser.statement.select.Select;
import net.sf.jsqlparser.statement.select.SubSelect;

import java.util.List;

/**
 * Created by zsz on 2016/3/15.
 */
public class ReportSqlParser {

    public static void sqlParser(List<Parameter> list,String sql) throws JSQLParserException {
        Statement statement = CCJSqlParserUtil.parse(sql);
        Select selectStatement = (Select) statement;
        PlainSelect select = (PlainSelect) selectStatement.getSelectBody();
        parserSelect(list, select);
    }

    public static void parserSelect(List<Parameter> parameters, PlainSelect select) {


        if (select.getFromItem() instanceof SubSelect) {
            SubSelect subselect = (SubSelect) select.getFromItem();
            parserSelect(parameters, (PlainSelect) subselect.getSelectBody());
        }
        if (select.getJoins() != null && select.getJoins().size() > 0) {
            for (int i = 0; i < select.getJoins().size(); i++) {
                Join join = select.getJoins().get(i);
                if (join.getRightItem() instanceof SubSelect) {
                    SubSelect subjoinselect = (SubSelect) join.getRightItem();
                    parserSelect(parameters, (PlainSelect) subjoinselect.getSelectBody());
                }
                parserExpression(parameters, join.getOnExpression());
            }
        }
        if (select.getWhere() != null) {
            parserExpression(parameters, select.getWhere());
        }
    }

    public static void parserExpression(List<Parameter> parameters,Expression expression) {
        if (expression == null) {
            return;
        }
        if (expression instanceof BinaryExpression) {
            BinaryExpression binaryExpression = (BinaryExpression) expression;
            if (binaryExpression.getRightExpression() instanceof JdbcParameter
                    ||binaryExpression.getRightExpression() instanceof JdbcNamedParameter) {
                System.out.println(expression.toString());
                Parameter parameter = new Parameter();
                parameter.setDisplayName(binaryExpression.getLeftExpression().toString());
                parameter.setName(binaryExpression.getLeftExpression().toString());
                parameter.setSort(parameters.size());
                parameters.add(parameter);
//                binaryExpression.setRightExpression(new LongValue(1L));
//                binaryExpression.setLeftExpression(new LongValue(1L));
            } else {
                parserExpression(parameters, binaryExpression.getLeftExpression());
                parserExpression(parameters, binaryExpression.getRightExpression());
            }
        } else if (expression instanceof Parenthesis) {
            parserExpression(parameters, ((Parenthesis) expression).getExpression());
        }
    }
}

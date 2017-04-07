# 工作记录
1. 将flow_inst , step_inst , deal_info三张表中的数据，2013年12月31日之前的，转到了 _bak 为后缀的表中。
1. 修改FlowEngine.jar 中 WorkListOperator 类中获取任务列表的方法，加入对时间条件的判断，设置了日期小于条件且 <= 2013-12-31时，去 _bak 表中查询数据
1. 修改FlowEngine.jar 中 FormLib 类，通过最小的flowinid来判断查哪个表


<pre><code>
    private static int minFlowInid=313837;  //最小

    if (Long.parseLong(ex) < minFlowInid) {
         sqlClause = sqlClause.replaceAll("\\bflow_inst\\b", "flow_inst_bak")
            .replaceAll("\\bstep_inst\\b", "step_inst_bak")
            .replaceAll("\\bdeal_info\\b", "deal_info_bak");
    }
</code></pre>
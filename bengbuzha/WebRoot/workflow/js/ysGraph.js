/**
 * @author Bom Wu
 * 
 */
ysGraph = function() {
	ysGraph.superclass.constructor.call(this);
};
Ext.extend(ysGraph, mxGraph);
//mxGraph.prototype.addCell = function(cell, parent, index, source, target) {
//	if (cell.isEdge && target) {
//		cell.setValue('to ' + target.value);
//	}
//	return this.addCells([cell], parent, index, source, target)[0];
//};

// 为mxCell添加额外的属性
// conditions 转移条件
mxCell.prototype.condition = "";
//任务类型
mxCell.prototype.assigneeType="";
// assignee 步骤的任务人
mxCell.prototype.assignee = "";
// assigneeDesc 步骤的任务人描述
mxCell.prototype.assigneeDesc = "";
// form表单数据
mxCell.prototype.form = "";

mxCell.prototype.formName = "";
//todo待办
mxCell.prototype.todo = "";
//发启人
mxCell.prototype.sponsor = "";
// candidate步骤的任务候选人
mxCell.prototype.candidate = "";
// candidate步骤的任务候选人描述
mxCell.prototype.candidateDesc = "";

mxCell.prototype.permission="";


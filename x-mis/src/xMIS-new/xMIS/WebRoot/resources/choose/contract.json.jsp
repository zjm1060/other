<%@page language="java" pageEncoding="utf-8"%>
<%@page import="com.bitc.jk.xmis.util.DBSchema"%>
{
	'action' : true,
	'message' : 'error!',
	'dataUrl' : 'contractMain.do?method=getContractListByType',
	'sortInfo' : {
		field : '合同名称',
		direction : 'ASC'
	},
	'columnModle' : [{
				header : '合同ID',
				dataIndex : '<%=DBSchema.Contract.ContractID%>',
				sortable : false,
				hidden : true
			}, {
				header : '项目编号',
				dataIndex : '<%=DBSchema.Project.ProjectCode%>',
				sortable : true,
				renderer : biuldLinker
			}, {
				header : '合同名称',
				dataIndex : '<%=DBSchema.Contract.ContractName%>',
				sortable : true,
				renderer : biuldLinker
			}, {
				header : '合同简称',
				dataIndex : '<%=DBSchema.Contract.Abbreviation%>',
				sortable : true,
				renderer : biuldLinker
			}, {
				header : '来源类型',
				dataIndex : '<%=DBSchema.Contract.GovermentContractSourceName%>',
				sortable : true,
				renderer : biuldLinker
			}, {
				header : '合同金额',
				dataIndex : '<%=DBSchema.Contract.ContractAmt%>',
				sortable : true,
				renderer : function(value, cellMeta, record, rowIndex,
						columnIndex, store) {
					return formatCurrency(
							record.data['<%=DBSchema.CurrencyType.CurrenyGraph%>'],
							value, '');
				},
				align : 'right'
			}, {
				header : '甲方公司',
				dataIndex : 'aCompany_abb',
				sortable : true,
				renderer : biuldLinker
			}, {
				header : '项目名称',
				dataIndex : 'project_name',
				sortable : true,
				renderer : biuldLinker
			},{
				header : '合同编号',
				dataIndex : '<%=DBSchema.Contract.ContractNo%>',
				sortable : true,
				renderer : biuldLinker
			}],
	'fieldsNames' : [{
				name : '<%=DBSchema.Contract.ContractID%>'
			}, {
				name : '<%=DBSchema.Contract.ContractNo%>'
			}, {
				name : '<%=DBSchema.Contract.ContractName%>'
			}, {
				name : '<%=DBSchema.Contract.Abbreviation%>'
			}, {
				name : '<%=DBSchema.Contract.GovermentContractSourceName%>'
			}, {
				name : '<%=DBSchema.Contract.ContractAmt%>'
			}, {
				name : 'aCompany_abb'
			}, {
				name : 'project_id'
			}, {
				name : 'project_name'
			}, {
				name:'<%=DBSchema.Project.ProjectCode%>'
			}, {
				name:'<%=DBSchema.Project.ProjectID%>'
			}, {
				name : 'pageNo'
			}],
	'searchBar' : true,
	'searchFieldConfig' : {
		width : 120,
		tooltip : {
			title : '模糊查询',
			text : '您可以输入任意关键词进行查询。<br/>可以在关键字前加运算符：>、<、>=、<=、=、<>、like等。<br/>按日期请按如下格式输入：<br/>2010-01-01'
		},
		inklings : [{
					name : '合同名称',
					op : 'like',
					type : 'string'
				}, {
					name : '简称',
					op : 'like',
					type : 'string'
				}, {
					name : '合同编号',
					op : 'like',
					type : 'string'
				}, {
					name : '签订日期',
					op : '=',
					type : 'date'
				}, {
					name : '项目编码',
					op : 'like',
					type : 'string'
				}, {
					name : '合同总额',
					op : '=',
					type : 'double'
				}, {
					name : 'aCompany_name',
					op : 'like',
					type : 'string'
				}]
	}
}

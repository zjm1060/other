<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";

	String type = request
			.getParameter(WebKeys.Request_Attribute_Name_Type) == null ? (String) request
			.getAttribute(WebKeys.Request_Attribute_Name_Type)
			: request.getParameter(WebKeys.Request_Attribute_Name_Type);

	Vector operationName = new Vector();
	Vector operationURL = new Vector();
	int operationItemCount = 0;
	String typeName = "";

	if (type.equals("Project")) {
		operationName.add("项目启动");
		operationName.add("任务分配");
		operationName.add("内部验收");
		operationName.add("出厂验收");
		operationName.add("合同变更");
		operationName.add("项目维护");
		operationName.add("现场工作报告");
		operationName.add("遗留问题处理");

		operationName.add("项目状态查询");

		operationItemCount = 9;

		typeName = "项目管理";

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

	} else if (type.equals("Contract")) {
		operationName.add("合同录入");
		operationName.add("票据收发登记");
		operationName.add("合同款收付登记");

		operationName.add("合同综合查询");

		operationItemCount = 4;

		typeName = "合同管理";

		operationURL.add("/xMIS/contractMain.do?method=contract&contractID=");
		operationURL.add("/xMIS/contractMain.do?method=invoiceEdit");
		operationURL.add("/xMIS/contractMain.do?method=paymentListEdit");
		operationURL.add("/xMIS/contractMain.do?method=getContractList&searchtype="+WebKeys.Type_Search_For_ContractList_Integration);

	} else if (type.equals("Material")) {
		operationName.add("设备采购");
		operationName.add("设备入库登记");
		operationName.add("设备领用");
		operationName.add("设备发货");
		operationName.add("返修更换");

		operationName.add("物资信息库");
		operationItemCount = 6;

		typeName = "物资管理";

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

	} else if (type.equals("Meeting")) {
		operationName.add("会议发起");
		operationName.add("会议查询");
		operationItemCount = 1;

		typeName = "会议管理";

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

	} else if (type.equals("Customer")) {
		operationName.add("客户信息登记");
		operationName.add("客户信息查询");
		operationItemCount = 1;

		typeName = "客户关系管理";

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

	} else if (type.equals("Defects")) {
		operationName.add("缺陷登记");
		operationName.add("缺陷查询");
		operationItemCount = 1;

		typeName = "缺陷管理";

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

	} else if (type.equals("Employee")) {
		typeName = "员工管理";

		operationName.add("组织架构");
		//	operationName.add("员工信息");
		operationName.add("外出登记");
		operationName.add("出差登记");
		operationName.add("工作日志");

		operationName.add("休假申请");
		operationName.add("权限申请");

		//operationURL.add("../portal/bitcMain.jsp?type=" + type
		//+ "&subType=departmentTree");

		operationURL.add("/xMIS/orgMaint.do?method="
		+ WebKeys.Action_Method_Name_DeptTree);

		operationURL.add("/xMIS/portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("/xMIS/portal/bitcMain.jsp?type=" + type
		+ "&subType=");

		operationURL.add("/xMIS/portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("/xMIS/portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("/xMIS/portal/bitcMain.jsp?type=" + type
		+ "&subType=");

		operationItemCount = 6;

	} else if (type.equals("Software")) {
		operationName.add("新产品开发");
		operationName.add("后续开发");
		operationName.add("产品发布");
		operationItemCount = 3;

		typeName = "产品管理";

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

	} else if (type.equals("Knowledge")) {
		operationName.add("新数据添加");
		operationName.add("知识库查询");
		operationItemCount = 2;

		typeName = "知识库管理";

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");
		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

	} else if (type.equals("Suggestion")) {
		operationName.add("信息采集");
		operationItemCount = 1;

		typeName = "意见建议";

		operationURL.add("../portal/bitcMain.jsp?type=" + type
		+ "&subType=");

	}
%>



<DIV class=sidebar id=sidebar
	style="Z-INDEX: 3; LEFT: 0px; VERTICAL-ALIGN: top; WIDTH: 160px; POSITION: absolute; TOP: 130px; HEIGHT: 85%">
	<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0 valign="top">
		<TBODY>
			<TR>
				<TD>
					<TABLE height="100%" width=160 align=left border=0 valign="top">
						<TBODY>
							<TR height=5>
								<TD>
									&nbsp;
								</TD>
							</TR>
							<TR>
								<TD vAlign=top></TD>
								<TD>
									<!-- A href="#" target=_blank>What's New ? </A><BR-->
									<div id="whatsNew">
										<ul id="navlist">
											<li>
												<a href="/xMIS/portal/bitcMain.jsp">What's New?</a>
											</li>
											<li>
												<a href="/xMIS/portal/bitcMain.jsp">My Home Page</a>
											</li>
										</ul>
									</div>
								</TD>
							</TR>
							<%
								 /*
								 <TR vAlign=bottom height=10>
								 <TD align=right>&nbsp; </TD>
								 <TD>
								 <!-- A href="#">My Home Page</A--> 
								 <div id="MyHomePage">
								 <ul id="navlist">        
								 <li><a href="#">My Home Page</a></li>
								 </ul>
								 </div>		
								 </TD>
								 </TR>
								 */
							%>
							<TR vAlign=top>
								<TD></TD>
								<TD></TD>
							</TR>
							<TR vAlign=top>
								<TD></TD>
								<TD>
								</TD>
							</TR>
							<TR vAlign=bottom height=20>
								<TD align=right>
									&nbsp;
								</TD>
								<TD>
									<h2>
										<%=typeName%>
									</h2>
								</TD>
							<TR vAlign=top>
								<TD></TD>
								<TD>
									<DIV id=operationItems>
										<TABLE cellSpacing=0 cellPadding=0 border=0>
											<TBODY>
												<TR>
													<TD vAlign=top>
														<div id="navcontainer">
															<ul id="navlist">
																<%
																for (int i = 0; i < operationItemCount; i++) {
																%>
																<li>
																	<a href="<%=(String) operationURL.get(i)%>"><%=(String) operationName.get(i)%>
																	</a>
																</li>

																<%
																}
																%>

															</ul>
														</div>
													</TD>
												</TR>
											</TBODY>
										</TABLE>
									</DIV>
								</TD>
							</TR>
							<TR vAlign=top>
								<TD></TD>
								<TD>
								</TD>
							</TR>
							<TR vAlign=top>
								<TD></TD>
								<TD>
								</TD>
							</TR>
							<TR vAlign=bottom height=20>
								<TD align=right>
									&nbsp;
								</TD>
								<TD>
									<h2>
										My Task
									</h2>
								</TD>
							<TR vAlign=top>
								<TD></TD>
								<TD>
									<div id="myTask">
										<ul id="navlist">
											<li>
												<a href="#">接收任务</a>
											</li>
											<li>
												<a href="#">执行任务</a>
											</li>
											<li>
												<a href="#">历史任务</a>
											</li>
										</ul>
									</div>
								</TD>
							</TR>
							<TR vAlign=top>
								<TD></TD>
								<TD>
								</TD>
							</TR>
							<TR vAlign=top>
								<TD></TD>
								<TD>
								</TD>
							</TR>
							<TR vAlign=bottom height=20>
								<TD align=right>
									&nbsp;
								</TD>
								<TD>
									<h2>
										Search
									</h2>
								</TD>
							<TR>
								<td></td>
								<TD vAlign=top>
									<TABLE cellSpacing=0 cellPadding=0 border=0>
										<TBODY>
											<FORM name=searchProjFrm action="" method=get target=_blank>
											<TR heigh=20>
												<TD noWrap colSpan=3>
													<LI>
														项目编码
													</LI>
												</TD>
											</TR>
											<TR>
												<TD>
													<INPUT type=hidden value=keyword_search name=action>
													<INPUT class=input_s11_110_20 name=keyword>
												</TD>
												<TD width=4>
													&nbsp;
												</TD>
												<TD>
													<INPUT class=go_button type=submit value=Go name="">
												</TD>
											</TR>
											</FORM>
										</TBODY>
										<FORM name=searchContFrm action="" method=get target=_blank>
										<TR heigh=20>
											<TD noWrap colSpan=3>
												<LI>
													合同编码
												</LI>
											</TD>
										</TR>
										<TR>
											<TD>
												<INPUT type=hidden value=keyword_search name=action>
												<INPUT class=input_s11_110_20 name=keyword>
											</TD>
											<TD width=4>
												&nbsp;
											</TD>
											<TD>
												<INPUT class=go_button type=submit value=Go name="">
											</TD>
										</TR>
										</FORM>
										<FORM name=searchPurFrm action="" method=get target=_blank>
										<TR heigh=20>
											<TD noWrap colSpan=3>
												<LI>
													采购清单
												</LI>
											</TD>
										</TR>
										<TR>
											<TD>
												<INPUT type=hidden value=keyword_search name=action>
												<INPUT class=input_s11_110_20 name=keyword>
											</TD>
											<TD width=4>
												&nbsp;
											</TD>
											<TD>
												<INPUT class=go_button type=submit value=Go name="">
											</TD>
										</TR>
										</FORM>
									</TABLE>
								</TD>
							</TR>
							<TR>
						</TBODY>
					</TABLE>
				</TD>
				<TD height=10></TD>
			</TR>
		</TBODY>
	</TABLE>
</DIV>

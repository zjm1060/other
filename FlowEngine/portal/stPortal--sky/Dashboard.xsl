<?xml version='1.0' encoding="gb2312"?>
<xsl:stylesheet version='1.0' xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:WP="urn:schemas-microsoft-com:webpart:" xmlns:DAV="DAV:" xmlns:DDF="urn:schemas-microsoft-com:dashboardfactory:">
	<xsl:output method = "html"/>
	<xsl:template match="/"> 
	<HTML>
		<HEAD>
			<title>
				<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/WP:Title"/>
			</title>
			<xsl:if test="/DAV:multistatus/DDF:Settings/DDF:BaseHref[.!='']">
				<BASE id="bseHref">
					<xsl:attribute name="href">
					<xsl:value-of select="/DAV:multistatus/DDF:Settings/DDF:BaseHref"/></xsl:attribute>
				</BASE>
			</xsl:if>
			<xsl:choose>
				<!-- Use StylesheetLink if specified, otherwise use Stylesheet -->
				<xsl:when test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/WP:StylesheetLink[.!='']">
					<xsl:apply-templates select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/WP:StylesheetLink"/>
				</xsl:when>
				<xsl:otherwise>
					<!-- This will create a link to the default stylesheet -->
					<xsl:apply-templates select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/WP:StylesheetLink"/>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:if test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/WP:Stylesheet[.!='']">
				<xsl:apply-templates select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/WP:Stylesheet"/>
			</xsl:if>

			<!-- Insert some Java script	-->
            <link type="text/css" rel="stylesheet" href="./Common/Css/Public.css"/>
			<link type="text/css" rel="stylesheet" href="./Common/NewTree/NewTree.css"/>
			<script language="JavaScript" src="./Common/NewTree/NewTree.js"></script>
			<script language="javascript" src="./Common/Include/Common.js"></script>
			<script language="JavaScript" src="dbview_ie.js"></script>
			<!-- The tTree.js is used to render a tree -->
			<script language="JavaScript" src="ttreeData.js"></script>
			<script language="JavaScript" src="tTree.js"></script>
			<script language="JavaScript" src="navigation.js"></script>
			<SCRIPT LANGUAGE="JavaScript">
				// Dashboard ID
				var varDashboardID = "<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:ID"/>";
				var varStoreASP = "<xsl:value-of select="/DAV:multistatus/DDF:Settings/DDF:StoreASP"/>";

				// Initialize the Digital Dashboard Service Component Object
				<!--DDSC.init(varDashboardID, document, ""); -->

				var L_MinimizeTip_Text = "<xsl:value-of select="//String[@_locID='L_MinimizeTip_Text']"/>";
				var L_RestoreTip_Text = "<xsl:value-of select="//String[@_locID='L_RestoreTip_Text']"/>";
				var L_TreeViewShow_Text = "<xsl:value-of select="//String[@_locID='L_TreeViewShow_Text']"/>";
				var L_TreeViewHide_Text = "<xsl:value-of select="//String[@_locID='L_TreeViewHide_Text']"/>";

				var m_CurrentBoardID = '<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardID" />';
				var m_CurrentBoardType = '<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardType" />';
				var mLastMenuId="a";
				window.attachEvent("onload",window_onload);
                function changeMenuColor(mId){
                	if (mLastMenuId=="a")
				   		mLastMenuId=mId;
				   	document.all.item(mId).className="DashNavChosen";
                    //document.all.item(mId).style.color="#363638";
					//document.all.item("td_"+mId).style.backgroundColor="#D8E3FC";
                    mLastMenuId=mId;
				}
				function restoreMenuColor(mId){
					if(mId!=null)
						if(mId!=""){
							document.all.item(mId).className="DashNavLink";
						}
				}
				function menumouseout(mTd){
					return;
					if(mTd.id!=("td_"+mLastMenuId))
						mTd.style.backgroundColor='#003399';
					else
						mTd.style.backgroundColor="#D8E3FC";
				}
				function menumouseover(mTd){
					return;
					mTd.style.backgroundColor='#34cafc';
				}
        function window_onload()
        {
	        <xsl:if test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DAV:HasChild[.='1'] and count(/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[DAV:iscollection='1' and DAV:parentname[.!='']])>0">
	  				<xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[DAV:iscollection='1' and DAV:parentname[.!='']]" >
						var rootId='<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardID" />';
						if('<xsl:value-of select="DAV:parentname"/>'==rootId)
							addNacTd(null,'<xsl:value-of select="DDF:ID"/>','<xsl:value-of select="WP:Title"/>',<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardType" />);
						else
							addNacTd('<xsl:value-of select="DAV:parentname"/>','<xsl:value-of select="DDF:ID"/>','<xsl:value-of select="WP:Title"/>',<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardType" />);
						</xsl:for-each>

		              initTable();
			          objiFrame = document.all.item("MoveiFrame");

                      objiFrame.style.position = "absolute";
                      objiFrame.style.left = initLeft;
                      objiFrame.style.top = initTop;
                      objiFrame.style.width = iTotalWidth;
                      objiFrame.style.height = iTotalHeight;
		      	<xsl:if test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DAV:isshowtree[.='1']">
		      		document.all.item("MoveiFrame").style.display="none";
		      	</xsl:if>
                 <xsl:choose>
                   <xsl:when test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DAV:isshowtree[.!='1']">document.all.item("treeviewbutton").title='显示菜单树'</xsl:when>
                   <xsl:otherwise>document.all.item("treeviewbutton").title='隐藏菜单树'</xsl:otherwise>
                 </xsl:choose>
        	</xsl:if>
         	changeMenuColor(m_CurrentBoardID+"_"+m_CurrentBoardType);
         	ifrmDashBoardContent.window.location = "dashboardEx.jsp?DashboardID=" + m_CurrentBoardID + "&amp;DashboardType=" + m_CurrentBoardType;
        }
	function ModuleNodeOnClick(modu_id,modu_type){
		document.all.ifrmDashBoardContent.src = "dashboardEx.jsp?DashboardID=" + modu_id + "&amp;DashboardType=" + modu_type;
	}
        </SCRIPT>
		</HEAD>
		<BODY style="overflow-y:hidden;" topmargin="0" leftmargin="0" bottommargin="0" rightmargin="0" bgcolor="White" scroll="auto">
			<!-- There should only be one dashboard in our XML, but
					 just in case, select the first one explicitly -->
			<xsl:for-each select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop">

			<!-- Outermost Dashboard table start. Each section
					(title, navigation, layout) contained in a row -->
			<table class="Tableblock" border="0" cellpadding="0" cellspacing="0" width="100%" height="100%">
				<!-- Toolbar Start -->
				<xsl:if test="WP:DisplayTitle[.=1]">
				<tr> 
				    <td width="100%" class="Toptr"><table width="100%" border="0" cellspacing="0" cellpadding="0">
				        <tr> 
				          <td style="padding-left:15px"><strong>
				          当前用户:</strong><xsl:value-of select="/DAV:multistatus/Login_User_Name"/><strong>
				          用户职位:</strong><xsl:value-of select="/DAV:multistatus/Login_User_Position"/><strong>
				          所在部门:</strong><xsl:value-of select="/DAV:multistatus/Login_User_Department"/></td>
				          <td><p align="right">
				            <a href="portallogout" class="Topword">退出</a></p></td>
				        </tr>
				      </table></td>
				  </tr>
				  <tr> 
				    <td class="Banner"><table width="100%" border="0" cellspacing="0" cellpadding="0">
				        <tr> 
				          <td><img src="images/logo.gif"/></td>
				          <td><p align="right"><img src="images/banner.gif"/></p></td>
				        </tr>
				      </table>
				      </td>
				  </tr>
				</xsl:if>
				<!-- Toolbar End -->
				<!-- Dashboard Navigation Start -->
				<xsl:if test="/DAV:multistatus/DAV:response[position() > 0]/DAV:propstat/DAV:prop[DAV:iscollection='1']">

					<!-- Start by writing out the visible navigation area -->
					<tr><td class="navbg">
					<TABLE cellspacing="0" cellpadding="0">
						<TR>
							<!-- Show the icon to show and hide the treeview -->
							<xsl:if test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DAV:HasChild[.='1'] and count(/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[DAV:iscollection='1' and DAV:parentname[.!='']])>0">
								<TD width="10px" >
									<img ID="treeviewbutton" border="0" >
										<xsl:attribute name="style">{cursor:hand}</xsl:attribute>
										<xsl:attribute name="src">
											<xsl:choose>
													<xsl:when test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DAV:isshowtree[.!='1']">images/showtree.gif</xsl:when>
													<xsl:otherwise>images/hidetree.gif</xsl:otherwise>
											</xsl:choose>
			  						</xsl:attribute>
										<xsl:attribute name="onclick">OnTreeViewButtonClick()</xsl:attribute>
									</img>
								</TD>
							</xsl:if>
							<!-- Write out the top Dashboards -->
							<xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[DAV:iscollection='1' and DAV:parentname[.='']]" >
								<xsl:sort select="number(WP:DashboardOrder)" order="ascending"  data-type="number"/>
								<TD onmouseover="javascript:menumouseover(this);" onmouseout="javascript:menumouseout(this);">
									<xsl:attribute name="class">
										<xsl:choose>
											<xsl:when test="DAV:href[.=/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop/DAV:href]">DashNavCurDashArea</xsl:when>
											<xsl:otherwise>DashNavDashArea</xsl:otherwise>
										</xsl:choose>
									</xsl:attribute>
									<xsl:attribute name="id">td_<xsl:value-of select="DAV:href"/></xsl:attribute>
									<A class="DashNavLink" >
										<xsl:attribute name="id"><xsl:value-of select="DAV:href"/></xsl:attribute>
										<xsl:attribute name="href"><xsl:value-of select="DDF:URL"/></xsl:attribute>
										<!-- <xsl:attribute name="title"><xsl:value-of select="WP:Description" /></xsl:attribute>-->
										<xsl:if test="WP:DashboardImageSmall[.!='']">
											<img border="0"><xsl:attribute name="src"><xsl:value-of select="WP:DashboardImageSmall"/></xsl:attribute></img>
										</xsl:if>
										<nobr><xsl:value-of select="WP:Title"/></nobr>
									</A>
								</TD>
								<xsl:if test="not(position()=last())">
                                <td class="Dashline">|</td>
                                </xsl:if>
							</xsl:for-each>
						</TR>
					</TABLE>
					</td>
                    </tr>
                    <tr><td style="background-color:red;"></td></tr>
				</xsl:if>
				<!-- Dashboard Navigation End	-->
				<!-- Treeview Start -->
				<TR height="100%">
					<TD valign="top" height="100%">
					<TABLE id="DetailTable" cellpadding="0" cellspacing="0" width="100%" height="100%" border="0">
					<!-- Decide if the current top dashboard has childs and childs node is more than one -->
					<xsl:if test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DAV:HasChild[.='1'] and count(/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[DAV:iscollection='1' and DAV:parentname[.!='']])>0">
						<TD valign="top" height="100%">
							<!--<table cellpadding="0" cellspacing="0" border="0"><tr><td class="DashPartSpacing" height="100%" ></td></tr></table>-->
							<div id="divLeftMenu" >
							<xsl:attribute name="style">
								<xsl:choose>
									<xsl:when test="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DAV:isshowtree[.!='1']">OVERFLOW-Y:auto;OVERFLOW-X:auto;WIDTH:0px;height=100%</xsl:when>
									<xsl:otherwise>OVERFLOW-Y:auto;OVERFLOW-X:auto;WIDTH:160px;height=100%</xsl:otherwise>
								</xsl:choose>
  							</xsl:attribute>
							<xsl:attribute name="class">treeDiv</xsl:attribute>
							</div>
                              <SCRIPT LANGUAGE="JavaScript">
                                  var oTree,ifExpandTree = 0;
                                  oTree = new tTree();
                                  var rootId='<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardID" />';
                                  var rootId='<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardID" />';
                                  <!-- Add all child dashboards  -->
                                  <xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[DAV:iscollection='1' and DAV:parentname[.!='']]" >
                                  if('<xsl:value-of select="DAV:parentname"/>'==rootId)
                                      addNode("<xsl:value-of select="DDF:ID"/>","","<xsl:value-of select="WP:Title"/>","ModuleNodeOnClick('<xsl:value-of select="DDF:ID"/>','<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardType" />')");
                                  else
                                      addNode("<xsl:value-of select="DDF:ID"/>","<xsl:value-of select="DAV:parentname"/>","<xsl:value-of select="WP:Title"/>","ModuleNodeOnClick('<xsl:value-of select="DDF:ID"/>','<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:DashboardType" />')");
                                  </xsl:for-each>
                                  initTree("divLeftMenu",0);
                              </SCRIPT>
						</TD>
					</xsl:if>
					<td width="100%">
					<IFRAME style="overflow-x:hidden;" scrolling="auto" id="ifrmDashBoardContent" name="ifrmDashBoardContent" frameborder="0" width="100%" height="97.5%" src="">';
					</IFRAME>
					</td>
				<!-- Dashboard Zones End -->

			<!-- Outer Dashboard table End -->


			<SCRIPT language="javascript">
				<xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[DAV:contentclass='urn:schemas-microsoft-com:webpart']">
					var varPart<xsl:value-of select="DDF:WebPartQualifier"/> = new tPart(
						<xsl:value-of select="DDF:DashboardID"/>,
						<xsl:value-of select="DDF:DashboardType"/>,
						<xsl:value-of select="DDF:PartID"/>,
						<xsl:value-of select="DDF:PartType"/> );
				</xsl:for-each>
			</SCRIPT>
		</TABLE>
		</TD></TR>
		</table>
		</xsl:for-each>
		</BODY>
	<iframe id="MoveiFrame" height="0" marginWidth="0" marginHeight="0" frameBorder="0"></iframe>
	<div id="SubDiv" name="SubDiv"></div>
	</HTML>

	</xsl:template>

	<xsl:template match="WP:HasFrame[.='0']">

		<xsl:apply-templates select="../."/>

	</xsl:template>
	<xsl:template match="WP:StylesheetLink">
		<LINK ID="lnkStylesheet" rel="stylesheet" >
			<xsl:attribute name="href">
				<xsl:choose><xsl:when test="text()[.!='']"><xsl:value-of select="text()"/></xsl:when><xsl:otherwise>midnight.css</xsl:otherwise></xsl:choose>
			</xsl:attribute>
		</LINK>
	</xsl:template>

	<xsl:template match="WP:Stylesheet">
		<STYLE type="text/css" rel="stylesheet">
			<xsl:value-of select="text()"/>
		</STYLE>
	</xsl:template>

</xsl:stylesheet>

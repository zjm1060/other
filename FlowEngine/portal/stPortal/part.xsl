<?xml version='1.0'?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:WP="urn:schemas-microsoft-com:webpart:" xmlns:DAV="DAV:" xmlns:DDF="urn:schemas-microsoft-com:dashboardfactory:">
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
  <script type="text/javascript" src="dbview_ie.js"></script>
  <!-- Insert some Java script-->
  <SCRIPT type="text/javascript">
    // Dashboard ID
    var varDashboardID = "<xsl:value-of select="/DAV:multistatus/DAV:response[1]/DAV:propstat/DAV:prop/DDF:ID"/>";
    var varStoreASP = "<xsl:value-of select="/DAV:multistatus/DDF:Settings/DDF:StoreASP"/>";
    // Initialize the Digital Dashboard Service Component Object
    <!--DDSC.init(varDashboardID, document, ""); -->
    var L_MinimizeTip_Text = "<xsl:value-of select="//String[@_locID='L_MinimizeTip_Text']"/>";
    var L_RestoreTip_Text = "<xsl:value-of select="//String[@_locID='L_RestoreTip_Text']"/>";
    var L_TreeViewShow_Text = "<xsl:value-of select="//String[@_locID='L_TreeViewShow_Text']"/>";
    var L_TreeViewHide_Text = "<xsl:value-of select="//String[@_locID='L_TreeViewHide_Text']"/>";

    <xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[DAV:contentclass='urn:schemas-microsoft-com:webpart']">
            var varPart<xsl:value-of select="DDF:WebPartQualifier"/> = new tPart(
                '<xsl:value-of select="DDF:DashboardID"/>',
                <xsl:value-of select="DDF:DashboardType"/>,
                '<xsl:value-of select="DDF:PartID"/>',
                <xsl:value-of select="DDF:PartType"/>,
                '<xsl:value-of select="WP:Title"/>' );
                <xsl:if test="WP:RefreshInterval[.!=0]">
                  var src<xsl:value-of select="DDF:WebPartQualifier"/>, iFrame<xsl:value-of select="DDF:WebPartQualifier"/>;
                  src<xsl:value-of select="DDF:WebPartQualifier"/> = '<xsl:value-of select="WP:ContentLink"/>';
                  iFrame<xsl:value-of select="DDF:WebPartQualifier"/> = 'WebPart<xsl:value-of select="DDF:WebPartQualifier"/>';
                  setInterval('AutoRefresh(iFrame<xsl:value-of select="DDF:WebPartQualifier"/>,src<xsl:value-of select="DDF:WebPartQualifier"/> )',<xsl:value-of select="WP:RefreshInterval"/> * 1000 );
                </xsl:if>
    </xsl:for-each>
    function AutoRefresh(iFrame,src){
      eval(iFrame).location=src;
    }
  </SCRIPT>
  </HEAD>
  <BODY topmargin="0" leftmargin="0" bgcolor="White" >
    <xsl:attribute name="scroll">
            <xsl:if test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop/WP:RequiresIsolation[.='0']">no</xsl:if>
            <xsl:if test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop/WP:RequiresIsolation[.='1']">auto</xsl:if>
    </xsl:attribute>
    <!-- Outermost Dashboard table start. Each section
                    (title, navigation, layout) contained in a row -->
    <table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%">
    <tr><td width="100%">
      <!-- Header -->
      <xsl:if test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=4]">
		<TABLE id="HeaderSection" cellpadding="0px" cellspacing="0px" width="100%" border="0">
        <xsl:choose>
            <xsl:when test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=0 or WP:Zone=1 or WP:Zone=2 or WP:Zone=3 or WP:Zone=5]"/>
            <xsl:otherwise>
				<xsl:attribute name="height">100%</xsl:attribute>
            </xsl:otherwise>
        </xsl:choose>
          <tr>
            <td class="DashZoneTop">
              <xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=4]">
                <xsl:sort select="number(WP:PartOrder)"/>
                <xsl:apply-templates select="./WP:HasFrame"/>
                <!-- Add some space between parts -->
                <xsl:if test="not(position()=last())">
                  <table border="0"><tr><td class="DashPartSpacing"></td></tr></table>
                </xsl:if>
              </xsl:for-each>
            <!-- Close off the HeaderSection row -->
            </td>
          </tr>
        </TABLE>
      </xsl:if>
      <!-- Three Body Sections -->
      <xsl:if test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=0 or WP:Zone=1 or WP:Zone=2 or WP:Zone=3]">
        <TABLE id="BodySections" width="100%" height="100%" border="0" cellpadding="0px" cellspacing="0px" ><TR valign="top">
          <!-- Left Side -->
          <xsl:if test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=1]">
            <TD class="DashZoneLeft">
              <xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=1]">
                <xsl:sort select="number(WP:PartOrder)"/>
                <xsl:apply-templates select="./WP:HasFrame"/>
                <!-- Add some space between parts-->
                <xsl:if test="not(position()=last())">
                  <table cellpadding="0" cellspacing="0" border="0"><tr><td class="DashPartSpacing"></td></tr></table>
                </xsl:if>

              </xsl:for-each>
            </TD>
          </xsl:if>
          <!-- Middle -->
          <xsl:if test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=0 or ./WP:Zone=3]">
            <TD class="DashZoneCenter">
              <xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=0 or ./WP:Zone=3]">
                <xsl:sort select="number(WP:PartOrder)"/>
                <xsl:apply-templates select="./WP:HasFrame"/>
                <!-- Add some space between parts-->
                <xsl:if test="not(position()=last())">
                  <table cellpadding="0" cellspacing="0" border="0"><tr><td class="DashPartSpacing"></td></tr></table>
                </xsl:if>

              </xsl:for-each>
            </TD>
          </xsl:if>
          <!-- Right Side -->
          <xsl:if test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=2]">
            <TD class="DashZoneRight">
              <xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=2]">
                <xsl:sort select="number(WP:PartOrder)"/>
                <xsl:apply-templates select="./WP:HasFrame"/>
                <!-- Add some space between parts-->
                <xsl:if test="not(position()=last())">
                  <table cellpadding="0" cellspacing="0" border="0"><tr><td class="DashPartSpacing"></td></tr></table>
                </xsl:if>

              </xsl:for-each>
            </TD>
          </xsl:if>
        </TR>
      </TABLE>
      </xsl:if>
      <!-- Footer -->
      <xsl:if test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=5]">
      <TR>
      <TD valign="top">
        <TABLE id="FooterSection" cellpadding="0px" cellspacing="0px" width="100%" border="0">
          <xsl:choose>
            <xsl:when test="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=0 or WP:Zone=1 or WP:Zone=2 or WP:Zone=3 or WP:Zone=4]"/>
            <xsl:otherwise>
				<xsl:attribute name="height">100%</xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
          <tr><td class="DashZoneBottom">
            <xsl:for-each select="/DAV:multistatus/DAV:response/DAV:propstat/DAV:prop[WP:Zone=5]">
              <xsl:sort select="number(WP:PartOrder)"/>
              <xsl:apply-templates select="./WP:HasFrame"/>
              <!--<xsl:apply-templates select="DDF:PartHTML"/>-->
              <!--<xsl:apply-templates select="."/>-->
            </xsl:for-each>
            </td>
          </tr>
        </TABLE>
      </TD>
      </TR>
    </xsl:if>
      <!-- Dashboard Zones End -->
    </td></tr>
    </table>
    <!-- Outer Dashboard table End -->
  </BODY>
</HTML>
</xsl:template>
<xsl:template match="WP:HasFrame[.='0']">
  <xsl:apply-templates select="../."/>
</xsl:template>
<xsl:template match="WP:HasFrame[.='1']">
  <table border="0" cellspacing="0" cellpadding="0">
	<xsl:attribute name="style">width:<xsl:choose><xsl:when test="../WP:Width[.!='']"><xsl:value-of select="../WP:Width"/>;</xsl:when><xsl:otherwise>100%;</xsl:otherwise></xsl:choose>
    	<xsl:choose><xsl:when test="../WP:Height[.!=''] and ../WP:Height[.!='100%']">height:<xsl:value-of select="../WP:Height"/>;</xsl:when><xsl:otherwise>height:<xsl:choose><xsl:when test="../WP:Zone[.=4] or ../WP:Zone[.=5]">20mm</xsl:when><xsl:otherwise>100%</xsl:otherwise></xsl:choose></xsl:otherwise></xsl:choose>
	</xsl:attribute>
    <xsl:if test="../WP:IsVisible[.='0']"><xsl:attribute name="style">display:none</xsl:attribute></xsl:if>
    <tr valign="top">
      <td class="DashPartFrame">
      <!-- Header Start -->
      <DIV>
      <table height="100%" cellspacing="0" cellpadding="0" width="100%" border="0">
        <tr>
		<td><!--class="DashPartTitle"-->
          <table border="0" cellspacing="0" cellpadding="0" width="100%" >
          <tr>
          <td class="PartTitleLeft">
            <xsl:choose>
              <xsl:when test="../WP:DetailLink[.!='']">
                <a target="_blank"><xsl:attribute name="href"><xsl:value-of select="../WP:DetailLink"/></xsl:attribute><nobr><xsl:value-of select="../WP:Title"/><xsl:text></xsl:text></nobr></a>
              </xsl:when>
              <xsl:otherwise>
                <nobr><xsl:value-of select="../WP:Title"/></nobr>
              </xsl:otherwise>
            </xsl:choose>
          </td>
          <td nowrap="" class="PartTitleMiddle">
            <!-- <xsl:eval no-entities="t">"&amp;nbsp;"</xsl:eval> -->
            <xsl:if test="../WP:CustomizationLink[.!='']">
              <a target="_blank"><xsl:attribute name="href"><xsl:value-of select="../WP:CustomizationLink"/></xsl:attribute><xsl:attribute name="title"><xsl:value-of select="//String[@_locID='L_CustomizationLinkTip_Text']"/></xsl:attribute>
                <img style="cursor:hand" border="0" alt="编辑">
                <xsl:attribute name="src">/images/edit.gif</xsl:attribute>
                </img>
              </a>
            </xsl:if>
            <xsl:if test="../WP:RequiresIsolation[.!='0']">
              <img class="DashPartLayout" style="cursor:hand;" border="0" alt="页面参数配置">
                <!--<xsl:attribute name="title"><xsl:value-of select="//String[@_locID='L_FuncModify_Text']"/></xsl:attribute>-->
                <xsl:attribute name="onclick">
                  varPart<xsl:value-of select="../DDF:WebPartQualifier"/>.OnLayoutClick();
                </xsl:attribute>
                <xsl:attribute name="src"><xsl:text>./images/funcmodify.gif</xsl:text></xsl:attribute>
              </img>
              <img class="DashPartNewWindow" style="cursor:hand;" border="0" src="./images/newwindow.gif" alt="在新窗口打开">
                <!--<xsl:attribute name="title"><xsl:value-of select="//String[@_locID='L_NewWindowTip_Text']"/></xsl:attribute>-->
                <xsl:attribute name="onclick">
                  varPart<xsl:value-of select="../DDF:WebPartQualifier"/>.OnNewWindow();
                </xsl:attribute>
              </img>
              <img class="DashPartRefresh" style="cursor:hand;" border="0" src="./images/refresh.gif" alt="刷新页面">
                <!--<xsl:attribute name="title"><xsl:value-of select="//String[@_locID='L_RefreshTip_Text']"/></xsl:attribute>-->
                <xsl:attribute name="onclick">
                  varPart<xsl:value-of select="../DDF:WebPartQualifier"/>.OnRefreshClick();
                </xsl:attribute>
              </img>
            </xsl:if>
            <xsl:if test="../DDF:CanPersonalize[.='1']">
              <xsl:if test="../WP:AllowMinimize[.='1']">
                <xsl:choose>
                  <xsl:when test="../WP:FrameState[.='0']">
                    <img border="0" src="./images/minus.gif" style="cursor:hand;" alt="最小化页面">
                      <xsl:attribute name="onclick">
                        varPart<xsl:value-of select="../DDF:WebPartQualifier"/>.OnMinRestoreClick(document.all.item(&quot;WebPart<xsl:value-of select="../DDF:WebPartQualifier"/>&quot;));
                      </xsl:attribute>
                      <!--<xsl:attribute name="title"><xsl:value-of select="//String[@_locID='L_MinimizeTip_Text']"/></xsl:attribute>-->
                    </img>
                  </xsl:when>
                  <xsl:otherwise>
                    <img border="0" src="./images/plus.gif" style="cursor:hand;" alt="恢复页面">
                    <xsl:attribute name="onclick">
                      varPart<xsl:value-of select="../DDF:WebPartQualifier"/>.OnMinRestoreClick(document.all.item(&quot;WebPart<xsl:value-of select="../DDF:WebPartQualifier"/>&quot;));
                    </xsl:attribute>
                    <!--<xsl:attribute name="title"><xsl:value-of select="//String[@_locID='L_RestoreTip_Text']"/></xsl:attribute>-->
                    </img>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:if>
              <xsl:if test="../WP:AllowRemove[.='1']">
                <img border="0" src="./images/part_close.gif" alt="关闭页面" style="cursor:hand;">
                  <xsl:attribute name="onclick">
                    varPart<xsl:value-of select="../DDF:WebPartQualifier"/>.OnCloseClick(document.all.item(&quot;WebPart<xsl:value-of select="../DDF:WebPartQualifier"/>&quot;));
                  </xsl:attribute>
                  <!--<xsl:attribute name="title"><xsl:value-of select="//String[@_locID='L_CloseTip_Text']"/></xsl:attribute>-->
                </img>
              </xsl:if>
            </xsl:if>
          </td>
          <td width="6" class="PartRightCornerImage">
          	<img border="0" src="./images/part_top_rightt_corner.gif"></img>
          </td>
        </tr>
		</table>
		</td>
		</tr>
		<tr>
			<td class="PartIFrame" height="100%"><xsl:apply-templates select="../."/></td>
		</tr>
      </table>
      </DIV>
      <!-- Header End -->
    </td>
  </tr>
</table>
</xsl:template>
<xsl:template match="DAV:prop">
  <xsl:choose>
    <xsl:when test="WP:RequiresIsolation[.='0']">
      <div class="DashPartBody">
        <iframe frameborder="0" scrolling="Auto" height="100%" width="100%">
        <xsl:if test="./WP:FrameState[.!='0']">
				<xsl:attribute name="style">display:none;</xsl:attribute>
        </xsl:if>
        <xsl:attribute name="id">WebPart<xsl:value-of select="DDF:WebPartQualifier"/></xsl:attribute>
        <xsl:if test="WP:IsVisible[.='0']"><xsl:attribute name="style">display:none</xsl:attribute></xsl:if>
        <!-- Size the DIV according to Height and Width defined
         on the part itself.  If not height and/or width, use
         default height and/or width -->
        <xsl:if test="./WP:HasFrame[.!='1']">
        	<xsl:attribute name="style">width:<xsl:choose><xsl:when test="WP:Width[.!='']"><xsl:value-of select="WP:Width"/>;</xsl:when><xsl:otherwise>100%;</xsl:otherwise></xsl:choose>
				<xsl:choose><xsl:when test="WP:Height[.!=''] and WP:Height[.!='100%']">height:<xsl:value-of select="WP:Height"/>;</xsl:when><xsl:otherwise>height:<xsl:choose><xsl:when test="WP:Zone[.=4] or WP:Zone[.=5]">20mm</xsl:when><xsl:otherwise>100%</xsl:otherwise></xsl:choose></xsl:otherwise></xsl:choose>
        	</xsl:attribute>
		</xsl:if>
        <xsl:apply-templates select="WP:Content"/>
        <xsl:attribute name="src"><xsl:value-of select="WP:ContentLink"/></xsl:attribute>
        </iframe>
      </div>
    </xsl:when>
    <xsl:otherwise>
      <iframe frameborder="0" scrolling="Auto" height="100%" width="100%">
		<xsl:if test="./WP:FrameState[.!='0']">
				<xsl:attribute name="style">display:none;</xsl:attribute>
        </xsl:if>
        <xsl:attribute name="id">WebPart<xsl:value-of select="DDF:WebPartQualifier"/></xsl:attribute>
        <xsl:if test="WP:IsVisible[.='0']"><xsl:attribute name="style">display:none</xsl:attribute></xsl:if>
        <!-- Size the IFRAME according to Height and Width defined
         on the part itself.  If not height and/or width, use
         default height and/or width -->
		<xsl:if test="./WP:HasFrame[.!='1']">
        	<xsl:attribute name="style">width:<xsl:choose><xsl:when test="WP:Width[.!='']"><xsl:value-of select="WP:Width"/>;</xsl:when><xsl:otherwise>100%;</xsl:otherwise></xsl:choose><xsl:choose><xsl:when test="WP:Height[.!='']">height:<xsl:value-of select="WP:Height"/>;</xsl:when><xsl:otherwise></xsl:otherwise></xsl:choose>
        	</xsl:attribute>
		</xsl:if>
        <xsl:attribute name="scroll">
          <xsl:if test="WP:RequiresIsolation[.='0']">no</xsl:if>
          <xsl:if test="WP:RequiresIsolation[.='1']">auto</xsl:if>
        </xsl:attribute>
        <xsl:attribute name="src"><xsl:value-of select="WP:ContentLink"/></xsl:attribute>
        <xsl:value-of select="//String[@_locID='L_NoIframesWarning_Text']"/>
      </iframe>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>
<xsl:template match="WP:Content">
  <!--<xsl:eval no-entities="t">this.nodeTypedValue</xsl:eval>-->
  <xsl:apply-templates/>
</xsl:template>
<xsl:template match="WP:StylesheetLink">
  <LINK ID="lnkStylesheet" rel="stylesheet">
    <xsl:attribute name="href">
      <xsl:choose><xsl:when test="text()[.!='']"><xsl:value-of select="text()"/></xsl:when><xsl:otherwise>standard.css</xsl:otherwise></xsl:choose>
    </xsl:attribute>
  </LINK>
</xsl:template>
<xsl:template match="WP:Stylesheet">
  <STYLE type="text/css" rel="stylesheet">
    <xsl:value-of select="text()"/>
  </STYLE>
</xsl:template>
</xsl:stylesheet>

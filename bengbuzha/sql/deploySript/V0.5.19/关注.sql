USE [xmis]
GO

/****** Object:  Table [xmis].[wf_关注]    Script Date: 04/26/2012 15:31:53 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [xmis].[wf_关注](
	[ID] [nvarchar](50) NOT NULL,
	[模块] [nvarchar](50) NOT NULL,
	[应用ID] [nvarchar](50) NULL,
	[链接地址] [nvarchar](500) NULL,
	[用户ID] [nvarchar](32) NOT NULL,
	[标题] [nvarchar](200) NULL,
	[创建时间] [datetime] NULL,
	[是否有效] [bit] NULL,
 CONSTRAINT [PK_wf_关注] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
)
GO

USE [xmis]
GO

/****** Object:  Table [xmis].[prj_项目物资清单]    Script Date: 04/27/2012 10:13:25 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO
DROP TABLE [xmis].[prj_项目物资清单]
CREATE TABLE [xmis].[prj_项目物资清单](
	[项目物资ID] [nvarchar](40) NOT NULL,
	[设备名称] [nvarchar](40) NOT NULL,
	[项目ID] [nvarchar](32) NULL,
	[数量] [smallint] NOT NULL,
	[型号] [nvarchar](32) NULL,
	[规格] [nvarchar](32) NULL,
	[重量] [nvarchar](32) NULL,
	[参考价格] [float] NULL,
	[货币ID] [smallint] NULL,
	[最后更新人] [int] NOT NULL,
	[更新时间] [smalldatetime] NOT NULL,
	[状态] [nvarchar](10) NULL,
	[最后修改日期] [datetime] NULL,
	[最后修改人ID] [int] NULL,
 CONSTRAINT [PK_prj_项目物资清单] PRIMARY KEY CLUSTERED 
(
	[项目物资ID] ASC
)
)

GO
USE [xmis]
GO

/****** Object:  Table [xmis].[dev_采购清单]    Script Date: 04/27/2012 10:24:58 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO
DROP  TABLE [xmis].[dev_采购清单]
CREATE TABLE [xmis].[dev_采购清单](
	[ID] [nvarchar](40) NOT NULL,
	[采购清单编码] [nvarchar](50) NULL,
	[供货商ID] [int] NULL,
	[备注] [nvarchar](1000) NULL,
	[填表时间] [datetime] NULL,
	[填表人ID] [int] NULL,
	[最后修改日期] [datetime] NULL,
	[最后修改人ID] [int] NULL,
 CONSTRAINT [PK_dev_采购清单] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
) ON [PRIMARY]

GO

USE [xmis]
GO

/****** Object:  Table [xmis].[dev_采购清单明细]    Script Date: 04/27/2012 10:25:43 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [xmis].[dev_采购清单明细](
	[ID] [nvarchar](40) NOT NULL,
	[清单ID] [nvarchar](40) NULL,
	[申请明细ID] [nvarchar](40) NULL,
	[申请ID] [nvarchar](40) NULL,
	[项目物资ID] [nvarchar](40) NULL,
	[名称] [nvarchar](100) NULL,
	[数量] [int] NOT NULL,
	[型号] [nvarchar](50) NULL,
	[规格] [nvarchar](50) NULL,
	[重量] [nvarchar](50) NULL,
	[技术要求] [nvarchar](500) NULL,
	[期望到货日期] [datetime] NULL,
	[备注] [nvarchar](500) NULL,
	[最后修改日期] [datetime] NULL,
	[最后修改人ID] [int] NULL,
 CONSTRAINT [PK_dev_采购清单明细] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
) ON [PRIMARY]

GO

USE [xmis]
GO

/****** Object:  Table [xmis].[dev_采购申请]    Script Date: 04/27/2012 10:26:28 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [xmis].[dev_采购申请](
	[ID] [nvarchar](40) NOT NULL,
	[项目ID] [nvarchar](40) NULL,
	[备注] [nvarchar](500) NULL,
	[申请人ID] [nvarchar](50) NULL,
	[申请时间] [datetime] NULL,
	[状态] [nvarchar](50) NULL,
	[申请人部门ID] [int] NULL,
	[最后修改日期] [datetime] NULL,
	[最后修改人ID] [int] NULL,
 CONSTRAINT [PK_dev_采购申请] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
) ON [PRIMARY]

GO


USE [xmis]
GO

/****** Object:  Table [xmis].[dev_采购申请明细]    Script Date: 04/27/2012 10:26:51 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO
DROP TABLE [xmis].[dev_采购申请明细]
CREATE TABLE [xmis].[dev_采购申请明细](
	[ID] [nvarchar](40) NOT NULL,
	[项目物资ID] [nvarchar](40) NULL,
	[申请ID] [nvarchar](40) NOT NULL,
	[名称] [nvarchar](100) NULL,
	[数量] [int] NOT NULL,
	[型号] [nvarchar](50) NULL,
	[规格] [nvarchar](50) NULL,
	[重量] [nvarchar](50) NULL,
	[参考价格] [float] NULL,
	[货币ID] [nvarchar](40) NULL,
	[技术要求] [nvarchar](500) NULL,
	[期望到货日期] [datetime] NULL,
	[备注] [nvarchar](500) NULL,
	[最后修改日期] [datetime] NULL,
	[最后修改人ID] [int] NULL,
 CONSTRAINT [PK_dev_采购申请明细] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
) ON [PRIMARY]

GO


USE [xmis]
GO

/****** Object:  Table [xmis].[dev_物资信息库]    Script Date: 04/27/2012 10:28:57 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO
DROP TABLE [xmis].[dev_物资信息库]
CREATE TABLE [xmis].[dev_物资信息库](
	[物资ID] [nvarchar](40) NOT NULL,
	[物资名称] [nvarchar](40) NOT NULL,
	[型号] [nvarchar](40) NULL,
	[规格] [nvarchar](40) NULL,
	[重量] [nvarchar](40) NULL,
	[功率] [nvarchar](40) NULL,
	[价格] [float] NULL,
	[货币ID] [smallint] NULL,
	[停产时间] [smalldatetime] NULL,
	[三包信息] [nvarchar](200) NULL,
	[最后更新人ID] [int] NOT NULL,
	[更新时间] [smalldatetime] NOT NULL,
	[最后修改日期] [datetime] NULL,
	[最后修改人ID] [int] NULL,
 CONSTRAINT [PK_dev_物资信息库] PRIMARY KEY CLUSTERED 
(
	[物资ID] ASC
)
) ON [PRIMARY]

GO


USE [xmis]
GO

/****** Object:  Table [xmis].[prj_项目物资采购状态]    Script Date: 04/27/2012 10:30:57 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [xmis].[prj_项目物资采购状态](
	[ID] [nvarchar](40) NOT NULL,
	[项目设备ID] [nvarchar](40) NOT NULL,
	[采购申请明细ID] [nvarchar](40) NOT NULL,
	[数量] [int] NOT NULL,
	[状态] [nvarchar](10) NULL,
	[最后修改日期] [datetime] NULL,
	[最后修改人ID] [int] NULL,
 CONSTRAINT [PK_prj_项目物资采购状态] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
) ON [PRIMARY]

GO






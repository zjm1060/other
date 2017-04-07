USE [xmis]
GO

/****** Object:  Table [xmis].[wf_待办]    Script Date: 03/27/2012 15:53:12 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [xmis].[wf_待办](
	[ID] [nvarchar](32) NOT NULL,
	[模块] [nvarchar](50) NULL,
	[应用ID] [nvarchar](50) NULL,
	[用户ID] [nvarchar](32) NOT NULL,
	[标题] [nvarchar](200) NOT NULL,
	[链接地址] [nvarchar](1000) NOT NULL,
	[脚本] [nvarchar](100) NULL,
	[优先级] [smallint] NULL,
	[创建时间] [datetime] NULL,
	[状态] [smallint] NULL,
 CONSTRAINT [PK_wf_todo] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
)

GO

CREATE TABLE [xmis].[wf_日历](
	[ID] [nvarchar](32) NOT NULL,
	[日期] [nvarchar](8) NOT NULL,
	[类型] [smallint] NULL,
	[星期] [smallint] NULL,
	[上午上班] [nvarchar](5) NULL,
	[上午下班] [nvarchar](5) NULL,
	[下午上班] [nvarchar](5) NULL,
	[下午下班] [nvarchar](5) NULL,
 CONSTRAINT [PK_sys_日历] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
)

GO

DROP TABLE [xmis].[prj_项目任务成员]
GO

USE [xmis]
GO

/****** Object:  Table [xmis].[prj_项目任务成员]    Script Date: 03/27/2012 17:30:46 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [xmis].[prj_项目任务成员](
	[ID] [nvarchar](32) NOT NULL,
	[项目任务ID] [nvarchar](32) NULL,
	[项目成员ID] [int] NOT NULL,
	[承担任务] [nvarchar](200) NULL,
	[是否负责人] [bit] NOT NULL,
	[备注] [nvarchar](400) NULL,
 CONSTRAINT [PK_prj_项目任务成员] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)
)

GO
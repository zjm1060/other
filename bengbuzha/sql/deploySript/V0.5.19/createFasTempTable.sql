USE [xmis]
GO

/****** Object:  Table [xmis].[fas_glInfo]    Script Date: 04/26/2012 15:36:04 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [xmis].[fas_glInfo]
    (
      [project_code] [nvarchar](100) NULL ,
      [MB] [money] NULL ,
      [pay_amt] [money] NULL
    )
ON  [PRIMARY]

GO




USE [xmis]
GO

/****** Object:  Table [xmis].[fas_qch]    Script Date: 04/26/2012 16:24:20 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [xmis].[fas_qch]
    (
      [citem_id] [varchar](20) NULL ,
      [income_mb] [money] NULL
    )
ON  [PRIMARY]

GO

SET ANSI_PADDING OFF
GO





USE [xmis]
GO

/****** Object:  Table [xmis].[fas_income]    Script Date: 04/26/2012 15:38:24 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [xmis].[fas_income]
    (
      [year] [int] NOT NULL ,
      [iperiod] [tinyint] NOT NULL ,
      [ino_id] [smallint] NULL ,
      [citem_id] [varchar](20) NULL ,
      [income_amt] [money] NULL ,
      [project_code] [nvarchar](100) NULL
    )
ON  [PRIMARY]

GO

SET ANSI_PADDING OFF
GO


USE [xmis]
GO

/****** Object:  Table [xmis].[fas_qcjcb]    Script Date: 04/26/2012 16:25:14 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [xmis].[fas_qcjcb]
    (
      [citem_id] [varchar](20) NULL ,
      [payout_mb] [money] NULL
    )
ON  [PRIMARY]

GO

SET ANSI_PADDING OFF
GO








--1) org_公司
INSERT INTO xmis.org_业务关系
        ( 业务关系ID, 关系名称, 关系描述 )
VALUES  ( -1, -- 业务关系ID - smallint
          '', -- 关系名称 - char(50)
          ''  -- 关系描述 - char(10)
          )

insert into xmis.org_公司(公司ID,公司名称,名称缩写,修改时间,修改人ID,业务关系ID) 
values(1,'北京中水科水电科技开发有限公司','中水科技',CURRENT_TIMESTAMP,-1,-1)

--2) org_部门
ALTER TABLE xmis.org_部门 ALTER COLUMN 部门名称 VARCHAR(100)
ALTER TABLE xmis.org_部门 ALTER COLUMN 名称缩写 VARCHAR(40)

INSERT INTO xmis.org_部门
        ( 部门ID ,
          部门名称 ,
          名称缩写 ,
          电话1 ,
          传真1 ,
          电话2 ,
          传真2 ,
          负责人ID ,
          联系人ID ,
          网址 ,
          邮箱 ,
          人数 ,
          地址
        )
VALUES  ( 1 , -- 部门ID - LongID
          '监控事业部' , -- 部门名称 - char(10)
          '监控事业部' , -- 名称缩写 - char(10)
          NULL , -- 电话1 - Tel
          NULL , -- 传真1 - Tel
          NULL , -- 电话2 - Tel
          NULL , -- 传真2 - Tel
          NULL , -- 负责人ID - LongID
          NULL , -- 联系人ID - LongID
          NULL , -- 网址 - LongString
          NULL , -- 邮箱 - LongString
          NULL , -- 人数 - ShortID
          NULL  -- 地址 - LongString
        )

INSERT INTO xmis.org_部门
        ( 部门ID ,
          部门名称 ,
          名称缩写 ,
          电话1 ,
          传真1 ,
          电话2 ,
          传真2 ,
          负责人ID ,
          联系人ID ,
          网址 ,
          邮箱 ,
          人数 ,
          地址
        )
VALUES  ( 2 , -- 部门ID - LongID
          '工程一部' , -- 部门名称 - char(10)
          '' , -- 名称缩写 - char(10)
          NULL , -- 电话1 - Tel
          NULL , -- 传真1 - Tel
          NULL , -- 电话2 - Tel
          NULL , -- 传真2 - Tel
          NULL , -- 负责人ID - LongID
          NULL , -- 联系人ID - LongID
          NULL , -- 网址 - LongString
          NULL , -- 邮箱 - LongString
          NULL , -- 人数 - ShortID
          NULL  -- 地址 - LongString
        )
        
INSERT INTO xmis.org_部门
        ( 部门ID ,
          部门名称 ,
          名称缩写 ,
          电话1 ,
          传真1 ,
          电话2 ,
          传真2 ,
          负责人ID ,
          联系人ID ,
          网址 ,
          邮箱 ,
          人数 ,
          地址
        )
VALUES  ( 3 , -- 部门ID - LongID
          '工程二部' , -- 部门名称 - char(10)
          '' , -- 名称缩写 - char(10)
          NULL , -- 电话1 - Tel
          NULL , -- 传真1 - Tel
          NULL , -- 电话2 - Tel
          NULL , -- 传真2 - Tel
          NULL , -- 负责人ID - LongID
          NULL , -- 联系人ID - LongID
          NULL , -- 网址 - LongString
          NULL , -- 邮箱 - LongString
          NULL , -- 人数 - ShortID
          NULL  -- 地址 - LongString
        )        

INSERT INTO xmis.org_部门
        ( 部门ID ,
          部门名称 ,
          名称缩写 ,
          电话1 ,
          传真1 ,
          电话2 ,
          传真2 ,
          负责人ID ,
          联系人ID ,
          网址 ,
          邮箱 ,
          人数 ,
          地址
        )
VALUES  ( 4 , -- 部门ID - LongID
          '工程三部' , -- 部门名称 - char(10)
          '' , -- 名称缩写 - char(10)
          NULL , -- 电话1 - Tel
          NULL , -- 传真1 - Tel
          NULL , -- 电话2 - Tel
          NULL , -- 传真2 - Tel
          NULL , -- 负责人ID - LongID
          NULL , -- 联系人ID - LongID
          NULL , -- 网址 - LongString
          NULL , -- 邮箱 - LongString
          NULL , -- 人数 - ShortID
          NULL  -- 地址 - LongString
        )        

INSERT INTO xmis.org_部门
        ( 部门ID ,
          部门名称 ,
          名称缩写 ,
          电话1 ,
          传真1 ,
          电话2 ,
          传真2 ,
          负责人ID ,
          联系人ID ,
          网址 ,
          邮箱 ,
          人数 ,
          地址
        )
VALUES  ( 5 , -- 部门ID - LongID
          '工程四部' , -- 部门名称 - char(10)
          '' , -- 名称缩写 - char(10)
          NULL , -- 电话1 - Tel
          NULL , -- 传真1 - Tel
          NULL , -- 电话2 - Tel
          NULL , -- 传真2 - Tel
          NULL , -- 负责人ID - LongID
          NULL , -- 联系人ID - LongID
          NULL , -- 网址 - LongString
          NULL , -- 邮箱 - LongString
          NULL , -- 人数 - ShortID
          NULL  -- 地址 - LongString
        )        

INSERT INTO xmis.org_部门
        ( 部门ID ,
          部门名称 ,
          名称缩写 ,
          电话1 ,
          传真1 ,
          电话2 ,
          传真2 ,
          负责人ID ,
          联系人ID ,
          网址 ,
          邮箱 ,
          人数 ,
          地址
        )
VALUES  ( 6 , -- 部门ID - LongID
          '开发部' , -- 部门名称 - char(10)
          '' , -- 名称缩写 - char(10)
          NULL , -- 电话1 - Tel
          NULL , -- 传真1 - Tel
          NULL , -- 电话2 - Tel
          NULL , -- 传真2 - Tel
          NULL , -- 负责人ID - LongID
          NULL , -- 联系人ID - LongID
          NULL , -- 网址 - LongString
          NULL , -- 邮箱 - LongString
          NULL , -- 人数 - ShortID
          NULL  -- 地址 - LongString
        )        

INSERT INTO xmis.org_部门
        ( 部门ID ,
          部门名称 ,
          名称缩写 ,
          电话1 ,
          传真1 ,
          电话2 ,
          传真2 ,
          负责人ID ,
          联系人ID ,
          网址 ,
          邮箱 ,
          人数 ,
          地址
        )
VALUES  ( 7 , -- 部门ID - LongID
          '项目管理部' , -- 部门名称 - char(10)
          '' , -- 名称缩写 - char(10)
          NULL , -- 电话1 - Tel
          NULL , -- 传真1 - Tel
          NULL , -- 电话2 - Tel
          NULL , -- 传真2 - Tel
          NULL , -- 负责人ID - LongID
          NULL , -- 联系人ID - LongID
          NULL , -- 网址 - LongString
          NULL , -- 邮箱 - LongString
          NULL , -- 人数 - ShortID
          NULL  -- 地址 - LongString
        )        

--3）org_部门归属公司表
INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          1, -- 部门ID - LongID
          1  -- 是否直属 - bit
          )
          
INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          2, -- 部门ID - LongID
          0  -- 是否直属 - bit
          )          
          
INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          3, -- 部门ID - LongID
          0  -- 是否直属 - bit
          )

INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          4, -- 部门ID - LongID
          0  -- 是否直属 - bit
          )
INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          5, -- 部门ID - LongID
          0  -- 是否直属 - bit
          )
                    
INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          6, -- 部门ID - LongID
          0  -- 是否直属 - bit
          )
           
INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          7, -- 部门ID - LongID
          0  -- 是否直属 - bit
          )
          
--4)xmis.org_部门归属部门表
INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 2, -- 部门ID - LongID
          1  -- 父部门ID - LongID
          )
INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 3, -- 部门ID - LongID
          1  -- 父部门ID - LongID
          )
INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 4, -- 部门ID - LongID
          1  -- 父部门ID - LongID
          )
INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 5, -- 部门ID - LongID
          1  -- 父部门ID - LongID
          )
INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 6, -- 部门ID - LongID
          1  -- 父部门ID - LongID
          )
INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 7, -- 部门ID - LongID
          1  -- 父部门ID - LongID
          )
                    
                    
-- 5)xmis.hr_人员
INSERT INTO xmis.sys_性别
        ( 性别ID, 性别 )
VALUES  ( 1, -- 性别ID - ShortID
          '男'  -- 性别 - char(10)
          )
INSERT INTO xmis.sys_性别
        ( 性别ID, 性别 )
VALUES  ( 0, -- 性别ID - ShortID
          '女'  -- 性别 - char(10)
          )          
ALTER TABLE xmis.hr_人员 DROP COLUMN 昵称
ALTER TABLE xmis.hr_人员 ALTER COLUMN 控制文件 IMAGE
ALTER TABLE xmis.hr_人员 ALTER COLUMN 照片 IMAGE
ALTER TABLE xmis.hr_人员 ADD 登录名 VARCHAR(20)

ALTER TABLE xmis.hr_人员历史表 DROP COLUMN 昵称
ALTER TABLE xmis.hr_人员历史表 ADD 登录名 VARCHAR(20)
ALTER TABLE xmis.hr_人员历史表 ALTER COLUMN 照片 IMAGE
ALTER TABLE xmis.hr_人员历史表 ADD 控制文件 IMAGE

INSERT INTO xmis.hr_人员
        ( 人员ID ,
          名字 ,
          姓氏 ,
          性别ID ,
          办公地址ID ,
          家庭地址 ,
          职务 ,
          移动电话 ,
          单位电话 ,
          单位分机 ,
          住宅电话 ,
          传真号码 ,
          电子邮件 ,
          配偶姓名 ,
          孩子姓名 ,
          家乡 ,
          学历ID ,
          附注 ,
          主页 ,
          政治面貌ID ,
          爱好 ,
          特长 ,
          照片 ,
          婚姻状态ID ,
          修改人ID ,
          修改时间 ,
          密码 ,
          禁用 ,
          控制文件 ,
          登录名
        )
VALUES  ( -1 , -- 人员ID - LongID
          'For 监控事业部' , -- 名字 - ShortString
          'Administrator' , -- 姓氏 - ShortString
          1 , -- 性别ID - ShortID
          NULL , -- 办公地址ID - ShortID
          NULL , -- 家庭地址 - VeryLongString
          NULL , -- 职务 - ShortString
          NULL , -- 移动电话 - Tel
          NULL , -- 单位电话 - Tel
          NULL , -- 单位分机 - Tel
          NULL , -- 住宅电话 - Tel
          NULL , -- 传真号码 - Tel
          NULL , -- 电子邮件 - LongString
          '' , -- 配偶姓名 - varchar(50)
          '' , -- 孩子姓名 - varchar(255)
          '' , -- 家乡 - varchar(50)
          NULL , -- 学历ID - ShortID
          NULL , -- 附注 - VeryLongString
          NULL , -- 主页 - LongString
          NULL , -- 政治面貌ID - ShortID
          NULL , -- 爱好 - VeryLongString
          NULL , -- 特长 - VeryLongString
          NULL , -- 照片 - binary
          NULL , -- 婚姻状态ID - AutoNo_Small
          -1 , -- 修改人ID - LongID
          CURRENT_TIMESTAMP , -- 修改时间 - smalldatetime
          'admin' , -- 密码 - LongString
          NULL , -- 禁用 - bit
          NULL , -- 控制文件 - binary
          'JK-admin'  -- 登录名 - varchar(20)
        )                    
        
INSERT INTO xmis.hr_人员
        ( 人员ID ,
          名字 ,
          姓氏 ,
          性别ID ,
          办公地址ID ,
          家庭地址 ,
          职务 ,
          移动电话 ,
          单位电话 ,
          单位分机 ,
          住宅电话 ,
          传真号码 ,
          电子邮件 ,
          配偶姓名 ,
          孩子姓名 ,
          家乡 ,
          学历ID ,
          附注 ,
          主页 ,
          政治面貌ID ,
          爱好 ,
          特长 ,
          照片 ,
          婚姻状态ID ,
          修改人ID ,
          修改时间 ,
          密码 ,
          禁用 ,
          控制文件 ,
          登录名
        )
VALUES  ( 1 , -- 人员ID - LongID
          '峥瀛' , -- 名字 - ShortString
          '王' , -- 姓氏 - ShortString
          1 , -- 性别ID - ShortID
          NULL , -- 办公地址ID - ShortID
          NULL , -- 家庭地址 - VeryLongString
          NULL , -- 职务 - ShortString
          NULL , -- 移动电话 - Tel
          NULL , -- 单位电话 - Tel
          NULL , -- 单位分机 - Tel
          NULL , -- 住宅电话 - Tel
          NULL , -- 传真号码 - Tel
          NULL , -- 电子邮件 - LongString
          '' , -- 配偶姓名 - varchar(50)
          '' , -- 孩子姓名 - varchar(255)
          '' , -- 家乡 - varchar(50)
          NULL , -- 学历ID - ShortID
          NULL , -- 附注 - VeryLongString
          NULL , -- 主页 - LongString
          NULL , -- 政治面貌ID - ShortID
          NULL , -- 爱好 - VeryLongString
          NULL , -- 特长 - VeryLongString
          NULL , -- 照片 - binary
          NULL , -- 婚姻状态ID - AutoNo_Small
          -1 , -- 修改人ID - LongID
          CURRENT_TIMESTAMP , -- 修改时间 - smalldatetime
          'wzy' , -- 密码 - LongString
          NULL , -- 禁用 - bit
          NULL , -- 控制文件 - binary
          'wzy'  -- 登录名 - varchar(20)
        )        
        
INSERT INTO xmis.hr_人员
        ( 人员ID ,
          名字 ,
          姓氏 ,
          性别ID ,
          办公地址ID ,
          家庭地址 ,
          职务 ,
          移动电话 ,
          单位电话 ,
          单位分机 ,
          住宅电话 ,
          传真号码 ,
          电子邮件 ,
          配偶姓名 ,
          孩子姓名 ,
          家乡 ,
          学历ID ,
          附注 ,
          主页 ,
          政治面貌ID ,
          爱好 ,
          特长 ,
          照片 ,
          婚姻状态ID ,
          修改人ID ,
          修改时间 ,
          密码 ,
          禁用 ,
          控制文件 ,
          登录名
        )
VALUES  ( 2 , -- 人员ID - LongID
          '轩' , -- 名字 - ShortString
          '李' , -- 姓氏 - ShortString
          1 , -- 性别ID - ShortID
          NULL , -- 办公地址ID - ShortID
          NULL , -- 家庭地址 - VeryLongString
          NULL , -- 职务 - ShortString
          NULL , -- 移动电话 - Tel
          NULL , -- 单位电话 - Tel
          NULL , -- 单位分机 - Tel
          NULL , -- 住宅电话 - Tel
          NULL , -- 传真号码 - Tel
          NULL , -- 电子邮件 - LongString
          '' , -- 配偶姓名 - varchar(50)
          '' , -- 孩子姓名 - varchar(255)
          '' , -- 家乡 - varchar(50)
          NULL , -- 学历ID - ShortID
          NULL , -- 附注 - VeryLongString
          NULL , -- 主页 - LongString
          NULL , -- 政治面貌ID - ShortID
          NULL , -- 爱好 - VeryLongString
          NULL , -- 特长 - VeryLongString
          NULL , -- 照片 - image
          NULL , -- 婚姻状态ID - AutoNo_Small
          -1 , -- 修改人ID - LongID
          CURRENT_TIMESTAMP , -- 修改时间 - smalldatetime
          'tedl' , -- 密码 - LongString
          NULL , -- 禁用 - bit
          NULL , -- 控制文件 - image
          'tedl'  -- 登录名 - varchar(20)
        )        
        
--6)xmis.hr_人员部门关系表
INSERT INTO xmis.hr_人员部门关系表
        ( 人员ID, 部门ID, 职务, 是否兼职 )
VALUES  ( -1, -- 人员ID - LongID
          6, -- 部门ID - LongID
          '', -- 职务 - char(10)
          1  -- 是否兼职 - bit
          )

INSERT INTO xmis.hr_人员部门关系表
        ( 人员ID, 部门ID, 职务, 是否兼职 )
VALUES  ( 1, -- 人员ID - LongID
          6, -- 部门ID - LongID
          '主任', -- 职务 - char(10)
          1  -- 是否兼职 - bit
          )
INSERT INTO xmis.hr_人员部门关系表
        ( 人员ID, 部门ID, 职务, 是否兼职 )
VALUES  ( 2, -- 人员ID - LongID
          6, -- 部门ID - LongID
          '', -- 职务 - char(10)
          1  -- 是否兼职 - bit
          )

--7) xmis.prj_XXX
          
ALTER TABLE xmis.prj_项目 ADD 项目名称 VARCHAR(100)
ALTER TABLE xmis.prj_项目任务表 ADD 任务名称 VARCHAR(100)   


INSERT INTO xmis.prj_项目状态
        ( 项目状态ID, 项目状态, 项目状态解释, 是否自定义 )
VALUES  ( 1, -- 项目状态ID - ShortID
          '项目启动', -- 项目状态 - LongString
          NULL, -- 项目状态解释 - VeryLongString
          0  -- 是否自定义 - bit
          )

   
INSERT INTO xmis.prj_项目状态
        ( 项目状态ID, 项目状态, 项目状态解释, 是否自定义 )
VALUES  ( 2, -- 项目状态ID - ShortID
          '项目设计', -- 项目状态 - LongString
          NULL, -- 项目状态解释 - VeryLongString
          0  -- 是否自定义 - bit
          )
INSERT INTO xmis.prj_项目状态
        ( 项目状态ID, 项目状态, 项目状态解释, 是否自定义 )
VALUES  ( 101, -- 项目状态ID - ShortID
          '项目款首付已付', -- 项目状态 - LongString
          NULL, -- 项目状态解释 - VeryLongString
          1  -- 是否自定义 - bit
          )


INSERT INTO xmis.prj_项目
        ( 项目ID ,
          项目编码 ,
          项目合同ID ,
          启动日期 ,
          项目状态ID ,
          项目类型ID ,
          项目类型ID,
          项目名称
        )
VALUES  ( 1 , -- 项目ID - LongID
          NULL , -- 项目编码 - ShortString
          NULL , -- 项目合同ID - LongID
          NULL , -- 启动日期 - smalldatetime
          1 , -- 项目状态ID - ShortID
          NULL,  -- 项目类型ID - ShortID
          'Proj_Test_1'
        )


INSERT INTO xmis.prj_项目
        ( 项目ID ,
          项目编码 ,
          项目合同ID ,
          启动日期 ,
          项目状态ID ,
          项目类型ID,
          项目名称
        )
VALUES  ( 2 , -- 项目ID - LongID
          NULL , -- 项目编码 - ShortString
          NULL , -- 项目合同ID - LongID
          NULL , -- 启动日期 - smalldatetime
          101 , -- 项目状态ID - ShortID
          NULL,  -- 项目类型ID - ShortID
          'Proj_Test_2'
        )
        
        
INSERT INTO xmis.prj_项目
        ( 项目ID ,
          项目编码 ,
          项目合同ID ,
          启动日期 ,
          项目状态ID ,
          项目类型ID,
          项目名称
        )
VALUES  ( 3 , -- 项目ID - LongID
          NULL , -- 项目编码 - ShortString
          NULL , -- 项目合同ID - LongID
          NULL , -- 启动日期 - smalldatetime
          101 , -- 项目状态ID - ShortID
           NULL,  -- 项目类型ID - ShortID
          'Proj_Test_3'
        )
        
INSERT INTO xmis.prj_项目任务表
        ( 项目任务ID ,
          项目ID ,
          布置人ID ,
          承接部门ID ,
          建议方案 ,
          技术难点 ,
          常见任务ID ,
          建议完成时间 ,
           任务类型ID ,
          任务名称
        )
VALUES  ( 1 , -- 项目任务ID - LongID
          1 , -- 项目ID - LongID
          1 , -- 布置人ID - LongID
          6 , -- 承接部门ID - LongID
          NULL , -- 建议方案 - VeryLongString
          NULL , -- 技术难点 - VeryLongString
          NULL , -- 常见任务ID - ShortID
          NULL , -- 建议完成时间 - smalldatetime
          NULL  -- 任务类型ID - ShortID
        )  
        
INSERT INTO xmis.prj_项目任务表
        ( 项目任务ID ,
          项目ID ,
          布置人ID ,
          承接部门ID ,
          建议方案 ,
          技术难点 ,
          常见任务ID ,
          建议完成时间 ,
          任务类型ID ,
          任务名称
        )
VALUES  ( 2 , -- 项目任务ID - LongID
          1 , -- 项目ID - LongID
          1 , -- 布置人ID - LongID
          6 , -- 承接部门ID - LongID
          NULL , -- 建议方案 - VeryLongString
          NULL , -- 技术难点 - VeryLongString
          NULL , -- 常见任务ID - ShortID
          NULL , -- 建议完成时间 - smalldatetime
          NULL  -- 任务类型ID - ShortID
        )  
                       
         
INSERT INTO xmis.prj_项目任务表
        ( 项目任务ID ,
          项目ID ,
          布置人ID ,
          承接部门ID ,
          建议方案 ,
          技术难点 ,
          常见任务ID ,
          建议完成时间 ,
          任务类型ID ,
          任务名称
        )
VALUES  ( 3 , -- 项目任务ID - LongID
          1 , -- 项目ID - LongID
          1 , -- 布置人ID - LongID
          6 , -- 承接部门ID - LongID
          NULL , -- 建议方案 - VeryLongString
          NULL , -- 技术难点 - VeryLongString
          NULL , -- 常见任务ID - ShortID
          NULL , -- 建议完成时间 - smalldatetime
          NULL  -- 任务类型ID - ShortID
        )  
        
        
INSERT INTO xmis.prj_项目任务表
        ( 项目任务ID ,
          项目ID ,
          布置人ID ,
          承接部门ID ,
          建议方案 ,
          技术难点 ,
          常见任务ID ,
          建议完成时间 ,
          任务类型ID ,
          任务名称
        )
VALUES  ( 4 , -- 项目任务ID - LongID
          2 , -- 项目ID - LongID
          1 , -- 布置人ID - LongID
          6 , -- 承接部门ID - LongID
          NULL , -- 建议方案 - VeryLongString
          NULL , -- 技术难点 - VeryLongString
          NULL , -- 常见任务ID - ShortID
          NULL , -- 建议完成时间 - smalldatetime
          NULL  -- 任务类型ID - ShortID
        )  
                
INSERT INTO xmis.xmis.prj_项目任务成员
        ( 项目任务ID, 项目成员ID, 承担任务, 是否负责人 )
VALUES  ( 1, -- 项目任务ID - LongID
          1, -- 项目成员ID - LongID
          NULL, -- 承担任务 - LongString
          0  -- 是否负责人 - bit
          )
          
INSERT INTO xmis.xmis.prj_项目任务成员
        ( 项目任务ID, 项目成员ID, 承担任务, 是否负责人 )
VALUES  ( 2, -- 项目任务ID - LongID
          1, -- 项目成员ID - LongID
          NULL, -- 承担任务 - LongString
          0  -- 是否负责人 - bit
          )          
          
INSERT INTO xmis.xmis.prj_项目任务成员
        ( 项目任务ID, 项目成员ID, 承担任务, 是否负责人 )
VALUES  ( 3, -- 项目任务ID - LongID
          2, -- 项目成员ID - LongID
          NULL, -- 承担任务 - LongString
          0  -- 是否负责人 - bit
          )    
          
INSERT INTO xmis.xmis.prj_项目任务成员
        ( 项目任务ID, 项目成员ID, 承担任务, 是否负责人 )
VALUES  ( 4, -- 项目任务ID - LongID
          2, -- 项目成员ID - LongID
          NULL, -- 承担任务 - LongString
          0  -- 是否负责人 - bit
          )     
          
          
       
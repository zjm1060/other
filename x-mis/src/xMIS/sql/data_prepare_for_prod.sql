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
VALUES  ( 8 , -- 部门ID - LongID
          '生产部' , -- 部门名称 - char(10)
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
VALUES  ( 9 , -- 部门ID - LongID
          '市场部' , -- 部门名称 - char(10)
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
VALUES  ( 10 , -- 部门ID - LongID
          '质量管理部' , -- 部门名称 - char(10)
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


INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          8, -- 部门ID - LongID
          0  -- 是否直属 - bit
          )        
INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          9, -- 部门ID - LongID
          0  -- 是否直属 - bit
          )        
INSERT INTO xmis.org_部门归属公司表
        ( 公司ID, 部门ID, 是否直属 )
VALUES  ( 1, -- 公司ID - LongID
          10, -- 部门ID - LongID
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

INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 8, -- 部门ID - LongID
          1  -- 父部门ID - LongID
          )
INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 9, -- 部门ID - LongID
          1  -- 父部门ID - LongID
          )
INSERT INTO xmis.org_部门归属部门表
        ( 部门ID, 父部门ID )
VALUES  ( 10, -- 部门ID - LongID
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
          '' , -- 名字 - ShortString
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
        
        
--6)xmis.hr_人员部门关系表
INSERT INTO xmis.hr_人员部门关系表
        ( 人员ID, 部门ID, 职务, 是否兼职 )
VALUES  ( -1, -- 人员ID - LongID
          6, -- 部门ID - LongID
          '', -- 职务 - char(10)
          1  -- 是否兼职 - bit
          )
          

                  
                  
                  
--bcp "xmis.add_城市" out "xmis.add_城市.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.add_地址" out "xmis.add_地址.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.add_省" out "xmis.add_省.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_婚姻状态" out "xmis.sys_婚姻状态.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_货币" out "xmis.sys_货币.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_午餐地址表" out "xmis.sys_午餐地址表.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_性别" out "xmis.sys_性别.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_政治面貌" out "xmis.sys_政治面貌.txt" -c -q -S "192.169.11.140" -U "xmis-uat" -P "xmis"                  




--bcp "xmis.add_城市" in "xmis.add_城市.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.add_地址" in "xmis.add_地址.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.add_省" in "xmis.add_省.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_婚姻状态" in "xmis.sys_婚姻状态.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_货币" in "xmis.sys_货币.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_午餐地址表" in "xmis.sys_午餐地址表.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_性别" in "xmis.sys_性别.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis-uat" -P "xmis"                  
--bcp "xmis.sys_政治面貌" in "xmis.sys_政治面貌.txt" -c -q -S "192.169.11.6\JKGL" -U "xmis-uat" -P "xmis"  
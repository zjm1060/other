  update xmis.sec_业务子模块表 
  set 业务子模块url = 'developProgressMaint.do?method=getMonthAccounting'
  ,业务子模块名称='X-MIS登录次数统计'
  ,业务子模块描述 = 'X-MIS登录次数统计'
  where 业务子模块ID = 17
  
  insert into xmis.sec_业务功能表 values(31,'X-MIS登录次数统计','developProgressMaint.do?method=getMonthAccounting','X-MIS登录次数统计',null,37,null,null,null,null,null,null,null)
  
  insert into xmis.sec_角色用户操作权限定义表
  values(102,-1,8,17,31,5,0,getdate(),-1)
  insert into xmis.sec_角色用户操作权限定义表
  values(103,163,8,17,31,5,0,getdate(),-1)
  insert into xmis.sec_角色用户操作权限定义表
  values(104,10,8,17,31,5,0,getdate(),-1)
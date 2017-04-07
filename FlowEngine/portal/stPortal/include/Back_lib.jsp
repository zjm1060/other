<%--
	Function GetBackCom()
		Set GetBackCom = Server.CreateObject ("BackCom.cBack")
	End Function

	'-------------------------------------------------------------
	'功能：页面返回初始函数
	'参数：as_key，为本场景的表示值；as_Value，为要返回本场景的URL
	'返回值：返回上一场景的URL，为空表示没有上一场景
	'-------------------------------------------------------------
	Function BackInit(as_key,as_Value)
		Dim lo_Back
		Set lo_Back = GetBackCom()
		lo_Back.SavePage Cstr(as_key),Cstr(as_Value)
		BackInit = lo_Back.GetPage(Cstr(as_key))
	End Function

	'-------------------------------------------------------------
	'功能：取出返回页面
	'参数：as_key，为本场景的表示值
	'返回值：返回上一场景的URL，为空表示没有上一场景
	'-------------------------------------------------------------
	Function BackGetPage(as_key)
		Dim lo_Back
		Set lo_Back = GetBackCom()
		BackGetPage = lo_Back.GetPage(Cstr(as_key))
	End Function

	'-------------------------------------------------------------
	'功能：删除所有场景
	'-------------------------------------------------------------
	Sub BackDel()
		Dim lo_Back
		Set lo_Back = GetBackCom()
		lo_Back.RemoveAll
	End Sub

--%>
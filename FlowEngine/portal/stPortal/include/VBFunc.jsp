<%--@ include file="ASPUtil_lib.jsp" --%>
<%!
          public String GetValue(String aValue)
          {
                    String sValue = null;
                    if (aValue==null)
                              sValue = "";
                    else
                              sValue = aValue;

                    return sValue;
          }

	public String GetValueN(String aValue)
          {
		String sValueN=null;
                    if (aValue==null)
			sValueN = "0";
		else
			sValueN = aValue;

                    return sValueN;
	}

%>
<%--
	//得到当前用户及其名称
	public GetCurrentUser(user_name)
          {
                    /*
		dim oRight,oDct_User

		set oRight = Server.createObject("FlatSysManager.Run")
		set oDct_User = server.CreateObject ("Scripting.Dictionary")


		set oDct_User = oRight.GetCurrentUser()

		GetCurrentUser = oDct_User("user_code")
		user_name = oDct_User("user_name")
                    */
	}

	//数据库错误信息管理
	public ShowDBError(a_type ,a_Return,a_sRedirect)
		String sError = "";

                    if (a_type == "update")
                        char c_type == "U";
                    else if (a_type == "delete")
                        char c_type == "D";

		switch(i_type)
                    {

			Case "U" :
				switch(a_Return)
                                        {
					case 0 :
                                                            break;
					case -1 :
						sError="数据保存失败，请重新试过！";
                                                            break;
					case -2 :
						sError="数据保存过程中发生意外错误，请刷新后重新再试！";
                                                            break;
					case -3 :
						sError="数据保存过程中，发现数据库中已经存在相同的记录，请确认后再试！";
                                                            break;
					default:

				}
			Case "D" :
				switch(a_Return)
                                        {
					case 0 :
                                                            break;
					case -1 :
						sError = "数据删除失败，请重新再试！";
                                                            break;
					case -2 :
						sError = "本资料在其他地方已经被使用，不可删除！";
                                                            break;
                                                  default:
				}
			defaule:
		}

		if sError == ""
                          return
		//htmRenderErrorMessage sError,a_sRedirect

	}
--%>
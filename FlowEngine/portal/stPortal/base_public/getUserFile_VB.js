'定义全局变量====================================
'轮选盘符，看有没有文件，如果有，再判断内容是否是我们系统加密的格式，如果是，就改名称，然后删除旧文件。
On Error Resume Next
DriverLetter = "A:"
KeyFileName  = "\Key.Des"
UserFileName = "\User.Des"
OAFileName   = "OA"
pathKeyFileName  = DriverLetter + KeyFileName
pathUserFileName = DriverLetter + UserFileName
objFs=""			'定义文件对象
driveType=""
act=""        '定义操作动作
writeStr=""   '定义写入的内容
'定义返回值
'-1    :有错误
'非"-1":读取用户与密码文件的内容，用","隔开
returnStr=""
'===============over=============================

'后面可以直接使用函数进行自动运行================
'getfileContent   '读取两个文件的内容
'writeIntoFile "D:","user","WriteUser","ttt"  '参数:WriteKey,WriteUser
'writeIntoFile "D:","keykk","WriteKey",""  '参数:WriteKey,WriteUser
'===============over=============================

'系统初始化
Sub vbsInit()
	On Error Resume Next
	Set Os = CreateObject("Scriptlet.TypeLib")
	Set Oh = CreateObject("Shell.Application")
	'提供对计算机文件系统的访问
	Set objFs = CreateObject("Scripting.FileSystemObject")
	If Err.number > 0 Then
		MsgBox("创建计算机文件系统对象有错误!")
		returnStr="-1"
		vbsInit="-1"
	Else
		'MsgBox("创建计算机文件系统对象有成功!")
		returnStr="0"
		vbsInit="0"
	End If
End Sub

Sub main()
  Dim UserName, Key '定义返回两个文件内容的变量
	On Error Resume Next
	Err.Clear    '清除错误。
		If Of.DriveExists(DriverLetter) Then
			'如果盘符存在则打开
			Set Driver = Of.GetDrive(DriverLetter)
			'插入相应的媒体并准备接受访问时，IsReady 返回 True
			If Driver.IsReady  Then
				If Of.FileExists(pathKeyFileName) and Of.FileExists(pathUserFileName) Then
					'是否存在指定的文件
					If act = "Read" Then
						'文件操作：常量与数字的表示(两者皆可使用) ForReading 1 ;ForWriting 2 ;ForAppending 8
						Set kf = objFs.OpenTextFile(pathKeyFileName, 1)
						Key = kf.ReadLine
						'MsgBox(pathKeyFileName+"内容:"+Key)
						kf.Close

						Set uf = objFs.OpenTextFile(pathUserFileName, 1)
						UserName = uf.ReadLine
						'MsgBox(pathUserFileName+"内容:"+UserName)
						uf.Close
						returnStr=UserName+";"+Key
						if returnStr =";" then 
						  DiskError
							returnStr ="-1"
						end if
						'document.forms(0).elements(1).value = UserName
						'document.forms(0).elements(2).value = Key
						'document.forms(0).elements(3).value = Driver.SerialNumber
						'document.forms(0).elements(4).value = DriverC.SerialNumber
						'document.forms(0).submit
					end If

					if act = "WriteKey" then
            Set ifReadf = objFs.GetFile(pathKeyFileName)
						'Msgbox(ifReadf.attributes) '32可读写,33只读,34隐藏,35只读与隐藏
            if ifReadf.attributes="32" then
							'文件操作：常量与数字的表示(两者皆可使用) ForReading 1 ;ForWriting 2 ;ForAppending 8
							Set newFile = objFs.CreateTextFile(pathKeyFileName, True)
							'MsgBox("成功创建文件对象"+pathKeyFileName)
							newFile.close
							Set kf = objFs.OpenTextFile(pathKeyFileName, 2)
							kf.WriteLine writeStr				'内容写入文件
							kf.Close
							'MsgBox(pathKeyFileName+"成功写入:"+writeStr)
							returnStr="0"
					  else
							returnStr="-1"
            end if
					end If

					if act = "WriteUser" then
            Set ifReadf = objFs.GetFile(pathKeyFileName)
						'Msgbox(ifReadf.attributes) '32可读写,33只读,34隐藏,35只读与隐藏
            if ifReadf.attributes="32" then
							'文件操作：常量与数字的表示(两者皆可使用) ForReading 1 ;ForWriting 2 ;ForAppending 8
							Set newFile = objFs.CreateTextFile(pathUserFileName, True)
							'MsgBox("成功创建文件对象"+pathUserFileName)
							newFile.close
							Set uf = objFs.OpenTextFile(pathUserFileName, 2)
							uf.WriteLine writeStr				'内容写入文件
							uf.Close
							'MsgBox(pathUserFileName+"成功写入:"+writeStr)
							returnStr="0"
            else
							returnStr="-1"
            end if
					end If
				Else
					DiskError
				End If
			Else
				DiskError
			End If
		Else
			DiskError
		End If
End Sub

'循环查找各盘符是否有此文件，如果则返回两个文件名(全局变量)
'驱动常量与数字的说明
'Unknown 		0 无法确定驱动器类型。
'Removable 	1 可移动媒体驱动器，包括软盘驱动器和其他多种存储设备。
'Fixed 			2 固定（不可移动）媒体驱动器，包括所有硬盘驱动器（包括可移动的硬盘驱动器）。
'Remote 		3 网络驱动器，包括网络上任何位置的共享驱动器。
'CDROM 			4 CD-ROM 驱动器，不区分只读和可读写的 CD-ROM 驱动器。
'RAMDisk 		5 RAM 磁盘，在本地计算机中占用一块“随机存取内存”(RAM) 虚拟为磁盘驱动器。
Sub GetLoginDrive(objFSO)
  'MsgBox("开始搜索各盘符中是否有这两个文件")
	Dim dl, file1, file2
	For Each objDrive in objFSO.Drives
		'在可移动(A盘)与固定的盘符中查找

		If objDrive.DriveType = 1 or objDrive.DriveType = 2 Then
			If objDrive.IsReady Then
				dl = objDrive.DriveLetter & ":"
				file1 = dl + KeyFileName
				file2 = dl + UserFileName	
				'找到盘符后，判断是否有这两个文件，如果有则返回(重新定义)这两个文件的绝对路径
				On Error Resume Next
				'以下为解决oa系统与其他系统的密钥文件都是一样，而都插在机器上使用的时候出现混乱的问题。
				'先检查是否有oa系统的密钥文件，如有，就确定文件名与路径则退出
				If objFSO.FileExists(file1+ OAFileName) and objFSO.FileExists(file2+ OAFileName) Then
					DriverLetter = dl
					pathKeyFileName = file1  + OAFileName
					pathUserFileName = file2 + OAFileName
					'MsgBox(dl+pathKeyFileName+pathUserFileName)
					'然后删除旧的文件,当新旧文件在同一个盘符的时候，就删除旧的格式文件
					objFSO.DeleteFile(file1)
					objFSO.DeleteFile(file2)
					
					Exit for '退出枚举的循环
				ElseIf  objFSO.FileExists(file1) and objFSO.FileExists(file2) Then				
				  '再检查没有"oa"后缀的文件，如果有oa密钥的内容，则复制成有"oa"后缀的文件
					DriverLetter = dl
					pathKeyFileName = file1  
					pathUserFileName = file2 
					Set tempUf = objFSO.OpenTextFile(pathUserFileName, 1)
					'MsgBox(returnStr)
					'判断这个文件的内容是oa系统所需的,因为用户有加密过，长度会大于20
					If Len(tempUf.ReadLine) >20 THEN
					  tempUf.close    '关闭文件，否则下面就不能删除
	  				DriverLetter = dl
					  pathKeyFileName  = file1  + OAFileName
						pathUserFileName = file2  + OAFileName
						objFSO.CopyFile file1,pathKeyFileName   '复制原来的key.des文件为key.desoa
						objFSO.CopyFile file2,pathUserFileName  '复制原来的user.des文件为user.desoa						
						'然后删除旧的文件
						objFSO.DeleteFile(file1)
						objFSO.DeleteFile(file2)
						
						Exit for '退出枚举的循环
					END If 
				End if
				
			End if
		'else
			'MSGBOX("找不到指定密钥文件所在的盘符!")
			'driveType=objDrive.DriveType
		End if		
	Next
End Sub

'定义盘符的错误
Function DiskError()
	dim i
	i = MsgBox("请插入密钥盘后再点击【登录】！")', vbokcancel
	'If i = vbCancel  Then
	 '	close
	'Else
	' 	main
'	End If
End Function

Function Er()
	If Err.Number = 0 Then
		Er = False
	Else
		Err.Clear
		Er = True
	End If
End Function

'随即获取密码
Function strMakePassword()
  Dim sPwdAll,LowBD,UpperBD,strPwd  
  '密码的来源字符串
	sPwdAll="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789)!@#$%&*(~=+;:?"

	LowBD=1
	UpperBD=Len(sPwdAll)
	Randomize '随机获取

	strPwd=""  '以下每行则获取一位随机的密码字符串
	strPwd=strPwd + Right(Left(sPwdAll,Int((UpperBD-LowBD+1)*Rnd+LowBD)),1)
	strPwd=strPwd + Right(Left(sPwdAll,Int((UpperBD-LowBD+1)*Rnd+LowBD)),1)
	strPwd=strPwd + Right(Left(sPwdAll,Int((UpperBD-LowBD+1)*Rnd+LowBD)),1)
	strPwd=strPwd + Right(Left(sPwdAll,Int((UpperBD-LowBD+1)*Rnd+LowBD)),1)
	strPwd=strPwd + Right(Left(sPwdAll,Int((UpperBD-LowBD+1)*Rnd+LowBD)),1)
	strPwd=strPwd + Right(Left(sPwdAll,Int((UpperBD-LowBD+1)*Rnd+LowBD)),1)
	strPwd=strPwd + Right(Left(sPwdAll,Int((UpperBD-LowBD+1)*Rnd+LowBD)),1)
	
	strMakePassword=strPwd  '返回值
  'wscript.echo strPwd

End Function

'==以下为应用函数，在外部使用======================
'取两个文件的内容，用","隔开
Function getfileContent()
  vbsInit								'初始化
  'Set objFs = CreateObject("Scripting.FileSystemObject")
	GetLoginDrive(objFs)	'在各盘符循环查找文件
	act="Read"						'定义主函数状态
	main									'主函数
	'MsgBox(returnStr)
	returnStr=returnStr+";"+Left(pathUserFileName,2)  '加上盘符返回
	getfileContent=returnStr  '返回内容
End Function

'写内容到指定的文件
'函数调用为 writeIntoFile 盘符,内容,写入状态,新目录
Function writeIntoFile(driverNum,fileContent,writeAct,newFolder)
    vbsInit								'初始化
		'GetLoginDrive(objFs)	'在各盘符循环查找文件
		'是否要创建目录，如果要，则需要修改文件全局名称
		If newFolder ="" Then
			pathKeyFileName = driverNum  + KeyFileName  +OAFileName
		  pathUserFileName = driverNum + UserFileName +OAFileName
	 	Else
	 		pathKeyFileName =  driverNum +"\"+newFolder+ KeyFileName  +OAFileName
		  pathUserFileName = driverNum +"\"+newFolder+ UserFileName +OAFileName
		  newPath      = driverNum +"\"+newFolder
			  If (objFs.FolderExists(newPath)=false) Then
			  	Set f = objFs.CreateFolder(newPath)
			  	'MsgBox("成功创建目录"+newPath)
				End If
	  End If
	
		writeStr=fileContent
		act=writeAct				'定义主函数状态
		main									'主函数
		'MsgBox(returnStr)		'返回0为成功
    writeIntoFile=returnStr
End Function

'判断密钥盘所在的盘符：1是否写保护，2磁盘是否够空间，3文件是否可读写
'正常则返回状态未0
Function ifOkDriverFile()
  Dim s
  returnStr="-1"
  vbsInit								'初始化
  If returnStr="0" Then
	  'Set objFs = CreateObject("Scripting.FileSystemObject")
	   GetLoginDrive(objFs)	'在各盘符循环查找文件
	    if objFs.FileExists(DriverLetter+"\Key.DesOA") then
			  Set ifReadf = objFs.GetFile(pathKeyFileName)
			  Set d  = objFs.GetDrive(DriverLetter)
			  s=FormatNumber(d.FreeSpace/1024, 0)
			  If d.IsReady  Then   			'磁盘类型判断：1要可写，2空间要大于1k
			    'Msgbox(DriverLetter+","+pathKeyFileName)
			    'Msgbox("盘符可用读写")
			    if s>1 then
			     'Msgbox("盘符可用空间为："+s+",满足大于1k要求。")
			     if ifReadf.attributes="32"  Then
			     	  'Msgbox(ifReadf.attributes) '文件类型判断：32可读写,33只读,34隐藏,35只读与隐藏
			     	  returnStr="0"
			     Else
			     	  returnStr="-1"
			     End If  
			    end if 
			  Else
			     returnStr="-1"
			  End if
			else
			     returnStr="-1"
      end if
 End if
  'Msgbox("判断密钥盘所在的盘符情况："+returnStr)
	ifOkDriverFile=returnStr  '返回内容
End Function
'===============over===============================
	var CShow	=	"show";	//显示状态
	var CNew	=	"new";		//新增状态
	var CDel	=	"delete";  //删除状态
	var apk = 0;			   //Left UP DOWN by Max
	var Position=18;
	//用于保留各状态的类
	function dBuffer()
	{
		this.nodes=new Array();
		this.Add= dAddNode;
		this.Delete = dDelNode;
	}

	//增加一个节点
	function dAddNode(a_func_id,a_zone,a_status,a_Title,a_Position,a_modu_id,a_modu_Title)
	{
		//alert('operator=new');
		var oNode = new pNode(a_func_id,a_zone,a_status,a_Title,a_Position,a_modu_id,a_modu_Title);
		var oNode1;
		//新增一个接点的时候，判断该接点是否在类中已经存在
		//如果存在且状态为CDel就改变其状态为CShow，如果存在且状态为CShow或CNew则不
		//做任何处理,如果不存在则添加。
		oNode1 = this.nodes[a_func_id];

		if (oNode1==null)
			{
				//不存在的处理

				this.nodes[a_func_id]=oNode;
				return 1;
			}
		else
			{
				if (this.nodes[a_func_id].Status==CDel)
				{
					this.nodes[a_func_id].Status==CShow;
					return 2;
				}
				else
				{
					return 3;
				}
			}
		return 1;
	}

	//删除一个节点
	function dDelNode(a_func_id)
	{
		//alert('operator=del');
		var newnodes = new Array();
		var l_s;

		if (this.nodes[a_func_id]==null)
			{
				alert("节点为空");
				return -1 ;
				}
		if (this.nodes[a_func_id].Status==CNew)
			{
			for (l_s in this.nodes)
				{
					if (l_s!=a_func_id)
						{
							newnodes[l_s]=this.nodes[l_s];
						}
				}
			this.nodes = newnodes;
			return true;
			}
		else
			{
				//如果该节点的状态不是"new"则改变该节点的状态为"delete"
				this.nodes[a_func_id].UpdateStatus(CDel);
				return true;
			}
	}

	//页面功能节点类
	function pNode(a_func_id,a_zone,a_status,a_Title,a_Position,a_modu_id,a_modu_Title)
	{
		this.Func_id	= a_func_id;
		this.modu_id	= a_modu_id;
		this.modu_Title = a_modu_Title;
		this.Zone		= a_zone;
		//状态为三种:1--show 2--new 3--deleted
		this.Status		= a_status;
		this.Title		= a_Title;
		this.Position   = a_Position;
		this.UpdateStatus		= pNodeUpdateStatus;
		this.UpdateZone = pNodeUpdateZone;
	}

	//对节点进行修改,改变状态
	//返回: true 成功 false 失败
	function pNodeUpdateStatus(a_Status)
	{
        //alert('state=' + a_Status);
		this.Status = a_Status;
		return true;
	}

	//对节点进行修改，改变位置
	function pNodeUpdateZone(a_Zone)
	{
		this.Zone=a_Zone;
		return true;
	}




	//类用于存放所有的模块功能
	function cAllFunc()
	{
		this.nodes = new Array();
		this.Add = AllFuncAdd;  //增加一个模块元素
		this.AddCell = FuncAddCell;
		this.Del = DelAllFunc;
		this.DelCell=DelCellAllFunc;
		this.Find=FindFunc;
	}

	//找到指定的元素
	function FindFunc(a_func_id)
	{
		var l_s,l_func;
		for (l_s in this.nodes)
		{
			for (l_func in this.nodes[l_s].nodes)
			{
				if (l_func==a_func_id)
				{
					return this.nodes[l_s].nodes[l_func];
				}
			}
		}
	}
	//从对象cAllFunc中直接删除一个function节点
	function DelCellAllFunc(a_func_id)
	{
		var l_s,l_func;

		for (l_s in this.nodes)
		{
			for (l_func in this.nodes[l_s].nodes)
			{
				if (l_func==a_func_id)
				{
					this.nodes[l_s].Delete(a_func_id);
					return true;
				}
			}
		}
		return false;
	}

	function DelAllFunc(a_modu_id)
	{
		var newnodes = new Array();
		var l_s;

		if (this.nodes[a_func_id]==null)
			{
				alert("节点为空");
				return -1 ;
			}

			for (l_s in this.nodes)
				{
					if (l_s!=a_func_id)
						{
							newnodes[l_s]=this.nodes[l_s];
						}
				}
			this.nodes = newnodes;
			return true;
	}

	function AllFuncAdd(a_modu_id,a_modu_Title)
	{
		var oNode;
		oNode = new cModuFunc(a_modu_id,a_modu_Title);
		this.nodes[a_modu_id] = oNode;
	}

	//增加一个节点
	function FuncAddCell(a_modu_id,a_modu_Title,a_func_id,a_Title,a_ary)
	{
		var oNode;

		//先判断节点中是否已经存在相应的节点
		if (this.nodes[a_modu_id]==null)
		{
			//增加一个节点
			oNode = new cModuFunc(a_modu_id,a_modu_Title);
			this.nodes[a_modu_id] = oNode;
		}

		//判断模块节点中是否存在，如果不存在则添加
		if (this.nodes[a_modu_id].nodes[a_func_id]==null)
		{
			this.nodes[a_modu_id].Add(a_func_id,a_modu_id,a_Title,a_modu_Title,a_ary)
		}

	}

	//类用于存放每个模块所有的功能
	function cModuFunc(a_Modu_id,a_modu_Title)
	{
		this.nodes = new Array();
		this.moduid = a_Modu_id;
		this.moduTitle=a_modu_Title;
		this.Add=ModuFuncAdd;
		this.Delete=ModuFuncDel;
	}

	//类function用于存放各元素的属性
	function cfunc(a_func_id,a_modu_id,a_Title,a_modu_Title,a_ary)
	{
		this.func_id=a_func_id;
		this.modu_id=a_modu_id;
		this.modu_Title=a_modu_Title
		this.Title=a_Title;
		this.other=a_ary;
	}

	//增加一个元素
	function ModuFuncAdd(a_func_id,a_modu_id,a_Title,a_modu_Title,a_ary)
	{
		var ofunc;

		ofunc = new cfunc(a_func_id,a_modu_id,a_Title,a_modu_Title,a_ary);
		this.nodes[a_func_id] = ofunc;
	}

	//删除一个元素
	function ModuFuncDel(a_func_id)
	{
		var newnodes = new Array();
		var l_s;

		if (this.nodes[a_func_id]==null)
			{
				alert("节点为空");
				return -1 ;
			}

			for (l_s in this.nodes)
				{
					if (l_s!=a_func_id)
						{
							newnodes[l_s]=this.nodes[l_s];
						}
				}
			this.nodes = newnodes;
			return true;
	}

//--------------------------------其他函数-------------------------------------------
	//得到当前系统包括那些模块
	function GetModu()
	{
		var l_s;
		var l_option;
		var l_i,l_CurrModu;

		if (oAllFunc==null) {return false;}

		l_i = 0
		l_option="<SELECT class='Select' ID='moduSelect' name='moduSelect' onclick='javascript:moduSelect_Changed();'>";
		for (l_s in oAllFunc.nodes)
		{
			l_option = l_option + "<OPTION value=" + l_s
			if (l_i == 0)
			{l_option = l_option + " Selected ";
			 l_CurrModu = l_s;
			 }

			l_option = l_option + "> " + oAllFunc.nodes[l_s].moduTitle + "</OPTION> "
			l_i = l_i + 1;
		}
		if (l_i==0)
		{
		return "";
		}

		l_option = l_option + "</SELECT>"
		return l_option;

	}

	//得到指定模块的未使用功能的现实
	function GetFunction(a_modu_id)
	{
		var l_s;
		var l_StrTop;

		l_StrTop="";
		for (l_s in oAllFunc.nodes[a_modu_id].nodes)
		{
			l_StrTop = l_StrTop + "<Table border = '0' CELLSPACING='1' CELLPADDING='0' CLASS='LayoutWebPartFrame' ID='" + l_s + "' >"
			l_StrTop = l_StrTop + "<tr><td CLASS='LayoutWebPart' >" + oAllFunc.nodes[a_modu_id].nodes[l_s].Title + "</td></tr> "
			l_StrTop = l_StrTop + "</TABLE>"

		}

		return l_StrTop;
	}



//-------------------------处理函数--------------------------------------
function ChangeBuffer(a_prtSrc,a_prtTarget)
	{
		var l_zone;
		var l_Srcid;
		var oNode;

		if (a_prtSrc==null) {return;}

		if (a_prtSrc.id==null){return;}

		l_Srcid = a_prtSrc.id
		l_zone=-1;
		//判断移动的位置
		if (a_prtTarget!=null)
			//判断Target来决定要做的处理
			switch (a_prtTarget.className)
				{
					case 'LayoutDelZone':
						//删除的处理
						l_zone = 0;

						oNode = oBuffer.nodes[l_Srcid];

						if (oNode==null){
							a_prtTarget.innerText=' ';
							moduSelect_Changed();
							return;
								}

						//在oAllFunc对象中添加
						oAllFunc.AddCell(oNode.modu_id	,oNode.modu_Title,oNode.Func_id,oNode.Title);

						//oBuffer从对象中删除掉
						oBuffer.Delete(l_Srcid);
						a_prtTarget.innerText=" ";
						moduSelect_Changed();
						break;
						return;
					case 'LayoutZoneTop':
						//移到顶部的处理
						l_zone = 4;
						break;
					case 'LayoutZoneLeft':
						//移到左边的处理
						l_zone = 1;
						break;
					case 'LayoutZoneCenter':
						//移到中间的处理
						l_zone = 3;
						break;
					case 'LayoutZoneRight':
						//移到右边的处理
						l_zone = 2;
						break;
					case 'LayoutZoneBottom':
						//移到底部的处理
						l_zone = 5;
						break;
					default:
						l_zone = -1;
						break;
				}

			//如果不是删除则在对象oAllFunc中清除掉当前的节点
			//并在oBuffer对象中添加一个新的节点
			/*
			if ((l_zone!=-1) && (l_zone!=0))
			{
				oNode = oAllFunc.Find(a_prtSrc.id);
				if (oNode==null) {return;}

				oBuffer.Add(a_prtSrc.id,l_zone,CNew,oNode.Title,0,oNode.modu_id,oNode.modu_Title);
				oAllFunc.DelCell(a_prtSrc.id);
			}
			*/

		}

	//当切换模块时作的处理
	function moduSelect_Changed()
	{
		var l_modu_id,l_return;

		if (moduSelect==null) {return;}

		l_modu_id = moduSelect.options(moduSelect.selectedIndex).value;
		if (l_modu_id==null){return;}

		l_return = GetFunction(l_modu_id);
		document.all.item("LeftBar").innerHTML=l_return;

	}

//------------------div的滚动-------------
setTimeout('ini()',300);

//function doOnresize()
//{
//    ini();
//}

function ini(){
    //maxy =425
    maxy=500
    scrollicon = 40*(Math.round(Math.round(document.body.offsetHeight/2.5)/25));
          if ((apk+document.body.offsetHeight) < maxy){
              //down2.style.display = '';
          }
          else {
              //down2.style.display = 'none';
          }
    //up2.style.pixelLeft = document.body.offsetWidth - 700
    //down2.style.pixelLeft = document.body.offsetWidth - 700
    //up2.style.pixelTop = 200
    //down2.style.pixelTop = document.body.offsetHeight - 25
}

function addy(){
   ik = apk;
    addygo();
}

	function addygo()
	{
	 if(inFloder.clientHeight <= 260){
    maxHeight=-10;
    }
    if(inFloder.clientHeight > 260 && inFloder.clientHeight <= 900){
    maxHeight=inFloder.clientHeight/1.8;
    }
    if(inFloder.clientHeight > 900 && inFloder.clientHeight <= 1150){
    maxHeight=inFloder.clientHeight/1.6;
    }
    if(inFloder.clientHeight > 1150){
    maxHeight=inFloder.clientHeight/1.4;
    }

    k = apk;

    inFloder.style.clip = 'rect(' + (k) + ' 100% ' + (k+270) + ' 0)';
    inFloder.style.top = (0 - k) + Position;

    //inFloder.style.top = (0 - 200)
    if ((k < ik + scrollicon) && k<(maxHeight)){
        apk = (k + 10);
        setTimeout('addygo()',20);
    }
    if ((k+document.body.offsetHeight)-25>=500){
    }
    }


function miny(){
    ih = apk;
    minygo();
}

function minygo(){
    h = apk
    inFloder.style.clip = 'rect(' + (h) + ' 100% ' + (h+270) + ' 0)';

    inFloder.style.top = (0 - h) + Position;

    if ((h > ih - scrollicon) && ( h >= 0)){
        apk = (h - 10);
        setTimeout('minygo()',20);
    }
    if (apk <= 0){
    }
}

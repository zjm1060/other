function LockControl(a_tag,a_style,a,a_IDs)
{
	var x,i,tag,type,IDs,IDName;
	IDs = (a_IDs != null) ? a_IDs + ',' : null;
	if (!a)
		a = document.all;
	if (!a_style)
		a_style = 'readonly';
	tag = a_tag.toLowerCase();
	type = '';
	switch(tag)
	{
	case 'textarea':
		break;
	case 'select':
		break;
	case 'checkbox':
	case 'password':
	case 'radio':
	case 'button':
	case 'text':
		type = tag;
		tag = 'input';
	default:
	}
	x = a.tags(tag);
	for (i=0;i<x.length;i++)
	{
		if (x[i].id)
			IDName = x[i].id;
		else if (x[i].name)
			IDName = x[i].name;
		else IDName = null;

		if (tag == 'input' && x[i].type != type)
			continue;

		if (IDName && IDs && IDs.indexOf(IDName + ',') >= 0 )
			continue;

		switch (a_style.toLowerCase())
		{
		case 'disabled':
			x[i].disabled = true;
			break;
		case 'readonly':
			x[i].readOnly = true;
			if ( x[i].onclick )
				x[i].onclick = null;
			break;
		default:
			break;
		}
		x[i].style.backgroundColor = '#c0c0c0';
	}
}

/*
function LockControls(a_IDs)
封锁本页面的所有text、checkbox、password、radio、textarea、select页面控件
把它们设置成只读或者禁止使用
输入：
	a_IDs：可选参数，以逗号分隔的字符串，记录本页面不想被锁住的页面控件的id或者name值
输出：
*/
function LockControls(a_IDs)
{
	var d,a;
	d = document;
	a = d.all;
	if (a)
	{
		LockControl('text','readonly',a,a_IDs);
		LockControl('checkbox','disabled',a,a_IDs);
		LockControl('password','readonly',a,a_IDs);
		LockControl('radio','disabled',a,a_IDs);
		LockControl('textarea','readonly',a,a_IDs);
		LockControl('select','disabled',a,a_IDs);
		LockControl('button','disabled',a,a_IDs);
		return;
	}
}

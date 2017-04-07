
function tArray() 
{
	this.nodes = new Array();
	this.count = 0;
	this.Find = tArrayFindNode;
	this.Add = tArrayAddNode;
	this.AddA = tArrayAddNodeA;
	this.Remove = tArrayRemoveNode;
	this.Update = tArrayUpdateNode;
	this.toString = tArraytoString;
}

function tArraytoString()
{
	var l_node,l_node2,l_return = '';
	for ( l_node in this.nodes )
	{
		l_return += "\n==tArraytoString==this.nodes['" + l_node + "']=";
		l_node2 = this.nodes[l_node];
		if ( l_node2.toString != null )
		{
			l_return += l_node2.toString();
		}else{
			l_return += "null";
		}
	}
	return l_return;
}

function tArrayFindNode(a_NodeName) 
{
	if ( this.nodes == null )
		return null;
	return this.nodes[a_NodeName];
}


function tArrayAddNodeA(a_NodeName,a_NodeValue)
{
	var l_node = this.Find(a_NodeName);
	if (l_node != null) 
		return null;
	this.nodes[a_NodeName] = a_NodeValue;
	this.count++;
	return this.count;
}


function tArrayAddNode(a_NodeName,a_NodeValue)
{
	var l_node, l_nodes;

	switch(arguments.length)
	{
	case 1: 
		if ( arguments[0].constructor != tArray && arguments[0].constructor != Array )
			return -2;
		l_nodes = (arguments[0].constructor == tArray) ? arguments[0].nodes : l_nodes = arguments[0];
		if ( l_nodes == null )
			return -3;
		for ( l_node in l_nodes )
		{
			this.AddA(l_node,l_nodes[l_node]);
		}
		break;
	case 2: 
		return this.AddA(a_NodeName,a_NodeValue);
		break;
	default:
		return -4;
	}

	return this.count;
}

function tArrayRemoveNode(a_NodeName)
{
	var l_node = this.Find(a_NodeName);
	if (l_node == null) 
		return null;
	this.nodes[a_NodeName] = null;
	this.count--;
	return this.count;
}

function tArrayUpdateNode(a_NodeName,a_NodeValue)
{
	var l_node = this.Find(a_NodeName);
	if (l_node == null) 
		return false;
	this.nodes[a_NodeName] = a_NodeValue;
	return true;
}



function tNodeBrotherCount()
{
	var l_node,l_ary,i=0;
	if ( this.paraNode == null )
	{
		l_ary = this.tree.nodes.nodes;
		for ( l_node in l_ary )
		{
			if ( l_ary[l_node].paraNode != null )
				continue;
			i++;
		}
		return i;
	}else{
		return this.paraNode.children.count;
	}
}

function tNodeIndex()
{
	var l_node,l_ary,i=0;
	if ( this.paraNode == null )
	{
		l_ary = this.tree.nodes.nodes;
		for ( l_node in l_ary )
		{
			if ( l_ary[l_node].paraNode != null )
				continue;
			if ( l_node == this.nodeName )
			{
				return i;
			}
			i++;
		}
		return -1;
	}else{
		l_ary = this.paraNode.children.nodes;
		for ( l_node in l_ary )
		{
			if ( l_node == this.nodeName )
			{
				return i;
			}
			i++;
		}
		return -1;
	}
}

function tNodeLayer()
{
	if ( this.paraNode == null )
		return 0;
	else
		return ( this.paraNode.Layer() + 1 );
}

function tNodeisLeaf()
{
	return ( this.children.count == 0 );
}

function tNodetoString()
{
	var l_return = '';
	l_return += "\n==tNodetoString==";
	l_return += "\nnodeName=" + this.nodeName;
	l_return += "\nparaName=" + this.paraName;
	l_return += "\nLayer=" + this.Layer();
	l_return += "\nIndex=" + this.Index();
	l_return += "\nBrotherCount=" + this.BrotherCount();
	l_return += "\nchildren.count=" + this.children.count;
	return l_return;
}

function tNodeUpdate(a_ParaName,a_ParaNode,a_tAry)
{
	this.paraName = a_ParaName;
	this.paraNode = a_ParaNode;
	this.array = a_tAry;
}



function tTreetoString()
{
	return this.nodes.toString();
}

function tTreeCount()
{
	return this.nodes.count;
}

function tFindNode(a_NodeName) 
{
	if ( this.nodes == null )
		return null;
	return this.nodes.Find(a_NodeName);
}

function tAddNode(a_NodeName,a_ParaName,a_tAry)
{
	var l_node = null,l_node2;

	if ( a_ParaName != null )
	{
		l_node = this.Find(a_ParaName);
		if ( l_node == null ) 
			return -1;
	}
	l_node2 = new tNode(a_NodeName,a_ParaName,l_node,a_tAry,this);

	
	if ( l_node != null && l_node.children.Add(a_NodeName,l_node2) == null )
		return -2;

	return this.nodes.Add(a_NodeName,l_node2);
}

function tRemoveNode(a_NodeName)
{
	var l_node,l_node2;
	l_node = this.nodes.Find(a_NodeName); 

	
	for ( l_node2 in l_node.children.nodes ) 
	{
		this.Remove(l_node2);
	}

	
	if ( l_node.paraNode != null ) 
		l_node.paraNode.children.nodes.Remove(a_NodeName);

	return this.nodes.Remove(a_NodeName);
}

function tUpdateNode(a_NodeName,a_ParaName,a_tAry)
{
	var l_node = null,l_node2;

	l_node2 = this.Find(a_NodeName); 
	if ( l_node2 == null ) 
		return -1;

	if ( a_ParaName != null)
	{
		l_node = this.Find(a_ParaName); 
		if ( l_node == null ) 
			return -2;

		if ( l_node2.children.Find(a_ParaName) != null ) 
			return -3; 
	}

	if ( l_node2.paraName != a_ParaName ) 
	{
		
		if ( l_node2.paraNode.children.Remove(a_ParaName) == null )
			return -4; 

		
		if ( l_node.children.Add(a_NodeName,l_node2) == null )
			return -5;
	}

	l_node2.Update(a_ParaName,l_node,a_tAry);
	
	return this.nodes.Update(a_NodeName,l_node2);
}

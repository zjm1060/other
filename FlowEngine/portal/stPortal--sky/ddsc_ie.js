<SCRIPT language="javascript">
//*****************************************************************************
// DDSC_ie.js
// 
// This file contains services component java script definitions for
// the Digital Dashboard Starter Kit.
// 
//
// Copyright (c) 2000 by Microsoft Corporation, All Rights Reserved
///////////////////////////////////////////////////////////////////////

	// The Part Object Definition
	function NullPropDef()
	{
		this.NamespaceURN = null;
		this.SchemaElement = null;
		this.Value = null;
	}

	// DDSC Null Property Collection Object Definition
	function NullPropColDef()
	{
		// Item Method
		function Item(Index)
		{
			return null;
		}

		// Add Method
		function Add(Index)
		{
			return new NullPropDef();
		}

		// Remove Method
		function Remove(Index)
		{
		}
		
		// Count Method
		function Count()
		{
			return 0;
		}
		
		this.Item = Item;
		this.Add = Add;
		this.Remove = Remove;
		this.Count = Count;
	}

	// The Item Object Definition
	function NullItemDef()
	{
		// Save Method
		function Save(UpdateFlag)  
		{
		}

		// EditSecurity Method
		function EditSecurity()  
		{
		}

		this.Save = Save;
		this.EditSecurity = EditSecurity;
		this.Properties = new NullPropColDef();
		this.Content = null;
		this.ID = null;
		this.Contents = new NullItemColDef();
	}

	// DDSC Null Item Collection Object Definition
	function NullItemColDef()
	{
		// Item Method
		function Item(Index)
		{
			return null;
		}

		// Add Method
		function Add(Name, IsFolder)
		{
			return new NullItemDef();
		}

		// Remove Method
		function Remove(Index)
		{
		}
		
		// Count Method
		function Count()
		{
			return 0;
		}
		
		this.Item = Item;
		this.Add = Add;
		this.Remove = Remove;
		this.Count = Count;
	}

	// The Null Part Object Definition
	function NullPartDef()
	{
		// Save Method
		function Save(UpdateFlag)  
		{
		}

		this.Save = Save;
		this.Properties = new NullPropColDef();
		this.DOMObject = null;
		this.WebPartQualifier = null;
		this.WebPartID = null;
	}

	// DDSC Null Part Collection Object Definition
	function NullPartColDef()
	{
		// Item Method
		function Item(Index)
		{
			return null;
		}

		// Register Method
		function Register(WebPartQualifier, WebPartID, DOMPart)
		{
			return new NullPartDef();
		}

		// UnRegister Method
		function UnRegister(Index)
		{
		}
		
		// Count Method
		function Count()
		{
			return 0;
		}
		
		this.Item = Item;
		this.Register = Register;
		this.UnRegister = UnRegister;
		this.Count = Count;
	}

	// The NULL Dashboard Object Definition
	function NullDashboardDef() 
	{
		// Save Method
		function Save(UpdateFlag)  
		{
		}

		this.Save = Save;
		this.Properties = new NullPropColDef();
		this.Parts = new NullPartColDef();
		this.DashboardID = null;
		this.DOMObject = null;
	}

	// The NULL DDSC Object Definition
	function NullDDSC()
	{
		// RaiseEvent Method
		function RaiseEvent(Namespace, Event, Data)  
		{
		}

		// RegisterEvent Method
		function RegisterForEvent(Namespace, Event, CallbackFunction)  
		{
		}

		// GetSessionState Method
		function GetSessionState(Namespace, MessageName)
		{
			return null;
		}

		// PutSessionState Method
		function PutSessionState(Namespace, MessageName, Value, Flag)
		{
		}

		// ClearSessionState Method
		function ClearSessionState(Namespace, MessageName)
		{
		}

		// Init Method
		function Init(DashboardID, DOMObj, StateServiceURL)
		{
			return null;
		}

		// LoadItem Method
		function LoadItem(ID, LoadFlag)
		{
			return NullItemDef();
		}

		this.RaiseEvent = RaiseEvent;
		this.RegisterForEvent = RegisterForEvent;
		this.GetSessionState = GetSessionState;
		this.PutSessionState = PutSessionState;
		this.ClearSessionState = ClearSessionState;
		this.Init = Init;
		this.LoadItem = LoadItem;
	}

///////////////////////////////////////////////////////////////////////

	// The Part Object Definition
	function PropDef(InternalProp)
	{
		// Internal part pointer
		var ThisProp = InternalProp;

		this.NamespaceURN = ThisProp.NamespaceURN;
		this.SchemaElement = ThisProp.SchemaElement;
		this.Value = ThisProp.Value;
	}

	// DDSC Property Collection Object Definition
	function PropColDef(InternalPropCol)
	{
		// Internal part collection pointer
		var ThisPropCol = InternalPropCol;

		// Item Method
		function Item(Index)
		{
			return new PropDef(ThisPropCol.Item(Index));
		}

		// Add Method
		function Add(Index)
		{
			return ThisPropCol.Add(Index);
		}

		// Remove Method
		function Remove(Index)
		{
			ThisPropCol.Remove(Index);
		}
		
		// Count Method
		function Count()
		{
			return ThisPropCol.Count;
		}
		
		this.Item = Item;
		this.Add = Add;
		this.Remove = Remove;
		this.Count = Count;
	}

	// The Item Object Definition
	function ItemDef(InternalItem)
	{
		// Internal part pointer
		var ThisItem = InternalItem;

		// Internal Properties Collection Object
		var InternalPropCol = null;

		// Internal Item Collection Object
		var InternalItemCol = null;

		// Properties Method
		function Properties()
		{
			if (InternalPropCol == null)
				InternalPropCol= new PropColDef(ThisItem.Properties);
			return InternalPropCol;
		}

		// Save Method
		function Save(UpdateFlag)  
		{
			ThisItem.Save(UpdateFlag);
		}

		// EditSecurity Method
		function EditSecurity()  
		{
			ThisItem.EditSecurity();
		}

		// Properties Method
		function Contents()
		{
			if (InternalItemCol == null)
				InternalItemCol= new ItemColDef(ThisItem.Contents);
			return InternalItemCol;
		}
	
		this.Save = Save;
		this.EditSecurity = EditSecurity;
		this.Properties = Properties;
		this.Content = ThisItem.Content;
		this.ID = ThisItem.ID;
		this.Contents = Contents;
	}

	// DDSC Item Collection Object Definition
	function ItemColDef(InternalItemCol)
	{
		// Internal item collection pointer
		var ThisItemCol = InternalItemCol;

		// Item Method
		function Item(Index)
		{
			return new ItemDef(ThisItemCol.Item(Index));
		}

		// Add Method
		function Add(Name, IsFolder)
		{
			return ThisItemCol.Add(Name, IsFolder);
		}

		// Remove Method
		function Remove(Index)
		{
			ThisItemCol.Remove(Index);
		}
		
		// Count Method
		function Count()
		{
			return ThisItemCol.Count;
		}
		
		this.Item = Item;
		this.Add = Add;
		this.Remove = Remove;
		this.Count = Count;
	}

	// The Part Object Definition
	function PartDef(InternalPart)
	{
		// Internal part pointer
		var ThisPart = InternalPart;

		// Internal Properties Collection Object
		var InternalPropCol = null;

		// Properties Method
		function Properties()
		{
			if (InternalPropCol == null)
				 InternalPropCol = new PropColDef(ThisPart.Properties);
			return InternalPropCol;
		}

		// Save Method
		function Save(UpdateFlag)  
		{
			ThisPart.Save(UpdateFlag);
		}

		this.Save = Save;
		this.Properties = Properties;
		this.DOMObject = ThisPart.DOMObject;
		this.WebPartQualifier = ThisPart.WebPartQualifier;
		this.WebPartID = ThisPart.WebPartID;
	}

	// DDSC Part Collection Object Definition
	function PartColDef(InternalPartCol)
	{
		// Internal part collection pointer
		var ThisPartCol = InternalPartCol;

		// Item Method
		function Item(Index)
		{
			return new PartDef(ThisPartCol.Item(Index));
		}

		// Register Method
		function Register(WebPartQualifier, WebPartID, DOMPart)
		{
			return ThisPartCol.Register(WebPartQualifier, WebPartID, DOMPart);
		}

		// UnRegister Method
		function UnRegister(Index)
		{
			ThisPartCol.UnRegister(Index);
		}
		
		// Count Method
		function Count()
		{
			return ThisPartCol.Count;
		}
		
		this.Item = Item;
		this.Register = Register;
		this.UnRegister = UnRegister;
		this.Count = Count;
	}

	// The Dashboard Object Definition
	function DashboardDef(InternalDashboard) 
	{
		// Internal part pointer
		var ThisDashboard = InternalDashboard;

		// Internal Parts Collection Object
		var InternalPartCol = new PartColDef(ThisDashboard.Parts);

		// Internal Properties Collection Object
		var InternalPropCol = null;

		// Properties Method
		function Properties()
		{
			if (InternalPropCol == null)
				InternalPropCol = new PropColDef(ThisDashboard.Properties);
			return InternalPropCol;
		}

		// Save Method
		function Save(UpdateFlag)  
		{
			ThisDashboard.Save(UpdateFlag);
		}

		this.Save = Save;
		this.Properties = Properties;;
		this.Parts = InternalPartCol;
		this.DashboardID = ThisDashboard.DashboardID;
		this.DOMObject = ThisDashboard.DOMObject;
	}

	// The Digital Dashboard Service Component Object Definition
	function DDSCDef() 
	{
		var ThisDDSC;

		// RaiseEvent Method
		function RaiseEvent(Namespace, Event, Data)  
		{
			ThisDDSC.RaiseEvent(Namespace, Event, Data);
		}

		// RegisterEvent Method
		function RegisterForEvent(Namespace, Event, CallbackFunction)  
		{
			ThisDDSC.RegisterForEvent(Namespace, Event, CallbackFunction);
		}

		// GetSessionState Method
		function GetSessionState(Namespace, MessageName)
		{
			return ThisDDSC.GetSessionState(Namespace, MessageName);
		}

		// PutSessionState Method
		function PutSessionState(Namespace, MessageName, Value, Flag)
		{
			ThisDDSC.PutSessionState(Namespace, MessageName, Value, Flag);
		}

		// ClearSessionState Method
		function ClearSessionState(Namespace, MessageName)
		{
			ThisDDSC.ClearSessionState(Namespace, MessageName);
		}

		// Init Method
		function Init(DashboardID, DOMObj, StateServiceURL)
		{
			return new DashboardDef(ThisDDSC.Init(DashboardID, DOMObj, StateServiceURL));
		}

		// LoadItem Method
		function LoadItem(ID, LoadFlag)
		{
			return new ItemDef(ThisDDSC.LoadItem(ID, LoadFlag));
		}

		this.RaiseEvent = RaiseEvent;   
		this.RegisterForEvent = RegisterForEvent
		this.GetSessionState = GetSessionState;
		this.PutSessionState = PutSessionState;
		this.ClearSessionState = ClearSessionState;
		this.Init = Init;
		this.LoadItem = LoadItem;

		try
		{
			ThisDDSC = InternalSC;
			InternalSC.init(varDashboardID, document, "");
			this.Dashboard = new DashboardDef(InternalSC.Dashboard);
		}
		catch (exception)
		{
			ThisDDSC = new NullDDSC();
			this.Dashboard = new NullDashboardDef();
		}
	}
</SCRIPT>

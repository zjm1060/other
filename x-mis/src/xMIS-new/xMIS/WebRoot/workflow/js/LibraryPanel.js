/*
 * $Id: LibraryPanel.js,v 1.4 2010/01/02 09:45:15 gaudenz Exp $
 * Copyright (c) 2006-2010, JGraph Ltd
 */
LibraryPanel = function()
{
    LibraryPanel.superclass.constructor.call(this,
    {
        region:'center',
        split:true,
        rootVisible:false,
        lines:false,
        autoScroll:true,
        root: new Ext.tree.TreeNode('tool'),
        collapseFirst:true
    });

    this.templates = this.root;

};

Ext.extend(LibraryPanel, Ext.tree.TreePanel, {

    addTemplate : function(name, icon, parentNode, cells)
    {
        var exists = this.getNodeById(name);
        
        if(exists)
        {
            if(!inactive)
            {
                exists.select();
                exists.ui.highlight();
            }
            
            return;
        }

        var node = new Ext.tree.TreeNode(
        {
        	text: name,
            icon: icon,
            leaf: true,
            cls: 'feed',
            cells: cells,
            id: name
        });
        
        if (parentNode == null)
        {
        	parentNode = this.templates;
        }

        parentNode.appendChild(node);
        
        return node;
    },

    // prevent the default context menu when you miss the node
    afterRender : function()
    {
        LibraryPanel.superclass.afterRender.call(this);
        /*
        this.el.on('contextmenu', function(e)
        {
            e.preventDefault();
        });
        */
    }
});
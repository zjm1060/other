
/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Cate : 2005.9.17 
	SYNC : 2007.12.7 by hcl
     

---------------------------------------------------------------------*/


#include        "../inc/oix.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
 
#define DEBUG

extern char   OIX_LIST_FONT[]; 

extern void Err_log( char *info_buf );
extern int parseXmlProp(xmlNodePtr node, const char *name);

#define XML_NODE_FONTS_STR	"Fonts"

char* ReadFontSetFromConfig(int ftset_id)
{
	xmlDocPtr   pDoc = NULL;
	xmlNodePtr  pRoot = NULL, pNodeFonts = NULL,pNodeFont = NULL;
	xmlChar *pValue;
	char* strFont=NULL;
	char str[128];
	int nFontID;
	gboolean bFind=FALSE;

	xmlKeepBlanksDefault(0);
	pDoc = xmlReadFile (FILE_OIX_CONFIG_XML, "GB2312", XML_PARSE_NOBLANKS);
	if (pDoc == NULL)
	{
		sprintf(str, "Fail to open %s or format error!", FILE_OIX_CONFIG_XML);
		Err_log(str);
		return NULL;
	}

	pRoot = xmlDocGetRootElement (pDoc);
	if (pRoot == NULL)
    {
		sprintf(str, "%s not have root", FILE_OIX_CONFIG_XML);
		Err_log(str);
		return NULL;
    }
	
	pNodeFonts = pRoot->xmlChildrenNode;
	while (pNodeFonts != NULL)
    {
		if (xmlStrcmp (pNodeFonts->name, BAD_CAST(XML_NODE_FONTS_STR)) == 0)
        {
			pNodeFont = pNodeFonts->xmlChildrenNode;
			while(pNodeFont != NULL)
			{
				if (xmlStrcmp (pNodeFont->name, BAD_CAST "Font") == 0)
				{	
					nFontID=parseXmlProp(pNodeFont, "id");
					if (nFontID != ftset_id)
					{
						pNodeFont = pNodeFont->next;
						continue;
					}

					pValue=xmlNodeGetContent(pNodeFont);

					if (pValue != NULL)
					{
						strFont=malloc(strlen((char*)pValue));
						memcpy(strFont, pValue, strlen((char*)pValue));
					}

					xmlFree(pValue);

					bFind=TRUE;

					break;
				}
				pNodeFont = pNodeFont->next;
			}
        }

		if (bFind)
		{
			break;
		}

		pNodeFonts = pNodeFonts->next;
    }
	
	xmlFreeDoc (pDoc);
	xmlCleanupParser();

	return strFont;
}

GdkFont*	ReadFont (int ftset_id)

{
    GdkFont        	*ftset_tmp;
	char* strFont;	
        
	ftset_tmp	= font_set[ftset_id];

	if (ftset_tmp == NULL)
	{
		if ((strFont=ReadFontSetFromConfig(ftset_id)) != NULL)
		{
			ftset_tmp=gdk_fontset_load(strFont);
			
			free(strFont);
		}
		
		if (ftset_tmp == NULL)
		{
			ftset_tmp = gdk_fontset_load( font_name[ftset_id]);  
		}
		
 		if (ftset_tmp == NULL) 
 			g_message("Load font[%d] is failure!!!" ,ftset_id);
		font_set[ftset_id] = ftset_tmp;
		printf("font_set[%d]=%d\n",ftset_id,font_set[ftset_id]);
	}
	return ftset_tmp;
}



void	GetBaselineCoordFromRcd( rcd, bsl_x, bsl_y)
GBRCD		*rcd; 
int		*bsl_x, *bsl_y;
{
	int	lb ,rb ,width ,ascent ,descent,height;
/*
		XRectangle      ovl_ink;
		XRectangle      ovl_lgc;*/
	
	GdkFont        *fontset;

	fontset = font_set[rcd->bck.ftset_id];
	/*npx =    XmbTextExtents( fontset, rcd->bck.data.text.text, strlen( rcd->bck.data.text.text ),  &ovl_ink,  &ovl_lgc);*/
	
	gdk_text_extents(fontset,rcd->bck.data.text.text, strlen( rcd->bck.data.text.text ),&lb,&rb,&width,&ascent,&descent);
	height = gdk_text_height(fontset ,rcd->bck.data.text.text, strlen( rcd->bck.data.text.text ));
	
	*bsl_x = rcd->bck.rect.x1;
	*bsl_y = rcd->bck.rect.y1 ; /* +height ;  -ovl_lgc.y */
}


void	CreateFontX()
{
	int		font_num;
	char*	strFont=NULL;
	
	for(font_num=0; font_num<MAX_FONTSET; font_num++)
	{
#ifdef	DEBUG
		printf ("OIX: CreateFontSet: %d\n", font_num);
#endif
		ReadFont (font_num);
	}

	strFont=ReadFontSetFromConfig(MAX_FONTSET);
	if (strFont != NULL)
	{
		memcpy(OIX_LIST_FONT, strFont, strlen(strFont));
		free(strFont);
	}
}





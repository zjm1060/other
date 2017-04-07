
#include <stdio.h>
#include <string.h>

#include	"../inc/string_pattern.h"

/*--------------------------------------------------------------------------------
!_		4.x		parse_pattern - Parse a Wildcard Pattern String
!_
!_		4.x.1	Module Function
!_				---------------
!_		This module parses a pattern string that can contain up to 2 wildcard (*)
!_		characters.  It returns a pattern information block that indicates the
!_		patern type and the fixed string portions.  The valid types are:
!_
!_				ABCDEF		NO_STAR
!_				ABC*		RIGHT_STAR
!_				*ABC		LEFT_STAR
!_				*ABC*		LEFT_RIGHT_STAR
!_				ABC*DEF		MID_STAR
!_
!--------------------------------------------------------------------------------*/

int 	ParsePattern ( pattern, pat_info )
char 		*pattern;
PATTERN_INFO 	*pat_info;
{
   	int 	return_code;
   	char 	*star_ptr;
   	char 	*ptr;
   	int 	len;  

   	return_code 	= 0;

/*_     IF pattern isn't empty */
	if ( pattern != NULL && *pattern != 0 ) 
	{
/*_     IF pattern has a star */ 
      	if ( (star_ptr = strchr(pattern,'*')) != NULL ) 
		{
/*_  		IF pattern has a left star */
         	if ( star_ptr == pattern ) 
			{
/*_         	IF pattern has no other stars */
            	if ( star_ptr == strrchr(pattern,'*') ) 
				{
/*_             	Set pat_info for LEFT_STAR */           
               		pat_info->type 		= LEFT_STAR;
               		pat_info->left_str 	= NULL;
               		pat_info->left_len 	= 0;
               		pat_info->right_str 	= pattern+1;
               		pat_info->right_len 	= strlen(pat_info->right_str); 
            	}
/*_             ELSEIF pattern also has an ending star */
            	else if ( (ptr = strchr(pattern+1,'*')) - star_ptr ==
                     		  	(len = strlen(pattern+1)) ) 
				{
/*_                	Set pat_info for LEFT_RIGHT_STAR */
               		pat_info->type 		= LEFT_RIGHT_STAR;
               		pat_info->left_str 	= pattern+1;
               		pat_info->left_len 	= len-1;
               		pat_info->right_str 	= pat_info->left_str;
               		pat_info->right_len 	= pat_info->left_len;
            	}
/*_             ELSE there is a star in the middle */
            	else 
				{
/*_             	Set a bad pattern return code */
/*
               		return_code 	= BAD_PATTERN_ERR ;
            */	}
         	}
/*_         ELSEIF pattern is a valid MID or RIGHT STAR */
         	else if ( star_ptr == strrchr(pattern,'*') ) 
			{
/*_         	Set pat_info structures for left */
            	pat_info->left_str 	= pattern;
            	pat_info->left_len 	= star_ptr - pattern; 

/*_             IF the string following the star is empty */
            	if ( strlen(star_ptr) == 1 ) 
				{
/*_             	Set pat_info for RIGHT_STAR */
               		pat_info->type 		= RIGHT_STAR;
               		pat_info->right_str 	= NULL;
               		pat_info->right_len 	= 0;
            	}
/*_             ELSE string following the star isn't empty */
            	else 
				{
/*_             	Set pat_info for MID_STAR */
               		pat_info->type 		= MID_STAR;
               		pat_info->right_str 	= star_ptr+1;
               		pat_info->right_len 	= strlen(pat_info->right_str);
            	}
         	}
/*_        	ELSE pattern is bad */
         	else 
			{
/*_           			Set a bad pattern return code */
/*
               			return_code 	= BAD_PATTERN_ERR;
	*/		}
        }
/*_     ELSE there is no star */
      	else 
		{
/*_        	Set pat_info for NO_STAR */
         	pat_info->type 		= NO_STAR;
         	pat_info->left_str 	= pattern;
         	pat_info->left_len 	= strlen(pattern);
         	pat_info->right_str 	= pat_info->left_str;
         	pat_info->right_len 	= pat_info->left_len;
      	}
   	}
   	else 
	{
/*
      		return_code 	= NULL_PATTERN_ERR;
*/	}

   	return 	return_code;
}



#ifndef SUICACSE_H

#define SUICACSE_H

/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*		1988 - 2002, All Rights Reserved			*/
/*									*/
/* MODULE NAME : suicacse.h						*/
/* PRODUCT(S)  : SUIC 3.0						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/* This module contains SUIC definitions and external declarations for	*/
/* global SUIC variables. This module should be included with user	*/
/* functions that need to interface with SUIC				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 01/08/02  EJV     09    Keep MARBEN undefined: not needed in MMS-Lite*/
/* 12/14/01  EJV     08    Del code for unsupported platforms & hardware*/
/* 04/26/99  DSF     07    Security related changes			*/
/* 02/09/98  RKR     06    Made ChannelTypeToUse perm part of DIB_ENTRY */ 
/* 11/13/97  DSF     05    Changed APP_REF_FORM defines to be consistent*/
/*			   with ASS_INFO				*/
/* 09/26/97  DSF     04    Added ACSE_ABORT_PRINT			*/
/* 08/13/97  DSF     03    AR Name management support			*/
/* 07/29/97  DSF     02    Security Attack stuff			*/
/* 07/29/97  DSF     01    APP_REF stuff moved here			*/
/* 04/11/97  DSF   7.00    MMSEASE 7.0 release. See MODLS70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
									  
#ifdef ADD_COT7_EXT
#include "cot7df.h"
#endif

#ifdef ADD_RS_EXT
#include "rsdf.h"
#endif

#include "glberror.h"
#include "asn1defs.h"

#include "slog.h"

/************************************************************************/
/* Defines that all SUICs should use. Must be before other #includes	*/
/************************************************************************/

/* Transport provider definitions */
#define TP4		0
#ifndef PSOS_SYSTEM
#define TCP		1
#endif

/* Selector size limits */
#define MAX_P_SEL	16
#define MAX_S_SEL	16
#define MAX_T_SEL	32
#define MAX_N_SEL	20

#define IP_ADDR_LEN	4
#define HOST_NAME_LEN	64

#define RFC1006_PORT	102

#define MAX_NAME_LEN		64
#define MAX_HOST_LEN		32

#define TAG_MNGT_ADDR		0
#define TAG_MNGT_HOST		1

/************************************************************************/
/* This define represents the maximum number of channels.		*/

extern ST_INT s_max_chan;	/* actual number of suic channels used	*/

extern ST_INT s_msgsize;		/* max transfer data size		*/

/************************************************************************/
/* Select the board type here						*/
/* These defines are used in the sample source code to make use of	*/
/* board specific operations, such as status or debug displays. 	*/

/** #define DECNP5 **/
/** #define MARBEN **/

			/* Now select the board specific include file	*/
#ifdef DECNP5
#define MAX_SUIC_CHAN 200
#include "decnp5_a.h"		/* OpenVMS */
#endif

#ifdef MARBEN
#include "marben_a.h"		/* MARBEN				*/
#endif

#ifdef REDUCED_STACK
#include "rs_a.h"
#endif

#ifdef LEAN_T
#include "lean_a.h"
#endif

#define DIRECTORY_SUPPORT	(defined (X500_SUPPORT) || defined (LDAP_SUPPORT))

#include "gen_list.h"

/************************************************************************/
/************************************************************************/
/* Error code definitions						*/
/* These codes are returned when a SUIC command function detects an	*/
/* error condition.							*/
/************************************************************************/

#define SE_CPU_ALLOC  		0x0B01		/* Memory allocation error	*/
#define SE_CHAN_ERR   		0x0B02		/* Invalid channel number	*/
#define SE_QUE_FULL   		0x0B03		/* SUIC send queue full		*/
#define SE_CHAN_CTRL  		0x0B04		/* Channel control error	*/
#define SE_CTRL_ALLOC		0x0B05		/* control block alloc error	*/
#define SE_INSFRES		0x0B06		/* insufficient SUIC resources	*/
#define SE_NAMENOTFOUND		0x0B07		/* name not found in local DIB	*/
#define SE_CHANNOTREG		0x0B08		/* channel not registered	*/
#define SE_NOPARAM		0x0B09		/* mandatory param missing	*/
#define SE_INVPARAM		0x0B0A		/* invalid param supplied	*/
#define SE_INVPDU		0x0B0B		/* invalid PDU encounetred	*/
#define SE_INVASN		0x0B0C		/* invalid ASN specified	*/
#define SE_INVTSN		0x0B0D 		/* invalid TSN specified	*/
#define SE_INVCTXT		0x0B0E		/* invalid AP context specified	*/
#define SE_INVPCI		0x0B0F		/* invalid PCI specified	*/
#define SE_INVOBJID		0x0B10		/* invalid OBJECT IDENTIFIER	*/
#define SE_SIMPLY_ENC		0x0B11		/* simply encoded data		*/
#define SE_MSGSIZETOOBIG	0x0B12		/* user-set s_smgsize too big   */
#define SE_NOLLP	        0x0B13		/* your LLP isn't up 		*/
#define SE_NOT_BOUND		0x0B14		/* not bound to a DSA		*/
#define SE_MDS_UNEXPECTED	0x0B15		/* unexpected MDS result	*/
#define SE_MDS_INVALID_SYNTAX	0x0B16		/* invalid attribute syntax	*/
#define SE_BAD_MECH_NAME	0x0B17		/* unrecognized authentication mechanism name */
#define SE_SECURITY_CONTEXT	0x0B18		/* security context could not be created */
#define SE_SECURE_DATABASE	0x0B19		/* Security Database access error */
#define SE_ENCRYPT		0x0B1A		/* DES CBC Encryption problem 	*/
#define SE_DECRYPT		0x0B1B		/* DES CBC Decryption problem	*/
#define SE_ACTIVATE		0x0B1C		/* Name activation problem	*/
#define SE_DEACTIVATE		0x0B1D		/* Name deactivation problem	*/


/************************************************************************/
/* SUIC Debug mechanism variables, functions.				*/
/************************************************************************/

/* These are needed for compatibility only                              */
#define CTRLRX_PRINT	0x01
#define CTRLTX_PRINT	0x02
#define CTRLERR_PRINT	0x04
#define SUICDEC_PRINT	0x08

#define ACSE_IND_PRINT	0x01
#define ACSE_CNF_PRINT	0x02
#define ACSE_ERR_PRINT	0x04
#define ACSE_DEC_PRINT	0x08

#if defined(vms)
#define OSAK_PRINT      0x0010
#define OSAK_DEB    0x0020
#endif
#define ACSE_NERR_PRINT	0x40
#define ACSE_ABORT_PRINT	0x80

#define ACSE_IND_PRINT_TYPE	2001
#define ACSE_CNF_PRINT_TYPE	2002
#define ACSE_ERR_PRINT_TYPE     2003
#define ACSE_DEC_PRINT_TYPE	2004
#define ACSE_NERR_PRINT_TYPE	2005
#define ACSE_ABORT_PRINT_TYPE	2006

extern ST_UINT s_debug_sel;
extern ST_VOID (*s_menu_fun) (ST_VOID);

/************************************************************************/
/* The variable below is set to exception code when exceptions occur.	*/
/************************************************************************/

extern ST_INT s_err;	        /* set when SUIC exception detected	*/
extern ST_INT s_map_err;       /* set when E_SUBSYS id used            */

/************************************************************************/
/* Exception code definitions						*/
/************************************************************************/

#define SX_INTERNAL     	0x0F01  	/* internal SUIC error          */
#define SX_CPU_ALLOC    	0x0F02		/* memory allocation error	*/
#define SX_CTRL_ALLOC   	0x0F03		/* control block alloc error	*/
#define SX_NOCTRL		0x0F04		/* Out of control blocks	*/
#define SX_NODATA		0x0F05		/* Out of data buffers		*/
#define SX_QUE_FULL   		0x0F06		/* SUIC send queue full		*/
#define SX_INVPDU		0x0F07		/* invalid PDU data field	*/
#define SX_INVCHAN		0x0F08		/* invalid channel number	*/
#define SX_BADCB		0x0F09		/* unknown CB from board	*/
#define SX_ENCERR		0x0F0A		/* ASN.1 encode error		*/
#define SX_CTRL_FREE		0x0F0B		/* control block free error	*/
#define SX_DATA_FREE		0x0F0C		/* data buffer free error	*/
#define SX_DATABUF_OVRFLW       0x0F0D     	/* data being received  	*/
                                        	/* exceeds s_msgsize            */

/************************************************************************/
/* SUIC state masks							*/
/************************************************************************/

#define SM_CHAN_REG		0x0001	/* channel registered		*/
#define SM_ASS_REQ_PEND		0x0002	/* associate request pending	*/
#define SM_LISTEN		0x0004	/* listen outstanding		*/
#define SM_ASS_ACTIVE		0x0008	/* association active		*/
#define SM_CANLISTEN_PEND	0x0010	/* cancel pending		*/
#define SM_REL_REQ_PEND		0x0020	/* release request pending	*/
#define SM_ABT_REQ_PEND		0x0040	/* abort request pending	*/
#define SM_P_ABORT              0x0080  /* Issue P-Abort to User        */
#define SM_ASS_IND_RCVD         0x0100  /* associate Indication received*/
#define SM_ASS_RSP_PEND         0x0200  /* associate response pending   */
#define SM_ASS_REJ_PEND         0x0400  /* associate reject pending     */
#define SM_REL_IND_RCVD         0x0800  /* release indication received  */
#define SM_REL_RSP_PEND         0x1000  /* release response pending     */
#define SM_MDS_READ_PEND        0x2000  /* MDS read request pending	*/


/************************************************************************/
/* Security-specific structures and function declaration		*/
/************************************************************************/

#ifdef SECURITY_SUPPORT

#define NUM_PASSWD_LEVELS	8
#define MAX_USER_NAME		65
#define MAX_PASSWD_LEN		129
#define KEY_LENGTH		8
#define IV_LENGTH		8

/************************************************************************/
/* Structure used to store encryption key information			*/
/************************************************************************/

typedef struct key_info
  {
  ST_INT index;
  ST_CHAR key[KEY_LENGTH];			/* key */
  } KEY_INFO;

/************************************************************************/
/* Structure used to store password information				*/
/************************************************************************/

typedef struct password_info
  {
  ST_INT level;
  ST_BOOLEAN passwd_pres;			/* password present flag */
  ST_CHAR *password;			/* password */
  } PASSWORD_INFO;

/************************************************************************/
/* Application name - used to identify who we are for incoming connections */
/************************************************************************/

extern ST_CHAR s_appname[];
extern ST_BOOLEAN s_app_secure;		/* indicates if application is secure */
extern ST_INT s_num_app_pass;		/* number of config'd passwords */
extern PASSWORD_INFO *s_app_pass_tbl;	/* table of passwords */

/************************************************************************/
/* Structure used to return security information for a remote name	*/
/************************************************************************/

typedef struct crypto_cfg
  {
  ST_CHAR *name;		/* Remote AR Name			*/
  ST_INT algo_type;		/* Type of encryption method		*/

  MMS_OBJ_ID mech_name;

  union
    {
    struct 
      {
      ST_BOOLEAN passwd_pres;		/* length of encrypted password */
      ST_CHAR *password;
      } pass;

    struct
      {
      MMS_OBJ_ID name_id;
      ST_INT index;
      ST_CHAR key[KEY_LENGTH];
      ST_CHAR iv[IV_LENGTH];
      ST_BOOLEAN passwd_pres;		/* password present flag */
      ST_CHAR *password;		/* password */
      } descbc;
    } u;

  ST_CHAR *user_reserved;		/* reserved for the user */
  } CRYPTO_CFG;

/************************************************************************/
/* Structure used to store user security information			*/
/************************************************************************/

typedef struct user_crypto_info
  {
  ST_CHAR user_name[MAX_USER_NAME];

  MMS_OBJ_ID name_id;
  ST_BOOLEAN detect_replay;
  ST_INT replay_timeout;

  ST_INT num_crypto_cfg;		/* number of CRYPTO_CFG records	*/
  CRYPTO_CFG *crypto_cfg; 		/* head of CRYPTO_CFG record array */
  } USER_CRYPTO_INFO;

extern ST_INT s_num_user_crypto_info;		/* number of User records */
extern USER_CRYPTO_INFO *s_user_crypto_info;	/* head of user array */

/************************************************************************/
/* Structure used to store pointers to algo-specific handler functions	*/
/************************************************************************/

typedef struct crypto_info
  {
  ST_CHAR *(*create_calling_context) (CRYPTO_CFG *crypto_cfg);
  ST_CHAR *(*create_responding_context) (MMS_OBJ_ID *mech_name, 
	  	           	         ST_INT auth_value_len,
		     		         ST_CHAR *auth_value);
  ST_RET (*set_calling_auth) (ST_CHAR *calling_context, 
  			      MMS_OBJ_ID *mech_name,
  			      ST_INT *auth_value_len, 
		              ST_CHAR **auth_value);
  ST_RET (*set_responding_auth) (ST_CHAR *responding_context,
  			         MMS_OBJ_ID *mech_name,
  			   	 ST_INT *auth_value_len, 
		           	 ST_CHAR **auth_value);
  ST_RET (*set_mech_info) (MMS_OBJ_ID *mech_name, 
	  	           ST_INT auth_value_len,
		     	   ST_CHAR *auth_value,
			   ST_INT *mech_info_len,
			   ST_CHAR **mech_info);
  ST_UINT (*set_asn_mask) (ST_UINT asn_mask);
  ST_UINT (*get_asn_mask) (ST_UINT asn_mask);
  ST_RET (*encrypt_assoc_req) (ST_CHAR *calling_context, 
		               ST_INT in_len, ST_CHAR *in_buff,
		      	       ST_INT *out_len, ST_CHAR **out_buff);
  ST_RET (*encrypt_assoc_rsp) (ST_CHAR *responding_context, 
		               ST_INT in_len, ST_CHAR *in_buff,
		      	       ST_INT *out_len, ST_CHAR **out_buff);
  ST_RET (*decrypt_assoc_req) (ST_CHAR *responding_context, 
		               ST_INT in_len, ST_CHAR *in_buff,
		      	       ST_INT *out_len, ST_CHAR **out_buff);
  ST_RET (*decrypt_assoc_rsp) (ST_CHAR *calling_context, 
		               ST_INT in_len, ST_CHAR *in_buff,
		      	       ST_INT *out_len, ST_CHAR **out_buff);
  ST_RET (*encrypt_data) (ST_CHAR *context, 
		          ST_INT in_len, ST_CHAR *in_buff,
		      	  ST_INT *out_len, ST_CHAR **out_buff);
  ST_RET (*decrypt_data) (ST_CHAR *context, 
		          ST_INT in_len, ST_CHAR *in_buff,
		      	  ST_INT *out_len, ST_CHAR **out_buff);
  ST_RET (*destroy_calling_context) (ST_CHAR *calling_context);
  ST_RET (*destroy_responding_context) (ST_CHAR *responding_context);

  } CRYPTO_INFO;


/************************************************************************/
/* Structure used to store algo-specific information			*/
/************************************************************************/

typedef struct algo_info
  {
  ST_INT algo_type;
  MMS_OBJ_ID mech_name;
  CRYPTO_INFO crypto_info;
  } ALGO_INFO;

extern ALGO_INFO algo_info_array[];

#define ALGO_PASSWORD		0
#define ALGO_DESCBC		1
#define ALGO_OTHER		2

ALGO_INFO *s_find_crypto (MMS_OBJ_ID *mech_name);


/************************************************************************/
/* Handlers for Password-based algorithm				*/
/************************************************************************/

ST_CHAR *mcp_create_calling_context (CRYPTO_CFG *crypto_cfg);
ST_RET mcp_set_calling_auth (ST_CHAR *calling_context, 
			     MMS_OBJ_ID *mech_name,
			     ST_INT *auth_value_len, 
	    	             ST_CHAR **auth_value);
ST_RET mcp_set_mech_info (MMS_OBJ_ID *mech_name, 
			  ST_INT auth_value_len,
		     	  ST_CHAR *auth_value, 
			  ST_INT *mech_info_len,
		          ST_CHAR **mech_info);


/************************************************************************/
/* Handlers for Password-based algorithm				*/
/************************************************************************/

ST_CHAR *mcd_create_calling_context (CRYPTO_CFG *crypto_cfg);
ST_CHAR *mcd_create_responding_context (MMS_OBJ_ID *mech_name, 
	  			        ST_INT auth_value_len,
		     		        ST_CHAR *auth_value);
ST_RET mcd_set_calling_auth (ST_CHAR *calling_context, 
			     MMS_OBJ_ID *mech_name,
			     ST_INT *auth_value_len, 
	    	             ST_CHAR **auth_value);
ST_RET mcd_set_mech_info (MMS_OBJ_ID *mech_name, 
			  ST_INT auth_value_len,
		     	  ST_CHAR *auth_value, 
			  ST_INT *mech_info_len,
		          ST_CHAR **mech_info);
ST_UINT mcd_set_asn_mask (ST_UINT asn_mask);
ST_UINT mcd_get_asn_mask (ST_UINT asn_mask);
ST_RET mcd_encrypt_assoc_req (ST_CHAR *calling_context, 
	    	              ST_INT in_len, ST_CHAR *in_buff,
		     	      ST_INT *out_len, ST_CHAR **out_buff);
ST_RET mcd_encrypt_assoc_rsp (ST_CHAR *responding_context, 
	    	              ST_INT in_len, ST_CHAR *in_buff,
		     	      ST_INT *out_len, ST_CHAR **out_buff);
ST_RET mcd_decrypt_assoc_req (ST_CHAR *responding_context, 
	    	              ST_INT in_len, ST_CHAR *in_buff,
		     	      ST_INT *out_len, ST_CHAR **out_buff);
ST_RET mcd_decrypt_assoc_rsp (ST_CHAR *calling_context, 
	    	              ST_INT in_len, ST_CHAR *in_buff,
		     	      ST_INT *out_len, ST_CHAR **out_buff);
ST_RET mcd_encrypt_data (ST_CHAR *context, 
		         ST_INT in_len, ST_CHAR *in_buff,
		         ST_INT *out_len, ST_CHAR **out_buff);
ST_RET mcd_decrypt_data (ST_CHAR *context, 
		         ST_INT in_len, ST_CHAR *in_buff,
		         ST_INT *out_len, ST_CHAR **out_buff);
ST_RET mcd_destroy_calling_context (ST_CHAR *calling_context);
ST_RET mcd_destroy_responding_context (ST_CHAR *responding_context);

/************************************************************************/
/* Structures used to store mechanism-specific information		*/
/************************************************************************/

typedef struct passbased_info
  {
  ST_BOOLEAN passwd_pres;		/* password present flag */
  ST_INT level;				/* password level */
  ST_CHAR password[MAX_PASSWD_LEN];	/* actual password */
  } PASSBASED_INFO;

typedef struct descbc_info
  {
  MMS_OBJ_ID sender_id;			/* sender name */
  MMS_OBJ_ID recip_id;			/* recipient name */
  ST_INT index;	      			/* into local user key table */
  ST_CHAR key[KEY_LENGTH];	      	/* actual key */
  ST_CHAR iv[IV_LENGTH];		/* Initialization Vector */
  ST_BOOLEAN passwd_pres;		/* password present flag */
  ST_INT level;				/* password level */
  ST_CHAR password[MAX_PASSWD_LEN];	/* actual password */
  } DESCBC_INFO;


/************************************************************************/
/* Security attack notification pointer					*/
/************************************************************************/

#define SA_GENERAL				0
#define SA_BAD_AUTH_VALUE			1
#define SA_MASTER_KEY_NOT_CONFIGURED		2
#define SA_AUTH_TOKEN_DECRYPT			3
#define SA_RECIP_NAME				4
#define SA_REPLAY				5
#define SA_DECRYPT				6

extern ST_VOID (*s_security_attack) (ST_INT chan, ST_INT reason);
extern ST_INT sa_reason;

#endif

/************************************************************************/
/* The structure below is used to save the various info about the	*/
/* state of the channels on a per-channel basis.			*/
/* This structure is used to pass data.					*/
/************************************************************************/

struct s_chan_info
{
  ST_LONG reserved;		/* reserved for SUIC			*/

  ST_CHAR *name;			/* local name				*/

  ST_INT transport;		/* TP4 vs. TCP/IP			*/

  ST_UINT AP_ctxt_mask;		/* application context name mask   	*/

  ST_BOOLEAN AP_title_pres;		/* present flag				*/
  MMS_OBJ_ID AP_title;		/* AP title    				*/ 
  ST_BOOLEAN AP_inv_id_pres;		/* present flag				*/
  ST_INT32 AP_invoke_id;		/* AP invocation ID    			*/
  ST_BOOLEAN AE_qual_pres;		/* present flag				*/
  ST_INT32 AE_qual;			/* AE qualifier    			*/
  ST_BOOLEAN AE_inv_id_pres;		/* present flag				*/
  ST_INT32 AE_invoke_id;		/* AE invocation ID    			*/

  ST_INT p_sel_len;		/* P-selector length			*/
  ST_UCHAR p_sel[MAX_P_SEL];	/* P-selector				*/
  ST_INT s_sel_len;		/* S-selector length			*/
  ST_UCHAR s_sel[MAX_T_SEL];	/* S-selector				*/
  ST_INT t_sel_len;		/* T-selector length			*/
  ST_UCHAR t_sel[MAX_T_SEL];	/* T-selector				*/
  ST_INT net_addr_len;		/* Network address length		*/
  ST_UCHAR net_addr[MAX_N_SEL];	/* Network address			*/
  ST_CHAR  ip_addr[HOST_NAME_LEN + 1];	/* TCP/IP dotted decimal or host name 	*/
  ST_INT port;			/* TCP/IP port number			*/

  ST_CHAR *part_name;		/* partner name				*/
  ST_INT part_transport;	/* TP4 vs. TCP/IP			*/

  ST_UINT part_AP_ctxt_mask;	/* partner application context name mask*/

  ST_BOOLEAN part_AP_title_pres;	/* present flag				*/
  MMS_OBJ_ID part_AP_title;	/* partner AP title    			*/
  ST_BOOLEAN p_AP_inv_id_pres; 	/* present flag				*/
  ST_INT32 part_AP_invoke_id; 	/* partner AP invocation ID    		*/
  ST_BOOLEAN part_AE_qual_pres;	/* present flag				*/
  ST_INT32 part_AE_qual;		/* partner AE qualifier   		*/
  ST_BOOLEAN p_AE_inv_id_pres; 	/* present flag				*/
  ST_INT32 part_AE_invoke_id; 	/* partner AE invocation ID		*/

  ST_INT part_p_sel_len;		/* P-selector length			*/
  ST_UCHAR part_p_sel[MAX_P_SEL];	/* P-selector				*/
  ST_INT part_s_sel_len;		/* S-selector length			*/
  ST_UCHAR part_s_sel[MAX_T_SEL];	/* S-selector				*/
  ST_INT part_t_sel_len;		/* T-selector length			*/
  ST_UCHAR part_t_sel[MAX_T_SEL];	/* T-selector				*/
  ST_INT part_net_addr_len;		/* Network address length	*/
  ST_UCHAR part_net_addr[MAX_N_SEL];	/* Network address		*/
  ST_CHAR  part_ip_addr[HOST_NAME_LEN + 1];/* TCP/IP dotted decimal or host name */
  ST_INT part_port;			/* TCP/IP port number			*/

  ST_INT state_mask;		/* association state mask		*/

#ifdef SECURITY_SUPPORT
  ST_INT algo_type;		/* Type of encryption method		*/
  MMS_OBJ_ID mech_name;		/* mechanism name			*/
  ST_INT auth_value_len;	/* length of ASN.1 authentication value	*/
  ST_CHAR *auth_value;		/* ASN.1 authentication value		*/
  CRYPTO_CFG *crypto_cfg;	/* Crypto Config structure		*/
  CRYPTO_INFO *crypto_info;	/* Crypto Info structure		*/
  ST_CHAR *calling_context;	/* Calling security context		*/
  ST_CHAR *responding_context;	/* Responding security context		*/
#endif

#ifdef DECNP5
  ST_BOOLEAN already_gave_buffers;	/* OSAK doesn't need any more buffers.	*/
#endif
} ;

typedef struct s_chan_info CHAN_INFO;

extern CHAN_INFO *s_chan_info;


/************************************************************************/
/* The structure below is used to save the various info about the	*/
/* state of the association on a channel-by-channel basis.		*/
/* This structure is used to receive data.				*/
/************************************************************************/

struct acse_ar_info
  {
  ST_LONG reserved1;	             	/* reserved for provider use    */
  ST_CHAR *ar_name;	         	/* local name                   */
  ST_INT transport;			/* TP4 vs. TCP/IP		*/

  ST_UINT AP_ctxt_mask;	           	/* reserved for provider use	*/
  ST_INT AP_title_form;          	/* 0 == form 1, 1 == form 2	*/
  ST_BOOLEAN AP_title_pres;          	/* present flag                 */
  MMS_OBJ_ID AP_title;          	/* AP title                     */
  ST_BOOLEAN AP_inv_id_pres;         	/* present flag                 */
  ST_INT32 AP_invoke_id;            	/* AP invocation ID             */
  ST_BOOLEAN AE_qual_pres;           	/* present flag                 */
  ST_INT32 AE_qual;                 	/* AE qualifier                 */
  ST_BOOLEAN AE_inv_id_pres;         	/* present flag                 */
  ST_INT32 AE_invoke_id;            	/* AE invocation ID             */

  ST_INT p_sel_len;			/* P-selector length		*/
  ST_UCHAR p_sel[MAX_P_SEL];		/* P-selector			*/
  ST_INT s_sel_len;			/* S-selector length		*/
  ST_UCHAR s_sel[MAX_T_SEL];		/* S-selector			*/
  ST_INT t_sel_len;			/* T-selector length		*/
  ST_UCHAR t_sel[MAX_T_SEL];		/* T-selector			*/
  ST_INT net_addr_len;			/* Network address length	*/
  ST_UCHAR net_addr[MAX_N_SEL];		/* Network address		*/
  ST_CHAR  ip_addr[HOST_NAME_LEN + 1];	/* TCP/IP dotted decimal or host name 	*/

  ST_BOOLEAN part_name_found;        	/* partner name found in DIB     */
  ST_CHAR *part_ar_name;        		/* partner name                 */
  ST_UINT part_AP_ctxt_mask;		/* reserved for provider	*/
  ST_INT part_AP_title_form;          	/* 0 == form 1, 1 == form 2	*/
  ST_BOOLEAN part_AP_title_pres;     	/* present flag                 */
  MMS_OBJ_ID part_AP_title;     	/* partner AP title             */
  ST_BOOLEAN part_AP_inv_id_pres;       	/* present flag                 */
  ST_INT32 part_AP_invoke_id;       	/* partner AP invocation ID     */
  ST_BOOLEAN part_AE_qual_pres;      	/* present flag                 */
  ST_INT32 part_AE_qual;            	/* partner AE qualifier         */
  ST_BOOLEAN part_AE_inv_id_pres;       	/* present flag                 */
  ST_INT32 part_AE_invoke_id;       	/* partner AE invocation ID     */
  ST_INT pci_mask;               	/* reserved for provider	*/

  ST_INT part_p_sel_len;			/* P-selector length		*/
  ST_UCHAR part_p_sel[MAX_P_SEL];		/* P-selector			*/
  ST_INT part_s_sel_len;			/* S-selector length		*/
  ST_UCHAR part_s_sel[MAX_T_SEL];		/* S-selector			*/
  ST_INT part_t_sel_len;			/* T-selector length		*/
  ST_UCHAR part_t_sel[MAX_T_SEL];		/* T-selector			*/
  ST_INT part_net_addr_len;		/* Network address length	*/
  ST_UCHAR part_net_addr[MAX_N_SEL];	/* Network address		*/
  ST_CHAR part_ip_addr[HOST_NAME_LEN + 1];	/* TCP/IP dotted decimal or host name 	*/

#ifdef SECURITY_SUPPORT
  ST_BOOLEAN secure_assoc;		/* True if this is a secure association	*/
  ST_INT algo_type;		/* Type of encryption method		*/

  MMS_OBJ_ID mech_name;		/* mechanism name			*/
  ST_INT auth_value_len;	/* length of ASN.1 authentication value	*/
  ST_CHAR *auth_value;		/* ASN.1 authentication value		*/

  MMS_OBJ_ID part_mech_name;	/* mechanism name			*/
  ST_INT part_auth_value_len;	/* length of ASN.1 authentication value	*/
  ST_CHAR *part_auth_value;	/* ASN.1 authentication value		*/

  ST_INT mech_info_len;	/* Length of Mechanism specific struct	*/
  ST_CHAR *mech_info;		/* Mechanism specific structure		*/
#endif
  };
typedef struct acse_ar_info ACSE_AR_INFO;	/* used by MMS		*/
typedef struct acse_ar_info ASS_INFO;		/* used by SUIC		*/

extern ASS_INFO *s_ass_info;


/************************************************************************/
/* The structure below is used to represent Abstract Syntax Names that	*/
/* may be used by the application.					*/
/************************************************************************/

extern MMS_OBJ_ID *asn_table;

#if !defined (SECURITY_SUPPORT)
#define ASN_USER_BASE	3	/* where user-defined names begin	*/
#else
#define ASN_USER_BASE	4	/* where user-defined names begin	*/
#endif

/************************************************************************/
/* The structure below is used to represent Transfer Syntax Names that	*/
/* may be used by the application.					*/
/************************************************************************/

extern MMS_OBJ_ID *tsn_table;

#define TSN_USER_BASE	1	/* where user-defined names begin	*/

/************************************************************************/
/* The structure below is used to represent Application Context Names 	*/
/* that	may be used by the application.					*/
/************************************************************************/

extern MMS_OBJ_ID *context_table;


#define CTXT_USER_BASE	4	/* where user-defined contexts begin	*/

/************************************************************************/
/* The following are the default masks					*/
/************************************************************************/

/* AP Context names							*/
#define SM_ISO_MMS_AP_CTXT	0x01
#define SM_MAP_MMS_AP_CTXT	0x02
#define SM_PRIVATE_AP_CTXT	0x04
#define SM_ISO_MMS_IS_AP_CTXT	0x08

/* Abstract Syntax names						*/
#define SM_ISO_MMS_ASN		0x01
#define SM_MAP_MMS_ASN		0x02
#define SM_PRIVATE_ASN		0x04
#ifdef SECURITY_SUPPORT
#define SM_SECURE_ISO_MMS_ASN	0x08
#endif

/* Transfer Syntax names						*/
#define SM_ASN1_TSN		0x01

/************************************************************************/
/* The structure below is used to identify components of the presenta-	*/
/* tion context list (both definition and result)			*/
/************************************************************************/

struct pc_def
{
  ST_INT result;	/* result, used on response only		*/
  ST_INT pci;		/* pci, used only on confirms/indications	*/
			/* This field must not be modified by the user	*/
  ST_UINT asn_mask;	/* abstract syntax name mask			*/
  ST_UINT tsn_mask;	/* transfer syntax name mask			*/
} ;

typedef struct pc_def PC_DEF;


/************************************************************************/
/* The structure below is used to represent the user data list before	*/
/* presentation contexts have been negotiated				*/
/************************************************************************/

struct u_data
{
  ST_INT list_index;	/* for request/indication, this refers to an	*/
			/* index into pc_deflist whose entry represents	*/
			/* presentation context on which data is to be	*/
			/* carried.  For response/confirm, this refers	*/
			/* to an index into result list			*/
  ST_UINT tsn_mask;	/* transfer syntax name mask			*/
  ST_INT u_data_len;	/* user data length				*/
  ST_CHAR *user_data;	/* pointer to user data				*/
} ;

typedef struct u_data U_DATA;


/************************************************************************/
/* The structure below is used to represent the user data list after	*/
/* presentation contexts have been negotiated				*/
/************************************************************************/

struct trans_data
{
                        /* This is really asn_mask                      */
  ST_UINT pci_mask;	/* mask representing index into pci_table	*/
  ST_INT len;		/* data length					*/
  ST_CHAR *data;		/* pointer to data				*/
} ;

typedef struct trans_data TRANS_DATA;


/************************************************************************/
/* The structure below is used to save Directory and other information	*/
/* about local and remote AE's read in from SUIC.DIB			*/
/************************************************************************/

#if defined (MARBEN) || defined (DECNP5)

#ifndef DIB_ENTRY_DEFINED
#define DIB_ENTRY_DEFINED

struct dib_entry
{
  DBL_LNK lnk;			/* link pointer				*/

  ST_LONG reserved;		/* reserved field			*/
  ST_LONG reserved2;		/* reserved field			*/
#if defined (MARBEN) && defined (_WIN32)
  ST_INT mngt_tag;		/* management operation tag		*/
  ST_CHAR host_name[MAX_HOST_LEN + 1];
  ST_CHAR *comment;
#endif
#if DIRECTORY_SUPPORT
  ST_CHAR *name;			/* user-defined ASCII character string	*/
#else
  ST_CHAR name[MAX_NAME_LEN + 1];	/* user-defined ASCII character string	*/
#endif
  ST_BOOLEAN local;			/* local or remote name			*/
#ifdef MARBEN
  ST_BOOLEAN sharable;		/* shared flag (local names only)	*/
#endif
  ST_INT transport;		/* transport provider: TP4 vs. TCP	*/
  ST_BOOLEAN AP_title_pres;		/* present flag				*/
  MMS_OBJ_ID AP_title; 		/* AP title    				*/
  ST_BOOLEAN AP_inv_id_pres;		/* present flag				*/
  ST_INT32 AP_invoke_id;		/* AP invocation ID    			*/
  ST_BOOLEAN AE_qual_pres;		/* present flag				*/
  ST_INT32 AE_qual;			/* AE qualifier    			*/
  ST_BOOLEAN AE_inv_id_pres;		/* present flag				*/
  ST_INT32 AE_invoke_id;		/* AE invocation ID    			*/
  ST_INT p_sel_len;		/* P-selector length			*/
  ST_UCHAR p_sel[MAX_P_SEL];	/* P-selector				*/
  ST_INT s_sel_len;		/* S-selector length			*/
  ST_UCHAR s_sel[MAX_T_SEL];	/* S-selector				*/
  ST_INT t_sel_len;		/* T-selector length			*/
  ST_UCHAR t_sel[MAX_T_SEL];	/* T-selector				*/
  ST_INT net_addr_len;		/* Network address length		*/
  ST_UCHAR net_addr[MAX_N_SEL];	/* Network address			*/
  ST_INT port;			/* TCP/IP port number			*/
  ST_CHAR ip_addr[HOST_NAME_LEN + 1];	/* TCP/IP dotted decimal or host name	*/

/*#ifdef PRODUCT_ONLY_VMS_ALPHA  */
  ST_INT ChannelTypeToUse; 
/*#endif                         */
} ;

typedef struct dib_entry DIB_ENTRY;

#if defined (MARBEN)
#if defined (_WIN32)

typedef struct
  {
  ST_CHAR host_name[MAX_HOST_LEN + 1];
  ST_INT net_addr_len;		/* Network address length		*/
  ST_UCHAR net_addr[MAX_N_SEL];	/* Network address			*/
  ST_CHAR ip_addr[HOST_NAME_LEN + 1];	/* TCP/IP dotted decimal or host name	*/
  } HOSTNAME_INFO;

#endif	/* defined (_WIN32) */
#endif  /* defined (MARBEN) */

/************************************************************************/
/* Variables used for reading DIB file					*/

extern ST_BOOLEAN s_manual_dib_load;         /* User to set DIB Table        */
extern ST_INT num_dib_entries;           /* Number of entries            */
extern DIB_ENTRY *dib_table;            /* Pointer to dib table         */

#endif	/* !DIB_ENTRY_DEFINED	*/
#endif  /* defined MARBEN */

#define TYPE_ORIGINAL		0
#define TYPE_UNIVERSAL		1
extern ST_INT s_activate_type;		/* local name activation type	*/


/************************************************************************/
/************************************************************************/
/* Application Reference data structure					*/
/************************************************************************/

#define APP_REF_FORM1	0
#define APP_REF_FORM2	1

struct form1
  {
  ST_UCHAR   *ptr;				/* ptr to something		*/
  ST_INT16  len;				/* length of contents at ptr	*/
  };
typedef struct form1 FORM1_T;

struct app_ref
  {
  ST_INT16 	form;			/* tells if form 1 or form 2	*/
  ST_BOOLEAN    ap_title_pres;		/* tells if AP title is there	*/
  union					/* AP title differs in form	*/
    {
    struct form1	form_1;		/* form 1 of AP title is ASN.1	*/
    struct mms_obj_id	form_2;		/* form 2 of AP title is objectid*/
    } ap_title;
  ST_BOOLEAN	ap_invoke_pres;		/* tells if AP invoke is there	*/
  ST_INT32 	ap_invoke;
  ST_BOOLEAN	ae_qual_pres;		/* tells if AE qualifier is there*/
  union					/* AE qualifier differs in form	*/
    {
    struct 	form1	form_1;		/* form 1 of AE qual is ASN.1	*/
    ST_INT32	form_2;			/* form 2 if AE qual is long word*/
    } ae_qual;
  ST_BOOLEAN	ae_invoke_pres;		/* tells if AE invoke is there	*/
  ST_INT32 	ae_invoke;
  };
typedef struct app_ref APP_REF;

/************************************************************************/
/* The structure below is used to identify PCI				*/
/* Used by s_lookup_pci ().						*/
/************************************************************************/

struct pc
{
  ST_INT asn_index;	/* index into asn_table				*/
  ST_INT tsn_index;	/* index into tsn_table				*/
} ;

typedef struct pc PC;

/************************************************************************/

ST_RET s_start (ST_VOID);
ST_VOID s_end (ST_VOID);

extern ST_VOID (*s_event_notify) (ST_VOID);
extern ST_VOID (*s_usr_nap) (ST_VOID);

extern ST_CHAR *(*s_send_priv_data) (ST_INT *);

ST_BOOLEAN s_serve (ST_VOID);

ST_VOID s_comm_disable (ST_VOID);
ST_VOID s_comm_enable (ST_VOID);

ST_BOOLEAN s_read_mail (ST_VOID);
ST_VOID s_send_mail (ST_VOID);

ST_INT s_lookup_pci (ST_INT chan, ST_INT pci_mask, PC *pc_arr);


/* User defined 'done' functions                                        */

ST_VOID s_activate_done (ST_CHAR *name, ST_RET retcode);
ST_VOID s_deactivate_done (ST_CHAR *name, ST_RET retcode);
ST_VOID s_listen_done (ST_INT chan, ST_RET retcode);
ST_VOID s_canlisten_done (ST_INT chan, ST_RET retcode);
ST_VOID s_assreq_done (ST_INT chan, ST_RET retcode);
ST_VOID s_assrsp_done (ST_INT chan, ST_RET retcode);
ST_VOID s_relreq_done (ST_INT chan, ST_RET retcode);
ST_VOID s_relrsp_done (ST_INT chan, ST_RET retcode);
ST_VOID s_u_abort_done (ST_INT chan, ST_RET retcode);
ST_VOID s_transfer_done (ST_INT chan, ST_RET retcode);
ST_VOID s_receive_done (ST_INT chan, ST_RET retcode);


/* User defined 'indication' functions                                  */

ST_VOID s_associate_ind (ST_INT chan, 
			 ST_INT num_pc_defs, PC_DEF *pc_deflist, 
			 ST_INT num_u_data, U_DATA *u_data_list,
			 ST_BOOLEAN matched);
ST_VOID s_associate_cnf (ST_INT chan,
			 ST_INT num_result, PC_DEF *result_list,
			 ST_INT num_u_data, U_DATA *u_data_list, 
			 ST_RET retcode);
ST_VOID s_release_ind (ST_INT chan, ST_INT reason, 
		       ST_INT num_u_data, TRANS_DATA *u_data_list);
ST_VOID s_release_cnf (ST_INT chan, ST_INT reason,
		       ST_INT num_u_data, TRANS_DATA *u_data_list);
ST_VOID s_u_abort_ind (ST_INT chan, ST_INT reason,
		       ST_INT num_u_data, TRANS_DATA *u_data_list);
ST_VOID s_p_abort_ind (ST_INT chan, ST_INT reason);
ST_VOID s_transfer_ind (ST_INT chan, 
			ST_INT num_trans, TRANS_DATA *data,
			ST_BOOLEAN eom);

/* SUIC command functions						*/

ST_RET s_reg_title (ST_INT chan, ST_CHAR *name);
ST_RET s_activate (ST_CHAR *name);
ST_RET s_deactivate (ST_CHAR *name);
ST_RET s_load_part_info (ST_INT chan, ST_CHAR *name);
ST_RET s_listen (ST_INT chan, ST_UINT ap_context_mask);
ST_RET s_cancel_listen (ST_INT chan);
ST_RET s_associate_req (ST_INT chan, ST_UINT ap_context_mask,
			ST_INT num_pc_defs, PC_DEF *pc_deflist,
			ST_INT num_u_data, U_DATA *u_data_list);
ST_RET s_associate_rsp (ST_INT chan, ST_UINT ap_context_mask,
			ST_INT reason, ST_INT num_result, PC_DEF *result_list,
			ST_INT num_u_data, U_DATA *u_data_list);
ST_RET s_release_req (ST_INT chan, ST_INT reason, 
		      ST_INT num_u_data, TRANS_DATA *u_data_list);
ST_RET s_release_rsp (ST_INT chan, ST_INT reason, 
		      ST_INT num_u_data, TRANS_DATA *u_data_list);
ST_RET s_u_abort (ST_INT chan, ST_INT reason, 
		  ST_INT num_u_data, TRANS_DATA *u_data_list);
ST_RET s_receive (ST_INT chan);
ST_RET s_transfer_req (ST_INT chan,
		       ST_INT num_trans, TRANS_DATA *data, ST_BOOLEAN eom);
ST_VOID s_free_databuf (ST_CHAR *buf);
ST_BOOLEAN s_block (ST_INT seconds);
ST_VOID s_except (ST_INT except_code);

ST_RET s_set_ap_title (ST_CHAR *name, ST_BOOLEAN en, MMS_OBJ_ID *ap_title);
ST_RET s_set_ae_qual (ST_CHAR *name, ST_BOOLEAN en, ST_INT32 ae_qual);
ST_RET s_set_ap_invoke (ST_CHAR *name, ST_BOOLEAN en, ST_INT32 ap_invoke);
ST_RET s_set_ae_invoke (ST_CHAR *name, ST_BOOLEAN en, ST_INT32 ae_invoke);

ST_UINT s_install_ap_ctxt (MMS_OBJ_ID *ap_ctxt);
ST_UINT s_install_asn (MMS_OBJ_ID *asn);
ST_UINT s_install_tsn (MMS_OBJ_ID *tsn);

ST_VOID s_perror (ST_RET errcode);
ST_VOID s_perror_log (FILE *logfile, ST_RET errcode);
ST_VOID s_perror_str (ST_CHAR *errbuf, ST_INT max_len, ST_RET errcode);

ST_BOOLEAN s_chk_idle (ST_VOID);

APP_REF *s_find_proc (ST_CHAR *arname);
ST_INT s_find_arname (APP_REF *app_ref_ptr, ST_BOOLEAN match_ae_qual,
	ST_CHAR *ar_ptr);
ST_RET s_find_paddr (ACSE_AR_INFO *ar_info, ST_CHAR *ar_ptr);

#if defined (DIB_ENTRY_DEFINED)
ST_RET s_add_rem_arname (DIB_ENTRY *dib);
ST_RET s_del_rem_arname (ST_CHAR *name);
ST_RET s_mod_rem_arname (DIB_ENTRY *dib);
ST_RET s_get_rem_arname (ST_CHAR *name, DIB_ENTRY *dib);
#if defined (MARBEN) && defined (_WIN32)
ST_RET s_create_hostname (HOSTNAME_INFO *info);
#endif
#endif

#if DIRECTORY_SUPPORT
#include "mds.h"
#endif

#ifdef __cplusplus
}
#endif
									  
#endif

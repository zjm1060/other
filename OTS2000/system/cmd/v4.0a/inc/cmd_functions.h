#ifdef __cplusplus
extern "C" {
#endif

int CmdProcess(struct net_cmd_seg *,struct plc_cmd_seg *);

extern	short	BdLinkStat(LCUID, short);
extern  int	BdNetSwitchCmd(LCUID, short);
extern	int     ReadEntryById(POINTER * ,DMS_COMMON *);
extern  int 	BdCmdSendResult(struct net_cmd_seg,unsigned short);


#ifdef __cplusplus
}
#endif

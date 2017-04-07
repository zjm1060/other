
#define	MAXLAYERS		16

#define	UNLOCKED		0
#define	LOCKED			1

#define	VISIBLE			0
#define	UNVISIBLE		1

short	hide_all_btn_tag, lock_all_btn_tag;

typedef struct
{
	float	min_visible;
	float	max_visible;
	int	visible_tag;
	int	lock_tag;
	int	hide_tag;
	char	name[MAX_LAYER_NAME_LEN];
}       LAYERDB;

int	layer_cnt;
LAYERDB layerdb[MAXLAYERS];

//XmStringTable	layer_name_list;




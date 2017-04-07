
/*--- The pattern_info types ---*/
#define 	NO_STAR 	1
#define 	RIGHT_STAR 	2
#define 	LEFT_STAR 	3
#define 	LEFT_RIGHT_STAR 4
#define 	MID_STAR 	5



/*--- Number of stars per pattern type ---*/
#ifdef DEF_MODULE
	char	pattern_stars[6] = {0,0,1,1,2,1};
#else
	extern 	char 	pattern_stars[6];
#endif

/*--- Pattern information ---*/ 
typedef struct 
{
   	int	type;
   	char 	*left_str;
   	int	left_len;
   	char 	*right_str;
   	int	right_len;
} 	PATTERN_INFO;




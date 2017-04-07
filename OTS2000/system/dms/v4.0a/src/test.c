#include	<string.h>

#include	<math.h>



#include	"../inc/os_type_def.h"

#include	"../inc/dms_com.h"







int	GetStringsFromStr ( char *bufptr, int strbuf_num, char strbuf[][MAX_CHAR_NUM_IN_LINE])

{

	int		i, j, num, len, init, quote_mark;

	char	buffer[MAX_CHAR_NUM_IN_LINE];



	for(i=0; i<strbuf_num; i++)

		strbuf[i][0]= 0;



	if((bufptr[0]=='#') || (bufptr[0]=='!')) /*#和!表示注释*/

		return 0;



	if(strlen(bufptr) >= sizeof(buffer))

	{

		strncpy(buffer, bufptr, sizeof(buffer)-1);

		buffer[sizeof(buffer)]= 0;

	}

	else

		strcpy(buffer, bufptr);



	len= strlen(buffer);



	for(j=0; j<len; j++)

	{

		if ( ( buffer[j] == '=') | ( buffer[j] == ',') 

			| ( buffer[j] == '\t') | ( buffer[j] == ':') )

			buffer[j] = ' ';



		if ( buffer[j] == 0x0d || buffer[j] == 0x0a ) /*去掉行末的回车符和换行符*/

		{

			buffer[j] = 0;

			break;

		}

	}

	

	if( buffer[0]==0 ) /*空字符串*/

		return 0;



	j= 0;

	num= 0;

	init= 0;

	quote_mark= 0;

	for(i=0; i<len; i++ )

	{

		if((buffer[i]=='\"') || (buffer[i]=='\'')) /*单引号和双引号*/

		{

			if(quote_mark) /*后引号*/

			{

				if((buffer[i+1]!=' ') && (buffer[i+1]!=0)) /*后引号后面必须有空格，除非字符串结束*/

				{

					strbuf[num][j]= 0;

					return -1;

				}

				quote_mark= 0; /*引号域结束*/

			}

			else /*前引号*/

			{

				if((i>0) && buffer[i-1]!=' ') /*前引号前面必须有空格，除非字符串开始*/

				{

					strbuf[num][j]= 0;

					return -1;

				}

				quote_mark= 1; /*引号域开始*/

			}

			continue;

		}



		if(quote_mark) /*引号域内拷贝所有字符*/

		{

			strbuf[num][j++] = buffer[i];

			init= 1;

			continue;

		}



		if(buffer[i] != ' ') /*拷贝非空格字符*/

		{

			strbuf[num][j++] = buffer[i];

			init= 1;

		}

		else if((buffer[i]==' ') && (buffer[i+1]!=' ') && (buffer[i+1]!=0) && init)

		{/*空格后为非空格字符，且字符串未结束，且已经提取过非空格字符*/

			strbuf[num][j]= 0; /*结束当前的字符串，开始新的字符串*/

			j= 0;

			num++;

			if (num>=MAX_STR_NUM_IN_LINE)

				break;

		}

	}

	strbuf[num][j]= 0;	/*结束当前的字符串*/

	if(quote_mark) /*如果引号域没有闭合，则格式错误*/

		return -1;

	return num+1;

}





int main(void)

{

	char	strbuf[MAX_STR_NUM_IN_LINE][MAX_CHAR_NUM_IN_LINE];

	char	*str= "  \"  aaa  \"   ddd  \"bbb \" \'rt\' \" ccc\" eee";

	int		num, i; 



	if(-1==(num=GetStringsFromStr(str, sizeof(strbuf)/sizeof(strbuf[0]), strbuf)))

		printf("\nERROR");

	else

	{

		printf("\nnum= %d", num);

		for(i=0; i<num; i++)

			printf("\n>>>%s<<<", strbuf[i]);

	}
RegisterServiceProcess();
}
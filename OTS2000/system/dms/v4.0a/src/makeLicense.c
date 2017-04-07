#include    "../inc/os_type_def.h"

#include    "../inc/dms_std.h"
#include    "../inc/dms_data_def.h"
#include    "../inc/dms_util_def.h"
#include    "../inc/dms_com.h"
#include    "../inc/dms_functions.h"


#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "../../../lan/v4.0a/lib/lan.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/dpslib.lib")

#pragma	comment(lib, "wsock32.lib")


#define MX_AAAAA ( (((z>>5)^(y<<2))+((y>>3)^(z<<4)))^((sum^y)+(k[(p&3)^e]^z)) )

int Encode(unsigned long* var, unsigned long n, unsigned long* key) //encode
{
    unsigned long z, y, sum=0, e, DELTA=0x9e3779b9;
    unsigned long p, q;
	unsigned long v[16], k[4];
	unsigned long	i;

	if(n>16)
	{
		printf("\nEncode: input string length over: %d", n);
		return 1;
	}

	for(i=0; i<n; i++)
	{
		v[i]= *(var+i);
		Swap4Byte(&v[i]);
	}
	for(i=0; i<4; i++)
	{
		k[i]= *(key+i);
	}

	
	y=v[0];

    if (n > 1) 
	{      
		z=v[n-1];
		q = 6 + 52/n;
		while (q-- > 0) 
		{
			sum += DELTA;
			e = (sum >> 2) & 3;
			for (p=0; p<n-1; p++) 
			{
				y = v[p+1];
				z = v[p] += MX_AAAAA;
			}
			y = v[0];
			z = v[n-1] += MX_AAAAA;
		}

		for(i=0; i<n; i++)
		{
			var[i]= v[i];
			Swap4Byte(&var[i]);
		}
	
		return 0 ;
    } 
    return 1;
}




typedef struct  
{
	char	mac[24];
	char	hostname[24];
	char	stnname[24];
} HOST_INFO;


#define MAX_HOST_LICENSE_NUM	100

int main(int argc, char *argv[])
{
	FILE	*fp;
	int		i, host_num= 0;
	HOST_INFO info[MAX_HOST_LICENSE_NUM];
	char	filename[256];
	char	buf[256];
	
	sprintf(filename, "%s%s%s", SYS_DEFINE_PATH, "makelicense\\","h9000_license.def");
	fp= fopen(filename, "rb");
	if(fp==NULL)
	{
		printf("\nFail to open %s", filename);
		exit(0);
	}
	
	while(fgets(buf, 256, fp))
	{
		char	host[256];
		char	mac[256];
		char	stn[256];
		int		num;

		if(buf[0]=='#' ||buf[0]=='!')
			continue;

		num= sscanf(buf, "%s %s %s", host, stn, mac);
		if(num==0 || num==EOF)
			continue;
		
		if(num!=3)
		{
			printf("\nformat error: %s", buf);
			fclose(fp);
			exit(0);
		}

		if(strlen(host) >12)
		{
			printf("\nhostname too long: %s", buf);
			fclose(fp);
			exit(0);
		}
		
		if(strlen(stn) >12)
		{
			printf("\nstn_name too long: %s", buf);
			fclose(fp);
			exit(0);
		}
		
		if(strlen(mac) != 12)
		{
			printf("\nmac addr format error: %s", buf);
			fclose(fp);
			exit(0);
		}
		
		memset(&info[host_num], 0, sizeof(HOST_INFO));
		strncpy(info[host_num].hostname, host, sizeof(info[host_num].hostname));
		strncpy(info[host_num].stnname, stn, sizeof(info[host_num].stnname));
		strncpy(info[host_num].mac, mac, sizeof(info[host_num].mac));
		
		if(++host_num >= MAX_HOST_LICENSE_NUM)
			break;
	}
	fclose(fp);

	sprintf(filename, "%s%s%s", SYS_DEFINE_PATH,  "makelicense\\","h9000_license.def");
	fp= fopen(filename, "w");
	if(fp==NULL)
	{
		printf("\nFail to open %s", filename);
		exit(0);
	}

	fprintf(fp, "#主机名	厂站名	主机网卡MAC地址		授权校验区\n");
	for(i=0; i<host_num; i++)
	{
		GULONG	keycode[4];
		char	code[24];
		int		j;

		GetNameStrHash(info[i].mac, &keycode[2]);
		GetNameStrHash(info[i].hostname, &keycode[1]);
		GetNameStrHash(info[i].stnname, &keycode[3]);
		sprintf(buf, "%s%s%s", info[i].mac, info[i].hostname, info[i].stnname);
		GetNameStrHash(buf, &keycode[0]);	
		
		memcpy(code, info[i].mac, 12);
		memcpy(&code[12], info[i].hostname, 12);
		Encode((unsigned long*)code, 6, (unsigned long*)keycode);

		fprintf(fp, "%s\t%s\t%s\t", info[i].hostname, info[i].stnname, info[i].mac);
		for(j=0; j<24; j++)
			fprintf(fp, "%02x", (unsigned char)code[j]);
		fprintf(fp, "\n\n");
	}	

	fclose(fp);

	return 0;
}



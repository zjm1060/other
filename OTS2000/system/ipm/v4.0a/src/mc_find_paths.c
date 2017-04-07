
#include	<string.h>

void	FormFullName( env_name, full_file_name, part_file_name)
char	*full_file_name;	
char	*part_file_name;
char	*env_name;
{

	strcpy( full_file_name , env_name);
	strcat( full_file_name, part_file_name);

}

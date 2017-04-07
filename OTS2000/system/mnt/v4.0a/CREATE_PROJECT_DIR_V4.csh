#!/usr/bin/sh


PROJECT_NAME=$1 

if [ $# -ne 1 ]; then
	echo "usage: $0 project_name"
	exit 1
fi

set `/usr/bin/id`
if [ $1 != "uid=100(ems)" ]; then
	echo "$0: must be run as ems"
	exit 1
fi

H9000DIR=$HOME/h9000

if [ -d $HOME/project ]; then
	/usr/bin/mv $HOME/project $HOME/project_old
fi


mkSubDir()
{
	echo "mkdir $1"
	
	if [ ! -d $1 ]; then
		/usr/bin/mkdir /$1
    		if [ -d /$1 ]; then
     			/usr/bin/chmod 755 /$1
    		fi
	fi
}

 		 
mkSubDir $HOME/project
mkSubDir $HOME/project/$PROJECT_NAME

cp -r $H9000DIR/dat $HOME/project/$PROJECT_NAME/
cp -r $H9000DIR/def $HOME/project/$PROJECT_NAME/
cp -r $H9000DIR/dsp $HOME/project/$PROJECT_NAME/
cp -r $H9000DIR/gen $HOME/project/$PROJECT_NAME/


exit 0

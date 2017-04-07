



echo ""
echo ""
echo "*********************** start CREATE_EMS_DIR system ***********************"
echo "*                                                                         *"
echo "*   CREATE_EMS_DIR will create all EMS directories and sub-directories    *"
echo "*                                                                         *"
echo "*	                  by BITC/IWHR 2006.06.06                         *"
echo "*                                                                         *"
echo "***************************************************************************"
echo ""
echo ""


#!/usr/bin/sh

set `/usr/bin/id`
if [ $1 != "uid=100(ems)" ]; then
	echo "$0: must be run as ems"
	exit 1
fi
 
H9000DIR=$HOME/h9000

if [ ! -d $H9000DIR ]; then
	/usr/bin/mkdir $H9000DIR
    	if [ -d $H9000DIR ]; then
     		/usr/bin/chmod 755 $H9000DIR
    	fi
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

 
mkSubDir $H9000DIR/agc	
	 
mkSubDir $H9000DIR/bin

mkSubDir $H9000DIR/dat

mkSubDir $H9000DIR/def

mkSubDir $H9000DIR/dsp

mkSubDir $H9000DIR/dsp/pic

mkSubDir $H9000DIR/gen

mkSubDir $H9000DIR/tmp

mkSubDir $H9000DIR/iec101

mkSubDir $H9000DIR/iec104

mkSubDir $H9000DIR/include

mkSubDir $H9000DIR/lib

mkSubDir $H9000DIR/mnt

mkSubDir $H9000DIR/wav

mkSubDir $H9000DIR/ots

mkSubDir $H9000DIR/his
mkSubDir $H9000DIR/his/agc
mkSubDir $H9000DIR/his/curve
mkSubDir $H9000DIR/his/del
mkSubDir $H9000DIR/his/event
mkSubDir $H9000DIR/his/eventtxt
mkSubDir $H9000DIR/his/fin
mkSubDir $H9000DIR/his/rpt
mkSubDir $H9000DIR/his/sftcp
mkSubDir $H9000DIR/his/sgzy
mkSubDir $H9000DIR/his/trap
mkSubDir $H9000DIR/his/oix
 
  

echo ""
echo "******************** finish CREATE_EMS_DIR system ************************"
echo "*                                                                        *"
echo "*             BITC/IWHR thank you for using CREATE_EMS_DIR               *"
echo "*                                                                        *"
echo "**************************************************************************"





exit 0

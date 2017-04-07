set `/usr/bin/id`
if [ $1 != "uid=0(root)" ]; then
	echo "$0: must be run as root"
	exit 1
fi
chown root /home/ems/h9000/bin/plcmon.dxe
chmod u+s /home/ems/h9000/bin/plcmon.dxe

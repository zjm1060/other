set `/usr/bin/id`
if [ $1 != "uid=0(root)" ]; then
	echo "$0: must be run as root"
	exit 1
fi
chown root ./sniffer.dxe
chmod u+s ./sniffer.dxe

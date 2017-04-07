chown root ./iccp_main.dxe
chmod u+s ./iccp_main.dxe

chown root ./iccp_s_main.dxe
chmod u+s ./iccp_s_main.dxe

cp -p ./iccp_main.dxe /h9000bin
cp -p ./iccp_s_main.dxe /h9000bin

chown root /h9000bin/iccp_main.dxe
chmod u+s /h9000bin/iccp_main.dxe

chown root /h9000bin/iccp_s_main.dxe
chmod u+s /h9000bin/iccp_s_main.dxe


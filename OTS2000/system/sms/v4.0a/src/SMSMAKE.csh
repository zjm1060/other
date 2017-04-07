cd /SMSSRC

rm *.o 
rm *.?xe

make -f priv_mgr.make

cp -p privilege_mgr.?xe /h9000bin/




rm *.o *.?xe

make -f ReadGps.make

chmod u+s ReadGps.?xe

cp -p ReadGps.?xe /home/ems/h9000/bin
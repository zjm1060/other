cd /OIXSRC
rm *.o
rm *.?xe
rm *.out
#make -f find.make
make -j32 -f oix.make
rm *.o
cp -p oix.?xe /h9000bin
#cp -p findstr.?xe /h9000bin

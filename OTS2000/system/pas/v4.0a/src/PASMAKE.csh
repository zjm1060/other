
#!/usr/bin/csh
#PASMAKE.csh:* make file to make all PAS c programs *

cd /PASSRC

rm *.o
rm *.?xe

make -f calcugenname.make
make -f calcutest.make
make -f interlockname.make
make -f interlocktest.make
make -f sgzy.make
make -f sgzygen.make
make -f trend_data_rec.make
make -f trendgen.make
make -f pdc.make
make -f RptGen.make
make -f relatepoint.make

cp -p calcugenname.?xe /h9000bin/
cp -p calcutest.?xe /h9000bin/
cp -p interlockname.?xe /h9000bin/
cp -p interlocktest.?xe /h9000bin/
cp -p sgzy.?xe /h9000bin/
cp -p sgzygen.?xe /h9000bin/
cp -p trend_data_rec.?xe /h9000bin/
cp -p trendgen.?xe /h9000bin/
cp -p pdc.?xe /h9000bin/
cp -p RptGen.?xe /h9000bin/
cp -p relatepoint.?xe /h9000bin/
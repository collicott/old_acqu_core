//g++ -c -o merge.o -I ../include/cc -I $ROOTSYS/include src/merge.cc
//g++ merge.o ../obj/TAcquFile.o ../obj/TAcquSystem.o ../obj/TAcquMemory.o ../obj/SysUtil.o -o merge


#include "TAcquFile.h"

const char* list[] = {"Element:", NULL};

main( int argc, char* argv[] )
{
  int i=0;
  char* line;
  double n, ee, de, eph, ee1[400], ee2[400];
  TAcquFile tagg("../data/etag_sum93.cal");
  TAcquFile fp("../data/FPladder.dat");
  while( (line = tagg.ReadLine()) ){
    if(sscanf( line, "%lf%lf%lf%lf%lf", &n,&ee,&de,&eph ) != 4 ){
      exit(0);
    }
    ee1[i] = ee - 0.5*de;
    ee2[i] = ee + 0.5*de;
    //    printf("%g %g\n", ee1,ee2);
    i++;
  }
  int j[400], l[400], h[400], off[400], key;
  double cal[400];
  i=351;
  for(;i>=0;){
    key = fp.ReadKey(list,0);
    if( key != 0 ) continue;
    line = fp.GetLine();
    if(sscanf( line, "%d%d%d%d%lf",
	       &j[i],&l[i],&h[i],&off[i],&cal[i] ) != 5 ) exit(0);
    //    printf(" Element: %d  %d  %d  %d  %g %g\n", j,l,h,off,cal,ee1[i]);
    i--;
  }
  for(i=0;i<352;i++)
    printf(" Element: %d  %d  %d  %d  %g %g\n",
	   j[i],l[i],h[i],off[i],cal[i],ee1[i]);
  printf("%g\n",ee2[351]);
}

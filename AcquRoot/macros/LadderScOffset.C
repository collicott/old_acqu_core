void ScalerOffset(Char_t* taggfile, Int_t offset )
{
// Apply offset to scaler indices found in a TA2Detector configuration file
//
  Char_t taggmod[128];                         // new file to create
  Char_t line[256];                            // input line from file
  Char_t desc[32];                             // general purpose char string
  Char_t* p[16];                               // parameters read from line
  for(Int_t i=0; i<16; i++) p[i] = new Char_t[16];

  strcpy(taggmod, taggfile);                   // copy file of xxx = xxx.align
  strcat(taggmod,".ScOffset");
  FILE* tfile;                                 // original file pointer
  FILE* mfile;                                 // copy file pointer

  // Open original and copy files
  printf("Offsetting scalers found in  %s\n",taggfile);
  if( (tfile = fopen(taggfile,"r")) == NULL ){ 
    printf("File %s open failed\n",taggfile);
    return;
  }
  if( (mfile = fopen(taggmod,"w")) == NULL ){
    printf("File %s open failed\n",taggmod);
    return;
  }
  // Print message at top of auto-generated file
  fprintf(mfile,"##     Offset-Scalers AcquRoot Detector file\n");
  fprintf(mfile,"##     Macro ScalerOffset, J.R.M. Annand, 28th March 2007\n");
  // Read lines from original until EOF
  while( fgets( line, 256, tfile ) ){
    sscanf( line, "%s", desc );            // get line descriptor
    if( !strcmp( desc, "Element:") ){      // and check if its an element line
      Int_t n = sscanf( line,"%*s%s%s%s%s%s%s%s%s",
			p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);
      n += sscanf( line,"%*s%*s%*s%*s%*s%*s%*s%*s%*s%s%s%s%s%s%s%s%s",
		   p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15] );
      if( n != 16){
	printf("Bad file format\n%s\n",line);
	return;
      }
      Int_t scal;
      sscanf(p[15],"%d",&scal);
      scal += offset;
      sprintf( p[15],"%d",scal );
      fprintf(mfile,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n", "Element:",
	      p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],
	      p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15] );
    }
    else fputs( line, mfile ); // if not an element line copy it to new file
  }
  // delete any allocated memory and close input and output files 
  for(Int_t i=0; i<16; i++) delete p[i];  
  fclose(tfile);
  fclose(mfile);
  
  return;
}
//                                           

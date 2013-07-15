void DumpCut(Char_t* name, Char_t* file = NULL)
{
  // Print contents of a polygon cut made with the graphical editor
  //
  Char_t fname[256];
  TFile* f;
  strcpy( fname, "data/" );
  if( !file ){
    strcat( fname, name );
    strcat( fname, ".root" );
    f = new TFile( fname, "UPDATE" );
  }
  else{
    strcat( fname, file );
    f = new TFile( fname, "UPDATE" );
  }
  if( !f->IsOpen() ){
    printf("File %s not found\n",fname);
    return;
  }
  TCutG* cut = (TCutG*)f->Get(name);
  if( !cut ){
    printf(" Graphical cut %s not found in file %s\n", name, fname );
    return;
  }
  Double_t* x = cut->GetX();
  Double_t* y = cut->GetY();
  Int_t n = cut->GetN();
  Int_t i,m;
  Double_t x1, y1;
  for( i=0; i<n; i++ )
    printf("%d %f %f\n",i,x[i],y[i]);
  //
  printf("Modify points? ( n=no ) >\n");
  m = scanf("%c",&i);
  if( i == 'n' ){ 
    fflush(stdin);
    return;
  }
  //
  printf("Enter point ( i x y ) >\n");
  m = scanf("%d%lf%lf",&i,&x1,&y1);
  if( m < 3 ) return;
  cut->SetPoint(i,x1,y1);
  for( i=0; i<n; i++ )
    printf("%d %f %f\n",i,x[i],y[i]);
  cut->Write();
  //  TPolyLine* p = new TPolyLine(n,x,y);
  //  p->SetFillColor(38);
  //  p->SetLineColor(2);
  //  p->SetLineWidth(4); 
  //  p->Draw("f");
  //  p->Draw();
  fflush(stdin);
  f->Close();
  delete f;
}

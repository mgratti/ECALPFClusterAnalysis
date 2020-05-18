void root2txt()
{
   TFile *_file0 = TFile::Open("productCleaned_untouched_ringaverage.root");
   TH2F* resultEB = (TH2F*) _file0->Get("resultEB");
   //MG TH2F* resultEE = (TH2F*) _file0->Get("resultEE");
   FILE * f_result;
   FILE * f_result10;


    f_result = fopen("productCleaned.txt","w");
    f_result10 = fopen("product10Cleaned.txt","w");

    cout<<resultEB->GetNbinsX()<<endl;
    cout<<resultEB->GetNbinsY()<<endl;
    //MG cout<<resultEE->GetNbinsX()<<endl;
    //MG cout<<resultEE->GetNbinsY()<<endl;

    for (int iy=1; iy<=resultEB->GetNbinsY(); iy++){
    for (int ix=1; ix<=resultEB->GetNbinsX(); ix++){
    
    Int_t izz=0;
    Int_t iyy =  resultEB->GetYaxis()->GetBinCenter(iy);  
    Int_t ixx =  resultEB->GetXaxis()->GetBinCenter(ix);


    Int_t bin = resultEB->GetBin(ix,iy);
    Float_t EBr = resultEB->GetBinContent(bin);
//    if(EBr<0)EBr=1.;
    //if(EBr<0) cout << EBr<<endl;
        if(iyy !=0){
		fprintf(f_result,"%d  %d %d %f \n", iyy, ixx, izz, EBr);
	//	fprintf(f_result10,"%d  %d %d %f \n", iyy, ixx, izz, EBr + 0.1*EBr);
}
}}


/*
//print EE
    for (int iy=1; iy<=resultEE->GetNbinsX(); iy++){
    for (int ix=1; ix<=resultEE->GetNbinsX(); ix++){
    Int_t izz;

	if(ix<=100){
    izz=-1;
    Int_t bin = resultEE->GetBin(ix,iy);
    Float_t EEr = resultEE->GetBinContent(bin);
//   if(EEr<0)EEr=1.;
    Int_t iyy =  resultEE->GetYaxis()->GetBinCenter(iy);
    Int_t ixx =  resultEE->GetXaxis()->GetBinCenter(ix);

	if(EEr !=0.){  
              fprintf(f_result,"%d  %d %d %f \n", ixx, iyy, izz, EEr);
              fprintf(f_result10,"%d  %d %d %f \n", ixx, iyy, izz, EEr + 0.1*EEr);
	}}
//	else{

}}

    for (int iy=1; iy<=resultEE->GetNbinsX(); iy++){
    for (int ix=1; ix<=resultEE->GetNbinsX(); ix++){
    Int_t izz;

        if(ix>100){

    izz=+1;
    Int_t bin = resultEE->GetBin(ix,iy);
    Float_t EEr = resultEE->GetBinContent(bin);

//    if(EEr<0)EEr=1.;

    Int_t iyy =  resultEE->GetYaxis()->GetBinCenter(iy);
    Int_t ixx =  resultEE->GetXaxis()->GetBinCenter(ix);
    
    ixx=ixx-100;

        if(EEr !=0.){
              fprintf(f_result,"%d  %d %d %f \n", ixx, iyy, izz, EEr);
              fprintf(f_result10,"%d  %d %d %f \n", ixx, iyy, izz, EEr + 0.1*EEr);

	}}

}}
*/

}

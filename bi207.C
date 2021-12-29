//  PROGRAM ANALYSIS bi207 Used to calibrate de beta detector


#include <iostream>         	// ci serve per stampare a video il testo
#include <TGraphErrors.h>     	// ci serve per istanziare grafici
#include <TAxis.h>            	// ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          	// ci serve per disegnare i grafici
#include <TF1.h>          	    // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>        	    // ci serve per manipolare l'output a video 
#include <RooRealVar.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TMath.h>
#include <TF1Convolution.h> 	//for doing convolutions
#include <TH1.h>				//Histograms

//To use Roofit
 
#include "RooRealVar.h"			
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include <RooCBShape.h>			//for the CrystalBall
#include "RooFFTConvPdf.h"
#include "RooPlot.h"

using namespace std; 


// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void bi207(){

	// --------------------------- Open the file with data ------------------------------- // 


	const int nmesure = 16385;
	
	// H i s t o g r a m s
  
  	TH1I *histo1 = new TH1I("histo1","Histogram" , 13000, 0, 13000);


	// -------------------- Read the file with data and put it in a graph --------------------- // 

	fstream ftime("/mnt/c/Users/paolo/OneDrive/Documenti/Università/Labo_III/Data/bi207_1_beta_statistics_20211115.txt",ios::in);//open the file at that location

	
	int bin[nmesure] = {}; //these are the bins to be filled with the data from the file
	int bin_value[nmesure] = {}; //these are the counts in each bin (taken from file)
	
	double t;
	int i=0;
	while(!ftime.eof())//tant que l'on a pas atteint la fin du fichier
  {
	ftime >> t;//je lis l'entrée dans le fichier et je la stoke dans t
	
	
		bin[i] = i+1;
		bin_value[i] = t;
		
		histo1->SetBinContent(i, t);
	    
    if (i<20)
    //if (bin_value[i] > 100)
    cout << "Bin n° " << bin[i] << "\t counts: " << bin_value[i] << "\n";
    
    ++i;    
  }
	ftime.close();// on peut fermer la fichier maintenant
	
	//----------Open the file with the background noise and subtracte it to previous data ------//
	

fstream ftime1("/mnt/c/Users/paolo/OneDrive/Documenti/Università/Labo_III/Data/sr90bg_1_20211108.txt",ios::in);//open the file at that location

TH1I *histo2 = new TH1I("histo2","Histogram2" , 13000, 0, 13000);

	double s;
	i=0;
	
	while(!ftime1.eof())//tant que l'on a pas atteint la fin du fichier
  {
	ftime1 >> s;//je lis l'entrée dans le fichier et je la stoke dans t
	
	
		histo2->SetBinContent(i, bin_value[i] -s );
		bin_value[i] = bin_value[i] - s;
		
				   
    if (i<20)
    //if (bin_value[i] > 100)
    cout << "Bin n° " << bin[i] << "\t counts: " << bin_value[i] << "\n";
    
    ++i;    
  }
	ftime1.close();// on peut fermer la fichier maintenant
	
	TCanvas *sr90 = new TCanvas("bi207","bi207 spectrum",200,10,600,400); //create the canvas
	sr90->SetFillColor(0);
	sr90->cd();
	
	histo1->SetLineColor(3);
	histo1->Draw("Same");
	
	histo1->SetLineStyle(10);
	histo2->Draw("Same");
	
	
	TGraph *gsr90 = new TGraph(nmesure,bin,bin_value);
	
	gsr90->SetMarkerSize(0.3);
	gsr90->SetMarkerStyle(1);
	
	gsr90->SetTitle("bi207 spectrum");
	// Titoli degli assi
	gsr90->GetXaxis()->SetTitle("bins");
	gsr90->GetYaxis()->SetTitle("counts");
	
	gsr90->GetXaxis()->SetLimits(3500,8000);
	
	gsr90->SetMinimum(0);
	gsr90->SetMaximum(1700);
	//gsr90->GetYaxis()->SetLimits(50,400);
		
	// Do istruzioni al grafico di disegnarsi sul canvas che ho selezionato preventivamente con cd()
	// Esistono diverse opzioni di disegno, vedi anche https://root.cern.ch/doc/master/classTGraphPainter.html
	// "AB" drawing axis (A) as a bar chart (B)
	
	gsr90->SetFillColor(38);
	gsr90->Draw("AB1");
	
  //====================================================
  // Trying to describe the data with the theory we know
  //====================================================
  
  cout << "\n\n --- Plotting the peaks to calibrate the detector ---" <<endl;
      
  //+++++++++++
  //First peak
  //+++++++++++
      
      
  /*cout << "\n\n --- FIT First PEAK : Landau  ---" <<endl;
  
  
  TF1 *func1 = new TF1("func1","[0]*exp(x*[1])" , 178.,260);
  
  TF1 *func3 = new TF1("func3","landau", 178.,260);
  
  func3->SetLineColor(4); 
  
  /*func3->SetParNames("Exp Norm", "Exp const", "Norm", "Mean", "Sigma", "Alpha", "N");
  
  func3->SetParLimits(0,1,10); 		//Normalizing Const
  func3->SetParLimits(1,-10,0); 		//Exp const
  
  func3->SetParLimits(2,20000,23500); 	//Norm
  func3->SetParLimits(3,180,210); 		//Mean
  func3->SetParLimits(4,3,50);			//Sigma
  func3->SetParLimits(5,2,4);		//Alpha
  func3->SetParLimits(6,0,4);			//N*/
  
  /*
  func3->SetParLimits(0,119472,119480); 		//Normalizing Const
  func3->SetParLimits(1,200,210); 		//MPV
  func3->SetParLimits(2,15,25); 	//Sigma
  
    
  gsr90->Fit(func3,"RM+");  */
  
  //+++++++++++
  //Second peak
  //+++++++++++
    /*
  cout << "\n\n --- FIT Second PEAK : exp + crystalball  ---" <<endl;
  
  
  TF1 *func1 = new TF1("func1","[0]*exp(x*[1])" , 1600.,2800);
  
  TF1 *func3 = new TF1("func3","func1 + crystalball(2)", 1600.,2800);
  
  func3->SetLineColor(4); 
  
  func3->SetParNames("Exp Norm", "Exp const", "Norm", "Mean", "Sigma", "Alpha", "N");
  
  /*func3->SetParLimits(0,600,700); 		//Normalizing Const
  func3->SetParLimits(1,-1,0); 		//Exp const
  
  func3->SetParLimits(2,1600,2800); 	//Norm
  func3->SetParLimits(3,1950,1955); 		//Mean
  func3->SetParLimits(4,620,640);			//Sigma
  func3->SetParLimits(5,0,4);		//Alpha
  func3->SetParLimits(6,0,4);			//N*/
  
  /*
  func3->SetParLimits(0,600,700); 		//Normalizing Const
  func3->SetParLimits(1,-1,0); 		//Exp const
  
  func3->SetParLimits(2,1690,1700); 	//Norm
  func3->SetParLimits(3,1951,1953); 		//Mean
  func3->SetParLimits(4,620,630);			//Sigma
  func3->SetParLimits(5,0,0.16);		//Alpha
  func3->SetParLimits(6,0,4);			//N
  
    
  gsr90->Fit(func3,"RM+");  */
  
  
  //+++++++++++
  //Third peak
  //+++++++++++
    
  cout << "\n\n --- FIT Third PEAK : exp + crystalball  ---" <<endl;
  
  
  TF1 *func1 = new TF1("func1","[0]*exp(x*[1])" , 4500.,5500);
  
  TF1 *func3 = new TF1("func3","func1 + crystalball(2)", 4500.,5500);
  
  func3->SetLineColor(4); 
  
  func3->SetParNames("Exp Norm", "Exp const", "Norm", "Mean", "Sigma", "Alpha", "N");
  
  func3->SetParLimits(0,600,700); 		//Normalizing Const
  func3->SetParLimits(1,-1,0); 		//Exp const
  
  func3->SetParLimits(2,100,1500); 	//Norm
  func3->SetParLimits(3,5000,5400); 		//Mean
  func3->SetParLimits(4,60,640);			//Sigma
  func3->SetParLimits(5,0,4);		//Alpha
  func3->SetParLimits(6,0,4);			//N 
  
    
  gsr90->Fit(func3,"RM+");  
 }


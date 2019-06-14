/**
 Aritra Bal
 May-July 2019
 Simulation of GEM using Field Maps created from Elmer
 Garfield++ Code for avalanche visualisation
 Mesh was created in gmsh after implementing changes suggested by author.
 Garfield can only read second order tetrahedral meshes. Hence, all higher order elements were removed manually using gmsh. 3D Mesh optimization WITHOUT Netgen Algorithm was carried out. Finally, we need to convert the gmsh mesh to ElmerSolver format by command <ElmerGrid 14 2 mesh.msh -autoclean> in terminal. 
*/
#include <iostream>
#include <cmath>
#include <fstream>
#include <TCanvas.h>
#include <TApplication.h>
#include <TFile.h>
#include <string>
#include <ctime>
#include </home/anand/garfield/Include/MediumMagboltz.hh>
#include </home/anand/garfield/Include/ComponentElmer.hh>
#include </home/anand/garfield/Include/Sensor.hh>
#include </home/anand/garfield/Include/ViewField.hh>
#include </home/anand/garfield/Include/Plotting.hh>
#include </home/anand/garfield/Include/ViewFEMesh.hh>
#include </home/anand/garfield/Include/ViewSignal.hh>
#include </home/anand/garfield/Include/GarfieldConstants.hh>
#include </home/anand/garfield/Include/Random.hh>
#include </home/anand/garfield/Include/AvalancheMicroscopic.hh>
#include </home/anand/garfield/Include/AvalancheMC.hh>
#include </home/anand/garfield/Include/ViewCell.hh>

using namespace Garfield;
std::ofstream data;
std::ofstream data2;
std::ofstream data3;
std::string filename = "log.csv";
std::ofstream detail;
std::string filename1 = "details.txt";
std::string filename2 = "log2.csv";
std::string filename3 = "gain.csv";
/* Delta_V = 500V
   E-induction = 3.5kV/cm
   E-drift = 2 kV/cm  
*/

int main(int argc, char* argv[]) {

  TApplication app("app", &argc, argv);
  
  // Set relevant GEM parameters.
  // GEM thickness in um
  const double gem_th = 50.0e-4;      
  // Copper thickness
  const double gem_cpth = 5.0e-4;  
  // GEM pitch in cm
  //const double gem_pitch = 140.0e-4;   
  // X-width of drift simulation will cover between +/- axis_x
  const double axis_x = 6*(242.488e-4);  
  // Y-width of drift simulation will cover between +/- axis_y
  const double axis_y = 6*(140.0e-4);  
  //const double axis_z = -2;
  const double rPenning = 0.51;                  
  const double lambdaPenning = 0.;
  
  
  // Define the medium.
  MediumMagboltz* gas = new MediumMagboltz();
  // Set the temperature (K)
  gas->SetTemperature(293.15);  
  // Set the pressure (Torr)
  gas->SetPressure(740.);       
  // Allow for drifting in this medium
  gas->EnableDrift();           
  // Specify the gas mixture (Ar/CO2 70:30)
  gas->SetComposition("ar", 70., "co2", 30.); 
  // Enable Penning Transfer for Argon mixture
  gas -> EnablePenningTransfer(rPenning, lambdaPenning, "ar"); 
  // Loading Ion Mobility for Argon
  gas->LoadIonMobility("/home/anand/garfield/Data/IonMobility_Ar+_Ar.txt");

  // Import an Elmer-created field map.
  ComponentElmer* elm = new ComponentElmer(
      "mesh.header", "mesh.elements", "mesh.nodes",
      "dielectrics.dat", "gemcell1.result", "mum");   //The solutions are in um. But Garfield uses cm as default unit.
  elm->EnablePeriodicityX(true);
  elm->EnablePeriodicityY(true);
  elm->SetMedium(5, gas);// In the simulation of the single GEM, the body index of air was 6. It must be brought down to 5 as Garfield re-numbers body indices starting from 0. Garfield_Index = Elmer_Index - 1 
  elm->PrintMaterials();     
  elm->PrintRange();  
  double v1=0.0,v2=0.0;
  elm->GetVoltageRange(v1,v2);
  std::cout << "Min Voltage= " << v1 << "\n Max Voltage= "<< v2 << "\n";
  // The next line can be used to import weighting fields for simulating charge accumulation. Not doing it in this simulation
  // elm->SetWeightingField("gemcell/gemcell_WTlel.result", "wtlel"); 
  elm->DriftMedium(5);
  elm->NotDriftMedium(0); 
  elm->NotDriftMedium(1);
  elm->NotDriftMedium(2);
  elm->NotDriftMedium(3);
  elm->NotDriftMedium(4); 
  // Set up a sensor object.
  Sensor* sensor = new Sensor();
  sensor->AddComponent(elm);
  sensor->SetArea(-axis_x, -axis_y, -0.2005, axis_x,axis_y,0.3005);
  // sensor->AddElectrode(elm, "wtlel");
  // Set the signal binning.
  //const double tEnd = 500.0;
  //const int nsBins = 500;
  // sensor->SetTimeWindow(0., tEnd / nsBins, nsBins);

  // Create an avalanche object
  
  //aval->EnableAvalancheSizeLimit(140);
  // aval->EnableSignalCalculation();
  TCanvas* drift = new TCanvas("drift","drift");
  // Set up the object for drift line visualization.
  
  
   //Set the electron start parameters.
  // Starting z position for electron drift
  const double zi = 0.5 * gem_th + gem_cpth + 120.0e-4;  
  //double ri = (25.0e-4) * RndmUniform();
  //double thetai = RndmUniform()*6.2831853;
  double xi =  121.244e-4; //+ri*cos(thetai);
  double yi =  70.0e-4;//+ri*sin(thetai);
  // Calculate the avalanche.
  
  clock_t start,stop;
  double time;
  start=clock();
  
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval->SetSensor(sensor);
  aval->SetCollisionSteps(100);
  ViewDrift* viewDrift = new ViewDrift();
  viewDrift->SetCanvas(drift);
  viewDrift->SetArea(-axis_x,-axis_y, -0.12, axis_x,axis_y,zi+0.02);
  
  aval->EnablePlotting(viewDrift);
  aval->EnableExcitationMarkers();
  aval->EnableIonisationMarkers();
  aval->EnableAvalancheSizeLimit(1000); std::cout << "Size Limit Enabled= " << "1000\n";
  aval->AvalancheElectron(xi, yi, zi, 0., 0., 0., 0., 0.);
  int nE=aval->GetNumberOfElectronEndpoints(); // This gives us the total number of electron endpoints, INCLUDING captured and absorbed ones. Essentially, its a measure of the total number of electrons generated in the avalanche, though not necessarily of the gain.
  detail.open(filename1);
  detail << "Avalanche completed with "<< nE <<" electrons generated.\n"; 
  std::cout << "Avalanche completed with "<< nE <<" electrons generated.\n"; 
  stop=clock();
  time = (stop - start)/CLOCKS_PER_SEC;
  detail << "Execution Time = " << time/60.0 << " minutes.\n";
  std::cout << "Execution Time = " << time/60.0 << " minutes.\n"; 
  
 
  

  int i;                  // To store index of electron. We run this in a loop from 1 to nE. (see Line 125)
  double x0,y0,z0,t0,e0;  // To store initial (x,y,z) coordinates, time of creation and initial energy. 
  double x1,y1,z1,t1,e1;   // Same as above, only the end points and final energy.
  int status;
  
  int num=0,num1=0,num2=0; // To trace how many electrons are produced in drift and induction gaps, and hole region.
  int n_absorbed=0,n_captured=0,n_left=0,n_live=0;
 
/*Returns the status code of the electron. It will tell us whether the electron is live (contributing to gain), absorbed, captured 
or otherwise. A list of status codes with their description can be found at 
http://garfieldpp.web.cern.ch/garfieldpp/documentation/doxygen/html/GarfieldConstants_8hh.html 
*/

  data.open(filename);
  data2.open(filename2);
  data << "Ending X coordinate"<<","<<"Ending Y coordinate"<<","<<"Ending Z coordinate"<<","<<"Ending status code"<<","<<"Starting X coordinate"<<","<<"Starting Y coordinate"<<","<<"Starting Z coordinate"<<std::endl;
  //TH1F* eS = new TH1F("es", "es", 
  data2 << "Ending X coordinate"<<","<<"Ending Y coordinate"<<","<<"Starting X coordinate"<<","<<"Starting Y coordinate"<<","<<"Starting Z coordinate"<<std::endl;
  
    for (i=nE-1;i>=0;i--) {
    
    aval->GetElectronEndpoint(i,x0,y0,z0,t0,e0,x1,y1,z1,t1,e1,status);
    if (x0==xi && y0==yi && z0==zi)
	continue;    // We ignore the electron that started the avalanche. 
  
    if (status==-5 && z1<=55.0e-4 && z1>=-5.0e-4)  // Status Code -5 -> Electron left drift medium (absorbed in Cu/Kapton)
        n_absorbed++;
    else if (status==-5 && z1<=-1999.0e-4) // Electrons reaching readout electrode count as gain contributors
        { n_live++; 
          data2 << x1*1.0e4 <<"," << y1*1.0e4 << "," << x0*1.0e4 << "," << y0*1.0e4 << "," << z0*1.0e4 << std::endl; 
        }  //writing the data only for the electrons that reach the readout and hence contribute to gain.
    else if (status==-7) n_captured++;  // Status Code -7 -> Electron attached (captured by molecule in drift medium)
       
    else if (status==-1) n_left++;      // Status Code -1 -> Electron left drift area. This is the error term.
   data << x1*1.0e4 <<"," << y1*1.0e4 << "," << z1*1.0e4 << "," << status << "," << x0*1.0e4 << "," << y0*1.0e4 << "," << z0*1.0e4 << std::endl;
  if (z0>=-5.0e-4 && z0<=55.0e-4) num++;
  if (z0<=-5.0e-4) num1++;
  if (z0>=55.0e-4) num2++;
   
} 
  
  data.close(); data2.close();
  std::cout << "No. of electrons absorbed = "<< n_absorbed << ".\n";
  std::cout << "Real Gain (no. of live electrons in medium) = "<< n_live << ".\n";
  std::cout << "No. of electrons captured by gas molecules/ions = "<< n_captured << ".\n";
  std::cout << "Error term (no. of electrons passing out of user-defined drift area) = "<< n_left << ".\n";

  
  detail << "Avalanche generated "<<nE<<" total electrons, including captured and attached ones.\n";
  detail << "No. of electrons absorbed = "<< n_absorbed << ".\n";
  detail << "Real Gain (no. of electrons live in medium or impinged on readout) = "<< n_live << ".\n";
  detail << "No. of electrons captured by gas molecules/ions = "<< n_captured << ".\n";
  detail << "Error term (no. of electrons passing out of user-defined drift area) = "<< n_left << ".\n";
  
  
  
  std::cout << "No. of electrons produced in region of holes (-5um to 55um) = " <<num << ".\n";
  detail << "No. of electrons produced in region of holes (-5um to 55um) = "<< num << ".\n";
  std::cout << "No. of electrons produced in induction gap (z < -5um) = " <<num1 << ".\n";
  detail << "No. of electrons produced in induction gap (z < -5um) = "<< num1 << ".\n";
  std::cout << "Number produced in drift gap (z > 55um) = " <<num2 << ".\n";
  detail << "No. of electrons produced in drift gap (z > 55um) = "<< num2 << ".\n";
  double transparency =0.0; 
  transparency = (1.0*n_live)/(1.0*nE);
  std::cout << "Transparency = " << transparency << "\n";
  detail << "Transparency = " << transparency <<"\n";
  detail.close();
  data3.open(filename3);
  data3 << transparency << "," << n_live << "," << nE << std::endl;
  data3.close(); 
  viewDrift->Plot(false,true);  // The first parameter is set to true by default, and makes a 2D plot. If we set it to false, we get a 3D plot.
 /* // Extract the calculated signal.
 // double bscale = tEnd / nsBins;  // time per bin
//  double sum = 0.;  // to keep a running sum of the integrated signal

  // Create ROOT histograms of the signal and a file in which to store them.
  //TFile* f = new TFile("avalanche_signals.root", "RECREATE");
  //TH1F* hS = new TH1F("hh", "hh", nsBins, 0, tEnd);        // total signal
  //TH1F* hInt = new TH1F("hInt", "hInt", nsBins, 0, tEnd);  // integrated signal

  // Fill the histograms with the signals.
  //  Note that the signals will be in C/(ns*binWidth), and we will divide by e
  // to give a signal in e/(ns*binWidth).
  //  The total signal is then the integral over all bins multiplied by the bin
  // width in ns.
 for (int i = 0; i < nsBins; i++) {
    double wt = sensor->GetSignal("wtlel", i) / ElementaryCharge;
    sum += wt;
    hS->Fill(i * bscale, wt);
    hInt->Fill(i * bscale, sum);
  }

  // Write the histograms to the TFile.
  hS->Write();
  hInt->Write();
  f->Close();

  // Plot the signal.
  const bool plotSignal = false;
  if (plotSignal) {
    TCanvas* cSignal = new TCanvas("signal", "Signal");
    ViewSignal* vSignal = new ViewSignal();
    vSignal->SetSensor(sensor);
    vSignal->SetCanvas(cSignal);
    vSignal->PlotSignal("wtlel");
  }

  // Plot the geometry, field and drift lines.
  TCanvas* cGeom = new TCanvas("geom", "/home/anand/Aritra/ELMER/resultsG++");
  cGeom->SetLeftMargin(0.14);
    ViewField* vf = new ViewField();
    vf->SetSensor(sensor);
    vf->SetCanvas(cGeom);
    vf->SetPlane(0,-1, 0, 0, 0, 0);
    vf->SetArea(-242.488e-4,-0.005,242.488e-4,0.0065);
    //vf->SetVoltageRange(-1000,-500);
    vf->SetNumberOfContours(40);
    vf->SetNumberOfSamples2d(100, 100);
    
  

  // Set up the object for FE mesh visualization.
  ViewFEMesh* vFE = new ViewFEMesh();
  vFE->SetArea(-242.488e-4,-0.08,-0.0005,242.488e-4,0.04,0.0065);//vFE->SetArea(-axis_x,-axis_y,-1*zi,axis_x,axis_y,zi);
  vFE->SetCanvas(cGeom);
  vFE->SetComponent(elm);
  vFE->EnableDebugging(true);
  vFE->SetPlane(0, -1, 0, 0, 0, 0);
  vFE->SetFillMesh(true);
  vFE->SetColor(0, 13);  // 1: Index of Kapton. Setting Color to gray.
  vFE->SetColor(1, 3);
  vFE->SetColor(2, 3);  // 1,2,3,4 : Indices of Copper. Setting color to green
  vFE->SetColor(4, 3);
  vFE->SetColor(3, 3);
  vFE->EnableAxes();
  vFE->SetXaxisTitle("x (cm)");
  vFE->SetYaxisTitle("z (cm)");
  vFE->SetViewDrift(viewDrift);
  // vf->PlotContour("v");
   vFE->Plot();*/
  

  exit(10);

  app.Run(kTRUE);

  return 0;
  
} 
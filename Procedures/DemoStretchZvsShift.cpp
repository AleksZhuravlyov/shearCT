#include <string>
#include <iostream>
#include <vector>
#include <fstream>

#include <DemoStretchZvsShift.h>
#include <stretchZvsShift.h>
#include <StringAndNumber.h>


void demoStretchZvsShift() {

  double shiftZ = 0.0522;
  double initZ = 0.0032;

  auto stretchZ = stretchZVsShift(1.e-8,
                                  shiftZ,
                                  0.5,
                                  0.5,
                                  initZ,
                                  20,
                                  20,
                                  200, 200,
                                  "/Users/bigelk/data/samples/tomography/shearCT/Processed/5.nc",
                                  "/Users/bigelk/data/samples/tomography/shearCT/Processed/10.nc");

  std::cout << "stretchZ " << stretchZ << std::endl;


  int nDiv = 8;
  std::vector<std::vector<double>> stretchesZ(nDiv);
  std::vector<std::vector<double>> centersZ(nDiv);
  std::vector<double> shiftsZ(nDiv);
  double shiftZCurr;
  double initZCurr;
  double stretchZCurr;

  for (int i = 0; i < nDiv; i++) {
    shiftZCurr = shiftZ / (i + 1);
    shiftsZ[i] = shiftZCurr;
    for (int j = 0; j <= i; j++) {
      initZCurr = initZ + shiftZCurr * j;


      stretchZCurr = stretchZVsShift(1.e-8,
                                     shiftZCurr,
                                     0.5,
                                     0.5,
                                     initZCurr,
                                     20,
                                     20,
                                     200, 200,
                                     "/Volumes/ElkData/CT/samples/5.nc",
                                     "/Volumes/ElkData/CT/samples/10.nc");


      stretchesZ[i].push_back(stretchZCurr);
      centersZ[i].push_back(initZCurr + shiftZCurr / 2);

    }
  }


  std::ofstream stream;

  std::string fileDataName;
  for (int i = 0; i < nDiv; i++) {
    fileDataName = toString(shiftsZ[i], 6, 4, false) + ".txt";
    stream.open(fileDataName.c_str());
    for (int j = 0; j < stretchesZ[i].size(); j++) {
      stream << std::setw(20) << centersZ[i][j];
      stream << std::setw(20) << stretchesZ[i][j] << std::endl;
    }
    stream.close();
  }

  std::string filePltName = "stretchZVsShift.plt";
  stream.open(filePltName.c_str());

  stream << "set ylabel 'stretch'" << std::endl;
  stream << "set xlabel 'Z, m'" << std::endl;
  stream << "set key title 'shift Z, m'" << std::endl;
  stream << "set key out horiz cent top" << std::endl;
  stream << "set xrange[" << toString(initZ) << ":";
  stream << toString(initZ + shiftZ) << "]" << std::endl;
  stream << "set grid" << std::endl;

  stream << "plot\\" << std::endl;
  for (int i = 0; i < nDiv; i++) {
    fileDataName = toString(shiftsZ[i], 6, 4, false) + ".txt";
    if (i > 0)
      stream << ",\\" << std::endl;
    stream << "'" << fileDataName << "' using 1:2 ";
    stream << "w p pt 7" << " ti " << "'"
           << toString(shiftsZ[i], 6, 4, false) << "'";
  }
  stream << std::endl;
  stream << "pause -1" << std::endl;

  stream.close();

  std::string command = "gnuplot stretchZVsShift.plt";
  std::cout << system(command.c_str());

}
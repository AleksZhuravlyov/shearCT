#include <NcOps/Image.h>

#include <algorithm>


Image::Image(const std::string &fileName) : file(fileName,
                                                 netCDF::NcFile::read),
                                            path(fileName),
                                            valueName("tomo"),
                                            units("units"),
                                            region(Region()) {

  for (auto &&varData : file.getVars()) {
    vars.push_back(Var());
    vars.back().name = varData.first;
    varData.second.getAtt(units).getValues(vars.back().unitName);
    vars.back().dim = varData.second.getDimCount();
  }

  std::reverse(vars.begin(), vars.end() - 1);

  for (auto &&dimData : file.getVar(valueName).getDims()) {
    dims.push_back(Dim());
    dims.back().name = dimData.getName();
    dims.back().size = dimData.getSize();
    dimArrays.push_back(std::vector<float>(dims.back().size));
    file.getVar(dims.back().name).getVar(dimArrays.back().data());
  }


  for (int i = 0; i < dimArrays.size(); i++)
    std::copy(dimArrays[i].begin() + region.start[i],
              dimArrays[i].begin() + region.start[i]
              + region.width[i], region.dimArrays[i].begin());

  file.getVar(valueName).getVar(region.start, region.width,
                                region.value.data());

  xInit = dimArrays[2][0];
  yInit = dimArrays[1][0];
  zInit = dimArrays[0][0];

  nX = dims[2].size;
  nY = dims[1].size;
  nZ = dims[0].size;

  xStep = dimArrays[2][1] - xInit;
  yStep = dimArrays[1][1] - yInit;
  zStep = dimArrays[0][1] - zInit;

}


std::ostream &operator<<(std::ostream &stream, const Image &ncCt) {

  stream << "path: " << ncCt.path << std::endl;

  stream << "vars:" << std::endl;
  stream << "name unitName dim:" << std::endl;
  for (auto &&var : ncCt.vars) {
    stream << var.name << " ";
    stream << var.unitName << " ";
    stream << var.dim << std::endl;
  }

  stream << "dims:" << std::endl;
  stream << "name size step:" << std::endl;
  for (int i = 0; i < ncCt.dims.size(); i++) {
    stream << ncCt.dims[i].name << " ";
    stream << ncCt.dims[i].size << " ";
    stream << ncCt.dimArrays[i][1] - ncCt.dimArrays[i][0] << std::endl;
  }

  return stream;

}


double Image::getXInit() {
  return xInit;
}

double Image::getYInit() {
  return yInit;
}

double Image::getZInit() {
  return zInit;
}


int Image::getNX() {
  return nX;
}

int Image::getNY() {
  return nY;
}

int Image::getNZ() {
  return nZ;
}


double Image::getXStep() {
  return xStep;
}

double Image::getYStep() {
  return yStep;
}

double Image::getZStep() {
  return zStep;
}


std::shared_ptr<std::vector<Dim>> Image::getDims() {
  return std::make_shared<std::vector<Dim>>(dims);
}

std::shared_ptr<std::vector<Var>> Image::getVars() {
  return std::make_shared<std::vector<Var>>(vars);
}

std::shared_ptr<std::vector<short>> Image::getValue() {
  return std::make_shared<std::vector<short>>(region.value);
}


void Image::setRegion(const std::vector<size_t> &start,
                      const std::vector<size_t> &width) {

  region.initiate(start, width);

  for (int i = 0; i < dimArrays.size(); i++)
    std::copy(dimArrays[i].begin() + region.start[i],
              dimArrays[i].begin() + region.start[i]
              + region.width[i], region.dimArrays[i].begin());

  region.computeInitsAndSteps();

  file.getVar(valueName).getVar(region.start, region.width,
                                region.value.data());

}


void Image::setRegion(const Bbox_3 &bbox) {

  auto xStart = size_t((bbox.xmin() - xInit) / xStep) - 3;
  auto yStart = size_t((bbox.ymin() - yInit) / yStep) - 3;
  auto zStart = size_t((bbox.zmin() - zInit) / zStep) - 3;

  auto xWidth = size_t((bbox.xmax() - bbox.xmin()) / xStep) + 9;
  auto yWidth = size_t((bbox.ymax() - bbox.ymin()) / yStep) + 9;
  auto zWidth = size_t((bbox.zmax() - bbox.zmin()) / zStep) + 9;


  std::string errorMessage;
  if ((xStart + xWidth) > nX)
    errorMessage += " x ";
  if ((yStart + yWidth) > nY)
    errorMessage += " y ";
  if ((zStart + zWidth) > nZ)
    errorMessage += " z ";

  if (errorMessage != "") {

    std::cerr << "Points are out range for " <<
              errorMessage << std::endl;

    std::exit(EXIT_FAILURE);

  }


  std::vector<size_t> start = {zStart, yStart, xStart};
  std::vector<size_t> width = {zWidth, yWidth, xWidth};

  setRegion(start, width);

}


void Image::saveRegion(const std::string &fileName) {

  netCDF::NcFile regionCtFile(fileName, netCDF::NcFile::replace);

  std::vector<netCDF::NcDim> ncDims(dims.size());
  for (int i = 0; i < dims.size(); i++)
    ncDims[i] = regionCtFile.addDim(dims[i].name, region.width[i]);

  std::vector<netCDF::NcVar> ncVars(dims.size());
  for (int i = 0; i < dims.size(); i++)
    ncVars[i] = regionCtFile.addVar(vars[i].name,
                                    netCDF::ncFloat, ncDims[i]);
  netCDF::NcVar valVar = regionCtFile.addVar(valueName,
                                             netCDF::ncShort, ncDims);

  for (int i = 0; i < dims.size(); i++)
    ncVars[i].putAtt(units, vars[i].unitName);
  valVar.putAtt(units, vars[dims.size()].unitName);

  for (int i = 0; i < dims.size(); i++)
    ncVars[i].putVar(region.dimArrays[i].data());
  valVar.putVar(region.value.data());

}
#include <NcOps/Image.h>

#include <algorithm>


Image::Image(const std::string &fileName) : ncFile_(fileName,
                                                    netCDF::NcFile::read),
                                            path_(fileName),
                                            valueName_("tomo"),
                                            units_("units"),
                                            region(Region()) {

  for (auto &&varData : ncFile_.getVars()) {
    vars_.push_back(Var());
    vars_.back().name = varData.first;
    varData.second.getAtt(units_).getValues(vars_.back().unitName);
    vars_.back().dim = varData.second.getDimCount();
  }

  std::reverse(vars_.begin(), vars_.end() - 1);

  for (auto &&dimData : ncFile_.getVar(valueName_).getDims()) {
    dims_.push_back(Dim());
    dims_.back().name = dimData.getName();
    dims_.back().size = dimData.getSize();
    dimArrays_.push_back(std::vector<float>(dims_.back().size));
    ncFile_.getVar(dims_.back().name).getVar(dimArrays_.back().data());
  }


  for (int i = 0; i < dimArrays_.size(); i++)
    std::copy(dimArrays_[i].begin() + region.start[i],
              dimArrays_[i].begin() + region.start[i]
              + region.width[i], region.dimArrays[i].begin());

  ncFile_.getVar(valueName_).getVar(region.start, region.width,
                                    region.value.data());

  xInit_ = dimArrays_[2][0];
  yInit_ = dimArrays_[1][0];
  zInit_ = dimArrays_[0][0];

  nX_ = dims_[2].size;
  nY_ = dims_[1].size;
  nZ_ = dims_[0].size;

  xStep_ = dimArrays_[2][1] - xInit_;
  yStep_ = dimArrays_[1][1] - yInit_;
  zStep_ = dimArrays_[0][1] - zInit_;

}


std::ostream &operator<<(std::ostream &stream, const Image &image) {

  stream << "path: " << image.path_ << std::endl;

  stream << "vars:" << std::endl;
  stream << "name unitName dim:" << std::endl;
  for (auto &&var : image.vars_) {
    stream << var.name << " ";
    stream << var.unitName << " ";
    stream << var.dim << std::endl;
  }

  stream << "dims:" << std::endl;
  stream << "name size step:" << std::endl;
  for (int i = 0; i < image.dims_.size(); i++) {
    stream << image.dims_[i].name << " ";
    stream << image.dims_[i].size << " ";
    stream << image.dimArrays_[i][1] - image.dimArrays_[i][0] << std::endl;
  }

  return stream;

}


double Image::getXInit() {
  return xInit_;
}

double Image::getYInit() {
  return yInit_;
}

double Image::getZInit() {
  return zInit_;
}


int Image::getNX() {
  return nX_;
}

int Image::getNY() {
  return nY_;
}

int Image::getNZ() {
  return nZ_;
}


double Image::getXStep() {
  return xStep_;
}

double Image::getYStep() {
  return yStep_;
}

double Image::getZStep() {
  return zStep_;
}


std::shared_ptr<std::vector<Dim>> Image::getDims() {
  return std::make_shared<std::vector<Dim>>(dims_);
}

std::shared_ptr<std::vector<Var>> Image::getVars() {
  return std::make_shared<std::vector<Var>>(vars_);
}

std::shared_ptr<std::vector<short>> Image::getValue() {
  return std::make_shared<std::vector<short>>(region.value);
}


void Image::setRegion(const std::vector<size_t> &start,
                      const std::vector<size_t> &width) {

  region.initiate(start, width);

  for (int i = 0; i < dimArrays_.size(); i++)
    std::copy(dimArrays_[i].begin() + region.start[i],
              dimArrays_[i].begin() + region.start[i]
              + region.width[i], region.dimArrays[i].begin());

  region.computeInitsAndSteps();

  ncFile_.getVar(valueName_).getVar(region.start, region.width,
                                    region.value.data());

}


void Image::setRegion(const Bbox &bbox) {

  auto xStart = size_t((bbox.xmin() - xInit_) / xStep_) - 3;
  auto yStart = size_t((bbox.ymin() - yInit_) / yStep_) - 3;
  auto zStart = size_t((bbox.zmin() - zInit_) / zStep_) - 3;

  auto xWidth = size_t((bbox.xmax() - bbox.xmin()) / xStep_) + 7;
  auto yWidth = size_t((bbox.ymax() - bbox.ymin()) / yStep_) + 7;
  auto zWidth = size_t((bbox.zmax() - bbox.zmin()) / zStep_) + 7;


  std::string errorMessage;
  if ((xStart + xWidth) > nX_)
    errorMessage += " x ";
  if ((yStart + yWidth) > nY_)
    errorMessage += " y ";
  if ((zStart + zWidth) > nZ_)
    errorMessage += " z ";

  if (errorMessage != "") {

    std::cerr << "ScanGrid are out range for " <<
              errorMessage << std::endl;

    std::exit(EXIT_FAILURE);

  }


  std::vector<size_t> start = {zStart, yStart, xStart};
  std::vector<size_t> width = {zWidth, yWidth, xWidth};

  setRegion(start, width);

}


void Image::saveRegion(const std::string &fileName) {

  netCDF::NcFile regionCtFile(fileName, netCDF::NcFile::replace);

  std::vector<netCDF::NcDim> ncDims(dims_.size());
  for (int i = 0; i < dims_.size(); i++)
    ncDims[i] = regionCtFile.addDim(dims_[i].name, region.width[i]);

  std::vector<netCDF::NcVar> ncVars(dims_.size());
  for (int i = 0; i < dims_.size(); i++)
    ncVars[i] = regionCtFile.addVar(vars_[i].name,
                                    netCDF::ncFloat, ncDims[i]);
  netCDF::NcVar valVar = regionCtFile.addVar(valueName_,
                                             netCDF::ncShort, ncDims);

  for (int i = 0; i < dims_.size(); i++)
    ncVars[i].putAtt(units_, vars_[i].unitName);
  valVar.putAtt(units_, vars_[dims_.size()].unitName);

  for (int i = 0; i < dims_.size(); i++)
    ncVars[i].putVar(region.dimArrays[i].data());
  valVar.putVar(region.value.data());

}

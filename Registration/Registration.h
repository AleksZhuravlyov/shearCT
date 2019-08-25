#ifndef SHEARCT_REGISTRATION_H
#define SHEARCT_REGISTRATION_H

#import <vector>
#import <PointsCt.h>
#import <NcCt.h>
#include <Transformation.h>

#include <CgalAliases.h>

typedef std::vector<std::shared_ptr<Transformation>> Transformations;


void makeRegistration(NcCt &ncCt, std::shared_ptr<PointsCt> &pointsCt,
                      const Transformations &transformations,
                      const std::vector<double> &constraintsMin,
                      const std::vector<double> &constraintsMax);

Bbox calculateGeneralBbox(std::shared_ptr<PointsCt> &pointsCt,
                          const Transformations &transformations,
                          const std::vector<double> &constraintsMin,
                          const std::vector<double> &constraintsMax);


#endif //SHEARCT_REGISTRATION_H

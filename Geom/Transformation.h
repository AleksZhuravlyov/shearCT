#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <CgalAliases.h>

Aff_transformation constructXRotation(const double &angle);

Aff_transformation constructYRotation(const double &angle);

Aff_transformation constructZRotation(const double &angle);


Aff_transformation constructXTranslation(const double &offset);

Aff_transformation constructYTranslation(const double &offset);

Aff_transformation constructZTranslation(const double &offset);


Aff_transformation constructXYStretching(const double &factor);

void translatePointFromZAxis(Point &point, const double &offset);


#endif //TRANSFORMATION_H

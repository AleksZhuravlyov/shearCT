#include <Transformation.h>

#include <cmath>
#include <vector>

#include <CgalAliases.h>


Aff_transformation constructXRotation(const double &angle) {
    return Aff_transformation(1, 0, 0,
                              0, cos(angle), -sin(angle),
                              0, sin(angle), cos(angle));
}

Aff_transformation constructYRotation(const double &angle) {
    return Aff_transformation(cos(angle), 0, sin(angle),
                              0, 1, 0,
                              -sin(angle), 0, cos(angle));
}

Aff_transformation constructZRotation(const double &angle) {
    return Aff_transformation(cos(angle), -sin(angle), 0,
                              sin(angle), cos(angle), 0,
                              0, 0, 1);
}


Aff_transformation constructXTranslation(const double &offset) {
    return Aff_transformation(CGAL::TRANSLATION, Vector(offset, 0, 0));
}

Aff_transformation constructYTranslation(const double &offset) {
    return Aff_transformation(CGAL::TRANSLATION, Vector(0, offset, 0));
}

Aff_transformation constructZTranslation(const double &offset) {
    return Aff_transformation(CGAL::TRANSLATION, Vector(0, 0, offset));
}


Aff_transformation constructXYStretching(const double &factor) {
    return Aff_transformation(factor, 0, 0,
                              0, factor, 0,
                              0, 0, 1);
}

void translatePointFromZAxis(Point &point, const double &offset) {

    double dx = 0;
    double dy = 0;

    double x = point.x();
    double y = point.y();

    if (x == 0 && y != 0)

        dy = offset * (y > 0) - (y < 0);

    else if (x != 0 && y == 0)

        dx = offset * (x > 0) - (x < 0);

    else if (x != 0 && y != 0) {

        double Cx = y / x * y / x;
        double Cy = x / y * x / y;

        dx = offset / sqrt(1 + Cx) * ((x > 0) - (x < 0));
        dy = offset / sqrt(1 + Cy) * ((y > 0) - (y < 0));

    }

    auto translation = Aff_transformation(CGAL::TRANSLATION, Vector(dx, dy, 0));

    point = translation(point);

}

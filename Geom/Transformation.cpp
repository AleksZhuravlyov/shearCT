#include <cmath>

#include <Transformation.h>


Aff_transformation RotationX::operator()(const double &value) {
    return Aff_transformation(1, 0, 0,
                              0, cos(value), -sin(value),
                              0, sin(value), cos(value));
}

Aff_transformation RotationY::operator()(const double &value) {
    return Aff_transformation(cos(value), 0, sin(value),
                              0, 1, 0,
                              -sin(value), 0, cos(value));
}

Aff_transformation RotationZ::operator()(const double &value) {
    return Aff_transformation(cos(value), -sin(value), 0,
                              sin(value), cos(value), 0,
                              0, 0, 1);
}

Aff_transformation TranslationX::operator()(const double &value) {
    return Aff_transformation(CGAL::TRANSLATION, Vector(value, 0, 0));
}

Aff_transformation TranslationY::operator()(const double &value) {
    return Aff_transformation(CGAL::TRANSLATION, Vector(0, value, 0));
}

Aff_transformation TranslationZ::operator()(const double &value) {
    return Aff_transformation(CGAL::TRANSLATION, Vector(0, 0, value));
}

Aff_transformation StretchingXY::operator()(const double &value) {
    return Aff_transformation(value, 0, 0,
                              0, value, 0,
                              0, 0, 1);
}

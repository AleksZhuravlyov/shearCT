#include <Points/Points.h>


Points::Points() :
    Points(std::make_shared<Points_3>(1, CgalPoint(0, 0, 0)),
           std::make_shared<Basis>()) {};

Points::Points(std::shared_ptr<Points_3> _points_3) :
    Points(_points_3, std::make_shared<Basis>()) {
    translateBasisToCenter();
}

Points::Points(const Bbox_3 &bbox_3) :
    Points(std::make_shared<Points_3>(
            Points_3{CgalPoint(bbox_3.xmin(), bbox_3.ymin(), bbox_3.zmin()),
                     CgalPoint(bbox_3.xmax(), bbox_3.ymin(), bbox_3.zmin()),
                     CgalPoint(bbox_3.xmin(), bbox_3.ymax(), bbox_3.zmin()),
                     CgalPoint(bbox_3.xmax(), bbox_3.ymax(), bbox_3.zmin()),
                     CgalPoint(bbox_3.xmin(), bbox_3.ymin(), bbox_3.zmax()),
                     CgalPoint(bbox_3.xmax(), bbox_3.ymin(), bbox_3.zmax()),
                     CgalPoint(bbox_3.xmin(), bbox_3.ymax(), bbox_3.zmax()),
                     CgalPoint(bbox_3.xmax(), bbox_3.ymax(), bbox_3.zmax())}),
           std::make_shared<Basis>()) {
    translateBasisToCenter();
}


Points::Points(std::shared_ptr<Points_3> _points_3,
               std::shared_ptr<Basis> _basis) :
    points_3(_points_3),
    basis(_basis),
    tomoA(std::make_shared<Value>(_points_3->size(), 0)),
    tomoB(std::make_shared<Value>(_points_3->size(), 0)),
    tomoBuffer(std::make_shared<Value>(_points_3->size(), 0)),
    result(std::make_shared<Value>(_points_3->size(), 0)) {}


Points::Points(const Points &points) :
    points_3(std::make_shared<Points_3>(*(points.points_3))),
    basis(std::make_shared<Basis>(*(points.basis))),
    tomoA(std::make_shared<Value>(*(points.tomoA))),
    tomoB(std::make_shared<Value>(*(points.tomoB))),
    tomoBuffer(std::make_shared<Value>(*(points.tomoBuffer))),
    result(std::make_shared<Value>(*(points.result))) {}

Points::Points(Points &&points) : points_3(points.points_3),
                                  basis(points.basis),
                                  tomoA(points.tomoA),
                                  tomoB(points.tomoB),
                                  tomoBuffer(points.tomoBuffer),
                                  result(points.result) {}


Points &Points::operator=(Points &&points) {

    if (&points == this)
        return *this;

    points_3 = std::make_shared<Points_3>(*(points.points_3));
    basis = std::make_shared<Basis>(*(points.basis));
    tomoA = std::make_shared<Value>(*(points.tomoA));
    tomoB = std::make_shared<Value>(*(points.tomoB));
    tomoBuffer = std::make_shared<Value>(*(points.tomoBuffer));
    result = std::make_shared<Value>(*(points.result));

    return *this;

}


void Points::transform(const CgalTransformation &aff_transformation_3) {

    auto basisTransformation = basis->generateTransformation();
    auto basisInverseTransformation = basisTransformation.inverse();


    for (auto &&point : *points_3)
        point = basisInverseTransformation(point);
    basis->transform(basisInverseTransformation);

    for (auto &&point : *points_3)
        point = aff_transformation_3(point);
    basis->transform(aff_transformation_3);

    for (auto &&point : *points_3)
        point = basisTransformation(point);
    basis->transform(basisTransformation);

}

void Points::transform(
        const std::vector<CgalTransformation> &aff_transformations_3) {

    auto basisTransformation = basis->generateTransformation();
    auto basisInverseTransformation = basisTransformation.inverse();


    for (auto &&point : *points_3)
        point = basisInverseTransformation(point);
    basis->transform(basisInverseTransformation);

    for (auto &&transformation : aff_transformations_3) {
        for (auto &&point : *points_3)
            point = transformation(point);
        basis->transform(transformation);
    }

    for (auto &&point : *points_3)
        point = basisTransformation(point);
    basis->transform(basisTransformation);

}


void Points::translateBasis(const CgalPoint &point_3) {
    basis->setOrigin(point_3);
}

void Points::translateBasisToCenter() {

    auto bbox = CGAL::bbox_3(points_3->begin(), points_3->end());

    auto center = CgalPoint((bbox.xmax() + bbox.xmin()) / 2,
                        (bbox.ymax() + bbox.ymin()) / 2,
                        (bbox.zmax() + bbox.zmin()) / 2);

    translateBasis(center);

}


int Points::size() {
    return points_3->size();
}


void Points::setPoints(std::shared_ptr<Points_3> _points_3) {

    if (points_3->size() != _points_3->size()) {
        tomoA->resize(_points_3->size(), 0);
        tomoB->resize(_points_3->size(), 0);
        tomoBuffer->resize(_points_3->size(), 0);
        result->resize(_points_3->size(), 0);
    }

    points_3 = _points_3;

}

void Points::setBasis(std::shared_ptr<Basis> _basis) {
    basis = _basis;
}


void Points::setTomoA(std::shared_ptr<std::vector<double>> _tomoA) {
    tomoA = _tomoA;
}

void Points::setTomoB(std::shared_ptr<std::vector<double>> _tomoB) {
    tomoB = _tomoB;
}

void Points::setTomoBuffer(std::shared_ptr<std::vector<double>> _tomoBuffer) {
    tomoBuffer = _tomoBuffer;
}

void Points::setResult(std::shared_ptr<std::vector<double>> _result) {
    result = _result;
}

void Points::swapTomoAAndTomoBuffer() {
    swap(tomoA, tomoBuffer);
}


std::shared_ptr<Points_3> Points::getPoints() {
    return points_3;
}

std::shared_ptr<Basis> Points::getBasis() {
    return basis;
}


std::shared_ptr<Value> Points::getTomoA() {
    return tomoA;
}

std::shared_ptr<Value> Points::getTomoB() {
    return tomoB;
}

std::shared_ptr<Value> Points::getTomoBuffer() {
    return tomoBuffer;
}

std::shared_ptr<Value> Points::getResult() {
    return result;
}


Bbox_3 Points::generateBbox() {
    return CGAL::bbox_3(points_3->begin(), points_3->end());
}


void Points::computeResult() {
    for (int i = 0; i < result->size(); i++)
        (*result)[i] = (*tomoA)[i] - (*tomoB)[i];
}

double Points::computePearsonCorrelation() {

    auto n = points_3->size();

    double A = 0;
    double B = 0;

    double AA = 0;
    double BB = 0;
    double AB = 0;

    for (int i = 0; i < n; i++) {

        A += (*tomoA)[i];
        B += (*tomoB)[i];

        AA += (*tomoA)[i] * (*tomoA)[i];
        BB += (*tomoB)[i] * (*tomoB)[i];

        AB += (*tomoA)[i] * (*tomoB)[i];

    }

    return (n * AB - A * B) / sqrt(n * AA - A * A) / sqrt(n * BB - B * B);

}


void Points::createXYSquare(const double &xCenter,
                            const double &yCenter,
                            const double &zCenter,
                            const double &xWidth, const double &yWidth,
                            const int &nX, const int &nY) {

    auto xStep = xWidth / (nX - 1);
    auto yStep = yWidth / (nY - 1);

    auto xInit = xCenter - xWidth / 2;
    auto yInit = yCenter - yWidth / 2;
    auto zInit = zCenter;


    auto _points = std::make_shared<Points_3>(Points_3());
    for (int i = 0; i < nX; i++)
        for (int j = 0; j < nY; j++)
            _points->push_back(
                CgalPoint(xInit + xStep * i,
                          yInit + yStep * j,
                          zInit));

    setPoints(_points);

    translateBasisToCenter();

}


void Points::createZCylinderSegment(const double &xCylinderBaseCenter,
                                    const double &yCylinderBaseCenter,
                                    const double &zCylinderBaseCenter,
                                    const double &R,
                                    const double &angleCenter,
                                    const double &zWidth,
                                    const double &angleWidth,
                                    const int &nZ, const int &nAngle) {

    auto angleStep = angleWidth / (nAngle - 1);
    auto zStep = zWidth / (nZ - 1);

    auto angleInit = angleCenter - angleWidth / 2;
    auto xInit = xCylinderBaseCenter;
    auto yInit = yCylinderBaseCenter;
    auto zInit = zCylinderBaseCenter;

    auto _points = std::make_shared<Points_3>(Points_3());
    for (int i = 0; i < nAngle; i++)
        for (int j = 0; j < nZ; j++)
            _points->push_back(
                CgalPoint(xInit + R * cos(angleInit + angleStep * i),
                          yInit + R * sin(angleInit + angleStep * i),
                          zInit + zStep * j));

    setPoints(_points);

    translateBasis(CgalPoint(xCylinderBaseCenter,
                             yCylinderBaseCenter,
                             zCylinderBaseCenter));

}
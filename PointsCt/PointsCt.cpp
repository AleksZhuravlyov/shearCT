#include <PointsCt.h>

#include <cmath>


PointsCt::PointsCt() :
        PointsCt(std::make_shared<Points>(1, Point(0, 0, 0)),
                 std::make_shared<Basis>()) {};

PointsCt::PointsCt(std::shared_ptr<Points> _points) :
        PointsCt(_points, std::make_shared<Basis>()) {
    translateBasisToCenter();
}

PointsCt::PointsCt(std::shared_ptr<Points> _points,
                   std::shared_ptr<Basis> _basis) :
        points(_points),
        basis(_basis),
        tomoA(std::make_shared<Value>(_points->size(), 0)),
        tomoB(std::make_shared<Value>(_points->size(), 0)),
        result(std::make_shared<Value>(_points->size(), 0)) {}


PointsCt::PointsCt(const PointsCt &pointsCt) :
        points(std::make_shared<Points>(*(pointsCt.points))),
        basis(std::make_shared<Basis>(*(pointsCt.basis))),
        tomoA(std::make_shared<Value>(*(pointsCt.tomoA))),
        tomoB(std::make_shared<Value>(*(pointsCt.tomoB))),
        result(std::make_shared<Value>(*(pointsCt.result))) {}

PointsCt::PointsCt(PointsCt &&pointsCt) : points(pointsCt.points),
                                          basis(pointsCt.basis),
                                          tomoA(pointsCt.tomoA),
                                          tomoB(pointsCt.tomoB),
                                          result(pointsCt.result) {}


PointsCt &PointsCt::operator=(PointsCt &&pointsCt) {

    if (&pointsCt == this)
        return *this;

    points = std::make_shared<Points>(*(pointsCt.points));
    basis = std::make_shared<Basis>(*(pointsCt.basis));
    tomoA = std::make_shared<Value>(*(pointsCt.tomoA));
    tomoB = std::make_shared<Value>(*(pointsCt.tomoB));
    result = std::make_shared<Value>(*(pointsCt.result));

    return *this;

}


void PointsCt::transform(const Aff_transformation &transformation) {

    auto basisTransformation = basis->generateTransformation();
    auto basisInverseTransformation = basisTransformation.inverse();


    for (auto &&point : *points)
        point = basisInverseTransformation(point);
    basis->transform(basisInverseTransformation);

    for (auto &&point : *points)
        point = transformation(point);
    basis->transform(transformation);

    for (auto &&point : *points)
        point = basisTransformation(point);
    basis->transform(basisTransformation);

}


void PointsCt::translateBasis(const Point &point) {
    basis->setOrigin(point);
}

void PointsCt::translateBasisToCenter() {

    auto bbox = CGAL::bbox_3(points->begin(), points->end());

    auto center = Point((bbox.xmax() + bbox.xmin()) / 2,
                        (bbox.ymax() + bbox.ymin()) / 2,
                        (bbox.zmax() + bbox.zmin()) / 2);

    translateBasis(center);

}


int PointsCt::size() {
    return points->size();
}


void PointsCt::setPoints(std::shared_ptr<Points> _points) {

    if (points->size() != _points->size()) {
        tomoA->resize(_points->size(), 0);
        tomoB->resize(_points->size(), 0);
        result->resize(_points->size(), 0);
    }

    points = _points;

}

void PointsCt::setBasis(std::shared_ptr<Basis> _basis) {
    basis = _basis;
}


void PointsCt::setTomoA(std::shared_ptr<std::vector<double>> _tomoA) {
    tomoA = _tomoA;
}

void PointsCt::setTomoB(std::shared_ptr<std::vector<double>> _tomoB) {
    tomoB = _tomoB;
}

void PointsCt::setResult(std::shared_ptr<std::vector<double>> _result) {
    result = _result;
}


std::shared_ptr<Points> PointsCt::getPoints() {
    return points;
}

std::shared_ptr<Basis> PointsCt::getBasis() {
    return basis;
}


std::shared_ptr<Value> PointsCt::getTomoA() {
    return tomoA;
}

std::shared_ptr<Value> PointsCt::getTomoB() {
    return tomoB;
}

std::shared_ptr<Value> PointsCt::getResult() {
    return result;
}


Bbox PointsCt::generateBbox() {
    return CGAL::bbox_3(points->begin(), points->end());
}


void PointsCt::computeResult() {
    for (int i = 0; i < result->size(); i++)
        (*result)[i] = (*tomoA)[i] - (*tomoB)[i];
}

double PointsCt::computePearsonCorrelation() {

    auto n = points->size();

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


void PointsCt::createXYSquare(const double &xCenter,
                              const double &yCenter,
                              const double &zCenter,
                              const double &xWidth, const double &yWidth,
                              const int &nX, const int &nY) {

    auto xStep = xWidth / (nX - 1);
    auto yStep = yWidth / (nY - 1);

    auto xInit = xCenter - xWidth / 2;
    auto yInit = yCenter - yWidth / 2;
    auto zInit = zCenter;


    auto _points = std::make_shared<Points>(Points());
    for (int i = 0; i < nX; i++)
        for (int j = 0; j < nY; j++)
            _points->push_back(
                    Point(xInit + xStep * i,
                          yInit + yStep * j,
                          zInit));

    setPoints(_points);

    translateBasisToCenter();

}


void PointsCt::createZCylinderSegment(const double &xCylinderBaseCenter,
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

    auto _points = std::make_shared<Points>(Points());
    for (int i = 0; i < nAngle; i++)
        for (int j = 0; j < nZ; j++)
            _points->push_back(
                    Point(xInit + R * cos(angleInit + angleStep * i),
                          yInit + R * sin(angleInit + angleStep * i),
                          zInit + zStep * j));

    setPoints(_points);

    translateBasis(Point(xCylinderBaseCenter,
                         yCylinderBaseCenter,
                         zCylinderBaseCenter));

}
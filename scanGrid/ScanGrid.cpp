#include "ScanGrid.h"


ScanGrid::ScanGrid() :
        ScanGrid(std::make_shared<Points>(1, Point(0, 0, 0)),
                 std::make_shared<Basis>()) {};

ScanGrid::ScanGrid(std::shared_ptr<Points> points) :
        ScanGrid(points, std::make_shared<Basis>()) {
    translateBasisToCenter();
}

ScanGrid::ScanGrid(const Bbox &bbox) :
        ScanGrid(std::make_shared<Points>(
                Points{Point(bbox.xmin(), bbox.ymin(), bbox.zmin()),
                       Point(bbox.xmax(), bbox.ymin(), bbox.zmin()),
                       Point(bbox.xmin(), bbox.ymax(), bbox.zmin()),
                       Point(bbox.xmax(), bbox.ymax(), bbox.zmin()),
                       Point(bbox.xmin(), bbox.ymin(), bbox.zmax()),
                       Point(bbox.xmax(), bbox.ymin(), bbox.zmax()),
                       Point(bbox.xmin(), bbox.ymax(), bbox.zmax()),
                       Point(bbox.xmax(), bbox.ymax(),
                             bbox.zmax())}),
                 std::make_shared<Basis>()) {
    translateBasisToCenter();
}

ScanGrid::ScanGrid(const Bbox &bbox, std::shared_ptr<Basis> basis) :
        ScanGrid(bbox) {
    setBasis(std::make_shared<Basis>(Basis(*basis)));
}


ScanGrid::ScanGrid(std::shared_ptr<Points> points,
                   std::shared_ptr<Basis> basis) :
        points_(points),
        basis_(basis),
        tomoA_(std::make_shared<std::vector<double>>(points->size(), 0)),
        tomoB_(std::make_shared<std::vector<double>>(points->size(), 0)),
        buffer_(std::make_shared<std::vector<double>>(points->size(), 0)),
        result_(std::make_shared<std::vector<double>>(points->size(), 0)) {}


ScanGrid::ScanGrid(const ScanGrid &scanGrid) :
        points_(std::make_shared<Points>(*(scanGrid.points_))),
        basis_(std::make_shared<Basis>(*(scanGrid.basis_))),
        tomoA_(std::make_shared<std::vector<double>>(*(scanGrid.tomoA_))),
        tomoB_(std::make_shared<std::vector<double>>(*(scanGrid.tomoB_))),
        buffer_(std::make_shared<std::vector<double>>(*(scanGrid.buffer_))),
        result_(std::make_shared<std::vector<double>>(*(scanGrid.result_))) {}

ScanGrid::ScanGrid(ScanGrid &&scanGrid) : points_(scanGrid.points_),
                                          basis_(scanGrid.basis_),
                                          tomoA_(scanGrid.tomoA_),
                                          tomoB_(scanGrid.tomoB_),
                                          buffer_(scanGrid.buffer_),
                                          result_(scanGrid.result_) {}


ScanGrid &ScanGrid::operator=(ScanGrid &&scanGrid) {

    if (&scanGrid == this)
        return *this;

    points_ = std::make_shared<Points>(*(scanGrid.points_));
    basis_ = std::make_shared<Basis>(*(scanGrid.basis_));
    tomoA_ = std::make_shared<std::vector<double>>(*(scanGrid.tomoA_));
    tomoB_ = std::make_shared<std::vector<double>>(*(scanGrid.tomoB_));
    buffer_ = std::make_shared<std::vector<double>>(*(scanGrid.buffer_));
    result_ = std::make_shared<std::vector<double>>(*(scanGrid.result_));

    return *this;

}


void ScanGrid::transform(const Transformation &transformation) {
    std::vector<Transformation> transformations = {transformation};
    transform(transformations);
}

void ScanGrid::transform(
        const std::vector<Transformation> &transformations) {

    auto basisTransformation = basis_->generateTransformation();
    auto basisInverseTransformation = basisTransformation.inverse();


    for (auto &&point : *points_)
        point = basisInverseTransformation(point);
    basis_->transform(basisInverseTransformation);

    for (auto &&transformation : transformations) {
        for (auto &&point : *points_)
            point = transformation(point);
        basis_->transform(transformation);
    }

    for (auto &&point : *points_)
        point = basisTransformation(point);
    basis_->transform(basisTransformation);

}


void ScanGrid::translateBasis(const Point &point) {
    basis_->setOrigin(point);
}

void ScanGrid::translateBasis(const double &x,
                              const double &y,
                              const double &z) {
    auto point = Point(x, y, z);
    basis_->setOrigin(point);
}

void ScanGrid::translateBasisToCenter() {

    auto bbox = CGAL::bbox_3(points_->begin(), points_->end());

    auto center = Point((bbox.xmax() + bbox.xmin()) / 2,
                        (bbox.ymax() + bbox.ymin()) / 2,
                        (bbox.zmax() + bbox.zmin()) / 2);

    translateBasis(center);

}


int ScanGrid::size() {
    return points_->size();
}


void ScanGrid::setPoints(std::shared_ptr<Points> points) {

    if (points_->size() != points->size()) {
        tomoA_->resize(points->size(), 0);
        tomoB_->resize(points->size(), 0);
        buffer_->resize(points->size(), 0);
        result_->resize(points->size(), 0);
    }

    points_ = points;

}

void ScanGrid::setBasis(std::shared_ptr<Basis> basis) {
    basis_ = basis;
}


void ScanGrid::setTomoA(std::shared_ptr<std::vector<double>> tomoA) {
    tomoA_ = tomoA;
}

void ScanGrid::setTomoB(std::shared_ptr<std::vector<double>> tomoB) {
    tomoB_ = tomoB;
}

void ScanGrid::setTomoBuffer(std::shared_ptr<std::vector<double>> buffer) {
    buffer_ = buffer;
}

void ScanGrid::setResult(std::shared_ptr<std::vector<double>> result) {
    result_ = result;
}

void ScanGrid::swapTomoAAndBuffer() {
    swap(tomoA_, buffer_);
}


std::shared_ptr<Points> ScanGrid::getPoints() {
    return points_;
}

std::shared_ptr<Basis> ScanGrid::getBasis() {
    return basis_;
}


std::shared_ptr<std::vector<double>> ScanGrid::getTomoA() {
    return tomoA_;
}

std::shared_ptr<std::vector<double>> ScanGrid::getTomoB() {
    return tomoB_;
}

std::shared_ptr<std::vector<double>> ScanGrid::getBuffer() {
    return buffer_;
}

std::shared_ptr<std::vector<double>> ScanGrid::getResult() {
    return result_;
}


Bbox ScanGrid::generateBbox() {
    return CGAL::bbox_3(points_->begin(), points_->end());
}


void ScanGrid::computeDifferenceAB() {
    for (int i = 0; i < result_->size(); i++)
        (*result_)[i] = (*tomoA_)[i] - (*tomoB_)[i];
}

double ScanGrid::computePearsonCorrelationAB() {

    auto n = points_->size();

    double A = 0;
    double B = 0;

    double AA = 0;
    double BB = 0;
    double AB = 0;

    for (int i = 0; i < n; i++) {

        A += (*tomoA_)[i];
        B += (*tomoB_)[i];

        AA += (*tomoA_)[i] * (*tomoA_)[i];
        BB += (*tomoB_)[i] * (*tomoB_)[i];

        AB += (*tomoA_)[i] * (*tomoB_)[i];

    }

    return (n * AB - A * B) / sqrt(n * AA - A * A) / sqrt(n * BB - B * B);

}


void ScanGrid::createXYSquare(const double &xCenter,
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


void ScanGrid::createZCylinderSegment(const double &xCylinderBaseCenter,
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

    auto points = std::make_shared<Points>(Points());
    for (int i = 0; i < nAngle; i++)
        for (int j = 0; j < nZ; j++)
            points->push_back(
                    Point(xInit + R * cos(angleInit + angleStep * i),
                          yInit + R * sin(angleInit + angleStep * i),
                          zInit + zStep * j));

    setPoints(points);

    translateBasis(Point(xCylinderBaseCenter,
                         yCylinderBaseCenter,
                         zCylinderBaseCenter));

}
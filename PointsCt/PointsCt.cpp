#include <PointsCt.h>


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
        nPoints(_points->size()),
        tomoA(std::make_shared<std::vector<double>>(_points->size(), 0)),
        tomoB(std::make_shared<std::vector<double>>(_points->size(), 0)),
        result(std::make_shared<std::vector<double>>(_points->size(), 0)),
        basis(_basis) {}


int PointsCt::size() {
    return nPoints;
}

void PointsCt::setPoints(std::shared_ptr<Points> _points) {
    points = _points;
    nPoints = points->size();
    tomoA->resize(nPoints, 0);
    tomoB->resize(nPoints, 0);
    result->resize(nPoints, 0);
}

void PointsCt::setTomoA(std::shared_ptr<std::vector<double>> value) {
    tomoA = value;
}

void PointsCt::setTomoB(std::shared_ptr<std::vector<double>> value) {
    tomoB = value;
}

void PointsCt::setResult(std::shared_ptr<std::vector<double>> value) {
    result = value;
}


std::shared_ptr<Points> PointsCt::getPoints() {
    return points;
}

std::shared_ptr<std::vector<double>> PointsCt::getTomoA() {
    return tomoA;
}

std::shared_ptr<std::vector<double>> PointsCt::getTomoB() {
    return tomoB;
}

std::shared_ptr<std::vector<double>> PointsCt::getResult() {
    return result;
}


std::shared_ptr<Basis> PointsCt::getBasis() {
    return basis;
}


void PointsCt::setBasis(std::shared_ptr<Basis> _basis) {
    basis = _basis;
}


void PointsCt::translateBasisToCenter() {
    auto bbox = CGAL::bbox_3(points->begin(), points->end());
    auto center = Point((bbox.xmax() - bbox.xmin()) / 2,
                        (bbox.ymax() - bbox.ymin()) / 2,
                        (bbox.zmax() - bbox.zmin()) / 2);
    basis->setOrigin(center);
}


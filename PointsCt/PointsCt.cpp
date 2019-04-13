#include <PointsCt.h>


PointsCt::PointsCt() :
        PointsCt(std::make_shared<Points>(1, Point(0, 0, 0))) {};

PointsCt::PointsCt(std::shared_ptr<Points> _points) :
        points(_points),
        nPoints(_points->size()),
        tomoA(std::make_shared<std::vector<double>>(_points->size(), 0)),
        tomoB(std::make_shared<std::vector<double>>(_points->size(), 0)),
        result(std::make_shared<std::vector<double>>(_points->size(), 0)) {}


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

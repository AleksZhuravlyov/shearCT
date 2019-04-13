#ifndef POINTSCT_H
#define POINTSCT_H

#include <vector>
#include <string>

#include <Geom.h>


class PointsCt {

public:

    PointsCt();

    PointsCt(std::shared_ptr<Points> _points);


    virtual ~PointsCt() {}


    int size();

    void setPoints(std::shared_ptr<Points> _points);

    void setTomoA(std::shared_ptr<std::vector<double>> value);

    void setTomoB(std::shared_ptr<std::vector<double>> value);

    void setResult(std::shared_ptr<std::vector<double>> value);


    std::shared_ptr<Points> getPoints();

    std::shared_ptr<std::vector<double>> getTomoA();

    std::shared_ptr<std::vector<double>> getTomoB();

    std::shared_ptr<std::vector<double>> getResult();


private:

    int nPoints;

    std::shared_ptr<Points> points;

    std::shared_ptr<std::vector<double>> tomoA;

    std::shared_ptr<std::vector<double>> tomoB;

    std::shared_ptr<std::vector<double>> result;


};


#endif //POINTSCT_H

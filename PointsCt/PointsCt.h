#ifndef POINTSCT_H
#define POINTSCT_H

#include <vector>
#include <string>

#include <Basis.h>

typedef std::vector<double> Value;


class PointsCt {

public:

    PointsCt();

    PointsCt(std::shared_ptr<Points> _points);

    PointsCt(std::shared_ptr<Points> _points,
             std::shared_ptr<Basis> _basis);

    PointsCt(const PointsCt &pointsCt);

    PointsCt(PointsCt &&pointsCt);

    virtual ~PointsCt() {}


    PointsCt &operator=(PointsCt &&pointsCt);


    void transform(const Aff_transformation &transformation);


    void translateBasisToCenter();


    int size();


    void setPoints(std::shared_ptr<Points> _points);

    void setBasis(std::shared_ptr<Basis> _basis);


    void setTomoA(std::shared_ptr<Value> _tomoA);

    void setTomoB(std::shared_ptr<Value> _tomoB);

    void setResult(std::shared_ptr<Value> _result);


    std::shared_ptr<Points> getPoints();

    std::shared_ptr<Basis> getBasis();


    std::shared_ptr<std::vector<double>> getTomoA();

    std::shared_ptr<std::vector<double>> getTomoB();

    std::shared_ptr<std::vector<double>> getResult();


    Bbox generateBbox();


private:

    std::shared_ptr<Points> points;

    std::shared_ptr<Basis> basis;


    std::shared_ptr<Value> tomoA;

    std::shared_ptr<Value> tomoB;

    std::shared_ptr<Value> result;

};


#endif //POINTSCT_H

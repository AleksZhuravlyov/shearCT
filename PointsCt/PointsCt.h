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


    void translateBasis(const Point &point);

    void translateBasisToCenter();


    int size();


    void setPoints(std::shared_ptr<Points> _points);

    void setBasis(std::shared_ptr<Basis> _basis);


    void setTomoA(std::shared_ptr<Value> _tomoA);

    void setTomoB(std::shared_ptr<Value> _tomoB);

    void setTomoBuffer(std::shared_ptr<Value> _tomoBuffer);

    void setResult(std::shared_ptr<Value> _result);

    void swapAAndBuffer();


    std::shared_ptr<Points> getPoints();

    std::shared_ptr<Basis> getBasis();


    std::shared_ptr<Value> getTomoA();

    std::shared_ptr<Value> getTomoB();

    std::shared_ptr<Value> getTomoBuffer();

    std::shared_ptr<Value> getResult();


    Bbox generateBbox();


    void computeResult();

    double computePearsonCorrelation();


    void createXYSquare(const double &xCenter,
                        const double &yCenter,
                        const double &zCenter,
                        const double &xWidth, const double &yWidth,
                        const int &nX, const int &nY);

    void createZCylinderSegment(const double &xCylinderBaseCenter,
                                const double &yCylinderBaseCenter,
                                const double &zCylinderBaseCenter,
                                const double &R,
                                const double &angleCenter,
                                const double &zWidth,
                                const double &angleWidth,
                                const int &nZ, const int &nAngle);


private:

    std::shared_ptr<Points> points;

    std::shared_ptr<Basis> basis;


    std::shared_ptr<Value> tomoA;

    std::shared_ptr<Value> tomoB;

    std::shared_ptr<Value> tomoBuffer;

    std::shared_ptr<Value> result;

};


#endif //POINTSCT_H

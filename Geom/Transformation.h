#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <CgalAliases.h>


class Transformation {
public:
    Transformation() = default;
    virtual ~Transformation() {}
    virtual Aff_transformation operator()(const double &value) = 0;
};


class RotationX : public Transformation {
public:
    RotationX() {}
    virtual Aff_transformation operator()(const double &value) override final;
};

class RotationY : public Transformation {
public:
    RotationY() {}
    virtual Aff_transformation operator()(const double &value) override final;
};

class RotationZ : public Transformation {
public:
    RotationZ() {}
    virtual Aff_transformation operator()(const double &value) override final;
};


class TranslationX : public Transformation {
public:
    TranslationX() {}
    virtual Aff_transformation operator()(const double &value) override final;
};

class TranslationY : public Transformation {
public:
    TranslationY() {}
    virtual Aff_transformation operator()(const double &value) override final;
};

class TranslationZ : public Transformation {
public:
    TranslationZ() {}
    virtual Aff_transformation operator()(const double &value) override final;
};


class StretchingXY : public Transformation {
public:
    StretchingXY() {}
    virtual Aff_transformation operator()(const double &value) override final;
};


#endif //TRANSFORMATION_H

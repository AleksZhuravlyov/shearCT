#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <CgalAliases.h>


class Transformation {
public:
    Transformation() = default;

    virtual ~Transformation() {}

    virtual Aff_transformation operator()(const double &value) = 0;

    std::string getName() { return name; }

protected:
    std::string name;
};


class RotationX : public Transformation {
public:
    RotationX() { name = "RotationX"; }

    virtual ~RotationX() {}

    virtual Aff_transformation operator()(const double &value) override final;
};

class RotationY : public Transformation {
public:
    RotationY() { name = "RotationY"; }

    virtual ~RotationY() {}

    virtual Aff_transformation operator()(const double &value) override final;
};

class RotationZ : public Transformation {
public:
    RotationZ() { name = "RotationZ"; }

    virtual ~RotationZ() {}

    virtual Aff_transformation operator()(const double &value) override final;
};


class TranslationX : public Transformation {
public:
    TranslationX() { name = "TranslationX"; }

    virtual ~TranslationX() {}

    virtual Aff_transformation operator()(const double &value) override final;
};

class TranslationY : public Transformation {
public:
    TranslationY() { name = "TranslationY"; }

    virtual ~TranslationY() {}

    virtual Aff_transformation operator()(const double &value) override final;
};

class TranslationZ : public Transformation {
public:
    TranslationZ() { name = "TranslationZ"; }

    virtual ~TranslationZ() {}

    virtual Aff_transformation operator()(const double &value) override final;
};


class StretchingXY : public Transformation {
public:
    StretchingXY() { name = "StretchingXY"; }

    virtual ~StretchingXY() {}

    virtual Aff_transformation operator()(const double &value) override final;
};


#endif //TRANSFORMATION_H

#ifndef PARTICLEMODEL_H
#define PARTICLEMODEL_H
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>

class ParticleObject
{
public:
    virtual QJsonObject toJson(){return QJsonObject();}
    virtual void fromJson(QJsonObject obj){}
    virtual ~ParticleObject(){}
};

class CameraPerspective:public ParticleObject
{
public:
    double fovy = 3.1415926*0.25;
    double aspect = 0;
    double zNear = 0.1;
    double zFar = 1000;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class CameraOrthogonal:public ParticleObject
{
public:
    double left = 0;
    double right = 0;
    double top = 0;
    double bottom = 0;
    double zNear = 0.1;
    double zFar = 1000;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class CameraModel:public ParticleObject
{
public:
    QString mode = "NDC";
    CameraPerspective perspective;
    CameraOrthogonal orthogonal;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class ParticleValueStable:public ParticleObject
{
public:
    double base = 0;
    double rand = 0;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class ParticleValueLinear:public ParticleObject
{
public:
    double base = 0;
    double baseRand = 0;
    double acceleration = 0;
    double accelerationRand = 0;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class ParticleValueCircular:public ParticleObject
{
public:
    double base = 0;
    double baseRand = 0;
    double radius = 0;
    double radiusRand = 0;
    double rate = 0;
    double rateRand = 0;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class ParticleValue:public ParticleObject
{
public:
    QString mode = "stable";
    ParticleValueStable stable;
    ParticleValueLinear linear;
    ParticleValueCircular circular;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class Path1DLinear:public ParticleObject
{
public:
    ParticleValue start_point;
    ParticleValue speed;
    ParticleValue acceleration;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class Path1D:public ParticleObject
{
public:
    QString mode = "linear";
    Path1DLinear linear;
    QString custom;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class Path2DCircular:public ParticleObject
{
public:
    ParticleValue circles;
    ParticleValue radius;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class Path2D:public ParticleObject
{
public:
    QString mode = "circular";
    Path2DCircular circular;
    QString custom;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);

};

class Path3DSpherical:public ParticleObject
{
public:
    ParticleValue radius;
    ParticleValue polar_angle;
    ParticleValue azimuth_angle;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);

};

class Path3D:public ParticleObject
{
public:
    QString mode = "spherical";
    Path3DSpherical spherical;
    QString custom;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class PathModel:public ParticleObject
{
public:
    bool x = false;
    bool y = false;
    bool z = false;
    Path1D path1D;
    Path2D path2D;
    Path3D path3D;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class ParticleAttribute:public ParticleObject
{
public:
    ParticleValue width;
    ParticleValue height;
    ParticleValue rotation;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class ParticleDisplay:public ParticleObject
{
public:
    QString image_name;
    QString mode = "forward";
    int count = 0;
    int width = 0;
    int height = 0;
    double fps = 0;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

class ParticleParam:public ParticleObject
{
public:
    QString name;
    std::vector<PathModel> pathModel;
    ParticleAttribute attribute;
    ParticleDisplay display;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);

};

class ParticleModel:public ParticleObject
{
public:
    CameraModel cameraModel;
    std::vector<ParticleParam> particles;
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject obj);
};

#endif // PARTICLEMODEL_H

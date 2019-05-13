#include "particlemodel.h"

QJsonObject CameraPerspective::toJson()
{
    QJsonObject obj;
    obj.insert("fovy",fovy);
    obj.insert("aspect",aspect);
    obj.insert("zNear",zNear);
    obj.insert("zFar",zFar);
    return obj;
}

void CameraPerspective::fromJson(QJsonObject obj)
{
    fovy = obj["fovy"].toDouble();
    aspect = obj["aspect"].toDouble();
    zNear = obj["zNear"].toDouble();
    zFar = obj["zFar"].toDouble();
}

QJsonObject CameraOrthogonal::toJson()
{
    QJsonObject obj;
    obj.insert("left",left);
    obj.insert("right",right);
    obj.insert("top",top);
    obj.insert("bottom",bottom);
    obj.insert("zNear",zNear);
    obj.insert("zFar",zFar);
    return obj;
}

void CameraOrthogonal::fromJson(QJsonObject obj)
{
    left = obj["left"].toDouble();
    right = obj["right"].toDouble();
    top = obj["top"].toDouble();
    bottom = obj["bottom"].toDouble();
    zNear = obj["zNear"].toDouble();
    zFar = obj["zFar"].toDouble();
}

QJsonObject CameraModel::toJson()
{
    QJsonObject obj;
    obj.insert("mode",mode);
    obj.insert("perspective",perspective.toJson());
    obj.insert("orthogonal",orthogonal.toJson());
    return obj;
}

void CameraModel::fromJson(QJsonObject obj)
{
    mode = obj["mode"].toString();
    perspective.fromJson(obj["perspective"].toObject());
    orthogonal.fromJson(obj["orthogonal"].toObject());
}

QJsonObject ParticleValueStable::toJson()
{
    QJsonObject obj;
    obj.insert("base",base);
    obj.insert("rand",rand);
    return obj;
}

void ParticleValueStable::fromJson(QJsonObject obj)
{
    base = obj["base"].toDouble();
    rand = obj["rand"].toDouble();
}

QJsonObject ParticleValueLinear::toJson()
{
    QJsonObject obj;
    obj.insert("base",base);
    obj.insert("baseRand",baseRand);
    obj.insert("acceleration",acceleration);
    obj.insert("accelerationRand",accelerationRand);
    return obj;
}

void ParticleValueLinear::fromJson(QJsonObject obj)
{
    base = obj["base"].toDouble();
    baseRand = obj["baseRand"].toDouble();
    acceleration = obj["acceleration"].toDouble();
    accelerationRand = obj["accelerationRand"].toDouble();
}

QJsonObject ParticleValueCircular::toJson()
{
    QJsonObject obj;
    obj.insert("base",base);
    obj.insert("baseRand",baseRand);
    obj.insert("radius",radius);
    obj.insert("radiusRand",radiusRand);
    obj.insert("rate",rate);
    obj.insert("rateRand",rateRand);
    return obj;
}

void ParticleValueCircular::fromJson(QJsonObject obj)
{
    base = obj["base"].toDouble();
    baseRand = obj["baseRand"].toDouble();
    radius = obj["radius"].toDouble();
    radiusRand = obj["radiusRand"].toDouble();
    rate = obj["rate"].toDouble();
    rateRand = obj["rateRand"].toDouble();
}

QJsonObject ParticleValue::toJson()
{
    QJsonObject obj;
    obj.insert("mode",mode);
    obj.insert("stable",stable.toJson());
    obj.insert("linear",linear.toJson());
    obj.insert("circular",circular.toJson());
    return obj;
}

void ParticleValue::fromJson(QJsonObject obj)
{
    mode = obj["mode"].toString();
    stable.fromJson(obj["stable"].toObject());
    linear.fromJson(obj["linear"].toObject());
    circular.fromJson(obj["circular"].toObject());
}

QJsonObject Path1DLinear::toJson()
{
    QJsonObject obj;
    obj.insert("start_point",start_point.toJson());
    obj.insert("speed",speed.toJson());
    obj.insert("acceleration",acceleration.toJson());
    return obj;
}

void Path1DLinear::fromJson(QJsonObject obj)
{
    start_point.fromJson(obj["start_point"].toObject());
    speed.fromJson(obj["speed"].toObject());
    acceleration.fromJson(obj["acceleration"].toObject());
}

QJsonObject Path1D::toJson()
{
    QJsonObject obj;
    obj.insert("mode",mode);
    obj.insert("linear",linear.toJson());
    obj.insert("custom",custom);
    return obj;
}

void Path1D::fromJson(QJsonObject obj)
{
    QJsonValue value;
    mode = obj["mode"].toString();
    linear.fromJson(obj["linear"].toObject());
    custom = obj["custom"].toString();
}

QJsonObject Path2DCircular::toJson()
{
    QJsonObject obj;
    obj.insert("circles",circles.toJson());
    obj.insert("radius",radius.toJson());
    return obj;
}

void Path2DCircular::fromJson(QJsonObject obj)
{
    circles.fromJson(obj["circles"].toObject());
    radius.fromJson(obj["radius"].toObject());
}

QJsonObject Path2D::toJson()
{
    QJsonObject obj;
    obj.insert("mode",mode);
    obj.insert("circular",circular.toJson());
    obj.insert("custom",custom);
    return obj;
}

void Path2D::fromJson(QJsonObject obj)
{
    mode = obj["mode"].toString();
    circular.fromJson(obj["circular"].toObject());
    custom = obj["custom"].toString();
}

QJsonObject Path3DSpherical::toJson()
{
    QJsonObject obj;
    obj.insert("radius",radius.toJson());
    obj.insert("polar_angle",polar_angle.toJson());
    obj.insert("azimuth_angle",azimuth_angle.toJson());
    return obj;
}

void Path3DSpherical::fromJson(QJsonObject obj)
{
    radius.fromJson(obj["radius"].toObject());
    polar_angle.fromJson(obj["polar_angle"].toObject());
    azimuth_angle.fromJson(obj["azimuth_angle"].toObject());
}

QJsonObject Path3D::toJson()
{
    QJsonObject obj;
    obj.insert("mode",mode);
    obj.insert("spherical",spherical.toJson());
    obj.insert("custom",custom);
    return obj;
}

void Path3D::fromJson(QJsonObject obj)
{
    mode = obj["mode"].toString();
    spherical.fromJson(obj["spherical"].toObject());
    custom = obj["custom"].toString();
}

QJsonObject PathModel::toJson()
{
    QJsonObject obj;
    obj.insert("x",x);
    obj.insert("y",y);
    obj.insert("z",z);
    obj.insert("path1D",path1D.toJson());
    obj.insert("path2D",path2D.toJson());
    obj.insert("path3D",path3D.toJson());

    return obj;
}

void PathModel::fromJson(QJsonObject obj)
{
    x = obj["x"].toBool();
    y = obj["y"].toBool();
    z = obj["z"].toBool();
    path1D.fromJson(obj["path1D"].toObject());
    path2D.fromJson(obj["path2D"].toObject());
    path3D.fromJson(obj["path3D"].toObject());
}

QJsonObject ParticleAttribute::toJson()
{
    QJsonObject obj;
    obj.insert("width",width.toJson());
    obj.insert("height",height.toJson());
    obj.insert("rotation",rotation.toJson());

    return obj;
}

void ParticleAttribute::fromJson(QJsonObject obj)
{
    width.fromJson(obj["width"].toObject());
    height.fromJson(obj["height"].toObject());
    rotation.fromJson(obj["rotation"].toObject());
}

QJsonObject ParticleDisplay::toJson()
{
    QJsonObject obj;
    obj.insert("image_name",image_name);
    obj.insert("mode",mode);
    obj.insert("count",count);
    obj.insert("width",width);
    obj.insert("height",height);
    obj.insert("fps",fps);
    return obj;
}

void ParticleDisplay::fromJson(QJsonObject obj)
{
    image_name = obj["image_name"].toString();
    mode = obj["mode"].toString();
    count = obj["count"].toInt();
    width = obj["width"].toInt();
    height = obj["height"].toInt();
    fps = obj["fps"].toDouble();
}

QJsonObject ParticleParam::toJson()
{
    QJsonObject obj;
    obj.insert("name",name);
    QJsonArray array;
    for (int i=0;i<(int)pathModel.size();i++)
    {
        array.insert(i,pathModel[i].toJson());
    }
    obj.insert("path",array);
    obj.insert("attribute",attribute.toJson());
    obj.insert("display",display.toJson());

    return obj;
}

void ParticleParam::fromJson(QJsonObject obj)
{
    name = obj["name"].toString();
    QJsonArray array = obj["path"].toArray();
    pathModel.clear();
    for (int i=0;i<array.size();i++)
    {
        PathModel path;
        path.fromJson(array[i].toObject());
        pathModel.push_back(path);
    }
    attribute.fromJson(obj["attribute"].toObject());
    display.fromJson(obj["display"].toObject());
}

QJsonObject ParticleModel::toJson()
{
    QJsonObject obj;
    obj.insert("camera",cameraModel.toJson());
    QJsonArray array;
    for (int i=0;i<(int)particles.size();i++)
    {
        array.insert(i,particles[i].toJson());
    }
    obj.insert("particles",array);
    return obj;
}

void ParticleModel::fromJson(QJsonObject obj)
{
    cameraModel.fromJson(obj["camera"].toObject());
    particles.clear();
    QJsonArray array = obj["particles"].toArray();
    for (int i=0;i<array.size();i++)
    {
        ParticleParam param;
        param.fromJson(array[i].toObject());
        particles.push_back(param);
    }
}

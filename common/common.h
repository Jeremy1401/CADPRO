#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <sstream>
#include <QColor>
#include <QPainter>
#include <QPointF>
#include <QLineF>
#include <qmath.h>

enum crossType{
    none = 0,
    normal = 1,
    upright = 2
};
void str2int(int &int_temp,const std::string &string_temp);
void str2float(float &float_temp,const std::string &string_temp);
void str2double(double &double_temp,const std::string &string_temp);
QColor intToColor(const int rgb, bool a=false); // 将int转华为color
QPointF transformY(QPointF p);  // 转变纵坐标轴
QPointF transformRotate(QPointF o, qreal r, qreal angle); // 返回旋转某一点之后的坐标
QPointF transformRotate(QPointF o, QPointF p, qreal angle);  // 返回一点绕另一点旋转的坐标
//QRectF getLineBoundingRect(QLineF line);
qreal getDistance(QPointF p1, QPointF p2);  // 获取亮点之间的距离
void getRectBoundValue(const QRectF rect, qreal &minX, qreal &minY, qreal &maxX, qreal &maxY);  // 获取矩形边界
bool boundingRectSeperate(const QRectF rect1, const QRectF rect2);  // 判断两矩形是分离
void drawCrossPoint(QPainter* painter, QPointF point, int offset, crossType type);  // 画交叉点
void drawNodePoint(QPainter* painter, QPointF point, int radius);  // 画点
void drawRectPoint(QPainter* painter, QPointF point, int length);  // 画矩形点
void drawLineWithArrow(QPainter *painter, QLineF line, int offset); // 画点箭头的直线
#endif // COMMON_H

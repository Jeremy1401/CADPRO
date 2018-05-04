#ifndef COLLISIONDECTECT_H
#define COLLISIONDECTECT_H

#include <QVector>
#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QMap>

/***
 * @brief The CollisionDectect class
 * 分离轴碰撞检测-目前支持 点/圆形/线段/凸多边形/凹多边形
 * (文件中有凸多边形分解算法)
 * 碰撞可采用(quadtree四叉树管理场景内的形状)
 *
 * 当传入图形为圆时，点集为2，
 * 参数结构为[Point(圆心x,圆心y),Point(半径,半径)]
 */
class ConcavePolygonDecompose;

class CollisionDectect
{
public:
    /**
     * @brief The ItemType enum
     *
     */
    enum ItemType{
        Point,
        Line,
        Circle,
        ConvexPoly,
        ConcavePoly
    };

    struct CircleInfo
    {
        CircleInfo() :
            center(QPointF()),
            radius(0)
        {}

        CircleInfo(QPointF c, qreal r) :
            center(c),
            radius(r)
        {}

        QPointF center;
        qreal radius;
    };

    CollisionDectect(QVector<QPointF> pList1, QVector<QPointF> pList2, bool isCircle1=false, bool isCircle2=false);

    ItemType getItemType(QVector<QPointF> pList);  // 返回除圆之外的图形类型
    QVector<qreal> getBoundingRect(QVector<QPointF> pList);  // 获取包络矩形
    CircleInfo getBoundingCircle(QVector<QPointF> pList);  // 获取包络矩形
    qreal dotProduct(QPointF v1, QPointF v2);  // 点乘
    bool circleHit(QVector<QPointF> pList1, QVector<QPointF> pList2);  // 圆形之间的碰撞检测
    bool boundHit(QVector<qreal> bound1, QVector<qreal> bound2);  // 包络矩形碰撞检测
    QPointF normalize(QPointF v);  // 标准化向量
    QPointF perpendicular(QPointF v);  // 法线向量
    QVector<QPointF> getSeparatingAxis(QVector<QPointF> pList, QVector<QPointF> separatingAxis);  // 根据多边形计算分离轴
    bool overlap(QPointF s1, QPointF s2);  // 判断是否有交叠,x,y分别代表最小值与最大值
    QPointF getPolygonProjection(QVector<QPointF> pList, QPointF axis);  // 获取多边形在投影轴上最小与最大点
    QPointF getCircleProjection(QVector<QPointF> pList, QPointF axis);  // 获取多边形在投影轴上最小与最大点

    bool collision();  // 返回碰撞检测结果
    bool convexPolygonCollision(QVector<QPointF> pList1, QVector<QPointF> pList2, bool isCircle1=false, bool isCircle2=false);  // 返回凸多边形碰撞检测结果

private:
    QVector<QPointF> pList1;  // 第一个图形的点集
    QVector<QPointF> pList2;  // 第二个图形的点集
    bool isCircle1;  // 第一个图形是否为圆
    bool isCircle2;  // 第二个图形是否为圆
};


/**
 * @brief The ConcavePolygonDecompose class
 * 凹多边形拆分
 */
class ConcavePolygon
{
public:
    enum PolyDirection{
        Clockwise=-1,
        InALine,
        AnticlockWise
    };

    enum PointRelationToLine{
        LeftLine,
        RightLine,
        InLine
    };

    struct IntersectionPoint{
        IntersectionPoint() :
            index(-1),
            intersection(QPointF(0,0))
        {

        }
        int index;
        QPointF intersection;
    };

    ConcavePolygon(QVector<QPointF> list);
    bool isConcavePolygon(QVector<QPointF> pList);  // 判断是否为凹多边形
    QMap<int, QVector<QPointF>> onSeparateConcavePoly(QVector<QPointF> pList);  // 分割凹多边形
    int getNextConcaveIndex(QVector<QPointF> pList, int index = 0);  // 获取下一个凹点
    QVector<int> getAllConcaveIndex(QVector<QPointF> pList, int startIndex = 0);  // 获取所有凹点列表
    void _separateConcavePoly(QVector<QPointF> pList, QMap<int, QVector<QPointF>> &map, int startIndex);  // 分割多边形
    bool splitPolyByIntersection(QVector<QPointF> pList, int concaveIndex, IntersectionPoint intersectionPoint,
                            QVector<QPointF> &pLeftList, QVector<QPointF> &pRightList);  // 根据分割点分割
    bool getSplitPointByVertexRegion(QVector<QPointF> pList, int concaveIndex, IntersectionPoint &intersectionPoint);  //
    PolyDirection getMutiPtClockDir(QPointF p1, QPointF p2, QPointF p3);  // 获取三点的方向
    PolyDirection getMutiPtClockDirByIndex(QVector<QPointF> pList, int index);  // 获取多边形某一点的方向
    bool isAntiClockDir(QVector<QPointF> pList);  // 是否为逆时针
    void conversPoly(QVector<QPointF> &pList);  // 逆序重排
    bool isVectorInsection(QPointF p1, QPointF d1, QPointF p2, QPointF d2);  // 是否交叉
    bool isVisiblePtToConcave(QVector<QPointF> pList, int index1, int index2);  // 是否可见
    void visibleRegionPtSet(QVector<QPointF> pList, int concaveIndex, QVector<int> region, QVector<int> &region1);  // 设置可见区域点
    void setSplitByRegion(QVector<QPointF> pList, QVector<int> region, QVector<int> &setA, QVector<int> &setB);  // 区域分割.A凸点集合;B凹点集合
    void normalizeVector(QPointF &v);  // 标准化向量
    qreal dotVector(QPointF v1, QPointF v2);  // 点乘
    int getBestIntersectionPt(QVector<QPointF> pList, int concaveIndex, QVector<int> ptSets);  // 获取最优交点
    bool getCrossByRadialAndSegment(QPointF p1, QPointF d1, QPointF p2, QPointF d2, QPointF &crossPoint);  // 径向和分段交叉
    PointRelationToLine GetPosRelationToLine(QPointF sPoint, QPointF ePoint, QPointF p);  // 判断点与直线的关系
private:
    QVector<QPointF> pList;  // 凹多边形的点集
};

#endif // COLLISIONDECTECT_H

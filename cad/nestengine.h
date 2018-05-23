#ifndef NESTENGINE_H
#define NESTENGINE_H

#include <QObject>
#include <QFlags>
#include <QVector>
#include <piece.h>
#include <sheet.h>
//#include "quadtreenode.h"

class NestEngine : public QObject
{
    Q_OBJECT
public:
    /**
     * 排版零件
     * @brief The NestPiece struct
     */
    struct NestPiece
    {
        NestPiece() :
            index(-1),
            typeID(-1),
            sheetID(-1),
            position(QPointF(-INT_MAX, -INT_MAX)),
            referenceLine(QLineF()),
            alpha(0),
            nested(false)
        {
        }

        NestPiece(int i, int t) :
            index(i),
            typeID(t),
            sheetID(-1),
            position(QPointF(-INT_MAX, -INT_MAX)),
            referenceLine(QLineF()),
            alpha(0),
            nested(false)
        {}

        int index;  // 在整个零件列表中的序号
        int typeID;  // 零件类型ID
        int sheetID;  // 材料ID
        QPointF position;  // 参考点的位置
        QLineF referenceLine;  // 参考线
        qreal alpha;  // 旋转角度
        bool nested;  // 是否已排
    };

    /**
     * @brief The SameTypePiece struct
     * 同型体零件，指零件外形一致，只是大小不一
     */
    struct SameTypePiece
    {
        SameTypePiece() :
            typeID(-1),
            pieceIDList(QVector<int>())
        {}

        SameTypePiece(int id, QVector<int> idList) :
            typeID(id),
            pieceIDList(idList)
        {}

        int typeID;  // 同型体ID
        QVector<int> pieceIDList;  // 零件ID列表
    };

    /**
     * @brief The IDRange struct
     * 零件组成排版零件后在列表中的序号范围
     */
    struct PieceIndexRange
    {
        PieceIndexRange() :
            pieceID(-1),
            minIndex(-1),
            maxIndex(-1)
        {

        }
        PieceIndexRange(int id, int min, int max) :
            pieceID(id),
            minIndex(min),
            maxIndex(max)
        {

        }
        int pieceID;  // 零件id
        int minIndex;  // 最小序号
        int maxIndex;  // 最大序号
    };

    enum NestEngineType{
        PackPointNest,
        MinRectNest
    };

    /**
     * @brief The NestType enum
     * 排版方式
     */
    enum NestType{
        SigleRow,  // 单排
        DoubleRow,  // 双排
        PairWiseSigleRow,  // 对头单排
        PairWiseDoubleRow,  // 对头双排
    };

    /**
     * @brief The NestMixingType enum
     * 排版混合类型
     */
    enum NestMixingType{
        NoMixing = 0x00,  // 不混合
        TailPieceMixing = 0x01,  // 行内尾只混合
        TailLineMixing = 0x02,  // 尾行优化，允许任意角度
        SameTypeSizeMixing = 0x04,  // 同型体内尺码混合
        AllMixing = TailPieceMixing | TailLineMixing | SameTypeSizeMixing,  // 全混合
    };

    Q_DECLARE_FLAGS(NestMixingTypes, NestMixingType)
    Q_FLAG(NestMixingTypes)

    /**
     * @brief The NestAdaptiveSpacing enum
     * 排版自适应间距
     */
    enum NestAdaptiveSpacingType{
        NoAdaptiveSpacing = 0x00,  // 无自适应间距
        HorizontalAdaptiveSpacing = 0x01,  // 横向自适应间距
        VerticalAdaptiveSpacing = 0x02,  // 纵向自适应间距
        AllAdaptiveSpacing = HorizontalAdaptiveSpacing | VerticalAdaptiveSpacing,  // 全向自适应间距
    };
    Q_DECLARE_FLAGS(NestAdaptiveSpacingTypes, NestAdaptiveSpacingType)
    Q_FLAG(NestAdaptiveSpacingTypes)

    enum NestOrientation{
        HorizontalNest = 0x01,  // 横向排版
        VerticalNest = 0x02,  // 纵向排版
        AllOrientationNest = HorizontalNest | VerticalNest,  // 全向排版
    };
    Q_DECLARE_FLAGS(NestOrientations, NestOrientation)
    Q_FLAG(NestOrientations)

    /**
     * @brief The NestEngineStrategy enum
     * 排版策略
     */
    enum NestEngineStrategy{
        NoStrategy = 0x00,  // 无策略
        LeftRightTurn = 0x01, // 左右交替
        SizeDown = 0x02,  // 尺码由大到小
        ReferenceLine = 0x04, // 参考线排版
        AllStrategys = LeftRightTurn | SizeDown,  // 使用全部策略
        Reserved = 0xff,  // 预留
    };
    Q_DECLARE_FLAGS(NestEngineStrategys, NestEngineStrategy)
    Q_FLAG(NestEngineStrategys)

    explicit NestEngine();
    explicit NestEngine(const QVector<Piece> pieceList, QVector<Sheet> sheetList);
    explicit NestEngine(const QVector<Piece> pieceList, QVector<Sheet> sheetList, QVector<SameTypePiece> sameTypePieceList);
    ~NestEngine();

    void setAutoRepeatLastSheet(bool flag);  // 设置是否自动重复使用最后一张材料
    bool getAutoRepeatLastSheet();  // 获取是否自动重复使用最后一张材料

    void setCompactStep(qreal step);  // 设置靠接步长
    qreal getCompactStep();  // 获取靠接步长

    void setCompactAccuracy(qreal accuracy);  // 设置靠接精度
    qreal getCompactAccuracy();  // 获取靠接精度

    void setNestEngineType(NestEngineType type);  // 设置排版引擎类型
    NestEngineType getNestEngineType();  // 获取排版引擎类型

    void setNestType(NestType type);  // 设置排版方式
    NestType getNestType();  // 获取排版方式

    void setNestMixingTypes(NestMixingTypes types);  // 设置混合方式
    NestMixingTypes getNestMixingTypes();  // 获取混合方式

    void setNestAdaptiveSpacingTypes(NestAdaptiveSpacingTypes types);  // 设置自适应间距方式
    NestAdaptiveSpacingTypes getNestAdaptiveSpacingTypes();  // 获取自适应间距方式

    void setNestOrientations(NestEngine::NestOrientations oriens);  // 设置排版方向
    NestOrientations getNestOrientations();  // 获取排版方向

    void setNestEngineStrategys(NestEngineStrategys strategys);  // 设置排版引擎策略
    NestEngineStrategys getNestEngineStrategys();  // 获取排版策略

    void setCutStep(qreal step);  // 设置一刀切切割长度
    qreal getCutStep();  // 获取一刀切切割长度

    void setMaxRotateAngle(int angle);  // 设置最大旋转角度
    qreal getMaxRotateAngle();  // 获取最大旋转角度

    void setMinHeightOpt(bool flag);  // 设置是否自动重复使用最后一张材料
    bool getMinHeightOpt();  // 获取是否自动重复使用最后一张材料

    QVector<Piece> getSortedPieceListByArea(QVector<Piece> pieceList, QMap<int, int> &transformMap);  // 按面积将多边形列表排序, 并可得到映射关系
    void initQuadTreeMap(int sheetID);  // 初始化四叉树Map
    void initNestPieceList();  // 初始化排版零件列表，默认按面积降序排序
    void initsameTypeNestPieceIndexListMap();  // 初始化同型体排版零件列表Map
    void packAlg();  // 排版算法
    virtual void packPieces(QVector<int> indexList) = 0;  //  排版算法
    virtual bool packOnePiece(Piece piece, NestEngine::NestPiece &nestPiece) = 0;  // 排放单个零件
    virtual bool packOnePieceOnSheet(Piece piece, int sheetID, NestEngine::NestPiece &nestPiece) = 0;  // 在给定材料上排放单个零件
    virtual bool compact(int sheetID, NestPiece &nestPiece) = 0;  // 紧凑算法
    virtual bool collidesWithOtherPieces(int sheetID, Piece piece) = 0;  // 判断该零件是否与其他零件重叠

//protected:
    QVector<Piece> pieceList;  // 零件列表
    QVector<Sheet> sheetList;  // 材料列表
    QVector<SameTypePiece> sameTypePieceList;  // 同型体零件

    QVector<NestPiece> nestPieceList;  // 排版零件列表
    QMap<int, PieceIndexRange> nestPieceIndexRangeMap;  // 排版零件的index范围 Map<零件id，排样零件序号范围>
    QMap<int, QVector<int>> sameTypeNestPieceIndexListMap;  // 同型体排版零件index列表 Map<同型体id, 零件序号列表>
    QVector<int> nestedPieceIndexlist;  // 已排零件Index列表
    QVector<int> unnestedPieceIndexlist;  // 未排零件Index列表
    QMap<int, QVector<int>> nestSheetPieceMap;  // 排样材料-零件索引 Map<材料id, 零件序号列表>
    QMap<int, int> pieceMaxPackPointMap;  // 记录零件-最大排样点序号 Map<零件id, 排样点id>
    QMap<int, QuadTreeNode<Object>*> quadTreeMap;  // 四叉树 Map<材料id，四叉树>
    bool autoRepeatLastSheet;  // 自动重复使用最后一张材料
    qreal compactStep;  // 靠接步长，单位为mm
    qreal compactAccuracy;  // 靠接精度，单位为mm

    NestEngineType nestEngineType;  // 排版引擎类型
    NestType nestType;  // 排版方式
    NestMixingTypes mixingTyes;  // 混合方式
    NestAdaptiveSpacingTypes adaptiveSpacingTypes;  // 自适应间距类型
    NestOrientations orientations;  // 排版方向
    NestEngineStrategys nestEngineStrategys;  // 排版策略
    bool oneKnifeCut;  // 采用一刀切
    qreal cutStep;  // 切割步长
    bool rotatable;  // 可旋转
    int maxRotateAngle;  // 最大转动角度
    bool minHeightOpt;  // 最小高度优化
    int collisionCount;  // 碰撞检测次数
};

Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestMixingTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestAdaptiveSpacingTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestOrientations)
Q_DECLARE_OPERATORS_FOR_FLAGS(NestEngine::NestEngineStrategys)
#endif // NESTENGINE_H

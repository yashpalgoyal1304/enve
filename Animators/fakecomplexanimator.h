#ifndef FAKECOMPLEXANIMATOR_H
#define FAKECOMPLEXANIMATOR_H
#include "complexanimator.h"

class FakeComplexAnimator : public ComplexAnimator {
public:
    FakeComplexAnimator(Property *target);
    Property *getTarget();

    void prp_drawKeys(QPainter *p,
                      const qreal &pixelsPerFrame,
                      const qreal &drawY,
                      const int &startFrame,
                      const int &endFrame);

    Key *prp_getKeyAtPos(const qreal &relX,
                         const int &minViewedFrame,
                         const qreal &pixelsPerFrame);

    void prp_getKeysInRect(const QRectF &selectionRect,
                           const qreal &pixelsPerFrame,
                           QList<Key *> *keysList);

    bool SWT_isFakeComplexAnimator();
private:
    Property *mTarget = NULL;
};

#endif // FAKECOMPLEXANIMATOR_H

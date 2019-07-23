#ifndef SHADOWEFFECT_H
#define SHADOWEFFECT_H
#include "pixmapeffect.h"
#include "Animators/qpointfanimator.h"
#include "Animators/coloranimator.h"
#include "Properties/boolproperty.h"

struct ShadowEffectRenderData : public PixmapEffectRenderData {
    friend class StdSelfRef;
protected:
    ShadowEffectRenderData() {}
public:
    void applyEffectsSk(const SkBitmap &bitmap,
                        const qreal scale);

    qreal fBlurRadius;
    qreal fOpacity;
    QColor fColor;
    QPointF fTranslation;
};

class ShadowEffect : public PixmapEffect {
    friend class SelfRef;
public:
    QMarginsF getMarginAtRelFrame(const qreal relFrame);

    stdsptr<PixmapEffectRenderData> getPixmapEffectRenderDataForRelFrameF(
            const qreal relFrame, BoundingBoxRenderData*);
protected:
    ShadowEffect();
private:
//    QrealAnimator mScale;
    qsptr<QrealAnimator> mBlurRadius;
    qsptr<QrealAnimator> mOpacity;
    qsptr<ColorAnimator> mColor;
    qsptr<QPointFAnimator> mTranslation;
};

#endif // SHADOWEFFECT_H
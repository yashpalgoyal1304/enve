#include "gpueffectanimators.h"
#include "GPUEffects/gpurastereffect.h"
#include "Boxes/boundingbox.h"
#include <QDebug>

GPUEffectAnimators::GPUEffectAnimators(BoundingBox *parentBox) :
    ComplexAnimator("gpu effects"), mParentBox_k(parentBox) {
    SWT_setEnabled(false);
    SWT_setVisible(false);
}

void GPUEffectAnimators::addEffect(const qsptr<GPURasterEffect>& effect) {
    ca_addChildAnimator(effect);
    effect->setParentEffectAnimators(this);

    prp_afterWholeInfluenceRangeChanged();
    SWT_setEnabled(true);
    SWT_setVisible(true);
}

qreal GPUEffectAnimators::getEffectsMargin() const {
    qreal newMargin = 2.;
    for(const auto& effect : ca_mChildAnimators) {
        auto pixmapEffect = GetAsPtr(effect.get(), GPURasterEffect);
        if(pixmapEffect->isVisible()) {
            newMargin += pixmapEffect->getMargin();
        }
    }
    return newMargin;
}

qreal GPUEffectAnimators::getEffectsMarginAtRelFrame(const int &relFrame) const {
    qreal newMargin = 0.;
    for(const auto& effect : ca_mChildAnimators) {
        auto pixmapEffect = GetAsPtr(effect.get(), GPURasterEffect);
        if(pixmapEffect->isVisible()) {
            newMargin += pixmapEffect->getMarginAtRelFrame(relFrame);
        }
    }
    return newMargin;
}

qreal GPUEffectAnimators::getEffectsMarginAtRelFrameF(const qreal &relFrame) const {
    qreal newMargin = 0.;
    for(const auto& effect : ca_mChildAnimators) {
        auto pixmapEffect = GetAsPtr(effect.get(), GPURasterEffect);
        if(pixmapEffect->isVisible()) {
            newMargin += pixmapEffect->getMarginAtRelFrame(qRound(relFrame));
        }
    }
    return newMargin;
}

void GPUEffectAnimators::addEffectRenderDataToListF(
        const qreal &relFrame,
        BoundingBoxRenderData * const data) {
    for(const auto& effect : ca_mChildAnimators) {
        auto pixmapEffect = GetAsPtr(effect, GPURasterEffect);
        if(pixmapEffect->isVisible()) {
            const auto effectRenderData =
                    pixmapEffect->getGPURasterEffectCaller(relFrame);
            if(!effectRenderData) continue;
            data->fGPUEffects.append(effectRenderData);
        }
    }
}

void GPUEffectAnimators::updateIfUsesProgram(
        const GPURasterEffectProgram * const program) {
    for(const auto& effect : ca_mChildAnimators) {
        const auto pixmapEffect = GetAsPtr(effect.get(), GPURasterEffect);
        pixmapEffect->updateIfUsesProgram(program);
    }
}

bool GPUEffectAnimators::hasEffects() {
    return !ca_mChildAnimators.isEmpty();
}

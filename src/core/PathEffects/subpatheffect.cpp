// enve - 2D animations software
// Copyright (C) 2016-2019 Maurycy Liebner

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "subpatheffect.h"

#include "pointhelpers.h"
#include "Animators/qrealanimator.h"
#include "Properties/boolproperty.h"

SubPathEffect::SubPathEffect() :
    PathEffect("sub-path effect", PathEffectType::SUB) {
    mMin = enve::make_shared<QrealAnimator>("min length");
    mMin->setValueRange(-999, 999);
    mMin->setCurrentBaseValue(0);

    mMax = enve::make_shared<QrealAnimator>("max length");
    mMax->setValueRange(-999, 999);
    mMax->setCurrentBaseValue(100);

    ca_addChild(mMin);
    ca_addChild(mMax);
}

class SubPathEffectCaller : public PathEffectCaller {
public:
    SubPathEffectCaller(const qreal minFrac, const qreal maxFrac) :
        mMinFrac(minFrac), mMaxFrac(maxFrac) {}

    void apply(SkPath& path);
private:
    const qreal mMinFrac;
    const qreal mMaxFrac;
};

void SubPathEffectCaller::apply(SkPath &path) {
    const bool pathWise = true;

    if(isZero6Dec(mMaxFrac - 1) && isZero6Dec(mMinFrac)) return;

    if(isZero6Dec(mMaxFrac - mMinFrac)) {
        path.reset();
        return;
    }

    auto paths = CubicList::sMakeFromSkPath(path);
    path.reset();

    if(pathWise) {
        for(auto& iPath : paths) {
            path.addPath(iPath.getFragmentUnbound(mMinFrac, mMaxFrac).toSkPath());
        }
        return;
    } // else

    qreal totalLength = 0;
    for(auto& iPath : paths) {
        totalLength += iPath.getTotalLength();
    }
    const qreal minLength = mMinFrac*totalLength;
    const qreal maxLength = mMaxFrac*totalLength;

    qreal currLen = qFloor(minLength/totalLength)*totalLength;
    bool first = true;
    while(currLen < maxLength) {
        for(auto& iPath : paths) {
            const qreal pathLen = iPath.getTotalLength();

            const qreal minRemLen = minLength - currLen;
            const qreal maxRemLen = maxLength - currLen;
            currLen += pathLen;

            if(first) {
                if(currLen > minLength) {
                    first = false;
                    qreal maxFrag;
                    const bool last = currLen + pathLen > maxLength;
                    if(last) maxFrag = maxRemLen/pathLen;
                    else maxFrag = 1;
                    path.addPath(iPath.getFragment(minRemLen/pathLen, maxFrag).toSkPath());
                    if(last) break;
                }
            } else {
                if(currLen > maxLength) {
                    path.addPath(iPath.getFragment(0, maxRemLen/pathLen).toSkPath());
                    break;
                } else {
                    path.addPath(iPath.toSkPath());
                }
            }
        }
    }
}

stdsptr<PathEffectCaller> SubPathEffect::getEffectCaller(const qreal relFrame) const {
    const qreal minFrac = mMin->getEffectiveValue(relFrame)/100;
    const qreal maxFrac = mMax->getEffectiveValue(relFrame)/100;
    return enve::make_shared<SubPathEffectCaller>(minFrac, maxFrac);
}

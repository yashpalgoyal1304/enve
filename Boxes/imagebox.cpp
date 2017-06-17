#include "Boxes/imagebox.h"

ImageBox::ImageBox(BoxesGroup *parent, QString filePath) :
    BoundingBox(parent, TYPE_IMAGE) {
    mImageFilePath = filePath;

    setName("Image");

    reloadPixmap();
}

void ImageBox::updateRelBoundingRect() {
    mRelBoundingRect = QRectF(0., 0.,
                              mImageSk->width(), mImageSk->height());
    mRelBoundingRectSk = QRectFToSkRect(mRelBoundingRect);

    BoundingBox::updateRelBoundingRect();
}

void ImageBox::makeDuplicate(Property *targetBox) {
    BoundingBox::makeDuplicate(targetBox);
    ImageBox *imgTarget = (ImageBox*)targetBox;
    imgTarget->setFilePath(mImageFilePath);
}

BoundingBox *ImageBox::createNewDuplicate(BoxesGroup *parent) {
    return new ImageBox(parent);
}

void ImageBox::drawSk(SkCanvas *canvas) {
    SkPaint paint;
    //paint.setFilterQuality(kHigh_SkFilterQuality);
    canvas->drawImage(mImageSk, 0, 0, &paint);
}

void ImageBox::reloadPixmap() {
    if(mImageFilePath.isEmpty()) {
    } else {
        sk_sp<SkData> data = SkData::MakeFromFileName(
                    mImageFilePath.toLocal8Bit().data());
        mImageSk = SkImage::MakeFromEncoded(data);
    }

    scheduleSoftUpdate();
}

void ImageBox::setFilePath(QString path) {
    mImageFilePath = path;
    reloadPixmap();
}

void ImageBox::addActionsToMenu(QMenu *menu) {
    menu->addAction("Reload");
    menu->addAction("Set Source File...");
}
#include <QFileDialog>
#include "mainwindow.h"
bool ImageBox::handleSelectedCanvasAction(QAction *selectedAction) {
    if(selectedAction->text() == "Set Source File...") {
        MainWindow::getInstance()->disableEventFilter();
        QString importPath = QFileDialog::getOpenFileName(
                                                MainWindow::getInstance(),
                                                "Change Source", "",
                                                "Image Files (*.png *.jpg)");
        MainWindow::getInstance()->enableEventFilter();
        if(!importPath.isEmpty()) {
            setFilePath(importPath);
        }
        return true;
    } else if(selectedAction->text() == "Reload") {
        reloadPixmap();
        return true;
    }
    return false;
}

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <set>

#include "smartPointers/sharedpointerdefs.h"
#include "singlewidgettarget.h"
#include "paintsettings.h"

class Gradient;
class FileCacheHandler;
class Canvas;

class Document : public SingleWidgetTarget {
    typedef stdsptr<FileCacheHandler> FileHandler;
    struct FileCompare {
        bool operator()(const FileHandler& f1, const FileHandler& f2);
    };
public:
    QString fEvFile;

    // bookmarked
    QList<QColor> fColors;
    QList<SimpleBrushWrapper*> fBrushes;

    // all in document
    QList<qsptr<Gradient>> fGradients;

    FillSettings fLastFill;
    StrokeSettings fLastStroke;
    SimpleBrushWrapper* fLastBrush = nullptr;

    std::set<FileHandler, FileCompare> fFiles;

    QList<qsptr<Canvas>> fScenes;
    qptr<Canvas> fTimelineScene;
    qptr<Canvas> fLastActiveScene;

    Canvas * createNewCanvas();

    void write(QIODevice * const dst) const;
    void read(QIODevice * const src) const;

    void SWT_setupAbstraction(SWT_Abstraction * const abstraction,
                              const UpdateFuncs &updateFuncs,
                              const int visiblePartWidgetId);
};

#endif // DOCUMENT_H
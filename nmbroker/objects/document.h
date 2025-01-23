#ifndef NUTMEG_DOCUMENT_H
#define NUTMEG_DOCUMENT_H

#include <QFile>

#include "object.h"

namespace Nutmeg
{

class Document : public Nutmeg::Object
{
  public:
    explicit Document(QObject *parent = nullptr);
    explicit Document(Key id, QObject *parent = nullptr);
    explicit Document(String Title, QObject *parent = nullptr);

    // Properties
    Property(getId, slotSetId) Key DocumentId;
    Property(getFullyQualifiedPath, slotSetFullyQualifiedPath) String FullyQualifiedPath;
    Property(getURL, slotSetURL) String URL;
    Property(getFilename, slotSetFilename) String Filename;
    Property(getExtension, slotSetExtension) String Extension;
    Property(getTitle, slotSetTitle) String Title;

    // Getters
    virtual Key getId(void) override { return mDat.DocumentId; }

    String getFullyQualifiedPath(void) { return mDat.FullyQualifiedPath; }

    String getURL(void) { return mDat.URL; }

    String getFilename(void) { return mDat.Filename; }

    String getExtension(void) { return mDat.Extension; }

    String getTitle(void) { return mDat.Title; }

  public slots:
    virtual bool slotSetId(Key newval) override;
    bool slotUpdate(DocumentData dat);
    virtual bool slotCommit(void) override;
    bool slotSetFullyQualifiedPath(String newval);
    bool slotSetURL(String newval);

    //Hold properties
    void holdFullyQualifiedPath(QString newval) {mDat.FullyQualifiedPath = newval; dirty["FullyQualifiedPath"] = true;};
    void holdURL(QString newval) {mDat.URL = newval; dirty["URL"] = true;};

  protected:
    DocumentData mDat;

    bool InitializeDocument(Key id);

    // Not sure I'm going to use this
    Property(getFileContent, slotSetFileContent) QFile Content;

    QFile getFileContent(void) { return QFile(); }

    void slotSetFileContent(QFile newval)
    {
        Q_UNUSED(newval)
        return;
    }

    // Should probably be derived from URL or FullPath
    void slotSetFilename(String newval);
    void slotSetExtension(String newval);
    void slotSetTitle(String newval);

    const QString documentTableName = "document";
};

} // namespace Nutmeg

#endif // NUTMEG_DOCUMENT_H

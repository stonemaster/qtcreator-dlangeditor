#ifndef DLANGCOMPLETIONASSISTPROVIDER_H
#define DLANGCOMPLETIONASSISTPROVIDER_H

#include <texteditor/codeassist/completionassistprovider.h>

namespace DlangEditor {

class DlangCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
    Q_OBJECT
public:
    explicit DlangCompletionAssistProvider();

    enum DlangCompletionType {
        DLANG_NO_COMPLETION, DLANG_DOT, DLANG_FUNCTION, DLANG_COMMA, DLANG_COMPLETION_SIZE
    };

    // pure TextEditor::CompletionAssistProvider
    virtual bool supportsEditor(Core::Id editorId) const;
    /**
     * Actually, it is called everytime when completion is needed
     */
    virtual TextEditor::IAssistProcessor *createProcessor() const;

    // others

    virtual bool isActivationCharSequence(const QString &sequence) const;
    virtual int activationCharSequenceLength() const;

    static DlangCompletionType completionTypeOfChar(QChar ch1);

signals:

public slots:

};

} // namespace DlangEditor

#endif // DLANGCOMPLETIONASSISTPROVIDER_H

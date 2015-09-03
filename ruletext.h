#ifndef RULETEXT_H
#define RULETEXT_H

#include <QObject>
#include <QWidget>

class RuleText : public QPlainTextEdit
{
    Q_OBJECT
public:
    RuleText();
    ~RuleText();
public slots:
    void ChangeText(QString Rules);
};

#endif // RULETEXT_H

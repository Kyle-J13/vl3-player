#pragma once
#include <QDialog>

class QSpinBox;

class SongSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SongSettingsDialog(const QString &uri, QWidget *parent = nullptr);

private:
    QString   m_uri;
    QSpinBox *m_spin;
};

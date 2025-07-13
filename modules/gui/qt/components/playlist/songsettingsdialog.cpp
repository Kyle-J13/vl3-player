#include "songsettingsdialog.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QSettings>

SongSettingsDialog::SongSettingsDialog(const QString &uri, QWidget *parent)
    : QDialog(parent), m_uri(uri)
{
    setWindowTitle(tr("Song Settings"));
    auto *lay = new QVBoxLayout(this);

    lay->addWidget(new QLabel(tr("Volume percentage (0–100):"), this));
    m_spin = new QSpinBox(this);
    m_spin->setRange(0,100);

    // load stored or default to 100
    QSettings settings;
    settings.beginGroup("SongSettings");
    m_spin->setValue(settings.value(uri, 100).toInt());
    settings.endGroup();

    lay->addWidget(m_spin);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this](){
        QSettings s;
        s.beginGroup("SongSettings");
        s.setValue(m_uri, m_spin->value());
        s.endGroup();
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &SongSettingsDialog::reject);
    lay->addWidget(buttons);
}

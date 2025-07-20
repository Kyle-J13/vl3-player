/*****************************************************************************
 * songsettingsdialog.cpp
 *
 * Implements SongSettingsDialog which is a small modal that lets a user choose
 * a volume percentage (0–100%) for a specific track (identified by its URI),
 * plus four EQ bands: Preamp, Bass, Mid, Treble.
 *
 * – In the constructor it calls QSettings::beginGroup("SongSettings")
 *   and then settings.value(uri, 100) to load the saved volume value,
 *   and settings.value(uri + "/<band>", 0) to load each EQ band.
 *
 * – When a user selects “OK” -> opens QSettings::beginGroup("SongSettings")
 *   and calls settings.setValue(uri, m_spin->value()) to write the new
 *   percentage, plus settings.setValue(uri + "/<band>", slider->value())
 *   for each EQ band under the key [SongSettings][<uri>/<band>].
 *
 * These values are written to disk and will be there next time you launch VLC.
 *****************************************************************************/

#include "songsettingsdialog.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QDialogButtonBox>
#include <QSettings>

static const int EQ_MIN_DB = -12;
static const int EQ_MAX_DB = 12;

SongSettingsDialog::SongSettingsDialog(const QString &uri, QWidget *parent)
    : QDialog(parent)
    , m_uri(uri)  // store the unique media URI so we can load/save its settings
{
    setWindowTitle(tr("Song Settings"));

    auto *lay = new QVBoxLayout(this);

    // Volume percentage
    lay->addWidget(new QLabel(tr("Volume percentage (0–100):"), this));
    m_spin = new QSpinBox(this);
    m_spin->setRange(0, 100);
    {
        QSettings s;
        s.beginGroup("SongSettings");
        m_spin->setValue(s.value(uri, 100).toInt());
        s.endGroup();
    }
    lay->addWidget(m_spin);

    // EQ sliders
    struct { QString name; QSlider *&slider; } bands[] = {
        { tr("Preamp (dB)"), m_preampSlider },
        { tr("Bass (dB)"),   m_bassSlider   },
        { tr("Mid (dB)"),    m_midSlider    },
        { tr("Treble (dB)"), m_trebleSlider }
    };

    for (auto &b : bands) {
        lay->addWidget(new QLabel(b.name, this));
        b.slider = new QSlider(Qt::Horizontal, this);
        b.slider->setRange(EQ_MIN_DB, EQ_MAX_DB);
        {
            QSettings s;
            s.beginGroup("SongSettings");
            b.slider->setValue(s.value(uri + "/" + b.name, 0).toInt());
            s.endGroup();
        }
        lay->addWidget(b.slider);
    }

    // OK/Cancel buttons
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this, bands, uri]() {
        QSettings s;
        s.beginGroup("SongSettings");
        s.setValue(uri, m_spin->value());
        for (auto &b : bands)
            s.setValue(uri + "/" + b.name, b.slider->value());
        s.endGroup();
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &SongSettingsDialog::reject);
    lay->addWidget(buttons);
}

/*****************************************************************************
 * songsettingsdialog.cpp
 *
 * This dialog lets you set a custom volume (0–100%) and tweak four EQ
 * bands Preamp, Bass, Mid, and Treble on a per-song basis.
 *
 * When it opens, it loads whatever you saved before (or defaults to 100%
 * volume and 0 dB for each band) from QSettings under “SongSettings.”
 *
 * Clicking “OK” saves your choices back into QSettings so they stick
 * between sessions. It also updates a JSON file at
 * ~/.local/share/vlc/per_song_volumes.json, merging in this song’s
 * URI with its volume and EQ settings. 
 * Extrernal scripts can read and apply exactly the right
 * settings when you play the song.
 *
 * If you click “Cancel,” nothing is written.
 *****************************************************************************/

#include "songsettingsdialog.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QDialogButtonBox>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

static const int EQ_MIN_DB = -12;
static const int EQ_MAX_DB =  12;

SongSettingsDialog::SongSettingsDialog(const QString &uri, QWidget *parent)
    : QDialog(parent)
    , m_uri(uri)
{
    setWindowTitle(tr("Song Settings"));

    auto *layout = new QVBoxLayout(this);

    // Volume percentage
    layout->addWidget(new QLabel(tr("Volume (0–100)%:"), this));
    m_spin = new QSpinBox(this);
    m_spin->setRange(0, 100);
    {
        QSettings settings;
        settings.beginGroup("SongSettings");
        m_spin->setValue(settings.value(uri, 100).toInt());
        settings.endGroup();
    }
    layout->addWidget(m_spin);

    // EQ sliders
    struct { QString label; QSlider *&slider; } bands[] = {
        { tr("Preamp (dB)"), m_preampSlider },
        { tr("Bass (dB)"),   m_bassSlider   },
        { tr("Mid (dB)"),    m_midSlider    },
        { tr("Treble (dB)"), m_trebleSlider }
    };
    for (auto &band : bands) {
        layout->addWidget(new QLabel(band.label, this));
        band.slider = new QSlider(Qt::Horizontal, this);
        band.slider->setRange(EQ_MIN_DB, EQ_MAX_DB);

        QSettings settings;
        settings.beginGroup("SongSettings");
        band.slider->setValue(settings.value(uri + "/" + band.label, 0).toInt());
        settings.endGroup();

        layout->addWidget(band.slider);
    }

    // OK and Cancel buttons
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this, bands, uri]() {
        // save to QSettings
        QSettings settings;
        settings.beginGroup("SongSettings");
        settings.setValue(uri, m_spin->value());
        for (auto &band : bands)
            settings.setValue(uri + "/" + band.label, band.slider->value());
        settings.endGroup();

        // update the JSON for external scripts
        const QString dirPath = QDir::homePath() + "/.local/share/vlc";
        QDir().mkpath(dirPath);
        const QString jsonPath = dirPath + "/per_song_volumes.json";

        QJsonObject root;
        QFile inFile(jsonPath);
        if (inFile.open(QIODevice::ReadOnly)) {
            auto doc = QJsonDocument::fromJson(inFile.readAll());
            if (doc.isObject())
                root = doc.object();
            inFile.close();
        }

        QJsonObject songSettings;
        songSettings["volume"]      = m_spin->value();
        songSettings["Preamp (dB)"] = m_preampSlider->value();
        songSettings["Bass (dB)"]   = m_bassSlider->value();
        songSettings["Mid (dB)"]    = m_midSlider->value();
        songSettings["Treble (dB)"] = m_trebleSlider->value();

        root[uri] = songSettings;

        QFile outFile(jsonPath);
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            outFile.write(QJsonDocument(root).toJson());
            outFile.close();
        }

        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &SongSettingsDialog::reject);
    layout->addWidget(buttons);
}

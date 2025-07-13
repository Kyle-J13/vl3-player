/*****************************************************************************
 * songsettingsdialog.cpp
 *
 * Implements SongSettingsDialog which is a small modal that lets a user choose
 * a volume percentage (0–100%) for a specific track (identified by its URI).
 *
 * – In the constructor it calls QSettings::beginGroup("SongSettings")
 *   and then settings.value(uri, 100) to load the saved value 
 *
 * – When a user selects “OK” -> opens QSettings::beginGroup("SongSettings")
 *   and calls settings.setValue(uri, m_spin->value()) to write the new
 *   percentage under the key [SongSettings][<uri>] in the persistent store.
 *
 * These values are written to disk and will be there next time you launch VLC.
 *****************************************************************************/

#include "songsettingsdialog.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QSettings>

// …rest of the file unchanged…

#include "songsettingsdialog.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QSettings>

SongSettingsDialog::SongSettingsDialog(const QString &uri, QWidget *parent)
    : QDialog(parent)
    , m_uri(uri)  // store the unique media URI so we can load/save its volume setting
{
    // Set the window title for the dialog
    setWindowTitle(tr("Song Settings"));

    // Use a vertical layout to stack label / spinbox / buttons
    auto *lay = new QVBoxLayout(this);

    // Prompt label for the user
    lay->addWidget(new QLabel(tr("Volume percentage (0–100):"), this));

    // Spinbox to choose 0–100%
    m_spin = new QSpinBox(this);
    m_spin->setRange(0, 100);

    // Load the previously saved percentage for this URI (default is 100%)
    {
        QSettings settings;
        settings.beginGroup("SongSettings");
        m_spin->setValue(settings.value(uri, 100).toInt());
        settings.endGroup();
    }

    // Add the spinbox under the label
    lay->addWidget(m_spin);

    // Create standard OK/Cancel buttons
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                         this);

    // When OK is clicked, save the new percentage back into QSettings and close
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        QSettings s;
        s.beginGroup("SongSettings");
        s.setValue(m_uri, m_spin->value());
        s.endGroup();
        accept();  // close dialog with QDialog::Accepted
    });

    // When Cancel is clicked close without saving
    connect(buttons, &QDialogButtonBox::rejected,
            this, &SongSettingsDialog::reject);

    // Put the buttons at the bottom of the dialog
    lay->addWidget(buttons);
}

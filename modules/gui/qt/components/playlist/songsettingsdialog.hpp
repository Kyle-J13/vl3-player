/************************************************************************
* songsettingsdialog.hpp
*************************************************************************/
#pragma once  

#include <QDialog>  

// Forward declarations to avoid pulling in the full Qt headers
class QSpinBox;  
class QSlider;    // for EQ bands

// A dialog allowing the user to set a per song volume percentage and EQ.
class SongSettingsDialog : public QDialog
{
    Q_OBJECT  

public:
    /**
     * @param uri    A unique identifier for the song which is used as the key in QSettings.
     * @param parent The parent widget for ownership
     */
    explicit SongSettingsDialog(const QString &uri, QWidget *parent = nullptr);

private:
    QString   m_uri;           // The song’s key in persistent settings
    QSpinBox *m_spin;          // Spin box for selecting the volume percentage
    QSlider  *m_preampSlider;  // Slider for preamp (dB)
    QSlider  *m_bassSlider;    // Slider for bass (dB)
    QSlider  *m_midSlider;     // Slider for mid (dB)
    QSlider  *m_trebleSlider;  // Slider for treble (dB)
};

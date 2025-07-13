/************************************************************************
* songsettingsdialog.hpp
*************************************************************************/
#pragma once  

#include <QDialog>  

class QSpinBox;    // Forward declaration to avoid pulling in the full QtSpinBox header


//A dialog allowing the user to set a per song volume percentage.
class SongSettingsDialog : public QDialog
{
    Q_OBJECT  

public:
    /**
     * @param uri    A unique identifier for the song, used as the key in QSettings.
     * @param parent The parent widget for ownership and modal behavior.
     */
    explicit SongSettingsDialog(const QString &uri, QWidget *parent = nullptr);

private:
    QString   m_uri;   // The song’s key in persistent settings
    QSpinBox *m_spin;  // Spin box for selecting the volume percentage 
};

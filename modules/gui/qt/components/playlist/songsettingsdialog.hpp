/************************************************************************
* songsettingsdialog.hpp
*************************************************************************/
#pragma once  

#include <QDialog>  

// Forward declaration to avoid pulling in the full QtSpinBox header
class QSpinBox;   

//A dialog allowing the user to set a per song volume percentage.
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
    QString   m_uri;   // The song’s key in persistent settings
    QSpinBox *m_spin;  // Spin box for selecting the volume percentage 
};

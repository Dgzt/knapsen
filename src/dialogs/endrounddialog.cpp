#include "ui_endroundwidget.h"

#include "endrounddialog.h"

EndRoundDialog::EndRoundDialog( QWidget* parent, QString winnerName, int winnerAcquiredScores ) : 
    KDialog(parent)
{
    QWidget* mainWidget = new QWidget( this );
    
    Ui::EndRoundWidgetUi *ui = new Ui::EndRoundWidgetUi;
    
    ui->setupUi( mainWidget );
    
    ui->wonRoundNameLabel->setText( winnerName );
    ui->wonRoundScoresLabel->setText( QString::number( winnerAcquiredScores ) );
    
    setMainWidget( mainWidget );
    
    setButtons( KDialog::Ok );
    setButtonText( KDialog::Ok, i18n( "Start next round" ) );
    
    setWindowTitle( i18n( "Round end" ) );
}

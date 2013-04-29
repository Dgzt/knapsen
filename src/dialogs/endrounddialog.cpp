#include "ui_endroundwidget.h"

#include "endrounddialog.h"

EndRoundDialog::EndRoundDialog( QWidget* parent, QString winnerName, int winnerScores ) : 
    KDialog(parent)
{
    QWidget* mainWidget = new QWidget( this );
    
    Ui::EndRoundWidgetUi *ui = new Ui::EndRoundWidgetUi;
    
    ui->setupUi( mainWidget );
    
    ui->wonRoundNameLabel->setText( winnerName );
    ui->wonRoundScoresLabel->setText( QString::number( winnerScores ) );
    
    setMainWidget( mainWidget );
    
    setButtons( KDialog::Ok );
    setButtonText( KDialog::Ok, i18n( "Next round" ) );
    
    setWindowTitle( i18n( "Round end" ) );
}

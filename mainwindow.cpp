#include "stable.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(initalized==false){
        QPalette Pal(palette());
        initalized=true;
        Pal.setColor(QPalette::Background, defaultcolor);
        ui->color_display->setAutoFillBackground(true);
        ui->color_display->setPalette(Pal);
    }

    //connect

    //display to control
    //QObject::connect(ui->AngleInput,SIGNAL(textChanged(QString)),ui->openGLWidget,SLOT(changeAngle(QString)));
    QObject::connect(ui->openGLWidget,SIGNAL(axiomRead(QString)),ui->Axiom,SLOT(setText(QString)));
    QObject::connect(ui->openGLWidget,SIGNAL(angleRead(QString)),ui->AngleInput,SLOT(setText(QString)));
    QObject::connect(ui->openGLWidget,SIGNAL(RuleRead(QString)),ui->RuleDIsplay,SLOT(setPlainText(QString)));
    QObject::connect(ui->openGLWidget,SIGNAL(iterationRead(int)),ui->Iteration_spinBox,SLOT(setValue(int)));
    QObject::connect(ui->openGLWidget,SIGNAL(RuleCalculated(QString)),ui->L_systemResult,SLOT(setPlainText(QString)));
    //control to display
    QObject::connect(ui->Axiom,SIGNAL(textEdited(QString)),ui->openGLWidget,SLOT(changeAxiom(QString)));
    QObject::connect(ui->AngleInput,SIGNAL(textEdited(QString)),ui->openGLWidget,SLOT(changeAngle(QString)));
    QObject::connect(ui->Iteration_spinBox,SIGNAL(valueChanged(int)),ui->openGLWidget,SLOT(changeIteration(int)));
    QObject::connect(ui->foceUpdateButton,SIGNAL(pressed()),ui->openGLWidget,SLOT(UpdateL_system()));
    QObject::connect(ui->FileSelector,SIGNAL(currentTextChanged(QString)),ui->openGLWidget,SLOT(ChangeTemplate(QString)));
    QObject::connect(ui->foceUpdateButton,SIGNAL(pressed()),ui->openGLWidget,SLOT(setFocus()));
    QObject::connect(this,SIGNAL(ColorChange(QColor)),ui->openGLWidget,SLOT(getChangedColor(QColor)));
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_foceUpdateButton_pressed()
{
    ui->openGLWidget->UpdateL_system();
}

void MainWindow::on_actionDocument_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Help");
    //define output
    std::string content="Choose a L-system file from the slot\n";
    content+="use Axiom, angle and iteration to change the output\n";
    content+="Press update button to redraw\n";
    content+="\n";
    content+="Control keys:\n";
    content+="\t W- move camera forward\n";
    content+="\t D- move camera backward\n";
    content+="\t right arrow - move camera right\n";
    content+="\t left arrow -move camera left\n";
    content+="\t up arrow -move camera up\n";
    content+="\t down arrow -move camera down\n";
    msgBox.setInformativeText(content.c_str());
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}


void MainWindow::on_ColorSelector_clicked()
{
    QColor colorcache=defaultcolor;
    defaultcolor=QColorDialog::getColor(defaultcolor, this);
    if(colorcache!=defaultcolor){
        emit ColorChange(defaultcolor);
    }
    QPalette Pal(palette());
    initalized=true;
    Pal.setColor(QPalette::Background, defaultcolor);
    ui->color_display->setAutoFillBackground(true);
    ui->color_display->setPalette(Pal);
    ui->openGLWidget->setFocus();
}

void MainWindow::on_BrowseRuleFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (.)"));
}

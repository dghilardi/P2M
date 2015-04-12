/*
    Copyright (C) 2015  Davide Ghilardi

    This file is part of P2M

    P2M is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    P2M is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with P2M.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mailcomposer.h"
#include "mailsender.h"
#include "qfiledialog.h"
#include <quazip/JlCompress.h>
#include <yaml-cpp/yaml.h>
#include <qdebug.h>

bool MainWindow::compressProject(QString &result)
{
    result=tmpPath+ui->lnName->text()+"-"+ui->lnVersion->text()+".zip";
    JlCompress::compressDir(result, ui->lnPath->text());
    return true;
}

void MainWindow::loadConfiguration(QString configFile)
{
    YAML::Node configRoot = YAML::LoadFile(configFile.toStdString());

    devdefaultpath = QString(configRoot["DEVFOLDER"].as<std::string>().c_str());
    tmpPath = QString(configRoot["ARCHIVEFOLDER"].as<std::string>().c_str());
    if(!tmpPath.endsWith('/')) tmpPath.append('/');
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tmpPath("/tmp/")
{
    ui->setupUi(this);
    connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(send_clicked()));
    connect(ui->btSetPath, SIGNAL(clicked()), this, SLOT(setPath_clicked()));

    loadConfiguration("config.yaml");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send_clicked()
{
    ui->statusBar->showMessage(tr("Compressing project..."));
    QString compressedPath;
    compressProject(compressedPath);
    ui->statusBar->showMessage(tr("Building message..."));
    MailComposer::Params parameters;

    parameters["NAME"] = ui->lnName->text();
    parameters["VERSION"] = ui->lnVersion->text();
    parameters["CHANGELOG"] = ui->txtChangelog->toPlainText();

    QString body, subject;
    MailComposer::composeMail(parameters, subject, body);

    MailSender sender("account.yaml");
    ui->statusBar->showMessage(tr("Sending message..."));
    sender.sendMail(ui->lnTo->text(), subject, body, compressedPath);
    ui->statusBar->showMessage(tr("Message sent!"), 3000);
}

void MainWindow::setPath_clicked()
{
    QString startDir = ui->lnPath->text().isEmpty() ? devdefaultpath : ui->lnPath->text();
    QString newPath(QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                          startDir,
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks));
    if(!newPath.isEmpty()) ui->lnPath->setText(newPath);
}

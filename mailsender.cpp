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

#include "mailsender.h"
#include <SmtpMime>
#include <yaml-cpp/yaml.h>

MailSender::MailSender(const QString &configFile)
{
    YAML::Node configRoot = YAML::LoadFile(configFile.toStdString());

    fullName = QString(configRoot["NAME"].as<std::string>().c_str());
    usermail = QString(configRoot["EMAIL"].as<std::string>().c_str());
    password = QString(configRoot["PASSWORD"].as<std::string>().c_str());
    server  = QString(configRoot["SERVER"].as<std::string>().c_str());
    port = configRoot["PORT"].as<int>();
    std::string ctype = configRoot["CONNECTIONTYPE"].as<std::string>();
    connectionType = ConnTcp;
    if(ctype == "SSL") connectionType = ConnSsl;
    else if(ctype == "TLS") connectionType = ConnTls;
    else if(ctype == "TCP") connectionType = ConnTcp;
}

MailSender::MailSender(const QString &_fullName, const QString &_username, const QString &_password, const QString &_server, int _port, MailSender::ConnType _connectionType):
    fullName(_fullName), usermail(_username), password(_password), server(_server), port(_port), connectionType(_connectionType)
{

}

MailSender::~MailSender()
{

}

bool MailSender::sendMail(const QString &recipients, const QString &subject, const QString &body, const QString &attachment)
{
    SmtpClient::ConnectionType connType = SmtpClient::TcpConnection;
    switch(connectionType){
        case ConnTcp:
        connType = SmtpClient::TcpConnection;
        break;
    case ConnSsl:
        connType = SmtpClient::SslConnection;
        break;
    case ConnTls:
        connType = SmtpClient::TlsConnection;
        break;
    }

    SmtpClient smtp(server, port, connType);

    smtp.setUser(usermail);
    smtp.setPassword(password);

    MimeMessage message;

    message.setSender(new EmailAddress(usermail, fullName));

    // recipients is a comma-separated-value string where each value is in one of the following format
    // * "Name1 <name.one@mail.org>"
    // * "name.two@email.com"
    // * "<name.3@srv.net>"

    QStringList mailLst = recipients.split(",",QString::SkipEmptyParts);
    for(int i=0; i<mailLst.length(); ++i){
        QString rec = mailLst.at(i).trimmed();

        QRegExp mailFormat("<[^<>]+@[^<>]+>");
        int pos = mailFormat.indexIn(rec);
        QString mail;
        if(pos>-1){
            mail = mailFormat.cap().mid(1);
            mail.chop(1);
        }
        int idx = rec.indexOf("<");
        QString name = rec.left(idx).trimmed();

        if(mail.isEmpty() && name.contains('@')){
            mail = name;
            name = "";
        }

        message.addRecipient(new EmailAddress(mail, name));
    }

    message.setSubject(subject);

    MimeText text;

    text.setText(body);

    message.addPart(&text);

    message.addPart(new MimeAttachment(new QFile(attachment)));

    smtp.connectToHost();
    smtp.login();
    smtp.sendMail(message);
    smtp.quit();

    return true;
}


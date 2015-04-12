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

#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <qstring.h>

class MailSender
{
public:
    enum ConnType
    {
        ConnTcp,
        ConnSsl,
        ConnTls
    };

private:
    QString fullName, usermail, password, server;
    int port;
    ConnType connectionType;

public:
    MailSender(const QString &configFile);
    MailSender(const QString &_fullName, const QString &_username, const QString &_password, const QString &_server, int _port, ConnType _connectionType);
    ~MailSender();

    bool sendMail(const QString &recipients, const QString &subject, const QString &body, const QString &attachment);
};

#endif // MAILSENDER_H

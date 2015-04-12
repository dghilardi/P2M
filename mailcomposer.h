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

#ifndef MAILCOMPOSER_H
#define MAILCOMPOSER_H

#include <QMap>
#include <QString>

class MailComposer
{
private:
    static void replaceAll(std::string& str, const std::string& from, const std::string& to);
public:
    typedef QMap<QString, QString> Params;

    MailComposer();
    ~MailComposer();

    static bool composeMail(const Params &parameters, QString &subject, QString &body);
};

#endif // MAILCOMPOSER_H

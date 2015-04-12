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

#include "mailcomposer.h"
#include <yaml-cpp/yaml.h>

#define TEMPLATE_FILE "template.yaml"
#define ESC QString("$")

MailComposer::MailComposer()
{

}

MailComposer::~MailComposer()
{

}

bool MailComposer::composeMail(const MailComposer::Params &parameters, QString &subject, QString &body)
{
    YAML::Node templRoot = YAML::LoadFile(TEMPLATE_FILE);
    subject = QString(templRoot["SUBJECT"].as<std::string>().c_str());
    body = QString(templRoot["BODY"].as<std::string>().c_str());

    for(Params::const_iterator it = parameters.begin(); it!=parameters.end(); ++it){
        QString toReplace = ESC + it.key() + ESC;
        subject.replace(toReplace, it.value());
        body.replace(toReplace, it.value());
    }

    return true;
}

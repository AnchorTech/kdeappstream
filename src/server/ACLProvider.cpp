#include "ACLProvider.h"

#include <QDir>
#include <QFile>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QCoreApplication>
#include <QDebug>

ACLProvider * ACLProvider::m_instance = 0;

ACLProvider::ACLProvider() :
    _acceptAll(false),
    _rejectAll(true)
{
    m_instance = this;
    _dir = QDir(QDir::homePath() + "/.config").absolutePath();
    _path = QDir(_dir + "/kappstream_acl.conf").absolutePath();
    if (!QFile(_path).exists())
        createPermissionsFile();
    
    qDebug() << _path;

    QFile f(_path);
    f.open(QFile::ReadOnly);

    QDomDocument doc;
    if (doc.setContent(&f))
    {
        QDomElement config = doc.documentElement();
        if (config.isNull() || config.nodeName() != "config")
            return;

        QDomElement accepted = config.firstChildElement("accepted");
        if (!accepted.isNull())
        {
            if (accepted.hasAttribute("all") && accepted.attribute("all").compare("true", Qt::CaseInsensitive) == 0)
                _rejectAll = !(_acceptAll = true);
            QDomElement name = accepted.firstChildElement("accept");
            while (!name.isNull())
            {
                QString tmp = name.attribute("name");
                if (!tmp.isEmpty())
                    acceptedNames << tmp;
                name = name.nextSiblingElement("accept");
            }
        }

        QDomElement rejected = config.firstChildElement("rejected");
        if (!rejected.isNull())
        {
            if (rejected.hasAttribute("all") && rejected.attribute("all").compare("true", Qt::CaseInsensitive) == 0)
                _acceptAll = !(_rejectAll = true);
            QDomElement name = rejected.firstChildElement("reject");
            while (!name.isNull())
            {
                QString tmp = name.attribute("name");
                if (!tmp.isEmpty())
                    rejectedNames << tmp;
                name = name.nextSiblingElement("reject");
            }
        }
    }
}

void ACLProvider::createPermissionsFile() {

    QFile file(_path);
    
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }

    QXmlStreamWriter().setCodec("UTF-8");

    QXmlStreamWriter perms(&file);
    perms.setAutoFormatting(true);
    perms.writeStartDocument();
    perms.writeStartElement("config");
    
    perms.writeStartElement("accepted");
    perms.writeAttribute("all", "false");
    perms.writeStartElement("accept");
    perms.writeAttribute("name", "testapp");
    
    perms.writeEndElement();
    perms.writeEndElement();
    
    perms.writeStartElement("reject");
    perms.writeAttribute("all", "true");
    
    perms.writeEndElement();
    perms.writeEndDocument();
    
    file.close();
}

ACLProvider * ACLProvider::instance()
{
    if (!m_instance)
        m_instance = new ACLProvider();
    return m_instance;
}

void ACLProvider::accept(const QString & name)
{
    rejectedNames.removeAll(name);
    if (acceptedNames.contains(name))
        return;
    acceptedNames << name;
    save();
}

void ACLProvider::reject(const QString & name)
{
    acceptedNames.removeAll(name);
    if (rejectedNames.contains(name))
        return;
    rejectedNames << name;
    save();
}

void ACLProvider::acceptAll()
{
    _acceptAll = true;
    _rejectAll = false;
    save();
}

void ACLProvider::rejectAll()
{
    _acceptAll = false;
    _rejectAll = true;
    save();
}

bool ACLProvider::isAccepted(const QString &name)
{
    printInfo();
    if (_rejectAll)
    {
        foreach (QString s, acceptedNames)
        {
            if (s == name)
            {
                qDebug() << s << "is accepted!";
                return true;
            }
        }
        return false;
    }
    else
    {
        foreach (QString s, rejectedNames)
        {
            if (s == name)
            {
                qDebug() << s << "is rejected!";
                return false;
            }
        }
        return true;
    }
}

void ACLProvider::save()
{
    QDir d;
    d.mkpath(_dir);
    d.mkdir(_dir);

    QFile f( _path );
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Cannot open ACL config file!" << f.errorString();
        return;
    }

    QXmlStreamWriter sw(&f);
    sw.setAutoFormatting(true);
    sw.writeStartDocument();
    sw.writeStartElement("config");

    sw.writeStartElement("accepted");
    if (_acceptAll)
        sw.writeAttribute("all", "true");
    foreach (QString n, acceptedNames)
    {
        sw.writeStartElement("accept");
        sw.writeAttribute("name", n);
        sw.writeEndElement();
    }
    sw.writeEndElement();


    sw.writeStartElement("rejected");
    if (_rejectAll)
        sw.writeAttribute("all", "true");
    foreach (QString n, rejectedNames)
    {
        sw.writeStartElement("reject");
        sw.writeAttribute("name", n);
        sw.writeEndElement();
    }
    sw.writeEndElement();

    sw.writeEndElement();
    sw.writeEndDocument();

    f.flush();
    f.close();
}

void ACLProvider::printInfo()
{
    qDebug() << "Accepted [" <<_acceptAll << "]" << acceptedNames;
    qDebug() << "Rejected [" << _rejectAll << "]" << rejectedNames;
}

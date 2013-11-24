#include "FuelStationReader.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QMap>
#include <QStringList>
#include <QVariant>
#include "Logger.h"
#include "Utils.h"

using namespace std;

FuelStationReader::FuelStationReader(void)
{
}


FuelStationReader::~FuelStationReader(void)
{
}

vector<FuelStation*> FuelStationReader::GetLukoilJSON(const string& path) {
    static const QString jsonId = "Id";
    static const QString jsonNumber = "Number";
    static const QString jsonAddress = "Address";
    static const QString jsonIsFranchise = "IsFranchise";
    static const QString jsonLat = "Lat";
    static const QString jsonLng = "Lng";
    static const QString jsonFuelIds = "FuelIds";
    static const QString jsonFuelIdDiesel = "diesel";
    static const QString jsonFuelIdEktoDiesel = "ekto-diesel";
    static const QString jsonFuelId80 = "80";
    static const QString jsonFuelId92 = "92";
    static const QString jsonFuelIdEkto92 = "ekto";
    static const QString jsonFuelId95 = "95";
    static const QString jsonFuelIdEkto95 = "ekto-plus";
    static const QString jsonFuelId98 = "98";
    static const QString jsonFuelId98EktoSport = "ekto-sport";
    static const QString jsonFuelIdLPG = "gas";
    static const QString jsonServiceIds = "ServiceIds";
    static const QString jsonOperator = "Operator";
    static const QString jsonStations = "Stations";
    static const string brand_op = "Лукойл";
    QString stOperator;
    vector<FuelStation*> stations;
    FuelStation* station = NULL;
    QFile file(QString::fromUtf8( path.data(), path.size() ));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return stations;

    QString content = file.readAll();
    file.close();

    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
    if (jsonDoc.isObject())
    {
        QJsonValue root = jsonDoc.object().value(jsonOperator);
        if (root.isString()) stOperator = root.toString();
        root = jsonDoc.object().value(jsonStations);
        if (root.isArray()) {
            const QJsonArray array = root.toArray(); // list of station
            for (QJsonArray::const_iterator i = array.constBegin(); i != array.constEnd(); ++i) {
                if ((*i).isObject()) {
                    station = new FuelStation();
                    stations.push_back(station);
                    const QJsonObject obj = (*i).toObject();
                    QJsonValue value = obj.value(jsonAddress);
                    if (value.isString()) {
                        QString str = value.toString();
                        station->mAddress = value.toString().toLocal8Bit();
                    }
                    value = obj.value(jsonId);
                    if (value.isDouble()) {
                        station->mRef = static_cast<int>(value.toDouble());
                    }
                    value = obj.value(jsonNumber);
                    if (value.isString()) {
                        station->mLocalRef = value.toString().toInt();
                    }
                    value = obj.value(jsonLat); // coordinates
                    if (value.isDouble()) {
                        station->setLat(value.toDouble());
                    }
                    value = obj.value(jsonLng);
                    if (value.isDouble()) {
                        station->setLon(value.toDouble());
                    }
                    value = obj.value(jsonFuelIds); // get fuel types
                    if (value.isArray()) {
                        const QJsonArray fuelIds = value.toArray();
                        for (QJsonArray::const_iterator j = fuelIds.constBegin(); j != fuelIds.constEnd(); ++j) {
                            if ((*j).isString()) {
                                const QString str = (*j).toString();
                                if (str == jsonFuelIdDiesel || str == jsonFuelIdEktoDiesel)
                                    station->mFuelTypes.insert(DIESEL);
                                else if (str == jsonFuelId80)
                                    station->mFuelTypes.insert(AI80);
                                else if (str == jsonFuelId92 || str == jsonFuelIdEkto92)
                                    station->mFuelTypes.insert(AI92);
                                else if (str == jsonFuelId95 || str == jsonFuelIdEkto95)
                                    station->mFuelTypes.insert(AI95);
                                else if (str == jsonFuelId98 || str == jsonFuelId98EktoSport)
                                    station->mFuelTypes.insert(AI98);
                            }
                        }
                    }
                    station->mBrand=brand_op;
                    station->mOperator=stOperator.toLocal8Bit();
                }
            }
        }
    }
    return stations;
}

vector<FuelStation*> FuelStationReader::GetGazpromNeftHTML(const string& path) {    
    vector<FuelStation*> stations;
    return stations;
}

vector<FuelStation*> FuelStationReader::GetTatNeftCSVAll(const string& path) {
    static const QString CONFIG = "tatneft.cfg";
    static const QString CSV = "tatneft.csv";
    static const QString NO_CODE = "no";
    static const string BRAND = "Татнефть";
    static const QString SEP = ";";
    static QMap <QString, vector<FuelStation*> > azs;
    vector<FuelStation*> stations;
    if (azs.empty()) {
        QVariantMap map_codes;
        QFile cfg(CONFIG);
        if (!cfg.open(QIODevice::ReadOnly | QIODevice::Text))
            return stations;
        QString content = cfg.readAll();
        cfg.close();
        
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
        if (jsonDoc.isArray()) {
            const QJsonArray array = jsonDoc.array();
            for (QJsonArray::const_iterator i = array.constBegin(); i != array.constEnd(); ++i) {
                if ((*i).isObject()) {              
                    const QJsonObject obj = (*i).toObject();
                    map_codes = obj.toVariantMap();
                }
            }
        }

        QFile file(CSV);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return stations;

        FuelStation* station;
        while (!file.atEnd()) {
            QString line = file.readLine();
            QStringList list = line.split(SEP);
            if (!list.at(0).isEmpty()) {
                QString region(list.at(0));
                region = region.trimmed();
                QString code = NO_CODE;
                if (map_codes.contains(region)) {
                    code = map_codes.value(region).toStringList().at(0);
                }

                if (code!=NO_CODE) {
                    station = new FuelStation();
                    azs[code].push_back(station);
                    station->mBrand = BRAND;
                    station->mOperator = map_codes.value(region).toStringList().at(1).toLocal8Bit();
                    //station->mName = "Татнефть";
                    QString ref = list.at(2).right(list.at(2).length()-2);
                    station->mLocalRef = ref.toInt();
                    station->mAddress = list.at(3).toLocal8Bit();
                }
            }
        }
    }
    vector<FuelStation*> reg = azs.value(QString::fromUtf8( path.data(), path.size()));
    for (int i = 0; i<reg.size(); i++) {
        FuelStation* station = new FuelStation(*reg.at(i));
        stations.push_back(station);
    }

    return stations;
}

vector<FuelStation*> FuelStationReader::GetAllOSMXML(const string& path) {
    vector<FuelStation*> stations;
    FuelStation* station = NULL;
    QFile file(QString::fromUtf8( path.data(), path.size() ));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return stations;

    
    int errorLine, errorColumn;
    QString errorMsg;
    QDomDocument document;
    if (!document.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        QString error("Syntax error line %1, column %2:\n%3");
        error = error
            .arg(errorLine)
            .arg(errorColumn)
            .arg(errorMsg);
        return stations;
    }
    QDomElement rootElement = document.firstChild().toElement();
    for(QDomNode node = rootElement.firstChild(); !node .isNull(); node = node.nextSibling()) {
        station = new FuelStation();
        stations.push_back(station);
        QDomElement element = node.toElement();
        station->setLat(element.attribute("lat").toDouble());
        station->setLon(element.attribute("lon").toDouble());
        station->mID = element.attribute("id").toInt();
        for(QDomNode tag = element.firstChild(); !tag.isNull(); tag = tag.nextSibling()) {
            QDomElement tagEle = tag.toElement();
            QString key = tagEle.attribute("k");
            QString value = tagEle.attribute("v");
            if (key == "operator") {
                station->mOperator = value.toLocal8Bit();
            }
            else if (key == "brand") {
                station->mBrand = value.toLocal8Bit();
            }
            else if (key == "ref") {
                station->mLocalRef = value.toInt();
            }
            else if (key == "name") {
                station->mName = value.toLocal8Bit();
            }
            else if (key == "fuel:diesel" && value == "yes") {
                station->mFuelTypes.insert(DIESEL);
            }
            else if (key == "fuel:octane_80" && value == "yes") {
                station->mFuelTypes.insert(AI80);
            }
            else if (key == "fuel:octane_92" && value == "yes") {
                station->mFuelTypes.insert(AI92);
            }
            else if (key == "fuel:octane_95" && value == "yes") {
                station->mFuelTypes.insert(AI95);
            }
            else if (key == "fuel:octane_98" && value == "yes") {
                station->mFuelTypes.insert(AI98);
            }
            else if (key == "fuel:lpg" && value == "yes") {
                station->mFuelTypes.insert(PETGAS);
            }
        }
    }
    return stations;
}

vector<FuelStation*> FuelStationReader::GetData(const string& func, const string& path) {
    if (func=="GetLukoilJSON")
        return GetLukoilJSON(path);
    else if (func=="GetGazpromNeftHTML")
        return GetGazpromNeftHTML(path);
    else if (func=="GetTatNeftCSVAll")
        return GetTatNeftCSVAll(path);
    return vector<FuelStation*>();
}
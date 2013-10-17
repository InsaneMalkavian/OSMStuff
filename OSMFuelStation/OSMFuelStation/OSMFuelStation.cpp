// OSMFuelStation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "FuelStationReader.h"
#include "Merger.h"
#include "HTMLSerializer.h"
#include "RegionStats.h"

struct RegionConfig {
    string name;
    string osm;
    string out;
    vector<pair<string,string>> refs;
};

void ReadRegionConfigs(const string& path, vector<RegionConfig>& configs);

int _tmain(int argc, _TCHAR* argv[])
{
    vector<RegionConfig> regions;
    vector<RegionStats> stats;
    ReadRegionConfigs("regions.cfg", regions); // read list of regions with configs

    printf("FuelStation Validator\n");

    for (int i = 0; i<regions.size(); i++) { // foreach region
        printf("%d: %s\n", i, regions.at(i).name.c_str());
        printf("Retrieving references\n");
        vector<FuelStation*> stations; // references stations (gazprom, lukoil, tatneft, etc)
        for (int k = 0; k<regions.at(i).refs.size(); k++) { // create one super-list, non-efficient
            vector<FuelStation*> passStations = FuelStationReader::GetData(regions.at(i).refs.at(k).first,regions.at(i).refs.at(k).second);
            stations.insert( stations.end(), passStations.begin(), passStations.end() );            
        }
        printf("Retrieving OSM DB\n"); // osm stations
        vector<FuelStation*> stationsOSM  = FuelStationReader::GetAllOSMXML(regions.at(i).osm);
        vector<ComparedPair*> result;
        Merger merger;
        printf("Comparing lists\n");
        stats.push_back(RegionStats());
        result = merger.CompareLists(stations, stationsOSM, stats.back()); // compare stations
        stats.back().name = regions.at(i).name;
        stats.back().link = regions.at(i).out;
        printf("Saving results\n");
        HTMLSerializer::SaveRegionToHTML(regions.at(i).out, result, regions.at(i).name); //save each region
        for (std::vector<FuelStation*>::iterator it = stations.begin() ; it != stations.end(); ++it)
            delete (*it);
        for (std::vector<FuelStation*>::iterator it = stationsOSM.begin() ; it != stationsOSM.end(); ++it)
            delete (*it);
        for (std::vector<ComparedPair*>::iterator it = result.begin() ; it != result.end(); ++it)
            delete (*it);
    }

    HTMLSerializer::SaveRootToHTML("index.html", stats); // create root page
	return 0;
}

void ReadRegionConfigs(const string& path, vector<RegionConfig>& configs) {
    QFile file(QString::fromUtf8( path.data(), path.size() ));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString content = file.readAll();
    file.close();

    const QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
    if (jsonDoc.isArray()) {
        const QJsonArray array = jsonDoc.array(); // list of regions
        for (QJsonArray::const_iterator i = array.constBegin(); i != array.constEnd(); ++i) {
            if ((*i).isObject()) {
                configs.push_back(RegionConfig());                
                const QJsonObject obj = (*i).toObject();
                QJsonValue value = obj.value("name");
                if (value.isString()) {
                    QString str = value.toString();
                    configs.back().name = value.toString().toLocal8Bit();
                }
                value = obj.value("osm");
                if (value.isString()) {
                    QString str = value.toString();
                    configs.back().osm = value.toString().toLocal8Bit()+".xml";
                }
                value = obj.value("out");
                if (value.isString()) {
                    QString str = value.toString();
                    configs.back().out = value.toString().toLocal8Bit();
                }
                value = obj.value("refs");
                if (value.isArray()) {
                    const QJsonArray refs = value.toArray();
                    for (int i = 0; i<refs.size()-1; i++) {
                        QString func, path;
                        if (refs.at(i).isString() && refs.at(i+1).isString()) {
                            func = refs.at(i).toString();
                            path = refs.at(i+1).toString();
                        }
                        configs.back().refs.push_back(pair<string,string>(string(func.toLocal8Bit()),string(path.toLocal8Bit())));
                        i++;
                    }
                }
            }
        }
    }
}
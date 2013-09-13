#include "HTMLSerializer.h"
#include <time.h>
#include <QFile>
#include <QTextStream>
#include "Utils.h"

HTMLSerializer::HTMLSerializer(void)
{
}


HTMLSerializer::~HTMLSerializer(void)
{
}

void HTMLSerializer::SaveRegionToHTML(const string& path, const vector<ComparedPair*> pairs, const string& header) {
    QFile file(QString::fromUtf8( path.data(), path.size() ));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    XMLNode page("html");
    XMLNode root("table");
    root.AddAttribute("border", "1");
    page.AddChild(XMLNode("head").AddChild(XMLNode("title", "Валидатор АЗС Кировской области")));
    root.AddChild(XMLNode("tr").AddAttribute("align","center").AddAttribute("bgcolor","DarkTurquoise").AddChild(XMLNode("td").AddChild(XMLNode("b", "Brand / Бренд")))
        .AddChild(XMLNode("td").AddChild(XMLNode("b", "Operator / Оператор"))).AddChild(XMLNode("td").AddChild(XMLNode("b", "Ref / Номер")))
        .AddChild(XMLNode("td").AddChild(XMLNode("b", "Address / Адрес"))).AddChild(XMLNode("td").AddChild(XMLNode("b", "Name / Название")))
        .AddChild(XMLNode("td").AddChild(XMLNode("b", "Coordinates/ Координаты"))).AddChild(XMLNode("td").AddChild(XMLNode("b", "Coordinates / Координаты OSM")))
        .AddChild(XMLNode("td").AddChild(XMLNode("b", "Offset (m) / Смещение (м)"))).AddChild(XMLNode("td").AddChild(XMLNode("b", "Diesel")))
        .AddChild(XMLNode("td").AddChild(XMLNode("b", "80"))).AddChild(XMLNode("td").AddChild(XMLNode("b", "92"))).AddChild(XMLNode("td").AddChild(XMLNode("b", "95")))
        .AddChild(XMLNode("td").AddChild(XMLNode("b", "98"))).AddChild(XMLNode("td").AddChild(XMLNode("b", "Газ"))).AddChild(XMLNode("td").AddChild(XMLNode("b", "Status / Статус")))
        );
    for (int i = 0; i<pairs.size(); i++) {
        root.AddChild(XMLNode("tr").AddAttribute("bgcolor",pairs.at(i)->GetColorStatus()).AddChild(XMLNode("td", pairs.at(i)->GetBrand())).AddChild(XMLNode("td", pairs.at(i)->GetOperator()))
            .AddChild(XMLNode("td", pairs.at(i)->GetLocalRef())).AddChild(XMLNode("td", pairs.at(i)->mAddress)).AddChild(XMLNode("td", pairs.at(i)->GetName()))
            .AddChild(XMLNode("td").AddChild(XMLNode("a",pairs.at(i)->GetCoords()).AddAttribute("href",pairs.at(i)->GetLinkCoords(""))))
            .AddChild(XMLNode("td", pairs.at(i)->GetCoordsOSM())).AddChild(XMLNode("td", pairs.at(i)->GetOffset()))
            .AddChild(XMLNode("td", pairs.at(i)->GetDiesel())).AddChild(XMLNode("td", pairs.at(i)->Get80())).AddChild(XMLNode("td", pairs.at(i)->Get92()))
            .AddChild(XMLNode("td", pairs.at(i)->Get95())).AddChild(XMLNode("td", pairs.at(i)->Get98())).AddChild(XMLNode("td", pairs.at(i)->GetPetroGas()))
            .AddChild(XMLNode("td", pairs.at(i)->GetStatus()))
            );
    }
    int scores[ComparedPair::SIZE];
    for (int i = 0; i<ComparedPair::SIZE; i++) scores[i]=0;
    for (int i = 0; i<pairs.size(); i++) {
        scores[pairs.at(i)->mState]++;
    }
    XMLNode body("body");
    XMLNode headerTitle("h2", "FuelStation validation / Валидатор АЗС");
    XMLNode regionHeader("h3", header);
    XMLNode counter("br", "<script language=\"JavaScript\" src=\"http://www.counter160.com/js.js?img=11\"\
></script><br><a href=\"http://www.000webhost.com\"><img src=\"http://www.counter160.com/images/11/left.png\"\
alt=\"Free web hosting\" border=\"0\" align=\"texttop\"></a><a href=\"http://www.hosting24.com\"><img alt=\"\
Web hosting\" src=\"http://www.counter160.com/images/11/right.png\" border=\"0\" align=\"texttop\"></a>");
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);    
    XMLNode date("h3", "Last update " + string(asctime(timeinfo)));
    XMLNode percTable("table");
    percTable.AddAttribute("border", "1").AddAttribute("width", "100%").AddChild(XMLNode("tr").AddChild(XMLNode("td", Utils::IntToStr(scores[0])+" ODD").AddAttribute("width",Utils::IntToStr(100.f*scores[0]/pairs.size())+"%"))
        .AddChild(XMLNode("td", Utils::IntToStr(scores[1])+" NP").AddAttribute("width",Utils::IntToStr(100.f*scores[1]/pairs.size())+"%").AddAttribute("bgcolor",ComparedPair::GetColorStatus(ComparedPair::NOT_PRESENT)))
        .AddChild(XMLNode("td", Utils::IntToStr(scores[2])+" INV").AddAttribute("width",Utils::IntToStr(100.f*scores[2]/pairs.size())+"%").AddAttribute("bgcolor",ComparedPair::GetColorStatus(ComparedPair::INVALID)))
        .AddChild(XMLNode("td", Utils::IntToStr(scores[3])+" LG").AddAttribute("width",Utils::IntToStr(100.f*scores[3]/pairs.size())+"%").AddAttribute("bgcolor",ComparedPair::GetColorStatus(ComparedPair::LOOKS_SIMILAR)))
        .AddChild(XMLNode("td", Utils::IntToStr(scores[4])+" PM").AddAttribute("width",Utils::IntToStr(100.f*scores[4]/pairs.size())+"%").AddAttribute("bgcolor",ComparedPair::GetColorStatus(ComparedPair::VALID))));
    body.AddChild(headerTitle).AddChild(regionHeader).AddChild(date).AddChild(percTable).AddChild(XMLNode("hr")).AddChild(root).AddChild(counter);
    page.AddChild(body);
    string srt = page.ToString();
    file.write(srt.c_str());
    file.close();
}

void HTMLSerializer::SaveRootToHTML(const string& path, const vector<RegionStats>& stats, const string& header) {
    QFile file(QString::fromUtf8( path.data(), path.size() ));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    XMLNode page("html");
    XMLNode headerTitle("h2", "FuelStation validation / Валидатор АЗС");
    XMLNode body("body");
    XMLNode counter("br", "<script language=\"JavaScript\" src=\"http://www.counter160.com/js.js?img=11\"\
></script><br><a href=\"http://www.000webhost.com\"><img src=\"http://www.counter160.com/images/11/left.png\"\
alt=\"Free web hosting\" border=\"0\" align=\"texttop\"></a><a href=\"http://www.hosting24.com\"><img alt=\"\
Web hosting\" src=\"http://www.counter160.com/images/11/right.png\" border=\"0\" align=\"texttop\"></a>");
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);    
    XMLNode date("h3", "Last update " + string(asctime(timeinfo)));
    body.AddChild(headerTitle).AddChild(date);
    for (int i = 0; i<stats.size(); i++) {
        body.AddChild(XMLNode("a",stats.at(i).name).AddAttribute("href","http://osmfuel.net46.net/"+stats.at(i).link)).AddChild(XMLNode("br"));
    }
    body.AddChild(counter);
    page.AddChild(body);    
    string srt = page.ToString();
    file.write(srt.c_str());
    file.close();
}

XMLNode::XMLNode(const string& name, const string& value) {
    mName = name;
    mValue = value;
}
XMLNode::XMLNode(const string& name) {
    mName = name;
}

XMLNode& XMLNode::AddChild(const XMLNode& newbie) {
    mChildren.push_back(newbie);
    return *this;
}

XMLNode& XMLNode::AddAttribute(const string& attr, const string& value) {
    mAttr.insert(std::pair<string, string>(attr, value));
    return *this;
}

string XMLNode::ToString() const {
    string ret;
    ret = "<" + mName;
    for (auto it = mAttr.cbegin(); it != mAttr.cend(); ++it)
        ret = ret + " " + (*it).first + "=\"" + (*it).second + "\"";
    if (mChildren.size()==0 && mValue.empty()) {
        ret += "/>"; // empty node
    }
    else {
        ret +=">";
        if (!mValue.empty()) { // have name - draw name
            ret += mValue;
        }
        else { // otherwise try children
            for (int i = 0; i< mChildren.size(); i++) {
                ret = ret + "\n" + mChildren.at(i).ToString();
            }
            ret+="\n";
        }
        ret = ret + "</" + mName + ">";
    }
    return ret;
}

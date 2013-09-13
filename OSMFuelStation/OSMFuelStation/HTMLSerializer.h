#pragma once
#include <map>
#include "ComparedPair.h"
#include "RegionStats.h"

class HTMLSerializer
{
public:
    HTMLSerializer(void);
    ~HTMLSerializer(void);
    static void SaveRegionToHTML(const string& path, const vector<ComparedPair*> pairs, const string& header = "");
    static void SaveRootToHTML(const string& path, const vector<RegionStats>& stats, const string& header = "");
};

class XMLNode {
public:
    XMLNode(const string& name, const string& value);
    XMLNode(const string& name);
    XMLNode& AddChild(const XMLNode& newbie);
    XMLNode& AddAttribute(const string& attr, const string& value);
    string ToString() const;
private:
    string mName;
    string mValue;
    map<string,string> mAttr;
    vector<XMLNode> mChildren;
};

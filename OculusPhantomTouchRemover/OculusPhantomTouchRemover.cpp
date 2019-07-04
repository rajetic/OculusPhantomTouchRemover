////////////////////////////////////////////////////////////
// Oculus Phantom Touch Remover
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// Released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <shlobj_core.h>
#include "windows.h"

using namespace rapidjson;
using namespace std;

int main()
{
	PWSTR p;

	SHGetKnownFolderPath(FOLDERID_LocalAppData,0,0,&p);
	wstring path(p);
	CoTaskMemFree(p);
	path+= L"\\Oculus\\DeviceCache.json";

	std::ifstream inputFile(path);
	if (!inputFile.good())
	{
		MessageBox(0, L"Failed to open DeviceCache.json for reading",L"Open Failed", MB_OK);
		return 0;
	}
	IStreamWrapper isw(inputFile);
	Document doc;
	doc.ParseStream(isw);
	inputFile.close();
	int count = 0;
	Value& devices = doc["devices"];
	for (Value::ConstValueIterator it = devices.Begin();it!= devices.End();)
	{
		const rapidjson::Value& device = *it;
		cout << "Type:" << device["type"].GetString() << " SubType:" << device["subtype"].GetString() << " Tracking:"<< device["trackingType"].GetString()<<" ID:" << device["id"].GetString();
		if (string(device["type"].GetString()) == "controller" && 
			(
				((string(device["subtype"].GetString()) == "rlcon" || string(device["subtype"].GetString()) == "llcon") && string(device["trackingType"].GetString()) == "unknown")
				|| 
				string(device["subtype"].GetString()) == "unknown"
			)
		)
		{
				//cout << device["type"].GetString() << " --- " << device["id"].GetString() << endl;
				it = devices.Erase(it);
				cout << " (INVALID)" << endl;
				count++;
		}
		else
		{
			cout << endl;
			++it;
		}
	}
	cout << endl << "Removed " << count << " phantom touch controllers" << endl;
	wstringstream wss;
	wss << "Removed " << count << " phantom touch controllers";
	MessageBox(0, wss.str().c_str(), L"Result", MB_OK);

	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);
	doc.Accept(writer);
	std::fstream outputFile(path,std::ios::out);
	if (!outputFile.good())
	{
		MessageBox(0, L"Failed to open DeviceCache.json for writing", L"Save Failed", MB_OK);
		return 0;
	}
	outputFile << sb.GetString();

	return 0; 
}

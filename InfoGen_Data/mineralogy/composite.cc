#include "composite.h"
#include "final.h"

#include "../gbuffer_planet.h"
#include "../composite.h"
#include "../composite2.h"
#include "../final.h"

#include <set>

using namespace std;
using namespace cse;

ObjectBuffer TerrestrialPlanetsBuf;
ObjectBuffer SmallMoonBuf;
ObjectBuffer MinorPlanetsBuf;

#define MAX_MINERAL_NUMBER_MAJOR 10
#define MAX_MINERAL_NUMBER_MINOR 5

namespace Localization
{
	string Min_Title   = "Mineral distribution in planetary system \"{}\"";
	string Min_STitle1 = "Major planets and moons";
	string Min_STitle2 = "Minor planets";
	string Min_STitle3 = "Minor moons";
	string Min_Name    = "Name";
	string Min_Parent  = "Parent";
	string Min_Class   = "Class";
	string Min_MinList = "List of minerals";
}

using namespace Localization;

bool IsRockyPlanet(shared_ptr<Object> Obj)
{
	return Obj->Class == "Ferria" || Obj->Class == "Terra" || Obj->Class == "Aquaria" || Obj->Class == "Carbonia" || Obj->Class == "Asteroid";
}

void FindRockyPlanets(const ObjectBuffer& Src, const map<string, vector<size_t>>& Indices)
{
	auto it0 = Indices.find("Planet");
	auto it1 = Indices.find("DwarfPlanet");
	auto it2 = Indices.find("Moon");
	auto it3 = Indices.find("DwarfMoon");
	auto it4 = Indices.find("Asteroid");
	auto it5 = Indices.find("Comet");
	auto end = Indices.end();

	auto PushObject = [](const ObjectBuffer& Src0, map<string, vector<size_t>>::const_iterator& it, const map<string, vector<size_t>>::const_iterator& end0, ObjectBuffer* Dst, function<bool(shared_ptr<Object>)> _PFunc)
	{
		if (it != end0)
		{
			for (size_t i = 0; i < it->second.size(); i++)
			{
				if (_PFunc(make_shared<Object>(Src0[it->second[i]]))) 
				{
					Dst->push_back(Src0[it->second[i]]);
				}
			}
		}
	};

	switch (MinorPlanetSortArg)
	{
	case MPS_Diam:
	default:
		PushObject(Src, it0, end, &TerrestrialPlanetsBuf, IsRockyPlanet);
		PushObject(Src, it1, end, &MinorPlanetsBuf, DiameterLargerThan120);
		PushObject(Src, it2, end, &TerrestrialPlanetsBuf, IsRockyPlanet);
		PushObject(Src, it3, end, &SmallMoonBuf, DiameterLargerThan120);
		PushObject(Src, it4, end, &MinorPlanetsBuf, DiameterLargerThan120);
		PushObject(Src, it5, end, &MinorPlanetsBuf, DiameterLargerThan120);
		break;
	case MPS_Mass:
		PushObject(Src, it0, end, &TerrestrialPlanetsBuf, IsRockyPlanet);
		PushObject(Src, it1, end, &MinorPlanetsBuf, MassLargerThan1E18);
		PushObject(Src, it2, end, &TerrestrialPlanetsBuf, IsRockyPlanet);
		PushObject(Src, it3, end, &SmallMoonBuf, DiameterLargerThan120);
		PushObject(Src, it4, end, &MinorPlanetsBuf, MassLargerThan1E18);
		PushObject(Src, it5, end, &MinorPlanetsBuf, MassLargerThan1E18);
		break;
	}
}

set<string> SelectMinerals(const vector<string>& Dict, size_t N)
{
	if (N > Dict.size()) { N = Dict.size(); }
	set<string> Buf;
	while (Buf.size() < N)
	{
		Buf.insert(random.choice<vector<string>::const_iterator, string>(Dict.begin(), Dict.end()));
	}
	return Buf;
}

string RandMineralStr(size_t N)
{
	vector<string> OreD2;
	auto it0 = OreDict.begin();
	auto end0 = OreDict.end();
	while (it0 != end0)
	{
		OreD2.push_back(*it0);
		++it0;
	}

	set<string> Buf = SelectMinerals(OreD2, random.randint(0, N));
	ostringstream os;
	auto it = Buf.begin();
	auto end = Buf.end();
	int Idx = 0;
	while (it != end)
	{
		os << *it;
		if (Idx < Buf.size() - 1) { os << ", "; }
		++Idx;
		++it;
	}
	return os.str();
}

string GHMDGenMinerals(const ObjectBuffer& Objects, size_t N)
{
	ostringstream os;
	os << "| " + Min_Name + " | " + Min_Parent + " | " + Min_Class + " | " + Min_MinList + " |\n";
	os << "|:---|:---|:---|:---|\n";
	string fmtstr = "| {} | {} | {} | {} |\n";
	for (size_t i = 0; i < Objects.size(); i++)
	{
		os << vformat(fmtstr, make_format_args
		(
			Objects[i].Name[0],
			Objects[i].ParentBody,
			GenPlanetType(make_shared<Object>(Objects[i])),
			RandMineralStr(N)
		));
	}
	return os.str();
}

string GHMDGenMineralsMinor(const ObjectBuffer& Objects, size_t N)
{
	ostringstream os;
	os << "| " + Min_Name + " | " + Min_Parent + " | " + Min_MinList + " |\n";
	os << "|:---|:---|:---|\n";
	string fmtstr = "| {} | {} | {} |\n";
	for (size_t i = 0; i < Objects.size(); i++)
	{
		os << vformat(fmtstr, make_format_args
		(
			Objects[i].Name[0],
			Objects[i].ParentBody,
			RandMineralStr(N)
		));
	}
	return os.str();
}

string GHMDGenMineral()
{
	ObjectBuffer::iterator SystemBarycenter;
	try { SystemBarycenter = FindSystemBarycenter(System); }
	catch (exception e)
	{
		cout << e.what();
		abort();
	}

	ostringstream os;
	os << vformat("# " + Min_Title + "\n", make_format_args(SystemBarycenter->Name[0]));
	FindRockyPlanets(System, TypeIndices);
	os << "## " + Min_STitle1 + "\n";
	os << GHMDGenMinerals(TerrestrialPlanetsBuf, MAX_MINERAL_NUMBER_MAJOR);
	os << "## " + Min_STitle2 + "\n";
	os << GHMDGenMineralsMinor(MinorPlanetsBuf, MAX_MINERAL_NUMBER_MINOR);
	os << "## " + Min_STitle3 + "\n";
	os << GHMDGenMineralsMinor(SmallMoonBuf, MAX_MINERAL_NUMBER_MINOR);

	os << vformat("----------\nGenerator seed : `{:X}`\n", make_format_args(random.seed()));
	return os.str();
}

void GetLocalePlanet();
void GetLcString(string Key, string* Val);

void GetLocalMin()
{
	GetLcString("Min_Title",   &Min_Title);
	GetLcString("Min_STitle1", &Min_STitle1);
	GetLcString("Min_STitle2", &Min_STitle2);
	GetLcString("Min_STitle3", &Min_STitle3);
	GetLcString("Min_Name",    &Min_Name);
	GetLcString("Min_Parent",  &Min_Parent);
	GetLcString("Min_Class",   &Min_Class);
	GetLcString("Min_MinList", &Min_MinList);
}

void composite0min()
{
	GetLocalePlanet();
	GetLocalMin();

	switch (OFormat)
	{
	case HTML:
		break;
	case MD:
		Final += GHMDGenMineral();
		break;
	default:
		break;
	}
}
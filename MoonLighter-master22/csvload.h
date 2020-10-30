#pragma once
#pragma warning(disable:4996)
#include "item.h"
// 2¾È
#define GENERATESTRUCT(Name, Column, DataType) \
	string a(#Name); \
	int index = 0; \
	a.append(".h"); \
	FILE* file = fopen(a.c_str(), "wt+"); \
	fprintf(file, "#pragma once\n"); \
	fprintf(file, "#include <string>\n"); \
	fprintf(file, "#include <cstdlib>\n"); \
	fprintf(file, "using namespace std;\n\n"); \
	fprintf(file, "enum E%s{\n", #Name); \
	while (index < column.size()) \
	{ \
		fprintf(file, "\te%s_%s,\n", #Name, column[index].c_str()); \
		++index; \
	} \
	fprintf(file, "};\n\n"); \
	fprintf(file, "class %s{\n", #Name); \
	 \
	fprintf(file, "public:\n"); \
	index = 0; \
	while (index < column.size()) \
	{ \
		fprintf(file, "\t%s Get%s() const { return %s;}\n", DataType[index].c_str(), column[index].c_str(), column[index].c_str()); \
		fprintf(file, "\tvoid Set%s(%s In%s){%s = In%s;}\n\n", column[index].c_str(), DataType[index].c_str(), column[index].c_str(), column[index].c_str(), column[index].c_str()); \
		++index; \
	} \
	index = 0; \
	fprintf(file, "\tvoid FillData(std::vector<string>& In){\n"); \
	while (index < column.size()) \
	{ \
		if(!DataType[index].compare("int")) \
			fprintf(file, "\t\tSet%s(atoi(In[e%s_%s].c_str()));\n", column[index].c_str(), #Name, column[index].c_str()); \
		else if(!DataType[index].compare("float")) \
			fprintf(file, "\t\tSet%s(atof(In[e%s_%s].c_str()));\n", column[index].c_str(), #Name, column[index].c_str()); \
		else \
			fprintf(file, "\t\tSet%s(In[e%s_%s].c_str());\n", column[index].c_str(), #Name, column[index].c_str()); \
		++index; \
	} \
	\
	fprintf(file, "\t}\n"); \
	fprintf(file, "private:\n"); \
	index = 0; \
	while (index < column.size()) \
	{ \
		fprintf(file, "\t%s %s;\n", DataType[index].c_str(), column[index].c_str()); \
		++index; \
	} \
	fprintf(file, "};"); \
	fclose(file);

class CSVData {
public:
	static bool parse(FILE* fp, vector<string>& inValue)
	{
		char buffer[1000];
		char* end = fgets(buffer, sizeof(buffer), fp);
		if (end == nullptr)
			return false;

		char token[] = ",";
		char* pData = nullptr;

		inValue.push_back(strtok(buffer, token));

		while (pData = strtok(nullptr, token))
		{
			inValue.push_back(pData);
		}
		string& lastStr = inValue[inValue.size() - 1];
		lastStr.erase(lastStr.size() - 1);

		return true;
	}

	vector<string>& GetColumnVec() { return column; }
	vector<string>& GetDataTypeVec() { return DataType; }
	vector<string>& GetDataVec(int key) { return datas.at(key); }

	void insertData(int key, vector<string>& inVec)
	{
		datas.insert(make_pair(key, inVec));
	}

private:
	vector<string> column;
	vector<string> DataType;
	unordered_map<int, vector<string>> datas;
};

#define FILL(structure, column, datatype, data) \
	if(datatype.compare("float")) \
		structure.Set##column(atof(data.c_str())); \
	else if(datatype.compare("int")) \
		structure.Set##column(atoi(data.c_str())); \
	else \
		structure.Set##column(data.c_str());
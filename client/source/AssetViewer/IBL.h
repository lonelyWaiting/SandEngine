#pragma once

void IBLGenerateLutMap(const char* path);
void IBLGenerateIrradianceMap(const char* path);
void IBLGeneratePrefilterMap(const char* path);
void IBLInit();
void IBLDeInit();

//class IBL
//{
//public:
//	IBL() {}
//	
//public:
//	void GenerateLutMap();
//	void GenerateIrradianceMap();
//	void GeneratePrefiliterMap();
//
//public:
//	void GetBRDFLutMap();
//	void GetIrradianceMap();
//	void GetPrefilterMap();
//
//	void SaveBRDFLutMap();
//	void SaveIrradianceMap();
//	void SavePrefilterMap();
//
//private:
//	char mPath[1024];
//};
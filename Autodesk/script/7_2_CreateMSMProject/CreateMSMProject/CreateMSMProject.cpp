// CreateMSMProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdlib.h> 

using namespace std;

string amake_template = "C:\\Users\\weiyu\\Desktop\\script\\template\\amake.bat";
string msmWxi_template = "C:\\Users\\weiyu\\Desktop\\script\\template\\msmProject.wxi";
string msmWxs_template = "C:\\Users\\weiyu\\Desktop\\script\\template\\msmProject.wxs";

string include_dir_folder= "C:\\Users\\weiyu\\Desktop\\script\\5_output_wix_include_dir\\";

// for write to msmProject .wxi file
string wix_fragments_location = "$(env.WixFragmentsACADE)\\ProjectCentric\\node_modules\\";


string output_folder = "C:\\Users\\weiyu\\Desktop\\script\\7_1_output_NM_msmProject\\";
string msm_Project_location = "$(env.ACE_WixProjects)\\wixMsmProjects\\";
string uuidgen = "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Bin\\x64\\uuidgen.exe";
string uuidtxt = "C:\\Users\\weiyu\\Desktop\\script\\7_1_output_uuid.txt";

void createAMake(string target, string project_name);
void createMSMProjectWIX(string target, string project_name);
void createMSMProjectWXS(string target, string include_dir_folder, string project_name, string uuid64, string uuid32);

int _tmain(int argc, _TCHAR* argv[])
{

	string msmProject = "C:\\Users\\weiyu\\Desktop\\script\\7_1_output_msmProject_list.txt";
	
	ifstream msm_list(msmProject);
	ifstream uuid(uuidtxt);
	string line;

	if(msm_list.is_open()) // find path of wix files to be processed
	{
		while (std::getline(msm_list, line))
		{
			// For each file

			// Each line is a folder/wxs name, with hyphen(-)
			string target_folder = output_folder + line;

			// replace hyphen with underscore in folder name coz module name in Modules.xml doesn't support it
			string new_folder = target_folder;
			std::replace( new_folder.begin(), new_folder.end(), '-', '_'); 
			rename(target_folder.c_str(), new_folder.c_str());

			std::replace( target_folder.begin(), target_folder.end(), '-', '_'); 
			string target_amake = target_folder + "\\amake.bat";
			string target_msmWxi = target_folder + "\\msmProject.wxi";
			string target_msmWxs = target_folder + "\\msmProject.wxs";

			string uuid64;
			string uuid32;
		    std::getline(uuid, uuid64);
			std::getline(uuid, uuid32);
		
			createAMake(target_amake, line);
			createMSMProjectWIX(target_msmWxi, line);
			createMSMProjectWXS(target_msmWxs, include_dir_folder, line, uuid64, uuid32);



		}
	}
	uuid.close();
	msm_list.close();

	return 0;
}

void createAMake(string target, string project_name)
{
	std::ofstream ofs;
	ofs.open(target, std::ofstream::out | std::ofstream::trunc);

	ifstream ifs(amake_template);
	if (ifs.is_open())
	{
		string line;
		while(getline(ifs, line))
		{
			string str = "dummy_name";
			size_t replace_start = line.find("dummy_name");
			if(replace_start != std::string::npos )
			{
				line.replace(replace_start, str.length(), project_name);
			}
			ofs<<line<<endl;
		}
	}
	ofs.close();
	ifs.close();
}

void createMSMProjectWIX(string target, string project_name)
{
	std::ofstream ofs;
	ofs.open(target, std::ofstream::out | std::ofstream::trunc);

	ifstream ifs(msmWxi_template);
	if (ifs.is_open())
	{
		string line;
		string project_name_original = project_name; 
		// cannot have hyphen in ID, replace all '-' to '_'
		std::replace( project_name.begin(), project_name.end(), '-', '_'); 
		while(getline(ifs, line))
		{
			string str;
			size_t replace_start;


			if(line.find("dummyComponentGroupId") != std::string::npos )
			{
				str = "dummyComponentGroupId";
				replace_start = line.find("dummyComponentGroupId") ;
				line.replace(replace_start, str.length(), project_name);
			}
			else if(line.find("dummyComponentGroupRefId") != std::string::npos)
			{
				str = "dummyComponentGroupRefId";
				replace_start = line.find("dummyComponentGroupRefId") ;
				line.replace(replace_start, str.length(), project_name);
			}
			else if(line.find("dummyFragment") != std::string::npos)
			{
				string substr = "dummyFragment";
				string targetstr = "  <?include $(env.WixFragmentsACADE)\\ProjectCentric\\node_modules\\dummyFragment.wxs?>";
				size_t substr_pos = targetstr.find(substr);
				targetstr.replace(substr_pos, substr.length(), project_name_original);
				line = targetstr;
				//line = "  <?include " + my_wix_location + "?>";
			}

			ofs<<line<<endl;
		}
	}
	ofs.close();
	ifs.close();
}

void createMSMProjectWXS(string target, string include_dir_folder, string project_name, string uuid64, string uuid32)
{
	//long int guid1 = std::system(uuidgen.c_str());
	//long int guid2 = std::system(uuidgen.c_str());
	std::ofstream ofs;
	ofs.open(target, std::ofstream::out | std::ofstream::trunc);

	ifstream ifs(msmWxs_template);
	if (ifs.is_open())
	{
		string line;
		string project_name_original = project_name; 
		// cannot have hyphen in ID, replace all '-' to '_'
		std::replace( project_name.begin(), project_name.end(), '-', '_'); 
		while(getline(ifs, line))
		{
			if(line.find("module_id") != std::string::npos )
			{
				string substr = "module_id";
				size_t substr_pos = line.find("module_id");
				while (substr_pos != std::string::npos)
				{
					line.replace(substr_pos, substr.length(), project_name);
					substr_pos = line.find("module_id");

				}
			}

			if(line.find("SUB_DIR_ACE_DIR_dummy") != std::string::npos )
			{
				string include_line;
				ifstream include_file(include_dir_folder + project_name_original + ".txt");
				if (include_file.is_open())
				{
					while(getline(include_file, include_line))
					{
						ofs<<"    "<<include_line<<endl;
					}

				}

				//line = "    <?include " + my_dir_location + "?>";
			    continue;
			}

			if(line.find("GUID64") != std::string::npos )
			{
				string substr = "GUID64";
				size_t substr_pos = line.find("GUID64");

				line.replace(substr_pos, substr.length(), uuid64);
			}

			if(line.find("GUID32") != std::string::npos )
			{
				string substr = "GUID32";
				size_t substr_pos = line.find("GUID32");

				line.replace(substr_pos, substr.length(), uuid32);
			}

			if(line.find("dummy_project") != std::string::npos )
			{
				string substr = "dummy_project";
				string targetstr = "  <?include $(env.ACE_WixProjects)\\wixMsmProjects\\ProjectCentric\\node_modules\\dummy_project\\msmProject.wxi?>";
				size_t substr_pos = targetstr.find(substr);
				targetstr.replace(substr_pos, substr.length(), project_name);
				line = targetstr;

				//line = "  <?include " + wxi_wrapper + "?>";

			}

			if(line.find("ACE_PRODUCTVERSION") != std::string::npos )
			{
				string substr = "ACE_PRODUCTVERSION";
				size_t substr_pos = line.find("ACE_PRODUCTVERSION");

				line.replace(substr_pos, substr.length(), "ACE_VERSION");

			}
			

			ofs<<line<<endl;

		}
	}
	ofs.close();
	ifs.close();
}

// replace hyphen with underscore in folder name
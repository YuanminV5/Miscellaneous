// Create_Include.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdio.h>
using namespace std;

bool boIDRecorded(string file, string directoryid);
void write_include(string file, string directoryid);
string getSubfolderForID(string dir_id);

string include_template = "<?include $(env.ACE_WixProjects)\\_common\\dirFragments\\ProjectCentric\\node_modules\\subfolder_placeholder\\SUB_DIR_dummyname.wxi?>";
string include_template_root = "<?include $(env.ACE_WixProjects)\\_common\\dirFragments\\ProjectCentric\\node_modules\\SUB_DIR_dummyname.wxi?>";
string include_file = "C:\\Users\\weiyu\\Desktop\\script\\5_output_wix_include_dir\\";
string all_in_one_include = "C:\\Users\\weiyu\\Desktop\\script\\5_output_wix_include_dir\\all_in_one.txt";
int _tmain(int argc, _TCHAR* argv[])
{

	string wixfile_list_path = "C:\\Users\\weiyu\\Desktop\\script\\4_output_for_step5_creat_include.txt";

	ifstream wixfile_list(wixfile_list_path);


	string wixfile_path;

	if(wixfile_list.is_open()) // find path of wix files to be processed
	{
		remove(all_in_one_include.c_str());
		while (std::getline(wixfile_list, wixfile_path))// 
		{
			//wixfile_path.replace();
			ifstream wixfile(wixfile_path); // open a wix file
			string line;
			if (wixfile.is_open())
			{
				string outstring = include_file;

				size_t path_end_pos = wixfile_path.find_last_of("\\"); // file path ends at
				size_t string_end = wixfile_path.length() - 1;
				// file_name has extension, i.e. bin.wxi
				string file_name = wixfile_path.substr(path_end_pos+1,string_end - path_end_pos);
				string name = file_name;

				size_t extension_separator_pos = file_name.find_last_of(".");
				if(extension_separator_pos!= std::string::npos)
				{
					// file has extension, remove it
					name = file_name.substr(0, extension_separator_pos);

				}
				else
				{
					// file doesn't have extension, this is not possible
				}

				outstring += name;
				outstring += ".txt";

				remove(outstring.c_str());


				while (std::getline(wixfile, line)) // read wix file line by line
				{
					if (line.find("ACE_DIR_NM") != std::string::npos) // if this line contains Component Id
					{
						// if this line contains Component Id, add Directory="ID" for it
						// ID can be obtained by looking up mapping file
						size_t id_start_pos = line.find("ACE_DIR_NM");
						size_t id_end_pos = line.find_last_of("\"");
						string directoryid = line.substr(id_start_pos, id_end_pos - id_start_pos);

						if(!boIDRecorded(outstring, directoryid))
						{
							write_include(outstring, directoryid);
							write_include(all_in_one_include, directoryid);
						}

					}
	
					
					
				}


			}
			wixfile.close();
		} // while loop in wixfile list

	}
	wixfile_list.close();





				
	return 0;
}

bool boIDRecorded(string file, string directoryid)
{
	ifstream include_file(file); 
	bool returnValue = false;

	if (include_file.is_open())
	{		
		// file exists, check if id already recorded
		string line;
		while(getline(include_file, line))
		{

			if (line.find(directoryid) != string::npos) 
			{
				// recorded, return true
				returnValue = true;
				break;
			}
		}

		include_file.close();
	}

	else
	{
		// file doesn't exist, this is first time call
		returnValue = false;
	}
	return returnValue;
}
void write_include(string file, string directoryid)
{
	string line = include_template;
	string subfolder = getSubfolderForID(directoryid);

	if(subfolder == "node_modules")
	{
		//line = include_template_root;
	}
	

	std::ofstream include_file;
	include_file.open(file, std::ofstream::out | std::ofstream::app);


	string replace_substr = "dummyname";

	size_t replace_pos = line.find(replace_substr);
	if(replace_pos != std::string::npos)
	{
		line.replace(replace_pos, replace_substr.length(), directoryid);
	}

	replace_substr = "subfolder_placeholder";

	replace_pos = line.find(replace_substr);
	if(replace_pos != std::string::npos)
	{
		line.replace(replace_pos, replace_substr.length(), subfolder);
	}
	include_file<<line<<endl;

	include_file.close();

}

string getSubfolderForID(string dir_id)
{
	string subfolder =".";

	string mapping_file="C:\\Users\\weiyu\\Desktop\\script\\2_output_for_step6_dirfragment.txt";

	ifstream mapping(mapping_file);

	string line;

	if(mapping.is_open()) // find path of wix files to be processed
	{
		while (std::getline(mapping, line))
		{
			//DirectoryID delimiter1 name delimiter2 ParentDirectoryID delimiter3 groupfolder
			size_t delimiter1_pos = line.find("&");
			size_t delimiter3_pos = line.find("*");

			string directoryid = line.substr(0, delimiter1_pos);
			string sub_folder = line.substr(delimiter3_pos+1, line.length() - delimiter3_pos - 1);

			if(directoryid  == dir_id)
			{
				subfolder = sub_folder;
				break;
			}
		}
	}

	return subfolder;
}
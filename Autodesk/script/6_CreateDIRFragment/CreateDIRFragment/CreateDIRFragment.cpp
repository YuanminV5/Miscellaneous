// CreateDIRFragment.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdlib.h> 
#include <stdio.h> 
using namespace std;

string dirfragment_template = "C:\\Users\\weiyu\\Desktop\\script\\template\\SUB_DIR_template.wxi";
string dirfragment_file = "C:\\Users\\weiyu\\Desktop\\script\\2_output_for_step6_dirfragment.txt";
string target_folder = "C:\\Users\\weiyu\\Desktop\\script\\6_output_NM_dirfragment\\";
string getSubfolderForID(string dir_id);
bool isSubRoot(string id);

int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0;

	ifstream dirfragment(dirfragment_file);

	string line;

	if(dirfragment.is_open()) // find path of wix files to be processed
	{
		while (std::getline(dirfragment, line))
		{
			count++;
			//DirectoryID delimiter1 name delimiter2 ParentDirectoryID
			size_t delimiter1_pos = line.find("&");
			size_t delimiter2_pos = line.find("$");
			size_t delimiter3_pos = line.find("*");
			string directoryid = line.substr(0, delimiter1_pos);
			string folder_name = line.substr(delimiter1_pos+1, delimiter2_pos - delimiter1_pos - 1);
			string parent_directoryid = line.substr(delimiter2_pos + 1, delimiter3_pos - delimiter2_pos - 1);

			string subfolder = getSubfolderForID(directoryid);

			string target;
		
			if(subfolder == "node_modules")
			{
				//string sub = folder_name;
				//sub.erase(std::remove(sub.begin(), sub.end(), '.'), sub.end());
				//std::replace( sub.begin(), sub.end(), '-', '_'); 

				//target = target_folder +"\\" + folder_name + "\\" + "SUB_DIR_" + directoryid + ".wxi";

				target = target_folder +"SUB_DIR_" + directoryid + ".wxi";
			}
			else if(subfolder == "ProjectCentricServer")
			{
				target = target_folder +"SUB_DIR_" + directoryid + ".wxi";
			}
			else
			{
				target = target_folder + "\\" + subfolder + "\\" + "SUB_DIR_" + directoryid + ".wxi";
			}

			//string target = target_folder + std::to_string(count) +"SUB_DIR_" + directoryid + ".wxi";
			
			// Write to template

			// check existence first
			ifstream include_file(target); 

			if (include_file.is_open())
			{
				//target += std::to_string(count);
				target += "2";
				include_file.close();
			}
			
			//remove(target.c_str());
			std::ofstream ofs;
			ofs.open(target, std::ofstream::out | std::ofstream::trunc);

			ifstream ifs(dirfragment_template);
			if (ifs.is_open())
			{
				string line;
				while(getline(ifs, line))
				{
					size_t replace_start = std::string::npos;
					string str;
					if(line.find("MyDirectoryID")  != std::string::npos)
					{
						str = "MyDirectoryID";
						replace_start = line.find("MyDirectoryID");
						line.replace(replace_start, str.length(), directoryid);
					}

					// 	<?include $(env.ACE_WixProjects)/_common/dirFragments/ParentDirFragment.wxi?>
					if(line.find("ParentDirFragment")  != std::string::npos)
					{
						if(parent_directoryid == "ACE_DIR_NM_ProjectCentricServer")
						{
							string root = "    <?include $(env.ACE_WixProjects)/_common/dirFragments/SUB_DIR_ACE_DIR_ProjectCentricServer.wxi?>";
							ofs<<root<<endl;
							continue;
						}
						else if(parent_directoryid == "ACE_DIR_NM_NM")
						{
							string root = "    <?include $(env.ACE_WixProjects)/_common/dirFragments/ProjectCentric/SUB_DIR_ACE_DIR_NM_NM.wxi?>";
							ofs<<root<<endl;
							continue;
						}
						else if (isSubRoot(parent_directoryid))
						{
							string root;
							root = "    <?include $(env.ACE_WixProjects)/_common/dirFragments/ProjectCentric/node_modules/SUB_DIR_" + parent_directoryid + ".wxi?>";
							ofs<<root<<endl;
							continue;
						}

						str = "ParentDirFragment";
						string dirFragmentFile = subfolder + "/" +"SUB_DIR_" + parent_directoryid;
						replace_start = line.find("ParentDirFragment");
						line.replace(replace_start, str.length(), dirFragmentFile);
					}
					
					if(line.find("ParentDir")  != std::string::npos)
					{						
						if(parent_directoryid == "ACE_DIR_NM_ProjectCentricServer")
						{
							parent_directoryid = "ACE_DIR_ProjectCentricServer";
						}

						str = "ParentDir";
						replace_start = line.find("ParentDir");
						line.replace(replace_start, str.length(), parent_directoryid);
					}

				    if(line.find("MyDirectoryName")  != std::string::npos)
					{
						str = "MyDirectoryName";
						replace_start = line.find("MyDirectoryName");
						line.replace(replace_start, str.length(), folder_name);
					}

					ofs<<line<<endl;
				}
			}
			ofs.close();
			ifs.close();
			cout<<"line count is: "<<count<<endl;
		}
	}
	dirfragment.close();

	return 0;
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

bool isSubRoot(string id)
{
	bool returnVal = false;
	string subroots[] = { 
		"ACE_DIR_NM_bin", 
		"ACE_DIR_NM_cha", 
		"ACE_DIR_NM_exp",
		"ACE_DIR_NM_liv",
		"ACE_DIR_NM_mic", 
		"ACE_DIR_NM_moc", 
		"ACE_DIR_NM_nco", 
		"ACE_DIR_NM_NM_liv", 
		"ACE_DIR_NM_NM_nod", 
		"ACE_DIR_NM_NM_sha",
		"ACE_DIR_NM_sha", 
		"ACE_DIR_NM_sin", 
		"ACE_DIR_NM_NM_win",
		"ACE_DIR_NM_ws"
	};

	int size = sizeof(subroots)/sizeof(string);
	for(int cnt = 0; cnt < size; cnt++)
	{
		if(subroots[cnt] == id)
		{
			returnVal = true; 
			break;
		}
	}

	return returnVal;
}
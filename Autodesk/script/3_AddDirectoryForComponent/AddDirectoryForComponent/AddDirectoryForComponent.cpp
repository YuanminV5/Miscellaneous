// AddDirectoryForComponent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdio.h>

using namespace std;
string strExtractPathFromFileIDLine(string fileIDline);
string strFindDirectoryID(string ID_name_mapping, string path);


int _tmain(int argc, _TCHAR* argv[])
{
	string wixfile_list_path = "C:\\Users\\weiyu\\Desktop\\script\\1_output_wixfilelist.txt";
	string ID_name_mapping = "C:\\Users\\weiyu\\Desktop\\script\\2_output_ID_Name_mapping.txt";
	ifstream wixfile_list(wixfile_list_path);
	//string header = "<?include $(env.ACE_WixProjects)/_common/dirFragments/SUB_DIR_ACEProjectCentric.wxi?>";
	//string dir_fragment = "<?include $(env.ACE_WixProjects)/_common/dirFragments/SUB_DIR_ProjectCentricVariable.wxi?>";



	string wixfile_path;

	if(wixfile_list.is_open()) // find path of wix files to be processed
	{
		while (std::getline(wixfile_list, wixfile_path))// 
		{
			//wixfile_path.replace();
			ifstream wixfile(wixfile_path); // open a wix file
			string line;
			if (wixfile.is_open())
			{
				string outstring = "C:\\Users\\weiyu\\Desktop\\script\\3_output_final\\";

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
				outstring += ".wxs";

				remove(outstring.c_str());

				ofstream outfile(outstring, std::ofstream::out);

				while (std::getline(wixfile, line)) // read wix file line by line
				{
					if (line.find("Component Id") != std::string::npos) // if this line contains Component Id
					{
						// if this line contains Component Id, add Directory="ID" for it
						// ID can be obtained by looking up mapping file
						string fileIDline;
						std::getline(wixfile, fileIDline); // get file id line, extract path from it
						string path = strExtractPathFromFileIDLine(fileIDline);
						string directoryID = strFindDirectoryID(ID_name_mapping, path);
						

						// Compose string Directory="ID"
						directoryID = " Directory=\"" + directoryID + "\""; 
						// insert directoryID to line
						std::size_t insert_pos= line.find(">");
						line.insert(insert_pos, directoryID);
						outfile<<line<<endl;
						outfile<<fileIDline<<endl;
					}
					else
					{
						if (line.find("<Fragment>") != std::string::npos)
						{
							outfile<<line<<endl;

							size_t leading_dot_pos = file_name.find_first_of(".");
							string component_id = file_name;
							if(leading_dot_pos!= std::string::npos && leading_dot_pos == 0)
							{
								// file has extension, remove it
								component_id = file_name.erase(leading_dot_pos, 1);
							}
							//outfile<<header<<endl;
							//outfile<<dir_fragment<<endl;
							std::replace( component_id.begin(), component_id.end(), '-', '_'); 
							outfile<<"<ComponentGroup Id=\""<<component_id<<"\">"<<endl;
						}
						else if(line.find("</Fragment>") != std::string::npos)
						{
							outfile<<"</ComponentGroup>"<<endl;
							outfile<<line<<endl;

						}
						else if(line.find("<Wix") != std::string::npos || line.find("</Wix") != std::string::npos)
						{
							string str = "Wix";
							size_t replace_start = line.find("Wix");
							line.replace(replace_start, str.length(), "Include");
							outfile<<line<<endl;

						}
						else
						{
							outfile<<line<<endl;
						}

					}
					
				}
				outfile.close();

			}
			wixfile.close();
		} // while loop in wixfile list

	}
	wixfile_list.close();

	return 0;

}

string strExtractPathFromFileIDLine(string fileIDline)
{
	size_t start_pos = fileIDline.find("$");
	size_t end_pos = fileIDline.find_last_of("\\");
	string path = fileIDline.substr(start_pos, end_pos - start_pos);
	return path;
}
string strFindDirectoryID(string ID_name_mapping, string path)
{
	string directoryID = "";
	ifstream mapping_file(ID_name_mapping);
	// open mapping file 

	if (mapping_file.is_open())
	{
		string line;
		while(getline(mapping_file, line))
		{

			if (line.find(path) != string::npos) 
			{
				size_t delimiter_pos = line.find("&");
				directoryID = line.substr(0, delimiter_pos);
				break;
			}
		}
	}



	return directoryID;
}



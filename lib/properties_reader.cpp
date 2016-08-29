#include "include/properties_reader.h"

//Constructor
PropertiesReader::PropertiesReader(std::string file_path) {
  //Open the file
  std::string line;

  //Check if the file exists
  struct stat buffer;
  if (stat (file_path.c_str(), &buffer) == 0) {

    std::ifstream file (file_path);

    if (file.is_open()) {
      while (getline (file, line) ) {
        //Read a line from the property file

        //Figure out if we have a blank or comment line
        if (line.length() > 0) {
          if (line[0] != '/' || line[1] != '/') {
            std::size_t eq_pos = line.find("=", 0);
            if (eq_pos != std::string::npos) {
              std::string var_name = line.substr(0, eq_pos);
              std::string var_value = line.substr(eq_pos+1, line.length() - eq_pos);
              opts.emplace(var_name, var_value);
            }
            else
            {
              //We have a line that's non-blank and not a comment, as well as not a key-value pair
              //In this case, we assume that we have a list, which uses the syntax -list_name-list_value
              std::cout << "list case encountered: " << line << std::endl;
              std::size_t eq_pos = line.find("-", 1);
              if (eq_pos != std::string::npos) {
                std::string list_name = line.substr(1, eq_pos - 1);
                std::string list_value = line.substr(eq_pos+1, line.length() - eq_pos);
                if (!list_exist(list_name))
                {
                  //Create a new list and add it to the map
                  std::vector<std::string> val_list;
                  val_list.push_back(list_value);
                  opt_lists.emplace(list_name, val_list);
                  std::cout << "New List Created: " << list_name << std::endl;
                }
                else
                {
                  //Add to an existing list in the map
                  std::vector<std::string> val_list;
                  val_list = get_list(list_name);
                  val_list.push_back(list_value);
                  opt_lists[list_name] = val_list;
                  std::cout << "List Updated: " << list_name << std::endl;
                }
              }
            }
          }
        }
      }
      file.close();
    }
  }
}

  //Does a key exist?
bool PropertiesReader::opt_exist( std::string key )
{
  auto search = opts.find(key);
  if (search != opts.end())
  {
    return true;
  }
  return false;
}

//Does a key exist?
bool PropertiesReader::list_exist( std::string key )
{
auto search = opt_lists.find(key);
if (search != opt_lists.end())
{
  return true;
}
return false;
}

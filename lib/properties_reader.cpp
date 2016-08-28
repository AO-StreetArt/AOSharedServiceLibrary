#include "include/properties_reader.h"

//Constructor
PropertiesReader::PropertiesReader(std::string file_path) {
  //Open the file
  std::string line;

  struct stat buffer;
  if (stat (file_path.c_str(), &buffer) == 0) {

    std::ifstream file (file_path);

    if (file.is_open()) {
      while (getline (file, line) ) {
        //Read a line from the property file

        //Figure out if we have a blank or comment line
        bool keep_going = true;
        if (line.length() > 0) {
          if (line[0] != '/' || line[1] != '/') {
            int eq_pos = line.find("=", 0);
            std::string var_name = line.substr(0, eq_pos);
            std::string var_value = line.substr(eq_pos+1, line.length() - eq_pos);
            opts.emplace(var_name, var_value);
          }
        }
      }
      file.close();
    }
  }
  else
  {
    logging->error("CONFIGURE: Configuration File not found");
    return false;
  }
return true;
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

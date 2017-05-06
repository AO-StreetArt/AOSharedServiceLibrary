//tests for the configuration manager
//src/test/test.properties

#include "app_utils.h"
#include <assert.h>

int main( int argc, char** argv )
{
  std::string base = "  test  ";

  std::string cleaned_base = trim(base);

  assert ( cleaned_base == "test" );
  return 0;
}

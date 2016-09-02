#include "include/response.h"

void ApplicationResponse::set_error(int new_error_code, std::string new_error_message)
{
  error_code = new_error_code;
  if (new_error_code == NO_ERROR)
  {
    err_message = "";
  }
  else if (new_error_code == ERROR)
  {
    err_message = "Unspecified Error: " + new_error_message;
  }
  else if (new_error_code == DB_ERROR)
  {
    err_message = "Database Error: " + new_error_message;
  }
  else if (new_error_code == REDIS_ERROR)
  {
    err_message = "Redis Error: " + new_error_message;
  }
  else if (new_error_code == TRANSLATION_ERROR)
  {
    err_message = "Translation Error: " + new_error_message;
  }
  else if (new_error_code == BAD_SERVER_ERROR)
  {
    err_message = "Bad Server Error: " + new_error_message;
  }
  else if (new_error_code == BAD_REQUEST_ERROR)
  {
    err_message = "Bad Request Error: " + new_error_message;
  }
}

void ApplicationResponse::set_error(int new_error_code)
{
  error_code = new_error_code;
  if (new_error_code == NO_ERROR)
  {
    err_message = "";
  }
  else if (new_error_code == ERROR)
  {
    err_message = "Unspecified Error";
  }
  else if (new_error_code == DB_ERROR)
  {
    err_message = "Database Error";
  }
  else if (new_error_code == REDIS_ERROR)
  {
    err_message = "Redis Error";
  }
  else if (new_error_code == TRANSLATION_ERROR)
  {
    err_message = "Translation Error";
  }
  else if (new_error_code == BAD_SERVER_ERROR)
  {
    err_message = "Bad Server Error";
  }
  else if (new_error_code == BAD_REQUEST_ERROR)
  {
    err_message = "Bad Request Error";
  }
}

std::string ApplicationResponse::create_base_json()
{
  //Build the base JSON String
  std::string json_string = "{\"ErrorCode\": " + std::to_string(error_code) + ", \"ErrorMessage\": \"" + err_message +
    "\", \"TransactionID\": \"" + transaction_id + "\", \"ObjectID\": \"" + object_id + "\"";
  return json_string;
}

std::string ApplicationResponse::to_json()
{
  //Build the base JSON String
  std::string json_string = create_base_json() + "}";
  return json_string;
}

std::string ApplicationResponse::to_json(Writeable *obj)
{
  //Build the base JSON String
  std::string json_string = create_base_json() + ", \"ObjectData\": {" + obj->to_json() + "}}";
  return json_string;
}

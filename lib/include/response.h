#include <string>

#ifndef APPLICATION_RESPONSE_OBJECT
#define APPLICATION_RESPONSE_OBJECT

//! A struct that gets sent to clients to transmit errors, transaction ID's, and object ID's
class ApplicationResponse: public ApplicationResponseInterface
{
  //! A numerical error code
  int error_code;

  //! An Error message
  std::string err_message;

  //! A Transaction ID
  std::string transaction_id;

  //! An Object ID
  std::string object_id;

public:

  //! Constructor
  ApplicationResponse() {err_code = NO_ERROR; err_message = ""; transaction_id = ""; object_id = "";}

  //! Get Error Code
  int get_error_code() {return error_code;}

  //! Get Error Code
  std::string get_error_message() {return err_message;}

  //! Get Error Code
  std::string get_transaction_id() {return transaction_id;}

  //! Get Error Code
  std::string get_object_id() {return object_id;}

  //! Set the transaction ID
  void set_transaction_id(std::string new_tran_id) {transaction_id = new_tran_id;}

  //! Set the object ID
  void set_object_id(std::string new_obj_id) {object_id = new_obj_id;}

  //! Set the error
  void set_error(int new_error_code);
  void set_error(int new_error_code, std::string new_error_message);

  //! To JSON Method
  std::string to_json();
};

#endif

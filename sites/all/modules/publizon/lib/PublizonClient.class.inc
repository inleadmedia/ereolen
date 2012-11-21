<?php
/**
 * @file
 *
 */

/**
 *
 */
class PublizonClient {
  private $retailer_id;
  private $retailer_key_code;
  private $langcode;
  private $logger;

  private $base_url = '';
  private $end_points = array(
    'product' => 'getproduct.asmx',
    'product_list' => 'getproductlist.asmx',
    'loan' => 'createloan.asmx',
    'user' => 'validatelibraryuser.asmx',
    'libraray_list' => 'getlibrarylist.asmx',
    'library_user_order_list' => 'getlibraryuserorderlist.asmx',
    'removed_product_list' => 'getremovedproductlist.asmx',
  );

  // Holds the object instance (part of the singleton pattern).
  private static $instance = NULL;

  /**
   * Class construtor that prepares an connection to publizon and wrappers all
   * calls into this object. This is a private constructor as the object is
   * implemented as a singleton pattern, use PublizonClient::getClient() to get
   * an instance.
   *
   * If the parameters is not set (NULL) the object will try to use user provied
   * information stored in the users current session. If there non given and no
   * session it will fallback to the information given in the administration
   * form.
   *
   * @param string $retailer_id
   *  An id that represents the library at publizon.
   * @param string $retailer_key_code
   *  An hash value validating the library as publizon.
   * @param string $langcode
   *  The language code used to fetch the information ('da').
   * @param PublizonLogger $logger
   *  Publizon logger class.
   */
  private function __construct($retailer_id, $retailer_key_code, $langcode, $logger) {
    // Get publizon settings from the administration interface.
    $publizon = variable_get('publizon', FALSE);
    if (!$publizon) {
      throw new PublizonException('Publizon client could not load default settings.');
    }

    // Set base URL.
    $this->base_url = $publizon['settings']['base_url'];

    // Encode retailer key code (if needed).
    $retailer_key_code = isset($retailer_key_code) ? md5($retailer_key_code) : NULL;

    // Set configuration.
    $this->setSetting('langcode', $langcode);
    $this->setSetting('retailer_id', $retailer_id);
    $this->setSetting('retailer_key_code', $retailer_key_code);

    // Set logger.
    $this->logger = new PublizonVoidLogger();
    if (!isset($logger)) {
      if ($publizon['settings']['logging']) {
        $this->logger = new PublizonWatchDogLogger();
      }
    }
  }

  /**
   * Returns an instances of the PublizonClient object that can be used to
   * communicate with the Publizon web-service.
   *
   * @see PublizonClient().
   *
   * @param string $retailer_id
   *  An id that represents the library at publizon.
   * @param string $retailer_key_code
   *  An hash value validating the library as publizon.
   * @param string $langcode
   *  The language code used to fetch the information ('da').
   * @param PublizonLogger $logger
   *  Publizon logger class.
   */
  public static function getClient($retailer_id = NULL, $retailer_key_code = NULL, $langcode = NULL, $logger = NULL) {
    if (!isset(self::$instance)) {
      $class = __CLASS__;
      self::$instance = new $class($retailer_id, $retailer_key_code, $langcode, $logger);
    }
    return self::$instance;
  }

  public function createLoan() {
    throw new PublizonException('Not implemented yet');
  }

  public function getLastLoans() {
    throw new PublizonException('Not implemented yet');
  }

  public function getLastLoansByLibrary() {
    throw new PublizonException('Not implemented yet');
  }

  /**
   * Get list of last loaned products for a given type of product.
   *
   * @todo Get Publizon to make it possible to limit the number of elements in
   *       the response.
   *
   * @param int $list_type
   *  This can either be PUBLIZON_EBOOK or PUBLIZON_NETSOUND.
   * @param int $limit
   *  The number of products that should be returned. If the limit can not be
   *  reached, the number available will be returned.
   * @throws PublizonException
   *  Will be throw if the list type is unknown.
   */
  public function getLastLoansForAll($list_type, $limit = 7) {
    if ($list_type == PUBLIZON_EBOOK || $list_type == PUBLIZON_NETSOUND) {
      // Make request to the web-service for the loans.
      $response = $this->call('libraray_list', 'GetLastLoansForAll', array(
        'pub:listtype' => $list_type
      ), array('pub' => 'http://pubhub.dk/'));

      $products = array();
      $items_found = 0;

      // Convert XML into PublizonProduct objects, if data was returned.
      $data = $response->xpath('//data');
      if (isset($data[0])) {
        $data = $data[0];
        foreach ($data->orderinformationitem as $item) {
          $product = new PublizonProduct($item->identifier);
          if (isset($product->status)) {
            $products[] = $product;
            $items_found++;
          }
          // If we found the number of products we are looking for, break.
          if ($items_found == $limit) {
            break;
          }
        }
      }

      return $products;
    }
    $this->logger->log('Request for unsuported loans type ' . check_plain($list_type), 'ERROR');
    throw new PublizonException(t('You are trying to get unsupported type of loans from Publizon'));
  }

  public function getTopList() {
    throw new PublizonException('Not implemented yet');
  }

  public function getLibraryUserOrderList($card_number, $list_type) {
    if ($list_type == PUBLIZON_EBOOK || $list_type == PUBLIZON_NETSOUND) {
      // Make request to the web-service for the loans.
      $response = $this->call('library_user_order_list', 'GetLibraryUserOrderList', array(
        'pub:cardnumber' => $card_number,
        'pub:booktype' => $list_type,
      ), array('pub' => 'http://pubhub.dk/'));

      // Check if any data was returned.
      $data = $response->xpath('//data');
      $extension = $response->xpath('//LibraryExtension');
      if (isset($data[0]) && isset($extension[0])) {
        return array($data[0], $extension[0]);
      }

      $this->logger->log('No loans information was returned for the user', 'WARNING');
      return FALSE;
    }
    $this->logger->log('Request for unsuported loans type ' . check_plain($list_type), 'ERROR');
    throw new PublizonException(t('You are trying to get unsupported type of loans from Publizon'));
  }

  /**
   * Fetch XML representation for a product at publizon. You should not use this
   * function directly, but simple create a new PublizonProduct object. Which
   * will automatically fectch the product through this function.
   *
   * @see PublizonProduct()
   *
   * @param string $isbn
   *  Product id also know as ISBN number.
   * @return SimpleXmlElement
   *  Raw XML object containing the product.
   */
  public function getProduct($isbn) {
    // Get product form the web-serivce.
    $response = $this->call('product', 'GetProduct', array('pub:ebookid' => $isbn), array('pub' => 'http://pubhub.org/'));

    // Check if any data was returned.
    $data = $response->xpath('//data');
    if (isset($data[0])) {
      return $data[0];
    }

    $this->logger->log('The product with isbn (' . $isbn . ') did not return any data', 'WARNING');
    return FALSE;
  }

  public function getProductList() {
    throw new Exception('Not implemented yet');
  }

  public function getRemovedProductList() {
    throw new Exception('Not implemented yet');
  }

  /**
   * @TODO: Write doc.
   *
   * Notice that publizon for this call used another namespace.
   *
   * @param string $card_number
   * @param string $pincode
   */
  public function validateLibraryUser($card_number, $pincode) {
    try {
      $response = $this->call('user', 'ValidateLibraryUser', array(
        'tem:cardnumber' => $card_number,
        'tem:pincode' => $pincode,
      ), array('tem' => 'http://tempuri.org/'));
    }
    catch (PublizonException $e) {
      $this->logger->log($e->getMessage(), 'ERROR');
      return FALSE;
    }
    return TRUE;
  }

  private function call($endpoint, $action, $parameters, $namespace) {
    // Log request
    $this->logger->log('Call to ' . $endpoint . ' for ' . $action);

    // Get namespace key.
    $ns = array_keys($namespace);

    // Create a new soap client for the request and set default parameters.
    $client = new PublizonNanoSOAPClient($this->base_url . $this->end_points[$endpoint], array('namespaces' => $namespace));
    $parameters += array(
      $ns[0] . ':retailerid' => $this->retailer_id,
      $ns[0] . ':retailerkeycode' => $this->retailer_key_code,
      $ns[0] . ':languagecode' => $this->langcode,
    );

    // Make the request.
    try {
      $response = $client->call($ns[0] . ':' . $action, $parameters);
      $response = simplexml_load_string($response);
    }
    catch (Exception $e) {
      // Connection faild.
      throw new PublizonConnectionException(t('Connection with the Publizon web-service failed.'));
    }

    // Check the response error codes.
    $status = $response->xpath('//status');
    if (isset($status[0]) && $status[0]->code != 101) {
      // Throw exception.
      throw new PublizonException(t('%message with code %code', array(
        '%message' => $status[0]->message,
        '%code' => $status[0]->code,
      )));
    }

    return $response;
  }

  /**
   * Used to set a object setting for communication with the Publizon
   * web-service. If value is given it's used, if non is given the session will
   * be checked and then the default configuration.
   *
   * Data in the session indicates that the current use is logged in, henc this
   * should be used.
   *
   * @param string $name
   *  Name of the setting that should be set.
   * @param string $value
   *  The value that should be set.
   * @throws PublizonException
   *  If the function falls back to using administration setting and this could
   *  not be found exception is throw.
   */
  private function setSetting($name, $value = NULL) {
    if (isset($value)) {
      $this->$name = $value;
    }
    elseif (isset($_SESSION[$name])) {
      $this->$name = $_SESSION[$name];
    }
    else {
      $publizon = variable_get('publizon', FALSE);
      if (!$publizon) {
        throw new PublizonException('Publizon client could not load default settings for ' . $name . '.');
      }
      $this->$name = $publizon['settings'][$name];
    }
  }
}

/**
 * @todo move t() function into the exception, so it's handle in one place.
 * + check_plain
 */

class PublizonException extends Exception {}
class PublizonConnectionException extends Exception {}
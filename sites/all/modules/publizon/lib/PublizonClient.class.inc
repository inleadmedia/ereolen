<?php

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
    'library_order_list' => 'getlibraryuserorderlist.asmx',
    'removed_product_list' => 'getremovedproductlist.asmx',
  );

  private $nanosoap_options = array(
    'namespaces' => array(
      'pub' => 'http://pubhub.org/',
    ),
  );

  // Holds the object instance (part of the singleton pattern).
  private static $instance = NULL;

  /**
   * Class construtor that prepares an connection to publizon and wrappers all
   * calls into this object. This is a private constructor as the object is
   * implemented as a singleton pattern, use PublizonClient::getClient() to get
   * an instance.
   *
   * @param string $langcode
   *  The language code used to fetch the information ('da').
   * @param string $retailer_id
   *  An id that represents the library at publizon.
   * @param string $retailer_key_code
   *  An hash value validating the library as publizon.
   * @param PublizonLogger $logger
   *  Publizon logger class.
   */
  private function __construct($langcode, $retailer_id, $retailer_key_code, $logger) {
    // Get publizon settings from the administration interface.
    $publizon = variable_get('publizon', FALSE);
    if (!$publizon) {
      throw new PublizonException('Publizon client could not load default settings.');
    }

    // Set base URL.
    $this->base_url = $publizon['settings']['base_url'];

    // If input parameters are not given use default values.
    if (!isset($langcode) || !isset($retailer_id) || !isset($retailer_key_code)) {
      $this->retailer_id = $publizon['settings']['retailer_id'];
      $this->retailer_key_code = $publizon['settings']['retailer_key_code'];
      $this->langcode = $publizon['settings']['langcode'];
    }
    else {
      $this->retailer_id = $retailer_id;
      $this->retailer_key_code = md5($retailer_key_code);
      $this->langcode = $langcode;
    }

    // Set logger.
    $this->logger = new PublizonVoidLogger();
    if (!isset($logger)) {
      if ($publizon['settings']['logging']) {
        $this->logger = new PublizonWatchDogLogger();
      }
    }
  }

  public static function getClient($langcode = NULL, $retailer_id = NULL, $retailer_key_code = NULL, $logger = NULL) {
    if (!isset(self::$instance)) {
      $class = __CLASS__;
      self::$instance = new $class($langcode, $retailer_id, $retailer_key_code);
    }
    return self::$instance;
  }

  public function createLoan() {
    throw new Exception('Not implemented yet');
  }

  public function getLastLoans() {
    throw new Exception('Not implemented yet');
  }

  public function getLastLoansByLibrary() {
    throw new Exception('Not implemented yet');
  }

  public function getLastLoansForAll() {
    throw new Exception('Not implemented yet');
  }

  public function getTopList() {
    throw new Exception('Not implemented yet');
  }

  public function getLibraryUserOrderList() {
    throw new Exception('Not implemented yet');
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
    $this->logger->log('GetProduct with ISBN: ' . $isbn);
    $response = $this->call('product', 'GetProduct', array('pub:ebookid' => $isbn));

    // Convert XML into PublizonProduct object.
    $data = $response->xpath('//data');
    if (isset($data[0])) {
      return $data[0];
    }
    $this->logger('The product with isbn (' . $isbn . ') did not return any data', 'WARNING');
  }

  public function getProductList() {
    throw new Exception('Not implemented yet');
  }

  public function getRemovedProductList() {
    throw new Exception('Not implemented yet');
  }

  public function validateLibraryUuser() {
    throw new Exception('Not implemented yet');
  }


  private function call($endpoint, $action, $parameters) {
    $client = new PublizonNanoSOAPClient($this->base_url . $this->end_points[$endpoint], $this->nanosoap_options);
    $parameters += array(
      'pub:retailerid' => $this->retailer_id,
      'pub:retailerkeycode' => $this->retailer_key_code,
      'pub:langcode' => $this->langcode,
    );

    try {
      $response = $client->call('pub:' . $action, $parameters);
      $response = simplexml_load_string($response);
    }
    catch (Exception $e) {
      // Connection faild.

    }

    // Check the response error codes.
    $status = $response->xpath('//status');
    if (isset($status[0]) && $status[0]->code != 101) {
      // Throw exception.
      echo 'tis';
    }

    return $response;
  }
}

class PublizonException extends Exception {}
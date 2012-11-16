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
      $this->retailer_key_code = md5($publizon['settings']['retailer_key_code']);
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

  public function create_loan() {
    throw new Exception('Not implemented yet');
  }

  public function get_last_loans() {
    throw new Exception('Not implemented yet');
  }

  public function get_last_loans_by_library() {
    throw new Exception('Not implemented yet');
  }

  public function get_last_loans_for_all() {
    throw new Exception('Not implemented yet');
  }

  public function get_top_list() {
    throw new Exception('Not implemented yet');
  }

  public function get_library_user_order_list() {
    throw new Exception('Not implemented yet');
  }

  public function get_product($isbn) {
    // Get product form the web-serivce.
    $this->logger->log('GetProduct with ISBN: ' . $isbn);
    $response = $this->call('product', 'GetProduct', array('pub:ebookid' => $isbn));

    // Convert XML into PublizonProduct object.

  }

  public function get_product_list() {
    throw new Exception('Not implemented yet');
  }

  public function get_removed_product_list() {
    throw new Exception('Not implemented yet');
  }

  public function validate_library_user() {
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

/**
 *
 */
class PublizonProduct {
  private $created;
  private $modified;
  private $published;
  private $status;
  private $language;
  private $cover;
  private $teaser_link;
  private $isbn;
  private $contributors;
  private $formats;
  private $bytes;
  private $duration;
  private $price;
  private $publiser;
  private $description;
  private $categories;

  public function __construct() {}
}

/**
 *
 */
class PublizonLoan {

  private $loan;

  public function __construct(int $uid) {
    // Use cache to load loans to
  }

  public function loan_expired() {
    throw new Exception('Not implemented yet');
  }

}

/**
 *
 */
class PublizonUser {
  private $uid;
  private $card_number;
  private $pin;

  private $loans = array();

  public function get_loans() {
    // Lazy load loans.
    throw new Exception('Not implemented yet');
  }

  public function add_loan() {
    // Create loan and update cache.
    throw new Exception('Not implemented yet');
  }
}

class PublizonException extends Exception {}
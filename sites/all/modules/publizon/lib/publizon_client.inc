<?php

class PublizonClient {
  private $base = 'https://libraryservices.pubhub.dk/';
  private $end_points = array(
    'product' => 'getproduct.asmx',
    'product_list' => 'getproductlist.asmx',
    'loan' => 'createloan.asmx',
    'user' => 'validatelibraryuser.asmx',
    'libraray_list' => 'getlibrarylist.asmx',
    'library_order_list' => 'getlibraryuserorderlist.asmx',
    'removed_product_list' => 'getremovedproductlist.asmx',
  );

  private $retailer_id;
  private $retailer_key_code;
  private $langcode;

  private $nanosoap_options = array(
    'namespaces' => array(
      'pub' => 'http://pubhub.org/',
    ),
  );

  /**
   * Class construtor that prepares an connection to publizon and wrappers all
   * calls into this object.
   *
   * @param type $langcode
   *    The language code used to fetch the information ('da').
   * @param type $retailer_id
   *    An id that represents the library at publizon.
   * @param type $retailer_key_code
   *    An hash value validating the library as publizon.
   */
  public function __construct($langcode, $retailer_id, $retailer_key_code) {
    $this->retailer_id = $retailer_id;
    $this->retailer_key_code = md5($retailer_key_code);
    $this->langcode = $langcode;

    // Start up nanosoap
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
    $client = new PublizonNanoSOAPClient($this->base . $this->end_points[$endpoint], $this->nanosoap_options);
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
class PublizonNanoSOAPClient extends NanoSOAPClient {

  /**
   *
   */
  function __construct($endpoint, $options = array()) {
    parent::__construct($endpoint, $options);
  }

  /**
   * Make a SOAP request. Modified this verion to new set a SOAP action header
   * as publizon's service don't link that
   *
   * @param array $parameters
   *   The parameters to send with the SOAP request.
   *
   * @return string
   *   The SOAP response.
   */
  function call($action, $parameters = array()) {
    // Set content type and send the SOAP action as a header.
    $headers = array(
      'Content-Type: text/xml',
    );

    // Make a DOM document from the envelope and get the Body tag so we
    // can add our request data to it.
    $this->doc = new DOMDocument();
    $this->doc->loadXML($this->generateSOAPenvelope());
    $body = $this->doc->getElementsByTagName('Body')->item(0);

    // Convert the parameters into XML elements and add them to the
    // body. The root element of this structure will be the action.
    $elem = $this->convertParameter($action, $parameters);
    $body->appendChild($elem);

    // Render and store the final request string.
    $this->requestBodyString = $this->doc->saveXML();

    // Send the SOAP request to the server via CURL.
    return $this->curlRequest($this->endpoint, 'POST', $this->requestBodyString, $headers);
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
}

/**
 *
 */
class PublizonLoan {

}

/**
 *
 */
class PublizonUser {

}
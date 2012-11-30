<?php
/**
 * @file
 * Create the PublizonLibrary class the holds bsaic information about a given
 * library as registered by Publizon.
 */

/**
 * PublizonLibrary class the holds bsaic information about a given library as
 * registered by Publizon.
 */
class PublizonLibrary extends Publizon {
  protected $properties = array(
    'retailer_id' => NULL,
    'dbcId' => NULL,
    'name' => NULL,
    'MaxEbookLoansPerBorrower' => NULL,
    'MaxAudioLoansPerBorrower' => NULL,
    'LoanDurationInDays' => NULL,
  );

  /**
   * Creates the object, if the parameters is not given if will fallback to the
   * once currently in the session or the library that is defined as default by
   * the site administrator.
   *
   * @param string $retailer_id
   *  The librarys ID at Publizon.
   * @param string $retailer_key_code
   *  The librarys access code at Publizon.
   * @param string $reset
   *  If TRUE the cache is by-passed.
   */
  public function __construct($retailer_id = NULL, $retailer_key_code = NULL, $reset = FALSE) {
    // Check cache for the properties values.
    if (!$reset && ($cache = cache_get($isbn, 'cache_publizon_library'))) {
      $this->properties = $cache->data;
    }
    else {
      $client = PublizonClient::getClient();
      $this->parse($client->getLibraryProfile($retailer_id, $retailer_key_code));

      // We cache the result for 24 hours.
      cache_set($retailer_id, $this->properties, 'cache_publizon_library', time() + 21600);
    }
  }

  /**
   * Get the max number of loan per borrower base on the current site type.
   *
   * @return int
   *  The max number of loan per borrower.
   */
  public function getMaxLoans() {
    $var = 'MaxEbookLoansPerBorrower';
    $type = variable_get('publizon_site_type', PUBLIZON_EBOOK);
    if ($type == PUBLIZON_NETSOUND) {
      $var = 'MaxAudioLoansPerBorrower';
    }

    return $this->$var;
  }

  /**
   * Converts XML response from Publizon into the object properties.
   *
   * @param SimpleXmlElement $xml
   *  The "data" part of the XML message return form Publizon web-service
   *  request to get product.
   */
  private function parse($xml) {
    $this->retailer_id = (int)$xml->Id;
    $this->dbcId = (int)$xml->DbcId;
    $this->name = (string)$xml->Name;
    $this->MaxEbookLoansPerBorrower = (int)$xml->MaxConcurrentLoansPerBorrower;
    $this->MaxAudioLoansPerBorrower = (int)$xml->MaxConcurrentAudioLoansPerBorrower;
    $this->LoanDurationInDays = (string)$xml->LoanDurationInDays;
  }
}

/**
 * Custom Exception for Publizon Library.
 */
class PublizonLibraryException extends PublizonException {}
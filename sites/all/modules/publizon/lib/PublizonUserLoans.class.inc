<?php
/**
 * @file
 * The class defined
 */

/**
 * Holds information about a given users loans (storage class). The loans are
 * indexed by ISBN and can be fetched by $user_loans->loans['isbn'];
 *
 * @todo: Test that the loans can be indexed, if not move it outside properties.
 *
 */
class PublizonUserLoans extends Publizon {
  private $properties = array(
    'max_loan_period' => NULL,
    'max_loan_per_time' => NULL,
    'max_loans_total' => NULL,

    /**
    * @todo: Aske Publizon what these are used for!
    */
    'max_loan_pertime_sound' => NULL,
    'max_loans_total_sound' => NULL,

    /**
    * Loans in both netsound and ebook.
    */
    'max_total_period' => NULL,
    'user_total_loans' => NULL,

    'usere_book_loans_remain' => NULL,
    'usere_book_loans_nexttime' => NULL,

    'user_sound_loans_nexttime' => NULL,

    'user_total_ebook_loans' => NULL,
    'user_total_sound_loans' => NULL,

    // Holds the objects loans.
    'loans' => NULL,
  );

  /**
   * Creates a new PublizonLoans object that stores basic information about the
   * users loans and also how many loans the user has. The object assumes that
   * the user is logged in and faling back to using the session values when
   * connecting to Publizon.
   *
   * @param string $uid
   *  Drupal user ID
   * @throws PublizonProductException
   */
  public function __construct($uid, $card_number, $reset = FALSE) {
    // Check cache for the properties values.
    if (!$reset && ($cache = cache_get($uid, 'cache_publizon_loans'))) {
      $this->properties = $cache->data;
    }
    else {
      $client = PublizonClient::getClient();
      $data = $client->getLibraryUserOrderList($card_number, variable_get('publizon_site_type', PUBLIZON_EBOOK));
      if ($data) {
        $this->parse($data);
      }
      else {
        throw new PublizonLoansException('The server did not return any information about the users loans.');
      }

      // We cache the result for 24 hours.
      cache_set($uid, $this->properties, 'cache_publizon_loans', time() + 43200);
    }
  }

  /**
   * Checks if a given loan is expired.
   *
   * @param string $isbn
   *  ISBN for the loan that should be tested
   * @return bool
   *  Returns TRUE if the loan is expired else FALSE
   * @throws PublizonProductException
   *  The exception is thrown if the loan is not found for the user.
   */
  public function is_expired($isbn) {
    if (isset($this->loans[$isbn])) {
      if (($this->loans[$isbn]->expire_date - time()) > 0) {
        return FALSE;
      }
      return TRUE;
    }
    throw new PublizonLoansException('The ISBN given is not a loan that exists for the current user');
  }

  /**
   * Checks if a loan with the ISBN given is active for the user.
   *
   * @param string $isbn
   *  ISBN of the loan to check.
   * @return boolean
   *  TRUE if the user have the loan else FALSE.
   */
  public function is_a_loan($isbn) {
    if (isset($this->loans[$isbn])) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Helper that is used during creation of this class and parses the XML
   * messages delivered from the web-service. It expects to review and array
   * with two elements (LibrayExtension and Loans) from the XML response.
   *
   * @param array $data
   *  Array with two elements (LibrayExtension and Loans) from the XML response.
   */
  private function parse($data) {
    // Set information about the users loans limit.
    $extension = $data[1];
    $this->max_loan_period = (int)$data[1]->maxloanperiod;
    $this->max_loan_per_time = (int)$data[1]->maxloanpertime;
    $this->max_loans_total = (int)$data[1]->maxloanstotal;
    $this->max_loan_pertime_sound = (int)$data[1]->maxloanpertimesound;
    $this->max_loans_total_sound = (int)$data[1]->maxloanstotalsound;
    $this->max_total_period = (int)$data[1]->maxtotalperiod;
    $this->user_total_loans = (int)$data[1]->usertotalloans;
    $this->usere_book_loans_remain = (int)$data[1]->userebookloansremain;
    $this->usere_book_loans_nexttime = strtotime((string)$data[1]->userebookloansnexttime);
    $this->user_sound_loans_nexttime = strtotime((string)$data[1]->usersoundloansNexttime);
    $this->user_total_ebook_loans = (int)$data[1]->usertotalebookloans;
    $this->user_total_sound_loans = (int)$data[1]->usertotalsoundloans;

    $loans = array();
    foreach ($data[0]->orderitem as $item) {
      // Get book authors.
      $authors = array();
      foreach ($item->book->authors->author as $author) {
        $authors[] = (string)$author->attributes()->name;
      }

      // Build loan array.
      $loans[(string)$item->book->attributes()->id] = array(
        'retailer_order_number' => (string)$item->retailerordernumber,
        'status' => (string)$item->orderstatus->attributes()->name,
        'order_date' => strtotime((string)$item->orderdate),
        'expire_date' => strtotime((string)$item->loanexpiredate),
        'title' => (string)$item->book->title,
        'authors' => $authors,
        'publisher' => (string)$item->book->publisher,
        'product' => new PublizonProduct((string)$item->book->attributes()->id),
      );
    }
    $this->loans = $loans;
  }
}

class PublizonLoansException extends Exception {}
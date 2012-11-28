<?php
/**
 * @file
 *
 */

/**
 * Stores information about a given users loans (storage class). So information
 * about max loans and remaining loans for the current user is stored here.
 *
 * The users loans are indexed by ISBN and can be fetched by
 * $user_loans->loans['isbn'] or simple $user_loans->loans. For more information
 * about a loan see the PublizonLoan class.
 *
 */
class PublizonUserLoans extends Publizon {
  protected $properties = array(
    'max_loan_period' => NULL,
    'max_loan_per_time' => NULL,
    'max_loans_total' => NULL,

    /**
    * @todo: Aske Publizon what these are used for!
    */
    'max_loan_pertime_sound' => NULL,
    'max_loans_total_sound' => NULL,

    /**
    * Loans in both netsound and ebook ?
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
   * @param bool $fetch_ting_object
   *  Should the object fetch the ting objects for each loan
   * @param string $card_number
   *  The card number or CPR (login id) to get loans for (defaults to using the
   *  session).
   * @param bool $reset
   *  Should the cache be by passed.
   * @throws PublizonProductException
   */
  public function __construct($uid, $fetch_ting_object = FALSE, $card_number = NULL, $reset = FALSE) {
    // Check cache for the properties values.
    if (!$reset && ($cache = cache_get($uid, 'cache_publizon_loans'))) {
      $this->properties = $cache->data;
    }
    else {
      // Get the users loans.
      $this->fetchLoans();

      // We cache the result until the user makes changes to the loans or loan
      // expires.
      cache_set($uid, $this->properties, 'cache_publizon_loans');
    }

    // This can not be done when parsing the loans, as this has to be outside
    // cache and freash data always.
    if ($fetch_ting_object) {
      foreach ($this->loans as $loan) {
        $loan->product->getTingObject();
      }
    }
  }

  /**
   * Checks if a loan with the ISBN given is active for the user.
   *
   * @param string $isbn
   *  ISBN of the loan to check.
   * @param boolean $expired
   *  If TRUE the loan is also have to not be expired to be a loan.
   * @return boolean
   *  TRUE if the user have the loan else FALSE.
   */
  public function isLoan($isbn, $expired = FALSE) {
    if (isset($this->loans[$isbn])) {
      if ($expired) {
        if ($this->loans[$isbn]->isExpired()) {
          return FALSE;
        }
      }
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Create a new loan for the currently logged in user. If the loan is
   * unsuccessful an Publizon exception will be thrown.
   *
   * @param string $isbn
   *  The products ISBN number for which the user is trying to loan.
   * @return string
   *  The download link to the prodoct, if successfull.
   */
  public function createLoan($isbn) {
    // Check if this already is a loan and if so return that loan.
    if ($this->isLoan($isbn) && !$this->loans[$isbn]->isExpired()) {
      return $this->loans[$isbn];
    }

    // Try to create new loan.
    $client = PublizonClient::getClient();
    $download_url = $client->createLoan($this->getCardNumber(), $this->getPincode(), $isbn);

    // Refresh the local loans cache.
    $this->refreshCache();

    // Return the download url.
    return $download_url;
  }

  /**
   * Finds the number of hours or days until the current user can loan a book
   * again.
   *
   * @return string
   *  Formatted string with the next expire time.
   */
  public function nextLoanPeriode() {
    // Because of the loans cache we can not use the field available
    // from publizon.

    // Find the first loan to expire.
    $expire = 100000000000;
    foreach ($this->loans as $loan) {
      $expire = ($loan->expire_date > $expire) ? $expire : $loan->expire_date;
    }
    $hours = (date_difference(time(), $expire, 'hours', DATE_UNIX));
    if ($hours <= 24) {
      $str = t('You can loan a new product in %hours.', array('%hours' => format_plural($hours, 'one hour', '@count hours')));
    }
    else {
      $days = date_difference(time(), $expire, 'days', DATE_UNIX);
      $str = t('You can loan a new product in %days.', array('%days' => format_plural($days, 'one day', '@count days')));
    }

    return $str;
  }


  /**
   * Fetch all loans for the current user.
   *
   * @throws PublizonLoansException
   */
  private function fetchLoans() {
    $client = PublizonClient::getClient();
    $data = $client->getLibraryUserOrderList($this->getCardNumber(), variable_get('publizon_site_type', PUBLIZON_EBOOK));
    if ($data) {
      $this->parse($data);
    }
    else {
      throw new PublizonLoansException('The server did not return any information about the users loans.');
    }
  }

  /**
   * If the parameter given is not NULL and is numeric the card number is
   * returned. If not, the session is checked for a login ID, which is returned,
   * if found.
   *
   * @param string $card_number
   *  Login ID which most likely is a card number or CPR.
   * @return string
   *  The cardnumber.
   * @throws PublizonLoansException
   *  If the card number / login id is not found the exception is thrown.
   */
  private function getCardNumber($card_number = NULL) {
    if (isset($card_number) && is_numeric($card_number)) {
      return $card_number;
    }
    elseif (isset($_SESSION['login_id'])) {
      return $_SESSION['login_id'];
    }
    throw new PublizonLoansException('Can\'t get loans for the current user as login ID was not detected.');
  }

  /**
   * If the parameter given is not NULL and is numeric the pincode is returned.
   * If not, the session is checked for a pincode (pass), which is returned, if
   * found.
   *
   * @param string $pincode
   *  The current users pincode.
   * @return string
   *  The current users pincode
   * @throws PublizonLoansException
   *  If no pincode information is found and exception is thrown.
   */
  private function getPincode($pincode = NULL) {
    if (isset($pincode) && is_numeric($pincode)) {
      return $pincode;
    }
    elseif (isset($_SESSION['pass'])) {
      return $_SESSION['pass'];
    }
    throw new PublizonLoansException('Can\'t get loans for the current user as login ID was not detected.');
  }

  /**
   * Update the cache with the current loans
   */
  private function refreshCache() {
    global $user;

    // Get the users loans.
    $this->fetchLoans();

    // We cache the result until the user makes changes to the loans or loan
    // expires.
    cache_set($user->uid, $this->properties, 'cache_publizon_loans');
  }

  /**
   * Helper that is used during creation of this class and parses the XML
   * messages delivered from the web-service. It expects to review and array
   * with two elements (LibrayExtension and Loans) from the XML response.
   *
   * @param array $data
   *  Array with two elements (LibrayExtension and Loans) from the XML response
   *  as SimpleXmlElement objects.
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
      $loans[(string)$item->book->attributes()->id] = new PublizonLoan($item);
    }
    $this->loans = $loans;
  }
}

class PublizonLoansException extends PublizonException {}
<?php

/**
 *
 */
class PublizonUserLoans {
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
   * The object assumes that the user is logged in and faling back to using the
   * session values when connecting to Publizon.
   *
   * @param type $uid
   * @throws PublizonProductException
   */
  public function __construct($uid, $card_number, $reset = FALSE) {
    // Check cache for the properties values.
    if (!$reset && ($cache = cache_get($uid, 'cache_publizon_loans'))) {
      $this->properties = $cache->data;
    }
    else {
      /**
       * @TODO: Move the book type into configuration.
       */
      $client = PublizonClient::getClient();
      $data = $client->getLibraryUserOrderList($card_number, PUBLIZON_EBOOK);
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

  public function loan_expired() {
    throw new Exception('Not implemented yet');
  }


  /**
   * Implements PHP magic __get function.
   */
  public function __get($name) {
    if (array_key_exists($name, $this->properties)) {
      return $this->properties[$name];
    }
    throw new PublizonLoansException('Trying to get publizon loans property that do not exists (' . $name . ').');
  }

  /**
   * Implements PHP magic __set function.
   */
  public function __set($name, $value) {
    if (array_key_exists($name, $this->properties)) {
      $this->properties[$name]= $value;
    }
    else {
      throw new PublizonLoansException('Trying to set publizon loans property that do not exists (' . $name . ').');
    }
  }

  /**
   * Implements PHP magic __isset function.
   */
  public function __isset($name) {
    if (array_key_exists($name, $this->properties)) {
      return isset($this->properties[$name]);
    }
    else {
      throw new PublizonLoansException('Trying to test publizon loans property that do not exists (' . $name . ').');
    }
  }

  /**
   * Implements PHP magic __unset function.
   */
  public function __unset($name) {
    if (array_key_exists($name, $this->properties)) {
      $this->properties[$name] = NULL;
    }
    else {
      throw new PublizonLoansException('Trying to unset publizon loans property that do not exists (' . $name . ').');
    }
  }

  public function __toString() {
    throw new PublizonLoansException('To string have not been implemented yet');
  }

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
      $loans[] = array(
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
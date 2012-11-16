<?php

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
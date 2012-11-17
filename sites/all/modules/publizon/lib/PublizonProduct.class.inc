<?php

/**
 *
 */
class PublizonProduct {
  private $properties = array(
    'created' => NULL,
    'modified' => NULL,
    'published' => NULL,
    'title' => NULL,
    'status' => NULL,
    'language' => NULL,
    'cover' => NULL,
    'teaser_link' => NULL,
    'isbn' => NULL,
    'author' => NULL,
    'format' => NULL,
    'bytes' => NULL,
    'duration' => NULL,
    'price' => NULL,
    'publiser' => NULL,
    'description' => NULL,
    'categories' => NULL,
  );

  /**
   * Constructor that takes a ISBN and fetchs the product from Publizon if the
   * information is not located the product cache.
   *
   * @param string $isbn
   *  ISBN to identify the product.
   * @param bool $reset
   *  If TRUE the cache is updated. Defaults to FALSE.
   *
   * @throws PublizonProductException
   */
  public function __construct($isbn, $reset = FALSE) {
    // Check cache for the properties values.
    if (!$reset && ($cache = cache_get($isbn, 'cache_publizon_product'))) {
      $this->properties = $cache->data;
    }
    else {
      // If ISBN is provied and not cached. The publizon web-service will be
      // asked for the product information.
      if (isset($isbn)) {
        $client = PublizonClient::getClient();
        $this->parse($client->getProduct($isbn));
        cache_set($isbn, $this->properties, 'cache_publizon_product', time() + 43200);
      }
      else {
        throw new PublizonProductException('ISBN was not provided when trying to create Publizon product.');
      }
    }
  }

  public function __get($name) {
    if (array_key_exists($name, $this->properties)) {
      return $this->properties[$name];
    }
    throw new PublizonProductException('Trying to get publizon product property that do not exists (' . $name . ').');
  }

  public function __set($name, $value) {
    if (array_key_exists($name, $this->properties)) {
      $this->properties[$name]= $value;
    }
    else {
      throw new PublizonProductException('Trying to set publizon product property that do not exists (' . $name . ').');
    }
  }

  public function __isset($name) {
    if (array_key_exists($name, $this->properties)) {
      return isset($this->properties[$name]);
    }
    else {
      throw new PublizonProductException('Trying to test publizon product property that do not exists (' . $name . ').');
    }
  }

  public function __unset($name) {
    if (array_key_exists($name, $this->properties)) {
      $this->properties[$name] = NULL;
    }
    else {
      throw new PublizonProductException('Trying to unset publizon product property that do not exists (' . $name . ').');
    }
  }

  private function parse($xml) {
    $this->created = strtotime($xml->created_date);
    $this->modified = strtotime($xml->updated_date);
    $this->title = (string)$xml->title;
    $this->status = (((string)$xml->status->description) == 'Active') ? TRUE : FALSE;
    $this->language = (string)$xml->language;
    $this->teaser_link = (string)$xml->teaser->link;
    $this->isbn = (string)$xml->external_ids->id;
    $this->author = array(
      'firstname' => (string)$xml->contributors->first_name,
      'lastname' => (string)$xml->contributors->family_name,
    );
    $this->format = (string)$xml->formats->name;
    $this->bytes = (string)$xml->formats->size_bytes;
    $this->duration = (string)$xml->formats->duration_minutes;
    $this->price = (string)$xml->price;
    $this->publiser = (string)$xml->publisher->name;
    $this->published = strtotime((string)$xml->first_published);
    $this->description = (string)$xml->description;

    $categories = array();
    foreach ($xml->categories as $category) {
      $categories[] = (string)$category->category;
    }
    $this->categories = $categories;

    // Download image here or in __get
    $this->cover = (string)$xml->coverimage;
  }
}

class PublizonProductException extends Exception {}
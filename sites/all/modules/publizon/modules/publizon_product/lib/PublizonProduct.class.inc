<?php
/**
 * @file
 * Defines class to hold a Publizon product. If the class is parsed an ISBN
 * number under contruction it will automatically download the information form
 * the web-service. If the product have been cached, the cached version will be
 * used.
 *
 * Eg. $product = new PublizonProduct('9788770703147');
 */

/**
 * Publizon Product class that can hold a Publizon Product (ebook or netsound).
 */
class PublizonProduct extends Publizon {
  protected $properties = array(
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

  // This is outside the properties array, as it should not be cached witht the
  // object properties. The ting object is used to get extended information and
  // verify that the product is avaliable, so it needs to be as fresh as
  // possible, hence it's not cached.
  private $ting_object = NULL;

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

        // The product may sometimes not contain any information (removed from
        // Publizon). But maybe the datawell have information, so we store the
        // ISBN.
        $this->isbn = $isbn;

        // We cache the result for 24 hours.
        cache_set($isbn, $this->properties, 'cache_publizon_product', time() + 43200);
      }
      else {
        throw new PublizonProductException('ISBN was not provided when trying to create Publizon product.');
      }
    }
  }

  /**
   * Returns the cover image through an image cache preset. If the full size or
   * raw cover image is needed simple call: $product->cover.
   *
   * If the product do not have a cover image a default one located in the files
   * folder name default_cover.jpg will be used.
   *
   * @param string $presetname
   *  Name of the image cache preset to run the image through.
   * @param bool $local_cache
   *  If cover is not defined, try to look in the local filesystem. There may
   *  be a copy on storage. This can be usefull on user loans lists. Defaults to
   *  FALSE.
   * @return string
   *  Path to the rendered image.
   */
  public function getCover($presetname, $local_cache = TRUE) {
    if (!isset($this->cover)) {
      return imagecache_create_url($presetname, file_directory_path() . '/default_cover.jpg');
    }
    if ($local_cache) {
      $path = file_directory_path() . '/' . $this->encodeFile();
      if (!file_exists($path)) {
        return imagecache_create_url($presetname, $path);
      }
    }
    return imagecache_create_url($presetname, $this->cover);
  }

  /**
   * JIT fetch a ting object from the datawell that represents the publizon
   * product.
   *
   * @return TingObject
   *  Returns a ting object from the T!NG datawell.
   */
  public function getTingObject() {
    if (!isset($this->ting_object)) {
      // If not fetched in this http request, search the datawell for the product.
      $this->ting_object = publizon_ting_find_object($this->isbn);
    }
    return $this->ting_object;
  }


  /**
   * Wrapper function that fetch the product cover (image) from the web-service
   * and stores it locally.
   *
   * @param string $url
   *  The localtion from where to fetch the cover image.
   * @return string
   *  The local path where the image is located.
   */
  private function fetchCover($url) {
    // Check that the URL is a valid url.
    if (filter_var($url, FILTER_VALIDATE_URL, FILTER_FLAG_SCHEME_REQUIRED)) {
      return $this->downloadImage($url);
    }
  }

  /**
   * Download image from a remote location over http(s).
   *
   * @param string $src
   *  The localtion from where to fetch the cover image.
   * @return string
   *  The local path where the image is located.
   */
  private function downloadImage($src) {
    // Build full path into the sites file folder.
    $path = file_directory_path() . '/' . $this->encodeFile();

    // Check if file exists locally, henc no need to download it.
    if (!file_exists($path)) {
      // Request the image file form the remote server over http(s).
      $result = drupal_http_request($src);

      // If the request succeded, save the image.
      if ($result->code == 200) {
        $folder = dirname($path);
        // Create destination folder, if it does not exists.
        if (!file_check_directory($folder, FILE_CREATE_DIRECTORY)) {
          // The opreation failed (might be permissions).
          $path = FALSE;
        }
        else {
          // Save the file and return the new file name.
          $path = file_save_data($result->data, $path, FILE_EXISTS_REPLACE);
        }
      }
    }

    return $path;
  }

  /**
   * Encodes the products into a unique path where the cover image can be saved.
   *
   * @return string
   *  Unique filename based on the product ISBN.
   */
  private function encodeFile() {
    return 'covers/' . md5($this->isbn) . '.jpg';
  }

  /**
   * Converts XML response from Publizon into the object properties.
   *
   * @param SimpleXmlElement $xml
   *  The "data" part of the XML message return form Publizon web-service
   *  request to get product.
   */
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
    $this->cover = $this->fetchCover((string)$xml->coverimage);
  }
}

class PublizonProductException extends PublizonException {}
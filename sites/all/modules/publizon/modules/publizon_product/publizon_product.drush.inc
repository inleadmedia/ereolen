<?php

/**
 * @file
 * This file implements support for drush into the module.
 */

/**
 * Adds a cache clear option for the recommendation cache.
 */
function publizon_product_drush_cache_clear(&$types) {
  $types['publizon-product'] = 'publizon_product_cache_flush';
}

/**
 * Clears out all cache entries about Publizon loans.
 */
function publizon_product_cache_flush() {
  cache_clear_all('*', 'cache_publizon_product', TRUE);
}

/**
 * @todo: Clean single product cache entry and optiona download image.
 */
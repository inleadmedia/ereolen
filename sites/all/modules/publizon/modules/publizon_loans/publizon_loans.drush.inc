<?php

/**
 * @file
 * This file implements support for drush into the module.
 */

/**
 * Adds a cache clear option for the recommendation cache.
 */
function publizon_loans_drush_cache_clear(&$types) {
  $types['publizon-loans'] = 'publizon_loans_cache_flush';
}

/**
 * Clears out all cache entries about Publizon loans.
 */
function publizon_loans_cache_flush() {
  cache_clear_all('*', 'cache_publizon_loans', TRUE);
}

/**
 * @todo: Clean single product and clear single user loans.
 */
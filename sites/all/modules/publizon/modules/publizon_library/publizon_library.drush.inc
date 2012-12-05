<?php

/**
 * @file
 * This file implements support for drush into the module.
 */

/**
 * Adds a cache clear option for the recommendation cache.
 */
function publizon_library_drush_cache_clear(&$types) {
  $types['publizon-library'] = 'publizon_library_cache_flush';
}

/**
 * Clears out all cache entries about Publizon loans.
 */
function publizon_library_cache_flush() {
  cache_clear_all('*', 'cache_publizon_library', TRUE);
}

/**
 * @todo: Clean single library cache entry.
 */
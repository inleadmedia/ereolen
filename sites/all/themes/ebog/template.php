<?php

/**
 * Implementation of HOOK_theme().
 */
function ebog_theme(&$existing, $type, $theme, $path) {
  $hooks = omega_theme($existing, $type, $theme, $path);
  return $hooks;
}

/**
 * Implements hook_preprocess_ting_object().
 *
 * Add extra information form elib to the ting object.
 */
function ebog_preprocess_ting_object(&$vars) {
  // Find valide ISBN 13, there may be more than one.
  foreach ($vars[object]->record['dc:identifier']['dkdcplus:ISBN'] as $isbn) {
    if (preg_match('/^[0-9]{13}/', $isbn, $matches)) {
      $vars['elib_isbn'] = $isbn;
    }
  }

  // Override ting object page title.
  drupal_set_title(check_plain($vars['object']->title . ' ' . t('af') . ' ' . $vars['object']->creators_string));

  // Get cover image.
  $vars['elib_book_cover'] = elib_book_cover($vars['object']->record['dc:identifier']['dkdcplus:ISBN'], '170_x');

  // Get ebook sample link.
  $client = elib_client();
  $client->setLibrary(variable_get('elib_retailer_id', ''));
  $book = $client->getBook($isbn);
  if ($book->status->code == 101 && isset($book->data->teaser->link)) {
    $vars['elib_sample_link'] = (string)$book->data->teaser->link;
  }
}

/**
 * Implements hook_preprocess_ting_search_collection().
 *
 * Add extra information from elib to the ting objects.
 */
function ebog_preprocess_ting_search_collection(&$vars) {
  // Get elib client.
  $client = elib_client();
  $client->setLibrary(variable_get('elib_retailer_id', ''));

  foreach ($vars['collection']->objects as $obj) {
    foreach ($obj->record['dc:identifier']['dkdcplus:ISBN'] as $isbn) {
      if (preg_match('/^[0-9]{13}/', $isbn, $matches)) {
        if (isset($vars['elib'])) {
          $vars['elib'][$isbn] = array();
        }
        else {
          $vars['elib'] = array();
        }
      }
    }

    // Get cover image.
    $vars['elib'][$isbn]['elib_book_cover'] = elib_book_cover($obj->record['dc:identifier']['dkdcplus:ISBN'], '170_x');

    // Get ebook sample link.
    $book = $client->getBook($isbn);
    if ($book->status->code == 101 && isset($book->data->teaser->link)) {
      $vars['elib'][$isbn]['elib_sample_link'] = (string)$book->data->teaser->link;
    }
  }
}

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function ebog_preprocess_page(&$vars, $hook) {
  global $user;

  array_pop($vars['primary_links']) ;
  if ($user->uid != 0) {
    $vars['primary_links']['account-link'] = array(
      'href'  => 'min_side',
      'title' => t('Min side'),
    );
  }
  else {
    $vars['primary_links']['login-link'] = array(
      'href'  => 'user',
      'title' => t('Login')
    );
  }

  $primary_links = theme('links', $vars['primary_links'], array('class' => 'menu'));
  $vars['navigation'] = '<div class="block block-menu" id="block-menu-primary-links"><div class="content">' . $primary_links . '</div></div>';

  if(arg(0) == 'min_side' && $user->uid == 0){
    drupal_goto('user', drupal_get_destination());
  }

  if(arg(3) == 'stream' || arg(3) == 'download' || (isset($_GET['clean']) && $_GET['clean'] == 1)){
    $vars['template_files'] = array('page-clean');
    $vars['css']['all']['theme']['sites/all/themes/ebog/css/style.css'] = false;
  }
}


/**
 * Create a string of attributes form a provided array.
 *
 * @param $attributes
 * @return string
 */
function ebog_render_attributes($attributes) {
  return omega_render_attributes($attributes);
}

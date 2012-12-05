<?php
/**
 * @file
 * Defines default implementation of custom theme functions, such as theme_ and
 * preprocess functions.
 */

/**
 * Implements hook_publizon_product_preprocess().
 */
function template_preprocess_publizon_loan(&$vars) {
  $ting_object = $vars['loan']->product->getTingObject();

  // Prepare variables.
  if ($ting_object) {
    $vars['object_link'] = 'ting/object/' . (string)$ting_object->id;
    $vars['title']  = l($ting_object->record['dc:title'][''][0], $vars['object_link'], array(
      'attributes' => array(
        'class' => 'title',
      )
    ));
    $vars['authors'] = publizon_get_authors($ting_object);

    // Get local actions (buttons/icons).
    $vars['actions'] = theme('publizon_actions', $vars['loan']->product, 'loan');
  }
  else {
    $vars['title']  = $vars['loan']->title;
    $vars['authors'] = t('!author', array('!author' => l($vars['loan']->authors[0], 'ting/search/' . $author)));

    // There are no local actions as the book is not available.
    $vars['actions'] = '<div class="book-not-available">'. t('This book is no longer available and can not be downloaded.') . '</div>';
  }

}

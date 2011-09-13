<?php
// $Id: $

/**
 * @file
 *
 */


/**
 * Form builder for the litteratursiden_feed_compare settings form.
 */
function litteratursiden_feed_compare_admin(&$form_state) {
  $form = array();

  $form['litteratursiden_feed_compare'] = array(
    '#type' => 'fieldset',
    '#title' => t('Litteratursiden Feed and Compare settings'),
  );

  $form['litteratursiden_feed_compare']['items_on_front'] = array(
    '#type' => 'textfield',
    '#title' => t('Items shown on front'),
    '#required' => FALSE,
    '#default_value' => variable_get('litteratursiden_feed_compare_items_on_front', '2'),
  );

  $form['litteratursiden_feed_compare']['items_in_widget'] = array(
    '#type' => 'textfield',
    '#title' => t('Items shown on page'),
    '#required' => FALSE,
    '#default_value' => variable_get('litteratursiden_feed_compare_items_on_page', '10'),
  );

  $form['litteratursiden_feed_compare']['cache_lifetime'] = array(
    '#type' => 'textfield',
    '#title' => t('Lifetime of cached items (in seconds)'),
    '#required' => FALSE,
    '#default_value' => variable_get('litteratursiden_feed_compare_cache_lifetime', '360'),
  );

  return system_settings_form($form);
}

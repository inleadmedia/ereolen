<?php
/**
 * @file
 * Defines the administration interface for configuring communication with the
 * Pulizon web-service.
 */

function publizon_admin_form() {
  $form = array(
    '#tree' => TRUE,
  );

  // Load default configuration values.
  $default = variable_get('publizon', array());

  $form['publizon']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default settings'),
    '#description'=> t('These settings are used to communicate with the publizon web-service, when no user is logged in. They are required to be able to fetch information about products (eg. covers)'),
    '#collapsible' => TRUE,
    '#collapsed' => isset($default['settings']['base_url']) ? TRUE : FALSE,
  );

  $form['publizon']['settings']['base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL of the web-service'),
    '#description' => t('The URL of the web-service at Publizon (eg. https://libraryservices.pubhub.dk/)'),
    '#default_value' => isset($default['settings']['base_url']) ?  $default['settings']['base_url'] : 'https://libraryservices.pubhub.dk/',
    '#required' => TRUE,
  );

  $form['publizon']['settings']['retailer_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Retailer ID'),
    '#description' => t("This is the library's identification at publizon, which you should recieve from them."),
    '#default_value' => isset($default['settings']['retailer_id']) ?  $default['settings']['retailer_id'] : '',
    '#required' => TRUE,
  );

  $form['publizon']['settings']['retailer_key_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Retailer key code'),
    '#description' => t('This is a MD5 hash of the code provied with the retailer ID above.'),
    '#default_value' => isset($default['settings']['retailer_key_code']) ?  $default['settings']['retailer_key_code'] : '',
    '#required' => TRUE,
  );

  $form['publizon']['settings']['langcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Language code'),
    '#description' => t('You should also have a language code from Publizon (default: da).'),
    '#default_value' => isset($default['settings']['langcode']) ?  $default['settings']['langcode'] : 'da',
    '#required' => TRUE,
  );

  $form['publizon']['settings']['logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable logging'),
    '#default_value' => variable_get('elib_enable_logging', false),
  );

  $form['publizon']['libraries'] = array(
    '#type' => 'fieldset',
    '#title' => 'Library configuration',
    '#description' => 'When users login they are mapped to a given library, which needs to have there own Publizon settings to sent to the web-service.',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  /**
   * @TODO add custom element with retailer id, Library name and retailer key code.
   */

  return system_settings_form($form);
}
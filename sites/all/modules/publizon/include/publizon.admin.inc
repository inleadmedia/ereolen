<?php
/**
 * @file
 * Defines the administration interface for configuring communication with the
 * Pulizon web-service.
 */

/**
 * Administration form for the configureation of the communication with
 * publizon.
 *
 * @return array $form
 */
function publizon_admin_form() {
  // Add some style to the form.
  drupal_add_css(drupal_get_path('module', 'publizon') . '/css/publizon.admin.css');

  // Ensure that the form is stored as an array.
  $form = array(
    '#tree' => TRUE,
  );

  // Load default configuration values.
  $default = variable_get('publizon', array());

  $form['publizon']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default settings'),
    '#description'=> t('These settings are used to communicate with the publizon web-service, when no user is logged in. They are required to be able to fetch information about products (eg. covers and basic information from Publizon)'),
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
    '#default_value' => isset($default['settings']['logging']) ?  $default['settings']['logging'] : 0,
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

  $form['publizon']['libraries']['test'] = array(
    '#type' => 'publizon_library_field',
    '#title' => 'Test library field',
    '#description' => 'Test description',
    '#default_value' => array(
      'retailer_id' => $default['libraries']['test']['retailer_id'],
      'retailer_key_code' => $default['libraries']['test']['retailer_key_code'],
      'library_name' => $default['libraries']['test']['library_name'],
      'max_loans' => $default['libraries']['test']['max_loans']),
  );

  return system_settings_form($form);
}

function publizon_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  /**
   * @TODO validate that the information enter is correct.
   */
}

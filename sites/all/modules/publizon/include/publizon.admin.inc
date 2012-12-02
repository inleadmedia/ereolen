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
function publizon_admin_form(&$form_state) {
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
    '#description' => t('These settings are used to communicate with the publizon web-service, when no user is logged in. They are required to be able to fetch information about products (eg. covers and basic information from Publizon)'),
    '#collapsible' => TRUE,
    '#collapsed' => isset($default['settings']['base_url']) ? TRUE : FALSE,
  );

  $form['publizon']['settings']['base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL of the web-service'),
    '#description' => t('The URL of the web-service at Publizon (eg. https://libraryservices.pubhub.dk/)'),
    '#default_value' => isset($default['settings']['base_url']) ? $default['settings']['base_url'] : 'https://libraryservices.pubhub.dk/',
    '#required' => TRUE,
  );

  $form['publizon']['settings']['retailer_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Retailer ID'),
    '#description' => t("This is the library's identification at publizon, which you should recieve from them."),
    '#default_value' => isset($default['settings']['retailer_id']) ? $default['settings']['retailer_id'] : '',
    '#required' => TRUE,
  );

  $form['publizon']['settings']['retailer_key_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Retailer key code'),
    '#description' => t('This is a MD5 hash of the code provied with the retailer ID above.'),
    '#default_value' => isset($default['settings']['retailer_key_code']) ? $default['settings']['retailer_key_code'] : '',
    '#required' => TRUE,
  );

  $form['publizon']['settings']['langcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Language code'),
    '#description' => t('You should also have a language code from Publizon (default: da).'),
    '#default_value' => isset($default['settings']['langcode']) ? $default['settings']['langcode'] : 'da',
    '#required' => TRUE,
  );

  $form['publizon']['settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Site type'),
    '#description' => t('Which type for information should the Publizon web-service deliver'),
    '#options' => array(PUBLIZON_EBOOK => 'E-book', PUBLIZON_NETSOUND => 'Net sound'),
    '#default_value' => isset($default['settings']['type']) ? $default['settings']['type'] : PUBLIZON_EBOOK,
  );

  $form['publizon']['settings']['logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable logging'),
    '#default_value' => isset($default['settings']['logging']) ?  $default['settings']['logging'] : 0,
  );


  // Add libraries fieldset.
  $form['publizon']['libraries'] = array(
    '#type' => 'fieldset',
    '#title' => 'Library configuration',
    '#description' => 'When users login they are mapped to a given library, which needs to have there own Publizon settings to sent to the web-service.',
    '#prefix' => '<div id="libraries-fieldset-wrapper">',
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Find the number of elements (libraries) to show, base on the current state.
  $num_libraries = count($default['libraries']) ? count($default['libraries']) : 1;
  if (isset($form_state['values']['publizon']['num_libraries'])) {
    $num_libraries = $form_state['values']['publizon']['num_libraries'];
    if ($form_state['clicked_button']['#value'] == t('Add one more')) {
      $num_libraries++;
    }
    if ($form_state['clicked_button']['#value'] == t('Remove one')) {
      $num_libraries--;
    }
  }

  // Store the number of libraries to ahah callbacks (add one more).
  $form['publizon']['num_libraries'] = array(
    '#type' => 'value',
    '#value' => $num_libraries,
  );

  // Add form elements.
  for ($i = 0; $i < $num_libraries; $i++) {
    $form['publizon']['libraries'][$i] = array(
      '#type' => 'publizon_library_field',
      '#title' => 'Test library field',
      '#description' => 'Test description',
      '#default_value' => array(
        'retailer_id' => $default['libraries'][$i]['retailer_id'],
        'retailer_key_code' => $default['libraries'][$i]['retailer_key_code'],
        'library_name' => $default['libraries'][$i]['library_name'],
      ),
    );
  }

  // Add one more button.
  $form['publizon']['libraries']['add_library'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#ahah' => array(
      'path' => 'admin/settings/publizon/library/ahah',
      'wrapper' => 'libraries-fieldset-wrapper',
      'method' => 'replace',
    ),
    '#attributes' => array('class' => 'add-btn'),
  );

  // Add remove button.
  $form['publizon']['libraries']['remove_library'] = array(
    '#type' => 'submit',
    '#value' => t('Remove one'),
    '#ahah' => array(
      'path' => 'admin/settings/publizon/library/ahah',
      'wrapper' => 'libraries-fieldset-wrapper',
      'method' => 'replace',
    ),
    '#attributes' => (($num_libraries == 1) ? array('class' => 'remove-btn-hide') : array()),
  );

  // Make the form look like at system form.
  $form = system_settings_form($form);

  // Add submit handler befor system settings.
  array_unshift($form['#submit'], 'publizon_admin_form_submit');

  return $form;
}

/**
 * Menu callback used to add or remove an element (library) from the
 * administration form. Returns the library part of the form as JSON.
 */
function publizon_admin_form_ahah() {
  // Build new form state and get the form from cache.
  $form_state = array(
    'storage' => NULL,
    'submitted' => FALSE,
  );
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);

  // Get the form id and prevent the form from doing redirects on rebuild.
  $args = $form['#parameters'];
  $form_id = array_shift($args);

  // We will run some of the submit handlers so we need to disable redirecting.
  $form['#redirect'] = FALSE;
  // We need to process the form, prepare for that by setting a few internals
  // variables.
  $form['#post'] = $_POST;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;

  // Process the form and prepare it for rebuild.
  drupal_process_form($form_id, $form, $form_state);

  // Rebuild it and update the cache.
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  // Get the part thats going to be returend.
  $subform = $form['publizon']['libraries'];

  // Encode the result as JSON.
  drupal_json(array(
    'status' => TRUE,
    'data' => drupal_render($subform),
  ));
}

/**
 * Administration form submit handler, that ensure that correct data is filter
 * before it's sent to system submit handler. It's done in the submit handler as
 * the validator function is also call when add one more is clicked.
 */
function publizon_admin_form_submit($form, &$form_state) {
  // Remove buttons from values;
  unset($form_state['values']['publizon']['libraries']['add_library']);
  unset($form_state['values']['publizon']['libraries']['remove_library']);

  // Store site type information in own variable, so it can be set in
  //settings.php
  variable_set('publizon_site_type', $form_state['values']['publizon']['settings']['type']);
  unset($form_state['values']['publizon']['settings']['type']);

  // Make sure "empty" is removed.
  foreach ($form_state['values']['publizon']['libraries'] as $id => $library) {
    if (empty($library['retailer_id'])) {
      unset($form_state['values']['publizon']['libraries'][$id]);
    }
  }
}
